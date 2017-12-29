#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_2->setValidator(new QIntValidator(0, 65535));
    ui->lineEdit_3->setValidator(new QIntValidator(0, 65535));
    ui->lineEdit_4->setValidator(new QIntValidator(0, 65535));
    ui->lineEdit_5->setValidator(new QIntValidator(0, 65535));
    ui->lineEdit_6->setValidator(new QIntValidator(0, 65535));
    ui->lineEdit_7->setValidator(new QIntValidator(0, 65535));
    ui->lineEdit_8->setValidator(new QIntValidator(0, 65535));
    ui->lineEdit_9->setValidator(new QIntValidator(0, 65535));
    emulator_.showState(ui);
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

void MainWindow::on_pushButton_2_clicked()
{
    emulator_.tryToEmulate();
}
