#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    emulator_()
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
    emulator_.setUi(ui);
    emulator_.showState();
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

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    *(emulator_.getVcpu()->getRegAddr(0)) = arg1.toUShort();
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    *(emulator_.getVcpu()->getRegAddr(1)) = arg1.toUShort();
}

void MainWindow::on_lineEdit_4_textChanged(const QString &arg1)
{
    *(emulator_.getVcpu()->getRegAddr(2)) = arg1.toUShort();
}

void MainWindow::on_lineEdit_5_textChanged(const QString &arg1)
{
    *(emulator_.getVcpu()->getRegAddr(3)) = arg1.toUShort();
}

void MainWindow::on_lineEdit_6_textChanged(const QString &arg1)
{
    *(emulator_.getVcpu()->getRegAddr(4)) = arg1.toUShort();
}

void MainWindow::on_lineEdit_7_textChanged(const QString &arg1)
{
    *(emulator_.getVcpu()->getRegAddr(5)) = arg1.toUShort();
}

void MainWindow::on_lineEdit_8_textChanged(const QString &arg1)
{
    *(emulator_.getVcpu()->getRegAddr(6)) = arg1.toUShort();
}

void MainWindow::on_lineEdit_9_textChanged(const QString &arg1)
{
    *(emulator_.getVcpu()->getRegAddr(7)) = arg1.toUShort();
}
