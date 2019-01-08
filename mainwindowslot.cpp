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
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>

/***********************************************
 *
 *        encryption transmission
 *
************************************************/
QString MainWindow::generateSign(QString input)
{
    QString ret;

    ret = QCryptographicHash::hash(input.toLocal8Bit(),QCryptographicHash::Md5).toHex();

    return ret;
}

QString MainWindow::httpGetGenerateSign(QString input)
{
    if( !ENCRYPTION_TRANSMISSION )
        return input;

    QString ret = input;

    int pos = input.indexOf('?');

    if(pos == -1)
    {
        QString MD5_in = userPwd;

        qDebug() << "MD5_in" << MD5_in;

        QString sign = generateSign(MD5_in);

        ret += QString("?keyid="+currentOperator+"&sign="+sign);

    }else{

        QString MD5_in = QString(ret.mid(pos+1)+userPwd);

        qDebug() << "MD5_in:" << MD5_in;

        QString sign = generateSign(MD5_in);

        ret += QString("&keyid="+currentOperator+"&sign="+sign);
    }

    return ret;
}

QString MainWindow::httpPostGenerateSign(QString input)
{
    if( !ENCRYPTION_TRANSMISSION )
        return input;

    QString ret = "{\"data\":"+input;

    QString MD5_in = input + userPwd;

    qDebug() << "MD5_in:" << MD5_in;

    QString sign = generateSign(MD5_in);

    ret += QString(",\"keyid\":\""+currentOperator+"\",\"sign\":\""+sign+"\"}");

    return ret;
}

/***********************************************
 *
 *        generate Messageboxs
 *
************************************************/
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

/***********************************************
 *
 *        widget page change management
 *
************************************************/

// 1.inport excel
void MainWindow::on_m_inportExcelBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(index_importExcel);
}

// 2.bat make card
void MainWindow::on_m_makeCardBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(index_makeCard);
}

// 3.traveler query
void MainWindow::on_m_travelerQueryBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(index_query);
}

// 4.card management
void MainWindow::on_m_cardMagBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(index_cardManage);
}
// 5.history query
void MainWindow::on_m_queryHistoryBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(index_history);
}

//slot..widget page changed
void MainWindow::on_stackedWidget_currentChanged(int index)
{
    ui->m_inportExcelBtn->setProperty("switchpage", false);
    ui->m_makeCardBtn->setProperty("switchpage", false);
    ui->m_travelerQueryBtn->setProperty("switchpage", false);
    ui->m_cardMagBtn->setProperty("switchpage", false);
    ui->m_queryHistoryBtn->setProperty("switchpage", false);

    switch (index) {
    case index_importExcel:
        ui->m_inportExcelBtn->setProperty("switchpage", true);
        break;
    case index_makeCard:
        ui->m_makeCardBtn->setProperty("switchpage", true);
        break;
    case index_query:
        ui->m_travelerQueryBtn->setProperty("switchpage", true);
        break;
    case index_cardManage:
        ui->m_cardMagBtn->setProperty("switchpage", true);
        break;
    case index_history:
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
 *              serial management
 *
************************************************/

// func: open serial
void MainWindow::serialOpen()
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


// slot..set serial infomation
void MainWindow::slot_serialData(QString portName, QString baudRate)
{
    m_serialPort = new QSerialPort();
    m_serialPort->setPortName(portName);
    m_serialPort->open(QIODevice::ReadWrite);
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

    m_serialPortLbl.setStyleSheet("color:green; font-size: 15px;");
    m_serialPortLbl.setText("串口："+m_serialPort->portName()+" 已连接！");

    connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(read_SerialPort()));
}


//slot..close serial
void MainWindow::slot_closeSerial(QString portName, QString baudRate)
{
    portName = portName;
    baudRate = baudRate;
    if(isSerialPortConnected())
    {
        m_isSerialPortConnected = false;
        m_serialPort->close();
        m_serialPortLbl.setStyleSheet("color:red; font-size: 15px;");
        m_serialPortLbl.setText("串口："+m_serialPort->portName()+" 已断开！");
    }
}

// slot..receive serial data
void MainWindow::read_SerialPort()
{
    QByteArray temp = m_serialPort->readAll();

    unsigned int cardid = (unsigned char)temp.at(5);
    cardid <<= 8;
    cardid |= (unsigned char)temp.at(6);
    cardid <<= 8;
    cardid |= (unsigned char)temp.at(7);

    ui->m_cardIDInputEdit->setText(QString::number(cardid));
    ui->m_queryInfoEdit2->setText(QString::number(cardid));

    setTip("当前卡号："+QString::number(cardid,10));
    emit signal_sendCardID(cardid);
}

/***********************************************
 *
 *              Json Parse
 *
************************************************/

/*
 *     { "rslt":"success"}  or
 *     { "rslt":"failed","reason":"xxx"}
 */
bool MainWindow::jsonParse(const QString& strResult, QString& rslt, QString& reason)
{
    bool ret = true;
    QString str = "default";
    QString buf = strResult;
    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);
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
        }
        else
        {
            str = "json is not object";
            ret = false;
        }
    }
    qDebug() <<"Json Parse Result: "<< str;
    return ret;
}

