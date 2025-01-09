#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Logger {
public:
    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };
    
    static Logger& instance();
    void log(Level level, const QString& message);
    
private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    QString levelToString(Level level);
    QString getLogFilePath();
    
    QFile logFile;
    QTextStream stream;
};

// Macros para facilitar el logging
#define LOG_DEBUG(msg) Logger::instance().log(Logger::DEBUG, msg)
#define LOG_INFO(msg) Logger::instance().log(Logger::INFO, msg)
#define LOG_WARNING(msg) Logger::instance().log(Logger::WARNING, msg)
#define LOG_ERROR(msg) Logger::instance().log(Logger::ERROR, msg)

#endif // LOGGER_H
