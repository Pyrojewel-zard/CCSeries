#include "mainwindow.h"
#include "ui_mainwindow.h"


int Achor1_dis;
int Achor2_dis;
int Achor3_dis;
int Achor4_dis;


//debugok
const double C = 299792458.0;       // Speed of light
const double tsfreq = 499.2e6 * 128;  // Timestamp counter frequency）

QByteArray Anchorsraw[4][4];

QByteArray Anchortdoa[2][6];


qlonglong Anchortdoaint[6][6];
double flytime;
int Anchors[4][4];
int Anchorsdis[4]={0,0,0,0};

QString tagname;
double td1,td2,ty1,ty2;
int tagnumber;
//end

bool calok = 0;
bool debugok = 0;
bool tdoatest = 1;
//need to be declared in the outside!!!!!!!!!当我没说
bool Anchors_4;
bool Anchors_3;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //开启显示串口数据
    showok = 1;
    //查找可用的串口，返回了一份可用串口的列表（函数return）
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;

       //QString sttr=QSerialPortInfo::availablePorts();

        serial.setPort(info);

       // qDebug() <<  serial.portName();

        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());

            serial.close();
        }
       // QObject::connect(ui->mapButton, SIGNAL(clicked()), this,  &MainWindow::on_mapButton_clicked());

    }

   // ui->PortBox->addItem("COM15");

    //设置波特率下拉菜单默认显示第二项
    ui->BaudBox->setCurrentIndex(5);
    //关闭发送按钮的使能
    ui->sendButton->setEnabled(false);
    Anchors_3 = 0;
    Anchors_4 = 0;
    qDebug() << "界面设定成功！";

    //定义协议包头
    bagbuff.resize(100); //缓冲区大小
    head.resize(2);
    tail.resize(2);
    head[0] = 0x55;
    head[1] = 0xaa;
    tail[0] = 0x0a;
    tail[1] = 0xdb;

#if 0
    QByteArray str("0102");
    bool ok;
    //Qbstr.reserve(str.size());
    qDebug() << 1 ;
    QByteArray data ="55aa130021..."
                     "0100000000..."
                     "0200000000..."
                     "0392000000..."
                     "fbdb..."
                     "0d0a";
    QByteArray head = "55aa";
    int head_place =0;

    if(!(data.isEmpty()))
    {
           head_place = data.indexOf(head)+4;
           for(int i=0;i < (data.mid(head_place,2)).toInt(&ok,16);i++)
               if(ok)qDebug()<<9;


    }
#endif
    /*
     * 调试用数据
     */
    Achor1_dis = 120;
    Achor2_dis = 60;
    Achor3_dis = 80;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//清空接受窗口
void MainWindow::on_clearButton_clicked()
{
    ui->textEdit->clear();
}
//发送数据
void MainWindow::on_sendButton_clicked()
{
    serial->write(ui->textEdit_2->toPlainText().toLatin1());
}
//读取接收到的数据
void MainWindow::Read_Data()
{
    QByteArray buf;
    QByteArray data;

    buf = serial->readAll();
    tempdata = buf;

    //qDebug() << tempdata.toHex()+".......";

#if 0
    if(tempdata.startsWith(head) && tempdata.size()>21)
    {

     Achor1 = tempdata.mid(9,1)+tempdata.mid(8,1)+tempdata.mid(7,1)+tempdata.mid(6,1);
     Achor2 = tempdata.mid(14,1)+tempdata.mid(13,1)+tempdata.mid(12,1)+tempdata.mid(11,1);
     Achor3 = tempdata.mid(19,1)+tempdata.mid(18,1)+tempdata.mid(17,1)+tempdata.mid(16,1);

     //qDebug()<< (Achor3.toHex()).toInt(&ok,16);

     Achor1_dis = (Achor1.toHex()).toInt(&ok,16);
     Achor2_dis = (Achor2.toHex()).toInt(&ok,16);
     Achor3_dis = (Achor3.toHex()).toInt(&ok,16);
#elif 0
     Achor1_dis = 120;
     Achor2_dis = 120;
     Achor3_dis = 120;
#elif 1

if(Anchors_3)
  {
    if(tempdata.startsWith(head[0]))  //是否是0x55开头
    {
        buffok = 1;

      //bagbuff = tempdata;

      }
    if(buffok)      //缓冲区开启
    {
        bagbuff += tempdata;
    }
    if(bagbuff.endsWith(tail[0]))   //是否结束
    {
        buffok = 0;
      // qDebug() << bagbuff.toHex();


        bagok = 1;
    }
        if(bagok&&calok)
        {
            Achor1 = bagbuff.mid(9,1)+bagbuff.mid(8,1)+bagbuff.mid(7,1)+bagbuff.mid(6,1);
            Achor2 = bagbuff.mid(14,1)+bagbuff.mid(13,1)+bagbuff.mid(12,1)+bagbuff.mid(11,1);
            Achor3 = bagbuff.mid(19,1)+bagbuff.mid(18,1)+bagbuff.mid(17,1)+bagbuff.mid(16,1);

            //qDebug()<< (Achor2.toHex()).toInt(&ok,16);
            //qDebug() <<Achor3;

            Achor1_dis = (Achor1.toHex()).toInt(&ok,16);
            Achor2_dis = (Achor2.toHex()).toInt(&ok,16);
            Achor3_dis = (Achor3.toHex()).toInt(&ok,16);
            bagok = 0;
            bagbuff.clear();

        }
}
if(Anchors_4)
{
    if(tempdata.startsWith(head[0]))  //是否是0x55开头
    {
        buffok = 1;

      //bagbuff = tempdata;

      }
    if(buffok)      //缓冲区开启
    {
        bagbuff += tempdata;
    }
    if(bagbuff.endsWith(tail[0]))   //是否结束
    {
        buffok = 0;
      // qDebug() << bagbuff.toHex();


        bagok = 1;
    }
        if(bagok&&calok)
        {
            Achor1 = bagbuff.mid(9,1)+bagbuff.mid(8,1)+bagbuff.mid(7,1)+bagbuff.mid(6,1);
            Achor2 = bagbuff.mid(14,1)+bagbuff.mid(13,1)+bagbuff.mid(12,1)+bagbuff.mid(11,1);
            Achor3 = bagbuff.mid(19,1)+bagbuff.mid(18,1)+bagbuff.mid(17,1)+bagbuff.mid(16,1);
            Achor4 = bagbuff.mid(24,1)+bagbuff.mid(23,1)+bagbuff.mid(22,1)+bagbuff.mid(21,1);
            qDebug()<< (Achor4.toHex()).toInt(&ok,16);
            //qDebug() <<Achor3;

            Achor1_dis = (Achor1.toHex()).toInt(&ok,16);
            Achor2_dis = (Achor2.toHex()).toInt(&ok,16);
            Achor3_dis = (Achor3.toHex()).toInt(&ok,16);
            Achor4_dis = (Achor4.toHex()).toInt(&ok,16);


            bagok = 0;
            bagbuff.clear();

        }
}
if(debugok)
{
    int lo = 3;
    int nl = 0;
    int ml = 0;
    if(tempdata.startsWith(head[0]))  //是否是0x55开头
    {
        buffok = 1;

      //bagbuff = tempdata;

      }
    if(buffok)      //缓冲区开启
    {
        bagbuff += tempdata;
    }
    if(bagbuff.endsWith(tail[1]))   //是否结束
    {
        buffok = 0;
      // qDebug() << bagbuff.toHex();


        bagok = 1;
    }
        if(bagok)
        {
            tagname = bagbuff.mid(2,1).toHex().toInt(&ok,16)+'0';
//qDebug() <<tagname;
//qDebug() << bagbuff.toHex();
            for(nl = 0;nl < 3;nl ++)
            {
                for(ml = 0;ml < 4;ml ++)
                {
                        Anchorsraw[nl][ml] = bagbuff.mid(lo+3,1)+bagbuff.mid(lo+2,1)+bagbuff.mid(lo+1,1)+bagbuff.mid(lo,1);
                        lo += 4;
                        Anchors[nl][ml] = (Anchorsraw[nl][ml].toHex()).toInt(&ok,16);
                        //qDebug() <<Anchors[nl][ml];


                }

                if((Anchors[nl][0] + Anchors[nl][2] + Anchors[nl][1] + Anchors[nl][3]) != 0)
                {
                    td1 = Anchors[nl][0];
                    td2 = Anchors[nl][2];
                    ty1 = Anchors[nl][1];
                    ty2 = Anchors[nl][3];
                   Anchorsdis[nl] = 1000.0 * C * ((td1*td2) - (ty1*ty2)) / (td1+td2+ty1+ty2) / tsfreq;
                }
               qDebug() << QString(" 0: %1 1: %2 2: %3").arg(Anchorsdis[0]).arg(Anchorsdis[1]).arg(Anchorsdis[2]);

            }
            bagok = 0;
            tagnumber = tagname.toInt();
            bagbuff.clear();
//            if(calok)
//            {
                emit serialok(Anchorsdis,tagnumber,1);
//                 calok = 0;
//            }

        }

}
if(tdoatest)
{
    int lo = 3;//数据包指针
    int nl = 0;//数据个数
    int ml = 0;
    if(tempdata.startsWith(head[0]))  //是否是0x55开头
    {
        buffok = 1;

      //bagbuff = tempdata;

      }
    if(buffok)      //缓冲区开启
    {
        bagbuff += tempdata;
    }
    if(bagbuff.endsWith(tail[1]))   //是否结束
    {
        buffok = 0;
       //qDebug() << bagbuff;


        bagok = 1;
    }
        if(bagok)
        {
            tagname = bagbuff.mid(2,1).toHex().toInt(&ok,16)+'0';
//qDebug() <<tagname;
//qDebug() << bagbuff.toHex();
            for(nl = 0;nl < 1;nl ++)
            {
                for(ml = 0;ml < 4;ml ++)
                {
                        Anchortdoa[nl][ml] =bagbuff.mid(lo+4,1)+bagbuff.mid(lo+3,1)+bagbuff.mid(lo+2,1)+bagbuff.mid(lo+1,1)+bagbuff.mid(lo,1);
                        lo += 5;
                        //一行数据的int类型
                       Anchortdoaint[nl][ml] = (Anchortdoa[nl][ml].toHex()).toLongLong(&ok,16);

                      //qDebug() <<Anchortdoaint[nl][ml];


                }


                if((Anchortdoaint[nl][0] + Anchortdoaint[nl][2] + Anchortdoaint[nl][1] + Anchortdoaint[nl][3]) != 0)
                {
                  flytime = Anchortdoaint[nl][1] -( Anchortdoaint[nl][3] - Anchortdoaint[nl][2] + Anchortdoaint[nl][0] + 300 ) ;
                   // Anchorsdis[nl] = 1000.0 * C * ((td1*td2) - (ty1*ty2)) / (td1+td2+ty1+ty2) / tsfreq;
                }

              qDebug() << flytime ;
            }
            bagok = 0;
            tagnumber = tagname.toInt();
            bagbuff.clear();
//            if(calok)
//            {
               // emit serialok();
//                 calok = 0;
//            }

        }

}


#endif
    if(!buf.isEmpty()&&showok)
    {
//       QString str = ui->textEdit->toPlainText();
//       str+=(buf);
//       ui->textEdit->clear();
       QString str = buf;
       ui->textEdit->append(str);
    }
    buf.clear();
}
void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率???
        serial->setBaudRate(ui->BaudBox->currentText().toInt());
        //设置数据位数
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
#if 0
        switch(ui->BitNumBox->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->StopBox->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
#endif
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
      //  ui->BitNumBox->setEnabled(false);
      //  ui->ParityBox->setEnabled(false);
      //  ui->StopBox->setEnabled(false);
        ui->openButton->setText(tr("关闭串口"));
        ui->sendButton->setEnabled(true);
        //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
        //connect(serial,SIGNAL(readyRead()),this,SLOT(Read_Data()));
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
      //  ui->BitNumBox->setEnabled(true);
      //  ui->ParityBox->setEnabled(true);
      //  ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));
        ui->sendButton->setEnabled(false);
    }
}


//void MainWindow::on_mapButton_clicked()
//{
 // QApplication b();
//    map *m=new map();
//    // connect(this,SIGNAL(serialok()),m,SLOT(serialokisr()));
//    m->show();
//    showok = !showok;
//    map r;
//    r.show();

//}

