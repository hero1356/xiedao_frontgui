#ifndef HISTORYINFO_H
#define HISTORYINFO_H

#include <QObject>

class HistoryInfo : public QObject
{
    Q_OBJECT

    QString m_username;
    QString m_telephone;
    QString m_score;
    QString m_ordertime;
    QString m_scoretime;
    QString m_openid;

    QString m_scorejson;
public:
    explicit HistoryInfo(QObject *parent = nullptr);
    explicit HistoryInfo(QString username, QString telephone,
                         QString score,QString ordertime,QString scoretime,
                         QString openoid, QString scorejson,QObject *parent = 0);
    HistoryInfo(const HistoryInfo& obj);
    HistoryInfo& operator = (const HistoryInfo& obj);


    QString username() {return m_username;}
    QString telephone() {return m_telephone;}
    QString score() {return m_score;}
    QString ordertime() {return m_ordertime;}
    QString scoretime() {return m_scoretime;}
    QString openid() {return m_openid;}
    QString scorejson(){return m_scorejson;}


    void set_username(QString username) { m_username = username;}
    void set_telephone(QString telephone) { m_telephone = telephone;}
    void set_score(QString score) { m_score = score;}

    void set_ordertime(QString ordertime) { m_ordertime = ordertime;}
    void set_scoretime(QString scoretime) { m_scoretime = scoretime;}
    void set_openid(QString openid) { m_openid = openid;}
    void set_scorejson(QString scorejson) {m_scorejson = scorejson;}


};

#endif // HISTORYINFO_H
