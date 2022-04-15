#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口的信息
#include <QDialog>
#include <QDebug>
#include <QByteArray>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_clearButton_clicked();
    void on_sendButton_clicked();
    void on_openButton_clicked();
    void Read_Data();

signals:
    void serialok(int*,int,int);

private:
    Ui::MainWindow *ui;

    QSerialPort *serial;
    QByteArray tempdata = "0";

};


#endif // MAINWINDOW_H
