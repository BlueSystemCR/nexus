#pragma once

#include <QObject>
#include <QString>
#include <memory>

#ifdef Q_OS_MACOS

// Declaración opaca de la implementación
class AVFAudioEngineImpl;

class AVFAudioEngine : public QObject {
    Q_OBJECT

public:
    explicit AVFAudioEngine(QObject* parent = nullptr);
    ~AVFAudioEngine();

    bool initialize();
    void cleanup();

    // Playback control
    bool play(const QString& filePath);
    void pause();
    void resume();
    void stop();
    void seek(qint64 position);

    // Audio processing
    void setVolume(float volume);
    void setEQBand(int band, float gain);
    void setPitch(float cents);
    float getCurrentTime() const;
    float getDuration() const;

signals:
    void playbackStarted();
    void playbackPaused();
    void playbackStopped();
    void playbackFinished();
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void errorOccurred(const QString& error);

private:
    friend class AVFAudioEngineImpl;
    std::unique_ptr<AVFAudioEngineImpl> d;
};

#endif // Q_OS_MACOS
