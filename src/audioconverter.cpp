#include "audioconverter.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QThread>
#include <QTimer>

AudioConverter::AudioConverter(QObject *parent)
    : QObject(parent)
{
}

QString AudioConverter::createTempFile(const QString& suffix)
{
    QString tempPath = QDir::tempPath() + "/ReproductorMusica_" + 
                      QString::number(QDateTime::currentMSecsSinceEpoch()) +
                      suffix;
    tempFiles.append(tempPath);
    return tempPath;
}

void AudioConverter::monitorearProgreso(QProcess* proceso, qint64 tamanoEntrada)
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (proceso && proceso->state() == QProcess::Running) {
            QFile outputFile(tempFiles.last());
            if (outputFile.exists()) {
                qint64 tamanoActual = outputFile.size();
                // Estimamos el progreso basándonos en el tamaño del archivo
                // El archivo WAV suele ser más grande que el FLAC, usamos un factor de 1.5
                int progreso = (tamanoActual * 100) / (tamanoEntrada * 1.5);
                progreso = qMin(progreso, 99); // Nunca mostramos 100% hasta que termine
                emit conversionProgress(progreso);
            }
        }
    });
    timer->start(100); // Actualizar cada 100ms
    
    // Autodestruir el timer cuando el proceso termine
    connect(proceso, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            timer, &QTimer::stop);
    connect(proceso, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            timer, &QTimer::deleteLater);
}

QString AudioConverter::convertFlacToWav(const QString& flacFile)
{
    QFileInfo fileInfo(flacFile);
    if (!fileInfo.exists()) {
        emit conversionError("El archivo FLAC no existe: " + flacFile);
        return QString();
    }

    QString outputFile = createTempFile(".wav");
    QProcess* proceso = new QProcess(this);
    QStringList arguments;
    
    // Configurar argumentos de FFmpeg para convertir FLAC a WAV
    arguments << "-i" << flacFile
             << "-acodec" << "pcm_s16le"
             << "-ar" << "44100"
             << "-ac" << "2"
             << outputFile;

    qDebug() << "Convirtiendo FLAC a WAV...";
    qDebug() << "Comando: ffmpeg" << arguments.join(" ");
    
    // Iniciar monitoreo de progreso
    monitorearProgreso(proceso, fileInfo.size());
    
    proceso->start("ffmpeg", arguments);
    
    if (!proceso->waitForStarted()) {
        delete proceso;
        emit conversionError("No se pudo iniciar FFmpeg");
        return QString();
    }

    proceso->waitForFinished();
    
    if (proceso->exitCode() != 0) {
        QString error = QString::fromUtf8(proceso->readAllStandardError());
        delete proceso;
        emit conversionError("Error al convertir el archivo: " + error);
        return QString();
    }

    QFileInfo outputInfo(outputFile);
    if (!outputInfo.exists()) {
        delete proceso;
        emit conversionError("El archivo de salida no se creó correctamente");
        return QString();
    }

    delete proceso;
    emit conversionProgress(100);
    emit conversionFinished(outputFile);
    return outputFile;
}

void AudioConverter::cleanupTempFiles()
{
    for (const QString& file : tempFiles) {
        QFile::remove(file);
    }
    tempFiles.clear();
}
