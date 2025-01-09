#include "threadpool.h"

ThreadPool& ThreadPool::instance() {
    static ThreadPool instance;
    return instance;
}

ThreadPool::ThreadPool(QObject* parent) : QObject(parent) {
    // Configurar el número de hilos basado en los núcleos del CPU
    int threadCount = QThread::idealThreadCount();
    setMaxThreadCount(threadCount);
    qDebug() << "ThreadPool inicializado con" << threadCount << "hilos";
}

void ThreadPool::submit(std::function<void()> task, std::function<void()> callback) {
    auto runnable = new Task(task, callback);
    QThreadPool::globalInstance()->start(runnable);
}

void ThreadPool::setMaxThreadCount(int count) {
    QThreadPool::globalInstance()->setMaxThreadCount(count);
}

int ThreadPool::maxThreadCount() const {
    return QThreadPool::globalInstance()->maxThreadCount();
}

int ThreadPool::activeThreadCount() const {
    return QThreadPool::globalInstance()->activeThreadCount();
}
