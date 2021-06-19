#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QThread>

#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QThread factThread;
    Worker *factWorker;

    QThread primeThread;
    Worker *primeWorker;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_factCompute_clicked();
    void on_factPause_clicked();
    void on_factCancel_clicked();

    void on_primeCompute_clicked();
    void on_primePause_clicked();
    void on_primeCancel_clicked();

public slots:
    void showFactResult(QVector<int> result);
    void updateFactProgress(int progress, int max);

    void showPrimeResult(QVector<int> result);
    void updatePrimeProgress(int progress, int max);

signals:
    void callFact(int number);
    void callPrime(int from, int to);

private:
    Ui::MainWindow *ui;

    void factIdle();
    void factBusy();
    void pauseFact();
    void cancelFact();

    void primeIdle();
    void primeBusy();
    void pausePrime();
    void cancelPrime();
};
#endif // MAINWINDOW_H
