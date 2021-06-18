#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_factCompute_clicked()
{

}


void MainWindow::on_factPause_clicked()
{

}


void MainWindow::on_factCancel_clicked()
{

}


void MainWindow::on_primeCompute_clicked()
{

}


void MainWindow::on_primePause_clicked()
{

}


void MainWindow::on_primeCancel_clicked()
{

}

