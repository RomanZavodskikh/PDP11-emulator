#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    emulator_.loadBin(binFilePath_.toStdString());
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    binFilePath_ = arg1;
}
