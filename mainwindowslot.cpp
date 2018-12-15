#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QModelIndex>
#include <QMessageBox>
#include <QTextCodec>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>

int MainWindow::showQueryMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("提示");
    msg.setText(message);
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setButtonText(QMessageBox::Yes, QString("确定"));
    msg.setButtonText(QMessageBox::No, QString("取消"));

    return msg.exec();
}

void MainWindow::showErrorMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("错误");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, QString("确定"));
    QTimer::singleShot(1000, &msg,SLOT(accept()));

    msg.exec();
}

void MainWindow::showInformationMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("提示");
    msg.setText(message);
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, QString("确定"));
    QTimer::singleShot(1000, &msg,SLOT(accept()));

    msg.exec();
}



// 2.信息导入
void MainWindow::on_m_inportExcelBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// 2.批量制卡
void MainWindow::on_m_makeCardBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//3.游客查询
void MainWindow::on_m_travelerQueryBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//4.卡片管理
void MainWindow::on_m_cardMagBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
//5.历史查询
void MainWindow::on_m_queryHistoryBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_stackedWidget_currentChanged(int index)
{
    qDebug() << "current index:" <<index;

    ui->m_inportExcelBtn->setProperty("switchpage", false);
    ui->m_makeCardBtn->setProperty("switchpage", false);
    ui->m_travelerQueryBtn->setProperty("switchpage", false);
    ui->m_cardMagBtn->setProperty("switchpage", false);
    ui->m_queryHistoryBtn->setProperty("switchpage", false);

    switch (index) {
    case 0:
        ui->m_inportExcelBtn->setProperty("switchpage", true);
        break;
    case 1:
        ui->m_makeCardBtn->setProperty("switchpage", true);
        break;
    case 2:
        ui->m_travelerQueryBtn->setProperty("switchpage", true);
        break;
    case 3:
        ui->m_cardMagBtn->setProperty("switchpage", true);
        break;
    case 4:
        ui->m_queryHistoryBtn->setProperty("switchpage", true);
        break;
    default:
        ui->m_inportExcelBtn->setProperty("switchpage", true);
        break;
    }

    ui->m_inportExcelBtn->style()->polish(ui->m_inportExcelBtn);
    ui->m_makeCardBtn->style()->polish(ui->m_makeCardBtn);
    ui->m_travelerQueryBtn->style()->polish(ui->m_travelerQueryBtn);
    ui->m_cardMagBtn->style()->polish(ui->m_cardMagBtn);
    ui->m_queryHistoryBtn->style()->polish(ui->m_queryHistoryBtn);

}


/***********************************************
 *
 *              串口管理相关
 *
************************************************/

// 串口设置
void MainWindow::on_m_serialManageBtn_clicked()
{
    if(isSerialPortConnected())
    {
        m_isSerialPortConnected = false;
        m_serialPort->close();
        m_serialPortLbl.setStyleSheet("color:red; font-size: 15px;");
        m_serialPortLbl.setText("串口："+m_serialPort->portName()+" 已断开！");
    }

    OpenSerialDlg* serialDlg = new OpenSerialDlg;
    serialDlg->setAttribute(Qt::WA_DeleteOnClose);
    connect(serialDlg,SIGNAL(signal_serialData(QString, QString)),this,SLOT(slot_serialData(QString, QString)));
    connect(serialDlg,SIGNAL(signal_closeSerial(QString,QString)),this,SLOT(slot_closeSerial(QString, QString)));
    serialDlg->exec();
}


////button slot: 串口设置
//void MainWindow::on_m_openSerialBtn_clicked()
//{
//    OpenSerialDlg* serialDlg = new OpenSerialDlg;
//    serialDlg->setAttribute(Qt::WA_DeleteOnClose);
//    connect(serialDlg,SIGNAL(signal_serialData(QString, QString)),this,SLOT(slot_serialData(QString, QString)));
//    serialDlg->exec();
//}

//断开串口
void MainWindow::slot_closeSerial(QString portName, QString baudRate)
{
    if(isSerialPortConnected())
    {
        m_isSerialPortConnected = false;
        m_serialPort->close();
        m_serialPortLbl.setStyleSheet("color:red; font-size: 15px;");
        m_serialPortLbl.setText("串口："+m_serialPort->portName()+" 已断开！");
    }
}

// 设置串口信息
void MainWindow::slot_serialData(QString portName, QString baudRate)
{
//    baudRate = baudRate;
    m_serialPort = new QSerialPort();
    m_serialPort->setPortName(portName);
    m_serialPort->open(QIODevice::ReadWrite);
//    m_serialPort->setBaudRate(115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if(baudRate == "115200")
    {
        m_serialPort->setBaudRate(QSerialPort::Baud115200);
    }
    else if(baudRate == "57600")
    {
        m_serialPort->setBaudRate(QSerialPort::Baud57600);
    }
    else if(baudRate == "38400")
    {
        m_serialPort->setBaudRate(QSerialPort::Baud38400);
    }
    else if(baudRate == "19200")
    {
        m_serialPort->setBaudRate(QSerialPort::Baud19200);
    }
    else if(baudRate == "9600")
    {
        m_serialPort->setBaudRate(QSerialPort::Baud9600);
    }

    m_isSerialPortConnected = true;
    qDebug() << "m_serialPort " << m_serialPort->portName();

    m_serialPortLbl.setStyleSheet("color:green; font-size: 15px;");
    m_serialPortLbl.setText("串口："+m_serialPort->portName()+" 已连接！");
    connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(read_SerialPort()));
}

// 串口数据接收槽函数
void MainWindow::read_SerialPort()
{
    // 读取卡号，存入data变量中
    QByteArray temp;
    temp = m_serialPort->readAll();
    unsigned int data = (unsigned char)temp.at(5);
    data <<= 8;
    data |= (unsigned char)temp.at(6);
    data <<= 8;
    data |= (unsigned char)temp.at(7);

    ui->m_cardIDInputEdit->setText(QString::number(data));
    ui->m_queryInfoEdit2->setText(QString::number(data));
    setTip("当前卡号："+QString::number(data,10));
    emit signal_sendCardID(data);
}


/*    解析JSON，返回结果
 *     { "rslt":"success"} 或者
 *     { "rslt":"failed","reason":"xxx"}
 */
bool MainWindow::jsonParse(const QString& strResult, QString& rslt, QString& reason)
{
    bool ret = true;
    QString str = "default";
    QString buf = strResult;
    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject rootObj = json.object();
            //是否含有key  rslt
            if(rootObj.contains("rslt"))
            {
                QJsonValue value = rootObj.value("rslt");
                //判断是否是string类型
                if (value.isString())
                {
                    rslt = value.toString();
                    str = "ok";
                }
            }

            //是否含有key  reason
            if(rootObj.contains("reason"))
            {
                QJsonValue value = rootObj.value("reason");
                if (value.isString())
                {
                    reason = value.toString();
                    str = "ok";
                }
            }

        }else{
            str = "json is not object";
            ret = false;
        }
    }
    qDebug() << str;
    return ret;
}

