#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    factWorker = new Worker;
    factWorker->moveToThread(&factThread);

    primeWorker = new Worker;
    primeWorker->moveToThread(&primeThread);

    factThread.start();
    primeThread.start();

    connect(this, &MainWindow::callFact, factWorker, &Worker::calcFact);
    connect(this, &MainWindow::callPrime, primeWorker, &Worker::calcPrime);

    connect(factWorker, &Worker::factResult, this, &MainWindow::showFactResult);
    connect(factWorker, &Worker::factProgress, this, &MainWindow::updateFactProgress);

    connect(primeWorker, &Worker::primeResult, this, &MainWindow::showPrimeResult);
    connect(primeWorker, &Worker::primeProgress, this, &MainWindow::updatePrimeProgress);
}

MainWindow::~MainWindow()
{
    delete ui;

    factThread.quit();
    primeThread.quit();

    factThread.wait();
    primeThread.wait();
}


void MainWindow::on_factCompute_clicked() {

    ui->factResult->setText("");

    QString inputString = ui->factInput->text();

    bool valid = false;
    int factInput = inputString.toInt(&valid);

    if (!valid) {

        QMessageBox errorMessage;
        errorMessage.critical(0, "Error", "Error parsing input.");
        errorMessage.setFixedSize(500, 200);
        errorMessage.show();

    } else if (factInput >= 0) {
        factBusy();
        emit callFact(factInput);

    } else {
        QMessageBox errorMessage;
        errorMessage.critical(0, "Error", "Input can't be negative.");
        errorMessage.setFixedSize(500, 200);
        errorMessage.show();
    }
}

void MainWindow::on_factPause_clicked() {
    pauseFact();
}

void MainWindow::on_factCancel_clicked() {

    cancelFact();
    factIdle();
}

void MainWindow::on_primeCompute_clicked() {

    ui->primeResult->setText("");

    QString fromInputString = ui->primeFromInput->text();
    QString toInputString = ui->primeToInput->text();

    bool fromValid, toValid = false;
    int fromInput = fromInputString.toInt(&fromValid);
    int toInput = toInputString.toInt(&toValid);

    if (fromValid && toValid) {

        if (fromInput < 2) {
            QMessageBox errorMessage;
            errorMessage.critical(0, "Error", "Bottom limit can't be less than 1.");
            errorMessage.setFixedSize(500, 200);
            errorMessage.show();

        } else if (toInput < 1) {
            QMessageBox errorMessage;
            errorMessage.critical(0, "Error", "Upper limit can't be negative.");
            errorMessage.setFixedSize(500, 200);
            errorMessage.show();

        } else if (fromInput > toInput) {

            QMessageBox errorMessage;
            errorMessage.critical(0, "Error", "Bottom limit can't be greater than upper limit.");
            errorMessage.setFixedSize(500, 200);
            errorMessage.show();

        } else {
            primeBusy();
            emit callPrime(fromInput, toInput);
        }

    } else {
        QMessageBox errorMessage;
        errorMessage.critical(0, "Error", "Error parsing input.");
        errorMessage.setFixedSize(500, 200);
        errorMessage.show();
    }
}

void MainWindow::on_primePause_clicked() {
    pausePrime();
}

void MainWindow::on_primeCancel_clicked() {

    cancelPrime();
    primeIdle();
}


void MainWindow::showFactResult(QVector<int> result) {

    printf("MainWindow - showFactResult, size = %d \n", result.size());
    fflush(stdout);

    QString resultString = "";
    for (int i = result.size() - 1; i >= 0; i--) {
        resultString.append(QString::number(result[i]));
    }
    ui->factResult->setText(resultString);
    factIdle();
}

void MainWindow::updateFactProgress(int progress, int max) {

    this->ui->factProgress->setMaximum(max);
    this->ui->factProgress->setValue(progress);
}

void MainWindow::showPrimeResult(QVector<int> result) {

    printf("MainWindow - showPrimeResult, size = %d \n", result.size());
    fflush(stdout);

    QString resultString = "";
    int size = result.size();
    int limit = std::min(size, 1000);

    for (int i = 0; i < limit; i++) {
        resultString.append(QString::number(result[i]));
        resultString.append(", ");
    }
    ui->primeResult->setText(resultString);
    primeIdle();
}

void MainWindow::updatePrimeProgress(int progress, int max) {

    ui->primeProgress->setMaximum(max);
    ui->primeProgress->setValue(progress);
}


void MainWindow::factIdle() {

    ui->factInput->setDisabled(false);
    ui->factCompute->setDisabled(false);

    ui->factPause->setDisabled(true);
    ui->factCancel->setDisabled(true);
}

void MainWindow::factBusy() {

    ui->factInput->setDisabled(true);
    ui->factCompute->setDisabled(true);

    ui->factPause->setDisabled(false);
    ui->factCancel->setDisabled(false);
}

void MainWindow::pauseFact() {
    factWorker->isPaused = !factWorker->isPaused;

    if (factWorker->isPaused) {
        ui->factPause->setText("RESUME");
    } else {
        ui->factPause->setText("PAUSE");
    }
}

void MainWindow::cancelFact() {

    factWorker->isPaused = false;
    factWorker->isCancelled = true;
}


void MainWindow::primeIdle() {

    ui->primeFromInput->setDisabled(false);
    ui->primeToInput->setDisabled(false);
    ui->primeCompute->setDisabled(false);

    ui->primePause->setDisabled(true);
    ui->primeCancel->setDisabled(true);
}

void MainWindow::primeBusy() {

    ui->primeFromInput->setDisabled(true);
    ui->primeToInput->setDisabled(true);
    ui->primeCompute->setDisabled(true);

    ui->primePause->setDisabled(false);
    ui->primeCancel->setDisabled(false);
}

void MainWindow::pausePrime() {
    primeWorker->isPaused = !primeWorker->isPaused;

    if (primeWorker->isPaused) {
        ui->primePause->setText("RESUME");
    } else {
        ui->primePause->setText("PAUSE");
    }
}

void MainWindow::cancelPrime() {

    primeWorker->isPaused = false;
    primeWorker->isCancelled = true;
}
