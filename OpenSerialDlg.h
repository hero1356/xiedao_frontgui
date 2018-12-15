#ifndef _OPENSERIALDLG_H_
#define _OPENSERIALDLG_H_

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>


class OpenSerialDlg : public QDialog
{
    Q_OBJECT

    QLabel m_serialLbl;
    QLabel m_baudRateLbl;
    QComboBox m_serialCbx;
    QComboBox m_baudRateCbx;
    QPushButton m_pushBtn;
    QPushButton m_closeBtn;
public:
    OpenSerialDlg(QDialog *parent = 0);
protected slots:
    void onBtnClicked();
    void onCloseBtnClicked();
signals:
    void signal_serialData(QString portName, QString baudRate);
    void signal_closeSerial(QString portName, QString baudRate);
};

#endif // OPENSERIALDLG_H
