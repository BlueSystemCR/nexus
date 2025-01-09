#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <QString>
#include <QProcess>
#include <QTemporaryFile>
#include <QObject>
#include <QFile>

class AudioConverter : public QObject
{
    Q_OBJECT

public:
    explicit AudioConverter(QObject *parent = nullptr);
    QString convertFlacToWav(const QString& flacFile);
    void cleanupTempFiles();

signals:
    void conversionFinished(const QString& outputFile);
    void conversionError(const QString& error);
    void conversionProgress(int percentage);

private:
    QList<QString> tempFiles;
    QString createTempFile(const QString& suffix);
    void monitorearProgreso(QProcess* proceso, qint64 tamanoEntrada);
};

#endif // AUDIOCONVERTER_H
