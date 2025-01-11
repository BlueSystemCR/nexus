#include "gstreamerplayer.h"
#include <QDebug>
#include <QFile>

GStreamerPlayer::GStreamerPlayer(QObject *parent)
    : QObject(parent)
    , pipeline(nullptr)
    , source(nullptr)
    , decoder(nullptr)
    , audioConvert(nullptr)
    , audioSink(nullptr)
{
    gst_init(nullptr, nullptr);
    setupPipeline();
}

GStreamerPlayer::~GStreamerPlayer()
{
    cleanup();
}

void GStreamerPlayer::setupPipeline()
{
    pipeline = gst_pipeline_new("audio-player");
    if (!pipeline) {
        emit error("No se pudo crear el pipeline");
        return;
    }

    // Configurar bus para mensajes
    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, (GstBusFunc)onBusMessage, this);
    gst_object_unref(bus);
}

bool GStreamerPlayer::play(const QString &filePath)
{
    if (!QFile::exists(filePath)) {
        emit error("El archivo no existe");
        return false;
    }

    cleanup();
    setupPipeline();

    // Crear elementos
    source = gst_element_factory_make("filesrc", "file-source");
    decoder = gst_element_factory_make("decodebin", "decoder");
    audioConvert = gst_element_factory_make("audioconvert", "converter");
    audioSink = gst_element_factory_make("autoaudiosink", "audio-output");

    if (!source || !decoder || !audioConvert || !audioSink) {
        emit error("No se pudieron crear todos los elementos necesarios");
        cleanup();
        return false;
    }

    // Configurar source
    g_object_set(G_OBJECT(source), "location", filePath.toUtf8().constData(), NULL);

    // Agregar elementos al pipeline
    gst_bin_add_many(GST_BIN(pipeline), source, decoder, audioConvert, audioSink, NULL);

    // Enlazar elementos
    if (!gst_element_link(source, decoder)) {
        emit error("No se pudo enlazar source con decoder");
        cleanup();
        return false;
    }

    if (!gst_element_link_many(audioConvert, audioSink, NULL)) {
        emit error("No se pudo enlazar audioConvert con audioSink");
        cleanup();
        return false;
    }

    // Iniciar reproducción
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        emit error("Error al iniciar la reproducción");
        cleanup();
        return false;
    }

    updateState();
    return true;
}

void GStreamerPlayer::pause()
{
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
        updateState();
    }
}

void GStreamerPlayer::stop()
{
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        updateState();
    }
}

void GStreamerPlayer::setVolume(double volume)
{
    if (pipeline) {
        // Buscar el elemento de volumen o crear uno si no existe
        GstElement *volume_element = gst_bin_get_by_name(GST_BIN(pipeline), "volume");
        if (!volume_element) {
            volume_element = gst_element_factory_make("volume", "volume");
            if (volume_element) {
                gst_bin_add(GST_BIN(pipeline), volume_element);
                // Enlazar el elemento de volumen en la cadena
                if (audioConvert && audioSink) {
                    gst_element_unlink(audioConvert, audioSink);
                    gst_element_link_many(audioConvert, volume_element, audioSink, NULL);
                }
            }
        }
        if (volume_element) {
            g_object_set(G_OBJECT(volume_element), "volume", volume, NULL);
            gst_object_unref(volume_element);
        }
    }
}

void GStreamerPlayer::setPosition(qint64 position)
{
    if (pipeline) {
        gst_element_seek_simple(pipeline,
                              GST_FORMAT_TIME,
                              GST_SEEK_FLAG_FLUSH,
                              position * GST_MSECOND);
    }
}

QString GStreamerPlayer::state() const
{
    if (!pipeline) return "stopped";

    GstState state;
    gst_element_get_state(pipeline, &state, nullptr, GST_CLOCK_TIME_NONE);

    switch (state) {
        case GST_STATE_PLAYING:
            return "playing";
        case GST_STATE_PAUSED:
            return "paused";
        default:
            return "stopped";
    }
}

qint64 GStreamerPlayer::position() const
{
    if (!pipeline) return 0;

    gint64 pos;
    if (gst_element_query_position(pipeline, GST_FORMAT_TIME, &pos)) {
        return pos / GST_MSECOND;
    }
    return 0;
}

qint64 GStreamerPlayer::duration() const
{
    if (!pipeline) return 0;

    gint64 dur;
    if (gst_element_query_duration(pipeline, GST_FORMAT_TIME, &dur)) {
        return dur / GST_MSECOND;
    }
    return 0;
}

void GStreamerPlayer::cleanup()
{
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = nullptr;
    }
    source = nullptr;
    decoder = nullptr;
    audioConvert = nullptr;
    audioSink = nullptr;
}

void GStreamerPlayer::updateState()
{
    emit stateChanged(state());
    emit positionChanged(position());
    emit durationChanged(duration());
}

void GStreamerPlayer::onBusMessage(GstBus *bus, GstMessage *msg, GStreamerPlayer *player)
{
    Q_UNUSED(bus);

    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_ERROR: {
            GError *err;
            gchar *debug_info;
            gst_message_parse_error(msg, &err, &debug_info);
            QString errorString = QString("Error: %1\nDebug info: %2")
                                    .arg(err->message)
                                    .arg(debug_info);
            g_error_free(err);
            g_free(debug_info);
            qDebug() << errorString;
            emit player->error(errorString);
            break;
        }
        case GST_MESSAGE_EOS:
            player->stop();
            break;
        case GST_MESSAGE_STATE_CHANGED: {
            if (GST_MESSAGE_SRC(msg) == GST_OBJECT(player->pipeline)) {
                player->updateState();
            }
            break;
        }
        default:
            break;
    }
}
