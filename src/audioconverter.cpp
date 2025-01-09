#include "audioconverter.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>

AudioConverter::AudioConverter(QObject *parent)
    : QObject(parent)
    , ffmpegProcess(nullptr)
{
    qDebug() << "AudioConverter inicializado";
}

AudioConverter::~AudioConverter()
{
    cleanupTempFiles();
    if (ffmpegProcess) {
        ffmpegProcess->kill();
        delete ffmpegProcess;
    }
}

QString AudioConverter::convertFlacToWav(const QString& inputFile)
{
    QFileInfo inputInfo(inputFile);
    if (!inputInfo.exists()) {
        emit conversionError("El archivo de entrada no existe");
        return QString();
    }

    tempOutputFile = generateTempFilename();
    QString taskId = AsyncTaskManager::instance().submitTask("audio_conversion",
        [this, inputFile]() {
            startConversion(inputFile, tempOutputFile);
        });
    
    monitorProgress(taskId);
    return tempOutputFile;
}

void AudioConverter::startConversion(const QString& inputFile, const QString& outputFile)
{
    if (ffmpegProcess) {
        delete ffmpegProcess;
    }
    
    ffmpegProcess = new QProcess(this);
    
    connect(ffmpegProcess, &QProcess::readyReadStandardOutput,
            this, &AudioConverter::handleFFmpegOutput);
    connect(ffmpegProcess, &QProcess::readyReadStandardError,
            this, &AudioConverter::handleFFmpegError);
    connect(ffmpegProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &AudioConverter::handleFFmpegFinished);

    QStringList arguments;
    arguments << "-i" << inputFile
             << "-acodec" << "pcm_s16le"
             << "-ar" << "44100"
             << "-ac" << "2"
             << outputFile;

    emit conversionStarted(inputFile);
    conversionTimer.start();
    ffmpegProcess->start("ffmpeg", arguments);
}

void AudioConverter::handleFFmpegOutput()
{
    if (!ffmpegProcess) return;
    
    QString output = ffmpegProcess->readAllStandardOutput();
    qDebug() << "FFmpeg output:" << output;
}

void AudioConverter::handleFFmpegError()
{
    if (!ffmpegProcess) return;
    
    QString error = ffmpegProcess->readAllStandardError();
    qDebug() << "FFmpeg error:" << error;
}

void AudioConverter::handleFFmpegFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        emit conversionFinished(tempOutputFile);
        tempFiles.append(tempOutputFile);
    } else {
        emit conversionError("Error en la conversión de audio");
        QFile::remove(tempOutputFile);
    }
}

QString AudioConverter::generateTempFilename()
{
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString tempFile = QString("%1/nexus_temp_%2.wav")
                          .arg(tempPath)
                          .arg(QDateTime::currentMSecsSinceEpoch());
    return tempFile;
}

void AudioConverter::monitorProgress(const QString& taskId)
{
    connect(&AsyncTaskManager::instance(), &AsyncTaskManager::progressUpdated,
        this, [this](const QString& id, const QString& type, int progress) {
            if (type == "audio_conversion") {
                emit conversionProgress(progress);
            }
        });
}

void AudioConverter::cleanupTempFiles()
{
    for (const QString& file : tempFiles) {
        QFile::remove(file);
    }
    tempFiles.clear();
}
