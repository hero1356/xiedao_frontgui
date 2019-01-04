#ifndef _LOGINDLG_H_
#define _LOGINDLG_H_

#include <QDialog>
#include <QColor>
#include <QTimer>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT

    QString m_user;
    QString m_pwd;
    QString m_captcha;
    Qt::GlobalColor* m_colors;
    QTimer m_timer;




private:
    QString getCaptcha();
    Qt::GlobalColor* getColors();
protected:
//    void paintEvent(QPaintEvent *event);
//    void mousePressEvent(QMouseEvent *event);

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();
    QString getUser() { return m_user; }
    QString getPwd() { return m_pwd; }

private slots:
    void on_loginBtn_clicked();
    void onTimeout();

    void on_shutdownBtn_clicked();

private:
    Ui::LoginDlg *ui;
};

#endif // _LOGINDLG_H_
