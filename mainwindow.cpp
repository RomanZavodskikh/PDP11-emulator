#include <iostream>
#include <QFileDialog>
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
    ui->lineEdit_10->setValidator(new QIntValidator(0, 1));
    ui->lineEdit_11->setValidator(new QIntValidator(0, 1));
    ui->lineEdit_12->setValidator(new QIntValidator(0, 1));
    ui->lineEdit_13->setValidator(new QIntValidator(0, 1));
    emulator_.setUi(ui);
    emulator_.showState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->show();
    if (fileDialog->exec())
    {
        emulator_.loadBin(fileDialog->selectedFiles().at(0).toStdString());
        ui->lineEdit->setText(fileDialog->selectedFiles().at(0));
        emulator_.showState();
    }
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

void MainWindow::on_pushButton_3_clicked()
{
    emulator_.stop();
}

void MainWindow::on_pushButton_4_clicked()
{
    emulator_.step();
}

void MainWindow::on_pushButton_5_clicked()
{
    emulator_.reset();
}
