#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTime"
#include "QWaitCondition"
#include "QCloseEvent"
#include "QSemaphore"
#include "QMessageBox"

#include "mythread.h" //ПОДКЛЮЧЕНИЕ КЛАССА

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupThreads();
}

QSemaphore semaphore(3); //УСТАНОВКА QSemaphore
bool closeMe=false; //ПЕРЕМЕННАЯ ОТВЕЧАЮЩАЯ ЗА ЗАКРЫТИЕ
bool closing=false;

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupThreads() //СОЗДАНИЕ ПОТОКОВ И ПРИВЗЯКА СИГНАЛОВ
{
    MyThread *thread = new MyThread;
    connect(this,SIGNAL(startSignal()),
            thread,SLOT(reciveStartSignal()),Qt::AutoConnection);

    connect(thread,SIGNAL(workEnd(int)),
            this,SLOT(reciveWorkEnd(int)),Qt::AutoConnection);

    connect(thread,SIGNAL(inWork()),this,
            SLOT(reciveInWorkSignal()),Qt::AutoConnection);

    connect(thread,SIGNAL(stoped(int)),this,
            SLOT(reciveStopedSignal(int)),Qt::AutoConnection);

    connect(this,SIGNAL(stopSignal()),
            thread,SLOT(reciveStopSignal()));

    thread->setSeed(QTime::currentTime().msec()+qrand()%2000+400);
    thread->setProperty("Номер",0);
    thread->setProperty("Название","First thread");

    MyThread *thread1 = new MyThread;
    connect(this,SIGNAL(startSignal()),
            thread1,SLOT(reciveStartSignal()),Qt::AutoConnection);

    connect(thread1,SIGNAL(workEnd(int)),
            this,SLOT(reciveWorkEnd(int)),Qt::AutoConnection);

    connect(thread1,SIGNAL(inWork()),this,
            SLOT(reciveInWorkSignal()),Qt::AutoConnection);

    connect(thread1,SIGNAL(stoped(int)),this,
            SLOT(reciveStopedSignal(int)),Qt::AutoConnection);

    connect(this,SIGNAL(stopSignal()),
            thread1,SLOT(reciveStopSignal()));

    thread1->setSeed(QTime::currentTime().msec()+qrand()%2000+100);
    thread1->setProperty("Номер",1);
    thread1->setProperty("Название","Second thread");

    MyThread *thread2 = new MyThread;

    connect(this,SIGNAL(startSignal()),
            thread2,SLOT(reciveStartSignal()),Qt::AutoConnection);

    connect(thread2,SIGNAL(workEnd(int)),
            this,SLOT(reciveWorkEnd(int)),Qt::AutoConnection);

    connect(thread2,SIGNAL(inWork()),this,
            SLOT(reciveInWorkSignal()),Qt::AutoConnection);

    connect(thread2,SIGNAL(stoped(int)),this,
            SLOT(reciveStopedSignal(int)),Qt::AutoConnection);

    connect(this,SIGNAL(stopSignal()),
            thread2,SLOT(reciveStopSignal()));

    thread2->setSeed(QTime::currentTime().msec()+qrand()%2000);
    thread2->setProperty("Номер",2);
    thread2->setProperty("Название","Third thread");
}

void MainWindow::on_pushButton_start_clicked() //ДЕЙСТВИЯ ПРИ НАЖАТИИИ НА КНОПКУ Запуск потоков
{
    ui->pushButton_start->setEnabled(false);
    emit startSignal();
}

void MainWindow::reciveInWorkSignal() //ПОЛУЧЕНИЕ СИГНАЛА О РАБОТЕ
{
    ui->textEdit->append((sender()->property("Название").toString().toLatin1())+" начал работу.");
}

void MainWindow::reciveWorkEnd(int time) //ПОЛУЧЕНИЕ СИГНАЛА О ЗАВЕРШЕНИИ РАБОТЫ
{
    ui->textEdit->append((sender()->property("Название").toString().toLatin1())+" закончил работу. Время работы: "+QString::number(time)+" миллисекунд.");
    ui->pushButton_start->setEnabled(true);
}

void MainWindow::reciveStopedSignal(int time) //ПОЛУЧЕНИЕ СИГНАЛА О ОСТАНОВКЕ
{
    semaphore.acquire(1);
    ui->textEdit->append((sender()->property("Название").toString().toLatin1())+" остановлен. Время завершения: "+QString::number(time)+" миллисекунд.");
    if (semaphore.available()<1)
    {
        closeMe=true;
        QMessageBox::information(this,"Внимание","Программа завершила свою работу.",QMessageBox::Ok);
        closing=false;
        this->close();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) //ПРОЦЕСС ЗАВЕРШЕНИИ РАБОТЫ
{
    if (closing)
        event->ignore();
    else
    {
        if (!closeMe)
        {
            ui->pushButton_start->setEnabled(false);
            ui->textEdit->append("\nИдет заавершение процесса потоков\n");
            ui->textEdit->repaint();
            closing=true;
            emit stopSignal();
            event->ignore();
        }
        else
            event->accept();
    }
}

