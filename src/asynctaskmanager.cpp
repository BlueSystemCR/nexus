#include "asynctaskmanager.h"

AsyncTaskManager& AsyncTaskManager::instance() {
    static AsyncTaskManager instance;
    return instance;
}

AsyncTaskManager::AsyncTaskManager(QObject* parent) : QObject(parent) {
    qDebug() << "AsyncTaskManager inicializado";
}

QString AsyncTaskManager::submitTask(const QString& taskType, std::function<void()> task) {
    QString taskId = QUuid::createUuid().toString();
    
    auto wrappedTask = [this, taskId, taskType, task]() {
        emit taskStarted(taskId, taskType);
        QElapsedTimer timer;
        timer.start();
        
        try {
            task();
            recordTaskCompletion(taskId, taskType, timer.elapsed(), true);
            emit taskCompleted(taskId, taskType);
        } catch (const std::exception& e) {
            QString error = QString("Error en tarea: %1").arg(e.what());
            recordTaskCompletion(taskId, taskType, timer.elapsed(), false, error);
            emit taskFailed(taskId, taskType, error);
        }
    };
    
    ThreadPool::instance().submit(wrappedTask);
    return taskId;
}

QList<AsyncTaskManager::TaskStats> AsyncTaskManager::getTaskHistory() const {
    QMutexLocker locker(&m_mutex);
    return m_taskHistory.values();
}

void AsyncTaskManager::recordTaskCompletion(const QString& taskId, const QString& taskType,
                                          qint64 duration, bool successful, const QString& error) {
    QMutexLocker locker(&m_mutex);
    
    TaskStats stats;
    stats.taskId = taskId;
    stats.taskType = taskType;
    stats.startTime = QDateTime::currentMSecsSinceEpoch() - duration;
    stats.endTime = QDateTime::currentMSecsSinceEpoch();
    stats.successful = successful;
    stats.error = error;
    
    m_taskHistory.insert(taskId, stats);
    
    // Mantener un historial limitado
    while (m_taskHistory.size() > 1000) {
        auto keys = m_taskHistory.keys();
        if (!keys.isEmpty()) {
            m_taskHistory.remove(keys.first());
        }
    }
}
