#ifndef GSTREAMERPLAYER_H
#define GSTREAMERPLAYER_H

#include <QObject>
#include <QString>
#include <gst/gst.h>

class GStreamerPlayer : public QObject
{
    Q_OBJECT

public:
    explicit GStreamerPlayer(QObject *parent = nullptr);
    ~GStreamerPlayer();

    bool play(const QString &filePath);
    void pause();
    void stop();
    void setVolume(double volume); // 0.0 to 1.0
    void setPosition(qint64 position);
    QString state() const;
    qint64 position() const;
    qint64 duration() const;

signals:
    void error(const QString &message);
    void stateChanged(const QString &state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);

private:
    GstElement *pipeline;
    GstElement *source;
    GstElement *decoder;
    GstElement *audioConvert;
    GstElement *audioSink;
    
    void cleanup();
    static void onBusMessage(GstBus *bus, GstMessage *msg, GStreamerPlayer *player);
    void updateState();
    void setupPipeline();
};

#endif // GSTREAMERPLAYER_H
