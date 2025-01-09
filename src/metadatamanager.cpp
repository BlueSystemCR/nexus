#include "metadatamanager.h"
#include <QFileInfo>
#include <QDebug>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <QTime>
#include <QMediaPlayer>
#include <QAudioOutput>

AudioMetadata MetadataManager::obtenerMetadata(const QString& ruta) {
    AudioMetadata metadata;
    QFileInfo fileInfo(ruta);
    
    // Usar TagLib para obtener los metadatos
    TagLib::FileRef f(ruta.toStdString().c_str());
    
    if (!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();
        
        // Obtener metadatos básicos
        QString titulo = QString::fromStdString(tag->title().toCString(true));
        metadata.titulo = titulo.isEmpty() ? fileInfo.baseName() : titulo;
        
        QString artista = QString::fromStdString(tag->artist().toCString(true));
        metadata.artista = artista.isEmpty() ? "Desconocido" : artista;
        
        QString album = QString::fromStdString(tag->album().toCString(true));
        metadata.album = album.isEmpty() ? "Desconocido" : album;
        
        QString comentario = QString::fromStdString(tag->comment().toCString(true));
        metadata.comentario = comentario;
        
        // Obtener duración
        if (f.audioProperties()) {
            metadata.duracion = formatearDuracion(f.audioProperties()->lengthInMilliseconds());
        }
    } else {
        // Si no se pueden leer los metadatos, usar información básica del archivo
        metadata.titulo = fileInfo.baseName();
        metadata.artista = "Desconocido";
        metadata.album = "Desconocido";
        metadata.comentario = "";
        metadata.duracion = "00:00";
    }
    
    return metadata;
}

bool MetadataManager::actualizarComentario(const QString& ruta, const QString& comentario) {
    TagLib::FileRef f(ruta.toStdString().c_str());
    
    if (!f.isNull() && f.tag()) {
        f.tag()->setComment(comentario.toStdString());
        return f.save();
    }
    
    return false;
}

QString MetadataManager::formatearDuracion(qint64 duracionMs) {
    int segundos = duracionMs / 1000;
    int minutos = segundos / 60;
    segundos = segundos % 60;
    
    return QString("%1:%2")
        .arg(minutos)
        .arg(segundos, 2, 10, QChar('0'));
}
