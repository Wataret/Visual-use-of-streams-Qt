#include "mythread.h"
#include "QMutex"

QWaitCondition *waitCondition = new QWaitCondition;
MyThread::MyThread()
{
    mode=0;
}


void MyThread::run() //ДЕЙСВИЯ ПОТОКОВ ПРИ ЗАПУСКЕ
{
  while(true)
    {
    this->lockMutex();
    emit inWork();
    qsrand(getSeed());
    int time=qrand()%5000+900;
    this->msleep(time);
    emit workEnd(time);
    if (mode!=1)
        waitCondition->wait(&mutex);
    if (mode==1)
    {
        qsrand(qrand()%time+getSeed());
        setSeed(qrand()%time+500);
        int time=qrand()%1000+900;
        this->msleep(time);
        emit stoped(time);
        this->exit();
        break;
    }
  }
}

void MyThread::reciveStartSignal() //ПОЛУЧЕНИЕ СИГНАЛОВ О ЗАПУСКЕ
{
    if (this->isRunning())
    {
        waitCondition->wakeAll();
    }
    else
    {
        this->start();
    }
}


void MyThread::reciveStopSignal() //ПОЛУЧЕНИЕ СИГНАЛОВ О ОСТАНОВКЕ
{
    mode=1;
    if (this->isRunning())
        waitCondition->wakeAll();
    else
    {
        qsrand(getSeed());
        int time=qrand()%1000+900;
        this->msleep(time);
        emit stoped(time);
        this->exit();
    }

}



void MyThread::setSeed(int i) //УСТАНОВКА ЗНАЧЕНИЯ SEED
{
    seed=i;
}

uint MyThread::getSeed() //ВОЗРАЩЕНИЕ ЗНАЧЕНИЯ SEED
{
    return(seed);
}

void MyThread::lockMutex() //БЛОКИРОВКА MUTEX
{
    mutex.lock();
}


void MyThread::unlockMutex() //РАЗБЛОКИРОВКА MUTEX
{
    mutex.unlock();
}

