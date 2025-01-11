#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <QString>
#include <QStringList>
#include <QMap>

// Includes de TagLib
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v2tag.h>
#include <taglib/xiphcomment.h>
#include <taglib/id3v1tag.h>

using namespace TagLib;

class MetadataManager {
public:
    MetadataManager();
    ~MetadataManager();
    
    bool cargarArchivo(const QString& ruta);
    
    // Getters
    QString obtenerTitulo() const;
    QString obtenerArtista() const;
    QString obtenerAlbum() const;
    QString obtenerGenero() const;
    QString obtenerFecha() const;
    QString obtenerDuracion() const;
    QString obtenerTonalidad() const;
    int obtenerBitrate() const;
    
    // Métodos de utilidad
    QString normalizarTonalidad(const QString& tonalidad) const;
    static QString formatearDuracion(int segundos);
    static QStringList obtenerFormatosSoportados();
    static bool esFormatoSoportado(const QString& extension);
    
private:
    // Métodos de extracción de metadatos específicos por formato
    void extraerMetadatosID3v2(TagLib::ID3v2::Tag* tag);
    void extraerMetadatosID3v1(TagLib::ID3v1::Tag* tag);
    void extraerMetadatosXiph(TagLib::Ogg::XiphComment* tag);
    void limpiarMetadatos();
    
    // Variables miembro
    FileRef* fileRef;
    QString titulo;
    QString artista;
    QString album;
    QString genero;
    QString fecha;
    QString duracion;
    QString tonalidad;
    int bitrate;
    
    // Lista estática de formatos soportados
    static const QStringList FORMATOS_SOPORTADOS;
};

#endif // METADATAMANAGER_H
