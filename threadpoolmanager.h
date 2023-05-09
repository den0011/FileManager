#ifndef THREADPOOLMANAGER_H
#define THREADPOOLMANAGER_H

#include <QObject>
#include <QThreadPool>

class ThreadPoolManager : public QObject
{
    Q_OBJECT


public:
    explicit ThreadPoolManager(QObject* parent = nullptr) : QObject(parent) {}

    void setMaxThreadCount(int count) {
        QThreadPool::globalInstance()->setMaxThreadCount(count);
    }

    void start(QRunnable* runnable) {
        QThreadPool::globalInstance()->start(runnable);
    }

    void clear() {
        QThreadPool::globalInstance()->clear();
    }

    int activeThreadCount() const {
        return QThreadPool::globalInstance()->activeThreadCount();
    }

    void waitForDone() {
        QThreadPool::globalInstance()->waitForDone();
    }

signals:

public slots:
};

#endif // THREADPOOLMANAGER_H
