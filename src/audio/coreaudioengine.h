#pragma once

#include <QObject>
#include <QString>
#include <memory>

#ifdef Q_OS_MACOS
#include <AudioToolbox/AudioToolbox.h>
#include <CoreAudio/CoreAudio.h>

class CoreAudioEngine : public QObject {
    Q_OBJECT

public:
    explicit CoreAudioEngine(QObject* parent = nullptr);
    ~CoreAudioEngine();

    bool initialize();
    void cleanup();

    // Audio setup
    bool setFormat(int sampleRate, int channels, int bitsPerSample);
    bool setBufferSize(UInt32 frames);

    // Playback control
    bool start();
    void stop();
    void pause();
    void resume();

    // Audio processing
    void processBuffer(float* buffer, UInt32 frames);
    void applyGain(float gain);
    void applyEffect(const QString& effectName, float value);

    // State and metrics
    bool isRunning() const;
    double getCurrentLatency() const;
    float getCPULoad() const;

signals:
    void bufferProcessed();
    void errorOccurred(const QString& error);
    void latencyChanged(double latency);
    void cpuLoadChanged(float load);

private:
    class Private;
    std::unique_ptr<Private> d;

    static OSStatus renderCallback(void* inRefCon,
                                 AudioUnitRenderActionFlags* ioActionFlags,
                                 const AudioTimeStamp* inTimeStamp,
                                 UInt32 inBusNumber,
                                 UInt32 inNumberFrames,
                                 AudioBufferList* ioData);

    void setupAudioUnit();
    void setupAudioFormat();
    void configureCallbacks();
    void handleError(OSStatus status, const QString& operation);
};

#endif // Q_OS_MACOS
