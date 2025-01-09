#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <QString>
#include <QObject>

struct AudioMetadata {
    QString titulo;
    QString artista;
    QString album;
    QString comentario;
    QString duracion;
};

class MetadataManager
{
public:
    MetadataManager() = default;
    AudioMetadata obtenerMetadata(const QString& ruta);
    bool actualizarComentario(const QString& ruta, const QString& comentario);

private:
    QString formatearDuracion(qint64 duracionMs);
};

#endif // METADATAMANAGER_H
