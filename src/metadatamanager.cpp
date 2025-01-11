#include "metadatamanager.h"
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <taglib/mp4properties.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/flacfile.h>
#include <taglib/wavfile.h>
#include <taglib/aifffile.h>
#include <QDebug>
#include <QTime>
#include <QFileInfo>
#include <QRegularExpression>

MetadataManager::MetadataManager() : fileRef(nullptr)
{
    limpiarMetadatos();
}

MetadataManager::~MetadataManager()
{
    delete fileRef;
}

bool MetadataManager::cargarArchivo(const QString& ruta)
{
    delete fileRef;
    limpiarMetadatos();
    
    QFileInfo fileInfo(ruta);
    QString extension = fileInfo.suffix().toLower();
    
    qDebug() << "DEBUG: Cargando archivo:" << ruta;
    qDebug() << "DEBUG: Extensión detectada:" << extension;
    
    // Manejo específico según el formato
    if (extension == "mp3") {
        TagLib::MPEG::File* mpegFile = new TagLib::MPEG::File(ruta.toStdString().c_str());
        if (!mpegFile->isValid()) {
            delete mpegFile;
            return false;
        }
        
        qDebug() << "DEBUG: Archivo MP3 detectado";
        if (mpegFile->ID3v2Tag()) {
            qDebug() << "DEBUG: Usando etiquetas ID3v2";
            extraerMetadatosID3v2(mpegFile->ID3v2Tag());
        } else if (mpegFile->ID3v1Tag()) {
            qDebug() << "DEBUG: Usando etiquetas ID3v1";
            extraerMetadatosID3v1(mpegFile->ID3v1Tag());
        }
        
        fileRef = new TagLib::FileRef(mpegFile);
    }
    else if (extension == "flac") {
        TagLib::FLAC::File* flacFile = new TagLib::FLAC::File(ruta.toStdString().c_str());
        if (!flacFile->isValid()) {
            delete flacFile;
            return false;
        }
        
        qDebug() << "DEBUG: Archivo FLAC detectado";
        if (flacFile->xiphComment()) {
            qDebug() << "DEBUG: Usando comentarios Xiph";
            extraerMetadatosXiph(flacFile->xiphComment());
        }
        
        fileRef = new TagLib::FileRef(flacFile);
    }
    else if (extension == "wav") {
        TagLib::RIFF::WAV::File* wavFile = new TagLib::RIFF::WAV::File(ruta.toStdString().c_str());
        if (!wavFile->isValid()) {
            delete wavFile;
            return false;
        }
        
        qDebug() << "DEBUG: Archivo WAV detectado";
        if (wavFile->ID3v2Tag()) {
            qDebug() << "DEBUG: Usando etiquetas ID3v2";
            extraerMetadatosID3v2(wavFile->ID3v2Tag());
        }
        
        fileRef = new TagLib::FileRef(wavFile);
    }
    else if (extension == "aiff") {
        TagLib::RIFF::AIFF::File* aiffFile = new TagLib::RIFF::AIFF::File(ruta.toStdString().c_str());
        if (!aiffFile->isValid()) {
            delete aiffFile;
            return false;
        }
        
        qDebug() << "DEBUG: Archivo AIFF detectado";
        if (aiffFile->tag()) {
            qDebug() << "DEBUG: Usando etiquetas ID3";
            extraerMetadatosID3v2(aiffFile->tag());
        }
        
        fileRef = new TagLib::FileRef(aiffFile);
    }
    else {
        // Para otros formatos, usar FileRef genérico
        fileRef = new TagLib::FileRef(ruta.toStdString().c_str());
        if (fileRef->isNull() || !fileRef->tag()) {
            delete fileRef;
            fileRef = nullptr;
            return false;
        }
    }
    
    // Extraer propiedades de audio comunes
    if (fileRef && fileRef->audioProperties()) {
        int segundos = fileRef->audioProperties()->length();
        duracion = formatearDuracion(segundos);
        bitrate = fileRef->audioProperties()->bitrate();
        qDebug() << "DEBUG: Duración:" << duracion;
        qDebug() << "DEBUG: Bitrate:" << bitrate;
    }
    
    return true;
}

void MetadataManager::extraerMetadatosID3v2(TagLib::ID3v2::Tag* tag)
{
    if (!tag) return;
    
    // Extraer metadatos básicos
    titulo = QString::fromStdString(tag->title().to8Bit(true));
    artista = QString::fromStdString(tag->artist().to8Bit(true));
    album = QString::fromStdString(tag->album().to8Bit(true));
    genero = QString::fromStdString(tag->genre().to8Bit(true));
    fecha = QString::number(tag->year());
    
    // Buscar INITIALKEY en frames ID3v2
    auto frameList = tag->frameList("TKEY");
    if (!frameList.isEmpty()) {
        tonalidad = QString::fromStdString(frameList.front()->toString().to8Bit(true));
        qDebug() << "DEBUG: Tonalidad encontrada en ID3v2:" << tonalidad;
    }
}

void MetadataManager::extraerMetadatosID3v1(TagLib::ID3v1::Tag* tag)
{
    if (!tag) return;
    
    titulo = QString::fromStdString(tag->title().to8Bit(true));
    artista = QString::fromStdString(tag->artist().to8Bit(true));
    album = QString::fromStdString(tag->album().to8Bit(true));
    genero = QString::fromStdString(tag->genre().to8Bit(true));
    fecha = QString::number(tag->year());
}

void MetadataManager::extraerMetadatosXiph(TagLib::Ogg::XiphComment* tag)
{
    if (!tag) return;
    
    titulo = QString::fromStdString(tag->title().to8Bit(true));
    artista = QString::fromStdString(tag->artist().to8Bit(true));
    album = QString::fromStdString(tag->album().to8Bit(true));
    genero = QString::fromStdString(tag->genre().to8Bit(true));
    fecha = QString::number(tag->year());
    
    // Buscar INITIALKEY en campos Xiph
    auto fieldList = tag->fieldListMap()["INITIALKEY"];
    if (!fieldList.isEmpty()) {
        tonalidad = QString::fromStdString(fieldList.front().to8Bit(true));
        qDebug() << "DEBUG: Tonalidad encontrada en XiphComment:" << tonalidad;
    }
}

void MetadataManager::limpiarMetadatos()
{
    titulo = "";
    artista = "";
    album = "";
    genero = "";
    fecha = "";
    duracion = "";
    tonalidad = "";
    bitrate = 0;
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
    if (!fileRef) {
        qDebug() << "DEBUG: No hay archivo cargado";
        return "Sin tonalidad";
    }
    
    // Intentar convertir a MP4File si el archivo es MP4
    TagLib::MP4::File* mp4File = dynamic_cast<TagLib::MP4::File*>(fileRef->file());
    if (mp4File && mp4File->tag()) {
        qDebug() << "DEBUG: Archivo detectado como MP4";
        
        // Para MP4, usar PropertyMap que es más estable entre versiones
        TagLib::PropertyMap propiedades = mp4File->properties();
        QStringList mp4Keys = {"KEY", "INITIALKEY"};
        
        for (const QString& key : mp4Keys) {
            qDebug() << "  Buscando etiqueta MP4:" << key;
            if (propiedades.contains(key.toStdString())) {
                TagLib::StringList valores = propiedades[key.toStdString()];
                if (!valores.isEmpty()) {
                    QString valor = QString::fromStdString(valores.front().to8Bit(true));
                    qDebug() << "    Encontrado valor:" << valor;
                    if (!valor.isEmpty()) {
                        QString normalizada = normalizarTonalidad(valor);
                        qDebug() << "    Normalizada a:" << normalizada;
                        return normalizada;
                    }
                }
            }
        }
    }
    
    // Si no es MP4 o no se encontró en formato MP4, buscar en formato general
    TagLib::PropertyMap propiedades = fileRef->file()->properties();
    qDebug() << "\nDEBUG: Buscando INITIALKEY en formato general:";
    
    // Buscar específicamente INITIALKEY
    if (propiedades.contains("INITIALKEY")) {
        TagLib::StringList valores = propiedades["INITIALKEY"];
        if (!valores.isEmpty()) {
            QString tonalidad = QString::fromStdString(valores.front().to8Bit(true));
            qDebug() << "  INITIALKEY encontrado:" << tonalidad;
            if (!tonalidad.isEmpty()) {
                QString normalizada = normalizarTonalidad(tonalidad);
                qDebug() << "  Tonalidad normalizada:" << normalizada;
                return normalizada;
            }
        }
    }
    
    // Si no se encuentra INITIALKEY, buscar en KEY como respaldo
    if (propiedades.contains("KEY")) {
        TagLib::StringList valores = propiedades["KEY"];
        if (!valores.isEmpty()) {
            QString tonalidad = QString::fromStdString(valores.front().to8Bit(true));
            qDebug() << "  KEY encontrado como respaldo:" << tonalidad;
            if (!tonalidad.isEmpty()) {
                QString normalizada = normalizarTonalidad(tonalidad);
                qDebug() << "  Tonalidad normalizada:" << normalizada;
                return normalizada;
            }
        }
    }
    
    qDebug() << "DEBUG: No se encontró ninguna etiqueta de tonalidad";
    return "Sin tonalidad";
}

QString MetadataManager::normalizarTonalidad(const QString& tonalidad) const
{
    qDebug() << "\nDEBUG: Normalizando tonalidad:" << tonalidad;
    
    // Si está vacía o es "Sin tonalidad", retornar directamente
    if (tonalidad.isEmpty() || tonalidad == "Sin tonalidad") {
        qDebug() << "  Tonalidad vacía o 'Sin tonalidad'";
        return "Sin tonalidad";
    }

    // Convertir a mayúsculas y eliminar espacios
    QString normalizada = tonalidad.toUpper().trimmed();
    qDebug() << "  Después de mayúsculas y trim:" << normalizada;
    
    // Mapa de conversión de notación Camelot a tonalidad musical
    QMap<QString, QString> camelotAMusical;
    camelotAMusical["1A"] = "Abm";  camelotAMusical["1B"] = "B";
    camelotAMusical["2A"] = "Ebm";  camelotAMusical["2B"] = "F#";
    camelotAMusical["3A"] = "Bbm";  camelotAMusical["3B"] = "Db";
    camelotAMusical["4A"] = "Fm";   camelotAMusical["4B"] = "Ab";
    camelotAMusical["5A"] = "Cm";   camelotAMusical["5B"] = "Eb";
    camelotAMusical["6A"] = "Gm";   camelotAMusical["6B"] = "Bb";
    camelotAMusical["7A"] = "Dm";   camelotAMusical["7B"] = "F";
    camelotAMusical["8A"] = "Am";   camelotAMusical["8B"] = "C";
    camelotAMusical["9A"] = "Em";   camelotAMusical["9B"] = "G";
    camelotAMusical["10A"] = "Bm";  camelotAMusical["10B"] = "D";
    camelotAMusical["11A"] = "F#m"; camelotAMusical["11B"] = "A";
    camelotAMusical["12A"] = "C#m"; camelotAMusical["12B"] = "E";
    
    // Verificar si es notación Camelot
    if (camelotAMusical.contains(normalizada)) {
        QString resultado = camelotAMusical[normalizada];
        qDebug() << "  Encontrada en notación Camelot:" << normalizada << "->" << resultado;
        return resultado;
    }
    
    // Lista de tonalidades válidas
    QStringList tonalidades = {
        "A", "Am", "Bb", "Bbm", "B", "Bm",
        "C", "Cm", "C#", "C#m", "D", "Dm",
        "Eb", "Ebm", "E", "Em", "F", "Fm",
        "F#", "F#m", "G", "Gm", "Ab", "Abm"
    };
    
    qDebug() << "  Aplicando reemplazos de notación alternativa";
    // Reemplazar notación alternativa
    normalizada.replace("MIN", "m");
    normalizada.replace("MAJ", "");
    normalizada.replace("MINOR", "m");
    normalizada.replace("MAJOR", "");
    normalizada.replace("M", "");  // Solo si no es parte de "m" minúscula
    
    qDebug() << "  Después de reemplazos:" << normalizada;
    
    // Manejar casos especiales
    if (normalizada == "DB") {
        normalizada = "C#";
        qDebug() << "  Convertido DB a C#";
    }
    if (normalizada == "DBM") {
        normalizada = "C#m";
        qDebug() << "  Convertido DBM a C#m";
    }
    if (normalizada == "GB") {
        normalizada = "F#";
        qDebug() << "  Convertido GB a F#";
    }
    if (normalizada == "GBM") {
        normalizada = "F#m";
        qDebug() << "  Convertido GBM a F#m";
    }
    
    // Si después de la normalización es válida, retornarla
    for (const QString& t : tonalidades) {
        if (t.toUpper() == normalizada || t == normalizada) {
            qDebug() << "  Tonalidad válida encontrada:" << t;
            return t;
        }
    }
    
    qDebug() << "  No se pudo normalizar la tonalidad";
    return "Sin tonalidad";
}

int MetadataManager::obtenerBitrate() const
{
    return bitrate;
}

QString MetadataManager::formatearDuracion(int segundos)
{
    int minutos = segundos / 60;
    segundos = segundos % 60;
    return QString("%1:%2").arg(minutos, 2, 10, QChar('0')).arg(segundos, 2, 10, QChar('0'));
}

const QStringList MetadataManager::FORMATOS_SOPORTADOS = {
    // Formatos de audio sin pérdida
    "flac",  // Free Lossless Audio Codec
    "wav",   // Waveform Audio File Format
    "aiff",  // Audio Interchange File Format
    "ape",   // Monkey's Audio
    "wv",    // WavPack
    
    // Formatos de audio con pérdida
    "mp3",   // MPEG Audio Layer III
    "m4a",   // MPEG-4 Audio
    "mp4",   // MPEG-4 Audio/Video
    "ogg",   // Ogg Vorbis
    "oga",   // Ogg Audio
    "opus",  // Opus Audio
    "wma",   // Windows Media Audio
    "aac",   // Advanced Audio Coding
    
    // Formatos contenedor
    "mpc",   // Musepack
    "asf",   // Advanced Systems Format
    "3gp",   // 3GPP Audio/Video
    "m4v",   // MPEG-4 Video
    "mov"    // QuickTime Movie
};

QStringList MetadataManager::obtenerFormatosSoportados()
{
    return FORMATOS_SOPORTADOS;
}

bool MetadataManager::esFormatoSoportado(const QString& extension)
{
    QString ext = extension.toLower();
    if (ext.startsWith(".")) {
        ext = ext.mid(1);
    }
    return FORMATOS_SOPORTADOS.contains(ext);
}
