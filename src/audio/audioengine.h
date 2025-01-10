#pragma once

#include <QObject>
#include <QString>
#include <memory>

#ifdef Q_OS_MACOS
#include "avfaudioengine.h"
#include "coreaudioengine.h"
#endif

class AudioEngine : public QObject {
    Q_OBJECT

public:
    enum class Backend {
        Qt,
        AVFoundation,
        CoreAudio
    };

    explicit AudioEngine(QObject* parent = nullptr);
    ~AudioEngine();

    bool setBackend(Backend backend);
    Backend currentBackend() const;

    // Common interface
    bool play(const QString& filePath);
    void pause();
    void resume();
    void stop();
    void seek(qint64 position);
    void setVolume(float volume);
    void setEQBand(int band, float gain);
    void setPitch(float cents);

    // Performance metrics
    struct Metrics {
        double latency;      // ms
        float cpuUsage;      // percentage
        size_t bufferSize;   // frames
        int underruns;       // count
    };

    Metrics getMetrics() const;

signals:
    void playbackStarted();
    void playbackPaused();
    void playbackStopped();
    void playbackFinished();
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void errorOccurred(const QString& error);
    void metricsChanged(const Metrics& metrics);

private:
    class Private;
    std::unique_ptr<Private> d;

    void initializeBackend();
    void cleanupBackend();
    void connectBackendSignals();
};
