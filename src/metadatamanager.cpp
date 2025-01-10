#include "metadatamanager.h"
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <QDebug>
#include <QTime>
#include <QRegularExpression>

MetadataManager::MetadataManager() : fileRef(nullptr), titulo(""), artista(""), album(""), genero(""), fecha(""), duracion(""), tonalidad(""), bitrate(0)
{
}

MetadataManager::~MetadataManager()
{
    delete fileRef;
}

bool MetadataManager::cargarArchivo(const QString &ruta)
{
    delete fileRef;
    TagLib::FileRef f(ruta.toStdString().c_str());
    if (f.isNull() || !f.tag()) {
        return false;
    }

    fileRef = new TagLib::FileRef(ruta.toStdString().c_str());
    TagLib::Tag *tag = fileRef->tag();
    TagLib::PropertyMap properties = fileRef->file()->properties();

    // Obtener metadatos básicos
    titulo = QString::fromStdString(tag->title().to8Bit(true));
    artista = QString::fromStdString(tag->artist().to8Bit(true));
    album = QString::fromStdString(tag->album().to8Bit(true));
    genero = QString::fromStdString(tag->genre().to8Bit(true));
    fecha = QString::number(tag->year());
    
    // Obtener duración y bitrate
    if (fileRef->audioProperties()) {
        int segundos = fileRef->audioProperties()->length();
        duracion = formatearDuracion(segundos);
        bitrate = fileRef->audioProperties()->bitrate();
    }

    // Obtener tonalidad (Key)
    tonalidad = obtenerTonalidad();

    return true;
}

QString MetadataManager::obtenerTitulo() const
{
    return titulo;
}

QString MetadataManager::obtenerArtista() const
{
    return artista;
}

QString MetadataManager::obtenerAlbum() const
{
    return album;
}

QString MetadataManager::obtenerGenero() const
{
    return genero.isEmpty() ? "Sin género" : genero;
}

QString MetadataManager::obtenerFecha() const
{
    return fecha.isEmpty() ? "Sin fecha" : fecha;
}

QString MetadataManager::obtenerDuracion() const
{
    return duracion;
}

QString MetadataManager::obtenerTonalidad() const
{
    if (!fileRef) return "Sin tonalidad";
    
    TagLib::PropertyMap propiedades = fileRef->file()->properties();
    
    // Intentar diferentes etiquetas comunes para la tonalidad
    QStringList etiquetasTonalidad = {
        "INITIALKEY",
        "KEY",
        "TKEY",
        "TONALITY"
    };
    
    for (const QString& etiqueta : etiquetasTonalidad) {
        if (propiedades.contains(etiqueta.toStdString())) {
            TagLib::StringList valores = propiedades[etiqueta.toStdString()];
            if (!valores.isEmpty()) {
                return QString::fromStdString(valores.front().to8Bit(true));
            }
        }
    }
    
    return "Sin tonalidad";
}

QString MetadataManager::normalizarTonalidad(const QString& tonalidad) const
{
    // Si está vacía o es "Sin tonalidad", retornar directamente
    if (tonalidad.isEmpty() || tonalidad == "Sin tonalidad") {
        return "Sin tonalidad";
    }

    // Convertir a mayúsculas y eliminar espacios
    QString normalizada = tonalidad.toUpper().trimmed();
    
    // Lista de tonalidades válidas
    QStringList tonalidades = {
        "A", "Am", "Bb", "Bbm", "B", "Bm",
        "C", "Cm", "C#", "C#m", "D", "Dm",
        "Eb", "Ebm", "E", "Em", "F", "Fm",
        "F#", "F#m", "G", "Gm", "Ab", "Abm"
    };
    
    // Si la tonalidad ya está en formato válido, retornarla
    if (tonalidades.contains(normalizada)) {
        return normalizada;
    }
    
    // Intentar interpretar la tonalidad
    // Reemplazar notación alternativa
    normalizada.replace("MIN", "m");
    normalizada.replace("MAJ", "");
    normalizada.replace("MINOR", "m");
    normalizada.replace("MAJOR", "");
    
    // Si después de la normalización es válida, retornarla
    if (tonalidades.contains(normalizada)) {
        return normalizada;
    }
    
    return "Sin tonalidad";
}

int MetadataManager::obtenerBitrate() const
{
    return bitrate;
}

QString MetadataManager::formatearDuracion(int segundos) const
{
    if (segundos <= 0) return "0:00";
    
    int horas = segundos / 3600;
    int minutos = (segundos % 3600) / 60;
    int segs = segundos % 60;
    
    if (horas > 0) {
        return QString("%1:%2:%3")
            .arg(horas)
            .arg(minutos, 2, 10, QChar('0'))
            .arg(segs, 2, 10, QChar('0'));
    } else {
        return QString("%1:%2")
            .arg(minutos)
            .arg(segs, 2, 10, QChar('0'));
    }
}
