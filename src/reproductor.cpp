#include "reproductor.h"
#include <QUrl>
#include <QFileInfo>
#include <QStringList>
#include <stdexcept>

Reproductor::Reproductor(QObject *parent)
    : QObject(parent)
    , reproductor(nullptr)
    , salidaAudio(nullptr)
{
    try {
        reproductor = new QMediaPlayer(this);
        salidaAudio = new QAudioOutput(this);
        
        if (!reproductor || !salidaAudio) {
            throw std::runtime_error("Error al inicializar componentes de audio");
        }
        
        reproductor->setAudioOutput(salidaAudio);
        
        // Conectar señales de error
        connect(reproductor, &QMediaPlayer::errorOccurred,
                this, &Reproductor::manejarError);
    } catch (const std::exception& e) {
        qCritical() << "Error en constructor de Reproductor:" << e.what();
        throw;
    }
}

Reproductor::~Reproductor()
{
    if (reproductor) {
        delete reproductor;
    }
    if (salidaAudio) {
        delete salidaAudio;
    }
}

void Reproductor::cargarArchivo(const QString &ruta)
{
    if (!reproductor || !salidaAudio) {
        qCritical() << "Componentes de audio no inicializados";
        return;
    }
    
    QFileInfo archivo(ruta);
    if (!archivo.exists()) {
        qWarning() << "El archivo no existe:" << ruta;
        emit errorReproductor("El archivo no existe: " + ruta);
        return;
    }
    
    if (!archivo.isReadable()) {
        qWarning() << "El archivo no tiene permisos de lectura:" << ruta;
        emit errorReproductor("Sin permisos de lectura para: " + ruta);
        return;
    }
    
    // Lista de formatos soportados
    QStringList formatosSoportados = {"mp3", "wav", "ogg", "flac", "m4a", "aac"};
    if (!formatosSoportados.contains(archivo.suffix().toLower())) {
        qWarning() << "Formato de archivo no soportado:" << ruta;
        emit errorReproductor("Formato no soportado: " + archivo.suffix());
        return;
    }
    
    reproductor->setSource(QUrl::fromLocalFile(ruta));
}

void Reproductor::reproducir()
{
    if (!reproductor || !salidaAudio) {
        qCritical() << "Componentes de audio no inicializados";
        return;
    }
    
    if (reproductor->source().isEmpty()) {
        qWarning() << "No hay archivo cargado para reproducir";
        emit errorReproductor("No hay archivo cargado para reproducir");
        return;
    }
    
    reproductor->play();
}

void Reproductor::pausar()
{
    if (!reproductor || !salidaAudio) {
        qCritical() << "Componentes de audio no inicializados";
        return;
    }
    
    reproductor->pause();
}

void Reproductor::manejarError(QMediaPlayer::Error error)
{
    QString mensaje;
    switch (error) {
        case QMediaPlayer::NoError:
            return;
        case QMediaPlayer::ResourceError:
            mensaje = "Error al acceder al archivo";
            break;
        case QMediaPlayer::FormatError:
            mensaje = "Formato de archivo no soportado";
            break;
        case QMediaPlayer::NetworkError:
            mensaje = "Error de red";
            break;
        case QMediaPlayer::AccessDeniedError:
            mensaje = "Acceso denegado al archivo";
            break;
        default:
            mensaje = "Error desconocido";
            break;
    }
    
    qCritical() << "Error del reproductor:" << mensaje;
    emit errorReproductor(mensaje);
}
