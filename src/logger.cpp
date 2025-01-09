#include "logger.h"
#include <QDir>
#include <QStandardPaths>

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    QString logPath = getLogFilePath();
    logFile.setFileName(logPath);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        stream.setDevice(&logFile);
    }
}

void Logger::log(Level level, const QString& message) {
    if (!logFile.isOpen()) return;
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    stream << QString("[%1] [%2] %3\n")
              .arg(timestamp)
              .arg(levelToString(level))
              .arg(message);
    stream.flush();
}

QString Logger::levelToString(Level level) {
    switch (level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

QString Logger::getLogFilePath() {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return dir.filePath("reproductor.log");
}
