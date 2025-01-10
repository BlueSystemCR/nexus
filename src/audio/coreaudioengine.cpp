#include "coreaudioengine.h"

#ifdef Q_OS_MACOS

#include <QDebug>
#include <cmath>

class CoreAudioEngine::Private {
public:
    AudioUnit audioUnit;
    AudioStreamBasicDescription format;
    bool isInitialized;
    bool isPlaying;
    float gain;
    double latency;
    float cpuLoad;
    
    std::vector<float> buffer;
    
    Private() : isInitialized(false), isPlaying(false),
                gain(1.0f), latency(0.0), cpuLoad(0.0) {}
};

CoreAudioEngine::CoreAudioEngine(QObject* parent)
    : QObject(parent), d(new Private) {
    initialize();
}

CoreAudioEngine::~CoreAudioEngine() {
    cleanup();
}

bool CoreAudioEngine::initialize() {
    if (d->isInitialized) return true;
    
    setupAudioUnit();
    setupAudioFormat();
    configureCallbacks();
    
    // Initialize audio unit
    OSStatus status = AudioUnitInitialize(d->audioUnit);
    if (status != noErr) {
        handleError(status, "Failed to initialize audio unit");
        return false;
    }
    
    d->isInitialized = true;
    return true;
}

void CoreAudioEngine::cleanup() {
    if (d->isInitialized) {
        stop();
        AudioUnitUninitialize(d->audioUnit);
        AudioComponentInstanceDispose(d->audioUnit);
        d->isInitialized = false;
    }
}

void CoreAudioEngine::setupAudioUnit() {
    // Define audio unit component
    AudioComponentDescription desc;
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_DefaultOutput;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;
    
    // Find and create audio unit
    AudioComponent component = AudioComponentFindNext(NULL, &desc);
    if (!component) {
        qWarning() << "Failed to find audio component";
        return;
    }
    
    OSStatus status = AudioComponentInstanceNew(component, &d->audioUnit);
    if (status != noErr) {
        handleError(status, "Failed to create audio unit");
        return;
    }
}

void CoreAudioEngine::setupAudioFormat() {
    d->format.mSampleRate = 44100;
    d->format.mFormatID = kAudioFormatLinearPCM;
    d->format.mFormatFlags = kAudioFormatFlagIsFloat |
                            kAudioFormatFlagIsPacked |
                            kAudioFormatFlagIsNonInterleaved;
    d->format.mBitsPerChannel = 32;
    d->format.mChannelsPerFrame = 2;
    d->format.mFramesPerPacket = 1;
    d->format.mBytesPerFrame = 4;
    d->format.mBytesPerPacket = 4;
    
    OSStatus status = AudioUnitSetProperty(d->audioUnit,
                                         kAudioUnitProperty_StreamFormat,
                                         kAudioUnitScope_Input,
                                         0,
                                         &d->format,
                                         sizeof(d->format));
    
    if (status != noErr) {
        handleError(status, "Failed to set audio format");
    }
}

void CoreAudioEngine::configureCallbacks() {
    AURenderCallbackStruct callback;
    callback.inputProc = renderCallback;
    callback.inputProcRefCon = this;
    
    OSStatus status = AudioUnitSetProperty(d->audioUnit,
                                         kAudioUnitProperty_SetRenderCallback,
                                         kAudioUnitScope_Input,
                                         0,
                                         &callback,
                                         sizeof(callback));
    
    if (status != noErr) {
        handleError(status, "Failed to set render callback");
    }
}

OSStatus CoreAudioEngine::renderCallback(void* inRefCon,
                                       AudioUnitRenderActionFlags* ioActionFlags,
                                       const AudioTimeStamp* inTimeStamp,
                                       UInt32 inBusNumber,
                                       UInt32 inNumberFrames,
                                       AudioBufferList* ioData) {
    CoreAudioEngine* engine = static_cast<CoreAudioEngine*>(inRefCon);
    if (!engine->isRunning()) return noErr;
    
    // Process each channel
    for (UInt32 i = 0; i < ioData->mNumberBuffers; i++) {
        float* buffer = static_cast<float*>(ioData->mBuffers[i].mData);
        engine->processBuffer(buffer, inNumberFrames);
    }
    
    engine->bufferProcessed();
    return noErr;
}

void CoreAudioEngine::processBuffer(float* buffer, UInt32 frames) {
    // Apply gain
    for (UInt32 i = 0; i < frames; i++) {
        buffer[i] *= d->gain;
    }
    
    // Update CPU load
    UInt32 propertySize = sizeof(Float32);
    Float32 cpuLoad;
    OSStatus status = AudioUnitGetProperty(d->audioUnit,
                                         kAudioUnitProperty_CPULoad,
                                         kAudioUnitScope_Global,
                                         0,
                                         &cpuLoad,
                                         &propertySize);
    
    if (status == noErr && cpuLoad != d->cpuLoad) {
        d->cpuLoad = cpuLoad;
        emit cpuLoadChanged(cpuLoad);
    }
}

bool CoreAudioEngine::start() {
    if (!d->isInitialized) return false;
    
    OSStatus status = AudioOutputUnitStart(d->audioUnit);
    if (status != noErr) {
        handleError(status, "Failed to start audio unit");
        return false;
    }
    
    d->isPlaying = true;
    return true;
}

void CoreAudioEngine::stop() {
    if (d->isPlaying) {
        AudioOutputUnitStop(d->audioUnit);
        d->isPlaying = false;
    }
}

void CoreAudioEngine::pause() {
    if (d->isPlaying) {
        AudioOutputUnitStop(d->audioUnit);
        d->isPlaying = false;
    }
}

void CoreAudioEngine::resume() {
    if (!d->isPlaying && d->isInitialized) {
        OSStatus status = AudioOutputUnitStart(d->audioUnit);
        if (status == noErr) {
            d->isPlaying = true;
        }
    }
}

bool CoreAudioEngine::setFormat(int sampleRate, int channels, int bitsPerSample) {
    if (d->isPlaying) return false;
    
    d->format.mSampleRate = sampleRate;
    d->format.mChannelsPerFrame = channels;
    d->format.mBitsPerChannel = bitsPerSample;
    
    OSStatus status = AudioUnitSetProperty(d->audioUnit,
                                         kAudioUnitProperty_StreamFormat,
                                         kAudioUnitScope_Input,
                                         0,
                                         &d->format,
                                         sizeof(d->format));
    
    return status == noErr;
}

bool CoreAudioEngine::setBufferSize(UInt32 frames) {
    if (d->isPlaying) return false;
    
    AudioUnitSetProperty(d->audioUnit,
                        kAudioDevicePropertyBufferFrameSize,
                        kAudioUnitScope_Global,
                        0,
                        &frames,
                        sizeof(frames));
    
    // Update latency
    Float64 latency;
    UInt32 propertySize = sizeof(Float64);
    OSStatus status = AudioUnitGetProperty(d->audioUnit,
                                         kAudioUnitProperty_Latency,
                                         kAudioUnitScope_Global,
                                         0,
                                         &latency,
                                         &propertySize);
    
    if (status == noErr && latency != d->latency) {
        d->latency = latency;
        emit latencyChanged(latency);
    }
    
    return true;
}

void CoreAudioEngine::applyGain(float gain) {
    d->gain = std::max(0.0f, std::min(gain, 1.0f));
}

void CoreAudioEngine::applyEffect(const QString& effectName, float value) {
    // Implement effects processing here
    // This is where you would add DSP effects
}

bool CoreAudioEngine::isRunning() const {
    return d->isPlaying;
}

double CoreAudioEngine::getCurrentLatency() const {
    return d->latency;
}

float CoreAudioEngine::getCPULoad() const {
    return d->cpuLoad;
}

void CoreAudioEngine::handleError(OSStatus status, const QString& operation) {
    char errorString[5];
    *reinterpret_cast<UInt32*>(errorString) = CFSwapInt32HostToBig(status);
    errorString[4] = '\0';
    
    QString error = QString("%1: %2 (%3)").arg(operation).arg(errorString).arg(status);
    qWarning() << error;
    emit errorOccurred(error);
}

#endif // Q_OS_MACOS
