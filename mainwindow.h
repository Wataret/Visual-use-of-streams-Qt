#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupThreads();
    void closeEvent(QCloseEvent *event);

signals:
    void startSignal();
    void stopSignal();

private slots:
    void on_pushButton_start_clicked();
    void reciveInWorkSignal();
    void reciveWorkEnd(int);
    void reciveStopedSignal(int time);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
