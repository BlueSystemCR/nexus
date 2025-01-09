#include "audioengine.h"
#include <QDebug>

class AudioEngine::Private {
public:
    Backend backend;
    Metrics metrics;
    
#ifdef Q_OS_MACOS
    std::unique_ptr<AVFAudioEngine> avfEngine;
    std::unique_ptr<CoreAudioEngine> coreAudioEngine;
#endif
    
    Private() : backend(Backend::Qt) {}
};

AudioEngine::AudioEngine(QObject* parent)
    : QObject(parent), d(new Private) {
    initializeBackend();
}

AudioEngine::~AudioEngine() {
    cleanupBackend();
}

bool AudioEngine::setBackend(Backend backend) {
    if (d->backend == backend) return true;
    
    cleanupBackend();
    d->backend = backend;
    initializeBackend();
    return true;
}

AudioEngine::Backend AudioEngine::currentBackend() const {
    return d->backend;
}

void AudioEngine::initializeBackend() {
#ifdef Q_OS_MACOS
    switch (d->backend) {
        case Backend::AVFoundation:
            d->avfEngine = std::make_unique<AVFAudioEngine>();
            connectBackendSignals();
            break;
            
        case Backend::CoreAudio:
            d->coreAudioEngine = std::make_unique<CoreAudioEngine>();
            connectBackendSignals();
            break;
            
        default:
            qDebug() << "Using Qt backend";
            break;
    }
#endif
}

void AudioEngine::cleanupBackend() {
#ifdef Q_OS_MACOS
    d->avfEngine.reset();
    d->coreAudioEngine.reset();
#endif
}

void AudioEngine::connectBackendSignals() {
#ifdef Q_OS_MACOS
    if (d->avfEngine) {
        connect(d->avfEngine.get(), &AVFAudioEngine::playbackStarted,
                this, &AudioEngine::playbackStarted);
        connect(d->avfEngine.get(), &AVFAudioEngine::playbackPaused,
                this, &AudioEngine::playbackPaused);
        connect(d->avfEngine.get(), &AVFAudioEngine::playbackStopped,
                this, &AudioEngine::playbackStopped);
        connect(d->avfEngine.get(), &AVFAudioEngine::playbackFinished,
                this, &AudioEngine::playbackFinished);
        connect(d->avfEngine.get(), &AVFAudioEngine::positionChanged,
                this, &AudioEngine::positionChanged);
        connect(d->avfEngine.get(), &AVFAudioEngine::durationChanged,
                this, &AudioEngine::durationChanged);
        connect(d->avfEngine.get(), &AVFAudioEngine::errorOccurred,
                this, &AudioEngine::errorOccurred);
    }
    
    if (d->coreAudioEngine) {
        connect(d->coreAudioEngine.get(), &CoreAudioEngine::errorOccurred,
                this, &AudioEngine::errorOccurred);
        connect(d->coreAudioEngine.get(), &CoreAudioEngine::latencyChanged,
                this, [this](double latency) {
                    d->metrics.latency = latency;
                    emit metricsChanged(d->metrics);
                });
        connect(d->coreAudioEngine.get(), &CoreAudioEngine::cpuLoadChanged,
                this, [this](float load) {
                    d->metrics.cpuUsage = load;
                    emit metricsChanged(d->metrics);
                });
    }
#endif
}

bool AudioEngine::play(const QString& filePath) {
#ifdef Q_OS_MACOS
    switch (d->backend) {
        case Backend::AVFoundation:
            return d->avfEngine && d->avfEngine->play(filePath);
            
        case Backend::CoreAudio:
            return d->coreAudioEngine && d->coreAudioEngine->start();
            
        default:
            break;
    }
#endif
    return false;
}

void AudioEngine::pause() {
#ifdef Q_OS_MACOS
    switch (d->backend) {
        case Backend::AVFoundation:
            if (d->avfEngine) d->avfEngine->pause();
            break;
            
        case Backend::CoreAudio:
            if (d->coreAudioEngine) d->coreAudioEngine->pause();
            break;
            
        default:
            break;
    }
#endif
}

void AudioEngine::resume() {
#ifdef Q_OS_MACOS
    switch (d->backend) {
        case Backend::AVFoundation:
            if (d->avfEngine) d->avfEngine->resume();
            break;
            
        case Backend::CoreAudio:
            if (d->coreAudioEngine) d->coreAudioEngine->resume();
            break;
            
        default:
            break;
    }
#endif
}

void AudioEngine::stop() {
#ifdef Q_OS_MACOS
    switch (d->backend) {
        case Backend::AVFoundation:
            if (d->avfEngine) d->avfEngine->stop();
            break;
            
        case Backend::CoreAudio:
            if (d->coreAudioEngine) d->coreAudioEngine->stop();
            break;
            
        default:
            break;
    }
#endif
}

void AudioEngine::seek(qint64 position) {
#ifdef Q_OS_MACOS
    if (d->backend == Backend::AVFoundation && d->avfEngine) {
        d->avfEngine->seek(position);
    }
#endif
}

void AudioEngine::setVolume(float volume) {
#ifdef Q_OS_MACOS
    switch (d->backend) {
        case Backend::AVFoundation:
            if (d->avfEngine) d->avfEngine->setVolume(volume);
            break;
            
        case Backend::CoreAudio:
            if (d->coreAudioEngine) d->coreAudioEngine->applyGain(volume);
            break;
            
        default:
            break;
    }
#endif
}

void AudioEngine::setEQBand(int band, float gain) {
#ifdef Q_OS_MACOS
    if (d->backend == Backend::AVFoundation && d->avfEngine) {
        d->avfEngine->setEQBand(band, gain);
    }
#endif
}

void AudioEngine::setPitch(float cents) {
#ifdef Q_OS_MACOS
    if (d->backend == Backend::AVFoundation && d->avfEngine) {
        d->avfEngine->setPitch(cents);
    }
#endif
}

AudioEngine::Metrics AudioEngine::getMetrics() const {
    return d->metrics;
}
