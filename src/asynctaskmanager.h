#ifndef ASYNCTASKMANAGER_H
#define ASYNCTASKMANAGER_H

#include <QObject>
#include <QHash>
#include <QElapsedTimer>
#include <QUuid>
#include <QDateTime>
#include <QMutex>
#include "threadpool.h"

class AsyncTaskManager : public QObject {
    Q_OBJECT

public:
    struct TaskStats {
        QString taskId;
        QString taskType;
        qint64 startTime;
        qint64 endTime;
        bool successful;
        QString error;
    };

    static AsyncTaskManager& instance();
    QString submitTask(const QString& taskType, std::function<void()> task);
    QList<TaskStats> getTaskHistory() const;

signals:
    void taskStarted(const QString& taskId, const QString& taskType);
    void taskCompleted(const QString& taskId, const QString& taskType);
    void taskFailed(const QString& taskId, const QString& taskType, const QString& error);
    void progressUpdated(const QString& taskId, const QString& taskType, int progress);

private:
    explicit AsyncTaskManager(QObject* parent = nullptr);
    AsyncTaskManager(const AsyncTaskManager&) = delete;
    AsyncTaskManager& operator=(const AsyncTaskManager&) = delete;

    void recordTaskCompletion(const QString& taskId, const QString& taskType, 
                            qint64 duration, bool successful, const QString& error = QString());

    mutable QMutex m_mutex;
    QHash<QString, TaskStats> m_taskHistory;
};

#endif // ASYNCTASKMANAGER_H
