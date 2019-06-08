#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "QThread"
#include "QMutex"
#include "QWaitCondition"

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread();
    void run();
    void setSeed(int i);
    uint getSeed();
    void lockMutex();
    void unlockMutex();
private:
    int seed;
    int mode;
    QMutex mutex;
private slots:
    void reciveStartSignal();
    void reciveStopSignal();
signals:
    void inWork();
    void workEnd(int);
    void stoped(int);
};

#endif // MYTHREAD_H
