#ifndef SHOWTRAVLERSCOREDLG_H
#define SHOWTRAVLERSCOREDLG_H

#include <QDialog>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QModelIndex>
#include <QJsonObject>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextBlock>
#include <QTextEdit>
#include <QPrinterInfo>
#include <QDateTime>
#include <QPrinter>
#include <QPrintDialog>
#include <QDebug>

typedef struct
{
    QString type;
    QString code;
    QString cardid;
    QString comid;
    QString name;
    QString finalscore;
    QString credit_point;
    QString grade_point;
    QString score_class;
    QString suggestion;
    QStringList remain_list;

}score_info_st, *score_info_t;

namespace Ui {
class ShowTravlerScoreDlg;
}

class ShowTravlerScoreDlg : public QDialog
{
    Q_OBJECT

    QString m_name;

public:
    explicit ShowTravlerScoreDlg(QString strResult, QWidget *parent = 0);
    ~ShowTravlerScoreDlg();
    bool static travelerScoreParse(QString Json, score_info_st& score_info, QString& errmsg);

private slots:
    void on_m_printBtn_clicked();

private:
    Ui::ShowTravlerScoreDlg *ui;

    void paiban(score_info_st score_info);
};

#endif // SHOWTRAVLERSCOREDLG_H
