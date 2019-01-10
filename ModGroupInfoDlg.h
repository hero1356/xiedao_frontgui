#ifndef MODGROUPINFODLG_H
#define MODGROUPINFODLG_H

#include <QDialog>

namespace Ui {
class ModGroupInfoDlg;
}

class ModGroupInfoDlg : public QDialog
{
    Q_OBJECT

    QString m_teamname;
    QString m_groupname;
public:
    explicit ModGroupInfoDlg(QWidget *parent = 0);
    ~ModGroupInfoDlg();
    void setTeamname(QString teamname);
    void setGroupname(QString groupname);
    QString getTeamname();
    QString getGroupname();

private slots:
    void on_ok_clicked();
    void on_cancel_clicked();

private:
    Ui::ModGroupInfoDlg *ui;
};

#endif // MODGROUPINFODLG_H
