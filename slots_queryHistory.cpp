#include "MainWindow.h"
#include "ui_MainWindow.h"
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
#include <QDebug>

void MainWindow::clearHistoryInfoModel()
{
    m_historyInfoModel.clear();
    ui->m_queryHistoryView->setColumnWidth(3, 200);
    ui->m_queryHistoryView->setColumnWidth(4, 200);
}


//查询历史————按照人名、手机号查询
void MainWindow::on_m_queryHistoryBtn1_clicked()
{
    ui->m_queryHistoryBtn1->setEnabled(false);
    ui->m_queryHistoryaBtn2->setEnabled(false);

    clearHistoryInfoModel();

    QString condition;
    QString value;
    int index = ui->m_queryHistoryCb->currentIndex();
    switch (index) {
    // 姓名
    case 0:
        condition = "username";
        qDebug() << "index = 0" << ui->m_queryHistoryCb->currentText();
        value = ui->m_queryHistoryEdit1->text().trimmed();
        if(value.isEmpty())
        {
            showInformationMessage("请输入...");
            ui->m_queryHistoryBtn1->setEnabled(true);
            ui->m_queryHistoryaBtn2->setEnabled(true);
            return;
        }
        break;

        // 手机号
    case 1:
        condition = "telephone";
        qDebug() << "index = 1" << ui->m_queryHistoryCb->currentText();
        value = ui->m_queryHistoryEdit1->text().trimmed();
        if(value.isEmpty())
        {
            showInformationMessage("请输入...");
            ui->m_queryHistoryBtn1->setEnabled(true);
            ui->m_queryHistoryaBtn2->setEnabled(true);
            return;
        }
        break;

    // 全部
    case 2:
        condition = "teamname";
        qDebug() << "index = 2" << ui->m_queryInfoCb->currentText();
        value = "";
        if(value.isEmpty())
        {
            showInformationMessage("请输入...");
            ui->m_queryHistoryBtn1->setEnabled(true);
            ui->m_queryHistoryaBtn2->setEnabled(true);
            return;
        }
        break;

    default:
        break;
    }

    qDebug() << "value:" << value;

    QJsonObject obj;
    obj.insert("condition", condition);
    obj.insert("value", value);
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    QString body = json;

    qDebug() <<"post body: "<< body;
    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/score/history";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getHistoryResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->post(strUrl,body);

}

//查询历史————按照日期查询
void MainWindow::on_m_queryHistoryaBtn2_clicked()
{
    ui->m_queryHistoryBtn1->setEnabled(false);
    ui->m_queryHistoryaBtn2->setEnabled(false);

    clearHistoryInfoModel();

    QString value_1 = ui->m_queryHistoryEdit2->text();
    QString value_2 = ui->m_queryHistoryEdit3->text();

    QString value = value_1 + "&" + value_2;

    QJsonObject obj;
    obj.insert("condition", "ordertimerange");
    obj.insert("value", value);
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    QString body = json;

    qDebug() <<"post body: "<< body;
    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/score/history";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getHistoryResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->post(strUrl,body);
}

void MainWindow::slot_getHistoryResult(bool success, const QString& strResult)
{
    ui->m_queryHistoryBtn1->setEnabled(true);
    ui->m_queryHistoryaBtn2->setEnabled(true);

    qDebug() << "slot_getTravelerResult :" << strResult;
    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }
    QString rslt, reason;
    QList<HistoryInfo> historyList;
    if( historyResultParse(strResult, rslt, reason, historyList))
    {
        if( rslt == "success")
        {
            if(historyList.isEmpty())
            {
                showInformationMessage("没有查询到结果！");
            }
            setTip("查询成功！"+reason);

            m_historyInfoModel.add(historyList);

        }
        else if( rslt == "failed")
        {
            setTip("查询失败！"+reason);
            showErrorMessage("查询失败！\n"+reason);
        }
    }
}

//查询历史结果解析
bool MainWindow::historyResultParse(const QString& strResult, QString& rslt, QString& reason, QList<HistoryInfo>& list)
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
                if (value.isString())
                {
                    rslt = value.toString();
                    QString data1 = value.toString();
                    if( data1 == "success")
                    {
                        if(rootObj.contains("data"))
                        {
                            QJsonValue value = rootObj.value("data");
                            if(value.isArray())
                            {
                                QJsonArray array = value.toArray();

                                // 循环读取数组中的数据
                                list.clear();
                                for(int i=0; i<array.count(); i++)
                                {
                                    HistoryInfo info;

                                    QJsonObject arrayObj = array.at(i).toObject();

                                    if(arrayObj.contains("username"))
                                    {
                                        QJsonValue value = arrayObj.value("username");
                                        if(value.isString())
                                        {
                                            info.set_username(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("telephone"))
                                    {
                                        QJsonValue value = arrayObj.value("telephone");
                                        if(value.isString())
                                        {
                                            info.set_telephone(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("score"))
                                    {
                                        QJsonValue value = arrayObj.value("score");

                                        if(value.isObject())
                                        {
                                            QString scoreJson = QString(QJsonDocument(value.toObject()).toJson(QJsonDocument::Compact));

                                            info.set_score(scoreParse(scoreJson));
                                            info.set_scorejson(scoreJson);
                                            qDebug() << "scoreJson : "<<scoreJson;

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("ordertime"))
                                    {
                                        QJsonValue value = arrayObj.value("ordertime");
                                        if(value.isString())
                                        {
                                            info.set_ordertime(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("scoretime"))
                                    {
                                        QJsonValue value = arrayObj.value("scoretime");
                                        if(value.isString())
                                        {
                                            info.set_scoretime(value.toString("yyyy年MM月dd日"));

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("openid"))
                                    {
                                        QJsonValue value = arrayObj.value("openid");
                                        if(value.isString())
                                        {
                                            info.set_openid(value.toString());

                                            str = "ok";
                                        }
                                    }

                                    list.append(info);

                                }
                            }
                        }
                    }

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


//查询历史--分数解析
QString MainWindow::scoreParse(QString Json)
{

    QString score = "";

    // json 解析
    score_info_st score_info;
    QString errmsg;

    bool ret = ShowTravlerScoreDlg::travelerScoreParse(Json, score_info, errmsg);

    if(ret == true)
    {
        score = score_info.finalscore;
    }

    qDebug() << "score:" << score;

    return score;
}

//右键--成绩
void MainWindow::onHistoryScoreClicked()
{
     QModelIndex index = ui->m_queryHistoryView->currentIndex();

     qDebug() << "current index: "<< index.row();

     if(index.row() == -1)
     {
         showErrorMessage("无效操作！");
         return;
     }

     HistoryInfo info = m_historyInfoModel.getItem(index.row());

     QString scorejson = info.scorejson();
     QString current_data = info.scoretime();

     ShowTravlerScoreDlg showScore(scorejson, current_data);
     showScore.exec();
}
