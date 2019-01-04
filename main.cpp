#include "MainWindow.h"
#include "LoginDlg.h"
#include <QCryptographicHash>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginDlg login;
    if( login.exec() == QDialog::Accepted )
    {
        qDebug() << "Username: "<< login.getUser();
        qDebug() << "Password:" <<login.getPwd();

        qDebug() << "Password md5:" << QCryptographicHash::hash(login.getPwd().toLatin1(),QCryptographicHash::Md5).toHex();

        MainWindow w;
        w.show();

        return a.exec();
    }
    else
    {
        return -1;
    }

}
