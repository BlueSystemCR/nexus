#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QTemporaryFile>
#include <QElapsedTimer>
#include "asynctaskmanager.h"

class AudioConverter : public QObject {
    Q_OBJECT

public:
    explicit AudioConverter(QObject *parent = nullptr);
    ~AudioConverter();

    QString convertFlacToWav(const QString& inputFile);
    void cleanupTempFiles();

signals:
    void conversionProgress(int percentage);
    void conversionFinished(const QString& outputFile);
    void conversionError(const QString& error);
    void conversionStarted(const QString& inputFile);

private slots:
    void handleFFmpegOutput();
    void handleFFmpegError();
    void handleFFmpegFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess* ffmpegProcess;
    QString tempOutputFile;
    QElapsedTimer conversionTimer;
    QList<QString> tempFiles;

    void startConversion(const QString& inputFile, const QString& outputFile);
    QString generateTempFilename();
    void monitorProgress(const QString& taskId);
};

#endif // AUDIOCONVERTER_H
