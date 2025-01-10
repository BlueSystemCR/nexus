#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <QString>
#include <taglib/fileref.h>

class MetadataManager {
public:
    MetadataManager();
    ~MetadataManager();
    
    bool cargarArchivo(const QString& ruta);
    QString obtenerTitulo() const;
    QString obtenerArtista() const;
    QString obtenerAlbum() const;
    QString obtenerGenero() const;
    QString obtenerFecha() const;
    QString obtenerDuracion() const;
    QString obtenerTonalidad() const;
    QString normalizarTonalidad(const QString& tonalidad) const;
    int obtenerBitrate() const;
    
private:
    QString formatearDuracion(int segundos) const;
    
    TagLib::FileRef* fileRef;
    QString titulo;
    QString artista;
    QString album;
    QString genero;
    QString fecha;
    QString duracion;
    QString tonalidad;
    int bitrate;
};

#endif // METADATAMANAGER_H
