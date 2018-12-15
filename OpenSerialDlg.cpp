#include "OpenSerialDlg.h"
#include <QGridLayout>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>

OpenSerialDlg::OpenSerialDlg(QDialog *parent):QDialog(parent,Qt::WindowCloseButtonHint)
{
    m_serialLbl.setText("串口号：");
    m_baudRateLbl.setText("波特率：");
    m_closeBtn.setText("断开");
    m_pushBtn.setText("连接");
    connect(&m_closeBtn, SIGNAL(clicked(bool)),this,SLOT(onCloseBtnClicked()));
    connect(&m_pushBtn, SIGNAL(clicked(bool)),this,SLOT(onBtnClicked()));

    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    for(int i=0; i<serialList.count(); i++)
    {
        QSerialPort serial;
        serial.setPort(serialList.at(i));
        if(serial.open(QIODevice::ReadWrite))
        {
            m_serialCbx.insertItem(i,serialList.at(i).portName());
            serial.close();
        }
    }


    m_baudRateCbx.insertItem(0, "115200");
    m_baudRateCbx.insertItem(1, "57600");
    m_baudRateCbx.insertItem(2, "38400");
    m_baudRateCbx.insertItem(3, "19200");
    m_baudRateCbx.insertItem(4, "9600");


    QGridLayout* layout = new QGridLayout;
    layout->addWidget(&m_serialLbl, 0 , 0);
    layout->addWidget(&m_serialCbx, 0 , 1);
    layout->addWidget(&m_baudRateLbl, 1 , 0);
    layout->addWidget(&m_baudRateCbx, 1 , 1);
    layout->addWidget(&m_closeBtn, 2, 0);
    layout->addWidget(&m_pushBtn, 2 , 1);
    setLayout(layout);
    setWindowTitle("打开串口");
    m_pushBtn.setFocus();
}

void OpenSerialDlg::onBtnClicked()
{
    QString portName = m_serialCbx.currentText();
    QString baudRate = m_baudRateCbx.currentText();
    emit signal_serialData(portName,  baudRate);
    close();
}

void OpenSerialDlg::onCloseBtnClicked()
{
    QString portName = m_serialCbx.currentText();
    QString baudRate = m_baudRateCbx.currentText();
    emit signal_closeSerial(portName,  baudRate);
    close();
}
