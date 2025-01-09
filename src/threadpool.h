#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <QObject>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QMutex>
#include <QQueue>
#include <QDebug>
#include <functional>
#include <memory>

class Task : public QRunnable {
public:
    using TaskFunction = std::function<void()>;
    using CompletionCallback = std::function<void()>;
    
    Task(TaskFunction func, CompletionCallback callback = nullptr)
        : m_function(func), m_callback(callback) {
        setAutoDelete(true);
    }
    
    void run() override {
        if (m_function) {
            m_function();
        }
        if (m_callback) {
            m_callback();
        }
    }

private:
    TaskFunction m_function;
    CompletionCallback m_callback;
};

class ThreadPool : public QObject {
    Q_OBJECT

public:
    static ThreadPool& instance();
    void submit(std::function<void()> task, std::function<void()> callback = nullptr);
    void setMaxThreadCount(int count);
    int maxThreadCount() const;
    int activeThreadCount() const;

signals:
    void taskStarted(const QString& taskId);
    void taskCompleted(const QString& taskId);
    void taskFailed(const QString& taskId, const QString& error);
    void progressUpdated(const QString& taskId, int progress);

private:
    explicit ThreadPool(QObject* parent = nullptr);
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
};

#endif // THREADPOOL_H
