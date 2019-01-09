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
        QString name = login.getUser();
        QString pwd = login.getPwd();

        MainWindow w(name, pwd);

        w.show();

        return a.exec();
    }
    else
    {
        return -1;
    }

}
