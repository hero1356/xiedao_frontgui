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
#include <QMessageBox>
#include <QDebug>

//清空游客查询界面的所有内容
void MainWindow::clearTravlerInfoModel()
{
    m_traverInfoModel.clear();
    ui->m_travlerInfoListWidget->clear();
    m_traverInfoMap.clear();

    ui->m_travlerInfoView->setColumnWidth(5, 150);
}

// 查询游客信息 ————姓名，手机号，团队名称等
void MainWindow::on_m_queryInfoBtn1_clicked()
{
    ui->m_queryInfoBtn1->setEnabled(false);
    ui->m_queryInfoBtn2->setEnabled(false);

    clearTravlerInfoModel();
    QString condition;
    QString value;
    int index = ui->m_queryInfoCb->currentIndex();
    switch (index) {
    // 姓名
    case 0:
        condition = "username";
        qDebug() << "index = 0" << ui->m_queryInfoCb->currentText();
        value = ui->m_queryInfoEdit1->text().trimmed();
        if(value.isEmpty())
        {
            showInformationMessage("请输入...");
            ui->m_queryInfoBtn1->setEnabled(true);
            ui->m_queryInfoBtn2->setEnabled(true);
            return;
        }
        break;

        // 手机号
    case 1:
        condition = "telephone";
        qDebug() << "index = 1" << ui->m_queryInfoCb->currentText();
        value = ui->m_queryInfoEdit1->text().trimmed();
        if(value.isEmpty())
        {
            showInformationMessage("请输入...");
            ui->m_queryInfoBtn1->setEnabled(true);
            ui->m_queryInfoBtn2->setEnabled(true);
            return;
        }
        break;

    // 团队名称
    case 2:
        condition = "teamname";
        qDebug() << "index = 2" << ui->m_queryInfoCb->currentText();
        value = ui->m_queryInfoEdit1->text().trimmed();
        if(value.isEmpty())
        {
            showInformationMessage("请输入...");
            ui->m_queryInfoBtn1->setEnabled(true);
            ui->m_queryInfoBtn2->setEnabled(true);
            return;
        }
        break;

    // 全部
    case 3:
        condition = "all";
        qDebug() << "index = 3" << ui->m_queryInfoCb->currentText();
        value = "";
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
    QString strUrl = dest_ip_and_port+"/user/traveler/get";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getTravelerResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->post(strUrl,body);
}

//查询游客信息————使用卡号
void MainWindow::on_m_queryInfoBtn2_clicked()
{
    ui->m_queryInfoBtn1->setEnabled(false);
    ui->m_queryInfoBtn2->setEnabled(false);

    clearTravlerInfoModel();

    QString condition = "cardid";
    QString value = ui->m_queryInfoEdit2->text();

    if(value.isEmpty())
    {
        showErrorMessage("无效操作！");
        ui->m_queryInfoBtn1->setEnabled(true);
        ui->m_queryInfoBtn2->setEnabled(true);
        return;
    }


    QJsonObject obj;
    obj.insert("condition", condition);
    obj.insert("value", value);
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    QString body = json;

    qDebug() <<"post body: "<< body;
    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/traveler/get";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getTravelerResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->post(strUrl,body);
}

//查询游客信息返回结果
void MainWindow::slot_getTravelerResult(bool success,const QString& strResult)
{
    ui->m_queryInfoBtn1->setEnabled(true);
    ui->m_queryInfoBtn2->setEnabled(true);

    qDebug() << "slot_getTravelerResult :" << strResult;
    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }
    QString rslt, reason;
    QList<TravlerInfo> travlerList;
    if( travelerResultParse(strResult, rslt, reason, travlerList))
    {
        if( rslt == "success")
        {
            if(travlerList.isEmpty())
            {
                showInformationMessage("没有查询到结果！");
            }
            setTip("查询成功！"+reason);

            m_traverInfoModel.add(travlerList);

            makeTravlerInfoListWidget(travlerList);
        }
        else if( rslt == "failed")
        {
            setTip("查询失败！"+reason);
            showErrorMessage("查询失败！\n"+reason);
        }
    }
}

//按组分类travlerinfo信息
void MainWindow::makeTravlerInfoListWidget(QList<TravlerInfo> list)
{
    for(int i=0; i<list.count(); i++)
    {
        QString teamname = list[i].teamname();
        QString groupname = list[i].groupname();
        QString key = teamname + " - " + groupname;

        QList<TravlerInfo> infoList = m_traverInfoMap.value(key);
        infoList.append(list[i]);
        m_traverInfoMap.insert(key, infoList);
    }

    QStringList keyList = m_traverInfoMap.keys();
    ui->m_travlerInfoListWidget->addItems(keyList);
    ui->m_travlerInfoListWidget->setIconSize(QSize(20,20));

    for(int i=0; i<keyList.count(); i++)
    {
        QListWidgetItem *item = ui->m_travlerInfoListWidget->item(i);
        item->setIcon(QIcon(":/icons/icons/team.png"));
    }
}

//右键菜单————退卡
void MainWindow::onTravelerBackCardClicked()
{
    //获取右键位置的 index
    QModelIndex index = ui->m_travlerInfoView->currentIndex();

    qDebug() << "Current index:" << index.row();
    if(index.row() == -1)
    {
        showErrorMessage("无效操作");
        return;
    }

    //获取所在行的信息
     TravlerInfo info(m_traverInfoModel.getItem(index.row()));

     Http* pHttpFun = new Http();
     QString strUrl = dest_ip_and_port+"/user/backcard?cardid="+info.cardid()+"&comid=255";
     connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
             this,SLOT(slot_travelerBackCardResult(bool,const QString&)));
     qDebug() <<"Send http: "<< strUrl;
     pHttpFun->get(strUrl);

}

//右键菜单————查询成绩
void MainWindow::onTravelerScoreClicked()
{
    //获取右键位置的 index
    QModelIndex index = ui->m_travlerInfoView->currentIndex();

    qDebug() << "Current index:" << index.row();
    if(index.row() == -1)
    {
        showErrorMessage("无效操作");
        return;
    }

    //获取所在行的信息
     TravlerInfo info(m_traverInfoModel.getItem(index.row()));


     Http* pHttpFun = new Http();
     QString strUrl = dest_ip_and_port+"/user/score?cardid="+info.cardid()+"&comid=255";
     connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
             this,SLOT(slot_getTravelerScoreResult(bool,const QString&)));
     qDebug() <<"Send http: "<< strUrl;
     pHttpFun->get(strUrl);

}

//成绩查询结果
void MainWindow::slot_getTravelerScoreResult(bool success, const QString& strResult)
{
    qDebug() << "slot_getTravelerScoreResult:" << strResult;

    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }

    ShowTravlerScoreDlg showScore(strResult);
    showScore.exec();

}

//退卡结果
void MainWindow::slot_travelerBackCardResult(bool success, const QString& strResult)
{
    qDebug() << "slot_travelerBackCardResult:" << strResult;

    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }

    QString rslt, name,cardid;
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
                //判断是否是string类型
                if (value.isString())
                {
                    rslt = value.toString();
                    str = "ok";
                }
            }
            //是否含有key  name
            if(rootObj.contains("name"))
            {
                QJsonValue value = rootObj.value("name");
                //判断是否是string类型
                if (value.isString())
                {
                    name = value.toString();
                    str = "ok";
                }
            }
            //是否含有key  cardid
            if(rootObj.contains("cardid"))
            {
                QJsonValue value = rootObj.value("cardid");
                //判断是否是string类型
                if (value.isString())
                {
                    cardid = value.toString();
                    str = "ok";
                }
            }

        }else{
            str = "json is not object";
            ret = false;
        }
    }
    qDebug() << str;
    if(rslt == "ok")
    {
        setTip(name+"退卡成功！");

        //从model中删除该行数据
        for(int i=0; i<m_traverInfoModel.count(); i++)
        {
            if(m_traverInfoModel.getItem(i).cardid() == cardid)
            {
                m_traverInfoModel.remove(i);
                break;
            }
        }

        //从m_traverInfoMap中移除
        QStringList keys = m_traverInfoMap.keys();
        if(keys.isEmpty())
            return;
        for(int i=0; i< keys.count(); i++)
        {
            QList<TravlerInfo> infoList = m_traverInfoMap.value(keys[i]);

            for(int j=0; j<infoList.count(); j++)
            {
                if(infoList[j].cardid() == cardid)
                {
                    infoList.removeAt(j);
                    m_traverInfoMap.insert(keys[i],infoList);
                    if(infoList.isEmpty())
                    {
                        m_traverInfoMap.remove(keys[i]);
                    }
                    break;
                }
            }
            if(infoList.isEmpty())
            {
                QListWidgetItem *item = ui->m_travlerInfoListWidget->takeItem(i);

                delete item;

                m_traverInfoMap.remove(keys[i]);
            }
        }

        showInformationMessage(name+"退卡成功！");

    }
}

//右键菜单————设为队长
void MainWindow::onTravelerSetCaptainClicked()
{

    //获取右键位置的 index
    QModelIndex index = ui->m_travlerInfoView->currentIndex();

    qDebug() << "Current index:" << index.row();
    if(index.row() == -1)
    {
        showErrorMessage("无效操作");
        return;
    }

    //获取所在行的信息
     TravlerInfo info(m_traverInfoModel.getItem(index.row()));

     //获取选中行的account
     QString account = info.account();
     QString leadername = info.username();


     //获取选中行的teamid
     QString teamid = info.teamid();

     setCaptain(account, leadername, teamid);

}

//右键菜单————设置导游
void MainWindow::onTravelerSetGuiderClicked()
{
    //获取右键位置的 index
    QModelIndex index = ui->m_travlerInfoView->currentIndex();

    qDebug() << "Current index:" << index.row();
    if(index.row() == -1)
    {
        showErrorMessage("无效操作");
        return;
    }

    //获取所在行的信息
     TravlerInfo info(m_traverInfoModel.getItem(index.row()));

    m_groupid = info.groupid();

    getGuider("idle");
}


//游客查询结果解析
bool MainWindow::travelerResultParse(const QString& strResult, QString& rslt, QString& reason, QList<TravlerInfo>& list)
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
                                    TravlerInfo info;

                                    QJsonObject arrayObj = array.at(i).toObject();

                                    if(arrayObj.contains("account"))
                                    {
                                        QJsonValue value = arrayObj.value("account");
                                        if(value.isString())
                                        {
                                            info.set_account(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("username"))
                                    {
                                        QJsonValue value = arrayObj.value("username");
                                        if(value.isString())
                                        {
                                            info.set_username(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("age"))
                                    {
                                        QJsonValue value = arrayObj.value("age");
                                        if(value.isString())
                                        {
                                            info.set_age(value.toString());

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
                                    if(arrayObj.contains("email"))
                                    {
                                        QJsonValue value = arrayObj.value("email");
                                        if(value.isString())
                                        {
                                            info.set_email(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("cardid"))
                                    {
                                        QJsonValue value = arrayObj.value("cardid");
                                        if(value.isString())
                                        {
                                            info.set_cardid(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("cardsn"))
                                    {
                                        QJsonValue value = arrayObj.value("cardsn");
                                        if(value.isString())
                                        {
                                            info.set_cardsn(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("teamid"))
                                    {
                                        QJsonValue value = arrayObj.value("teamid");
                                        if(value.isString())
                                        {
                                            info.set_teamid(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("teamname"))
                                    {
                                        QJsonValue value = arrayObj.value("teamname");
                                        if(value.isString())
                                        {
                                            info.set_teamname(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("groupid"))
                                    {
                                        QJsonValue value = arrayObj.value("groupid");
                                        if(value.isString())
                                        {
                                            info.set_groupid(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("groupname"))
                                    {
                                        QJsonValue value = arrayObj.value("groupname");
                                        if(value.isString())
                                        {
                                            info.set_groupname(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("teamleader"))
                                    {
                                        QJsonValue value = arrayObj.value("teamleader");
                                        if(value.isString())
                                        {
                                            info.set_teamleader(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("leadername"))
                                    {
                                        QJsonValue value = arrayObj.value("leadername");
                                        if(value.isString())
                                        {
                                            info.set_leadername(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("guider"))
                                    {
                                        QJsonValue value = arrayObj.value("guider");
                                        if(value.isString())
                                        {
                                            info.set_guider(value.toString());

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

//travlerinfo listwidget的点击操作
void MainWindow::onTravelerListWidgetClicked(QListWidgetItem* item)
{
    QString key = item->text();
    QList<TravlerInfo> list = m_traverInfoMap.value(key);
    m_traverInfoModel.clear();
    m_traverInfoModel.add(list);

    ui->m_travlerInfoView->setColumnWidth(5, 150);
}


//右键打印
void MainWindow::onTravelerListWidgetPrintClicked()
{
    qDebug() <<"onTravelerListWidgetPrintClicked";

    qDebug() << "current row" << ui->m_travlerInfoListWidget->currentRow();

    if(ui->m_travlerInfoListWidget->currentRow() == -1)
    {
        showErrorMessage("无效操作！");
        return;
    }

    QString key = ui->m_travlerInfoListWidget->currentItem()->text();
    QList<TravlerInfo> list = m_traverInfoMap.value(key);

    if(list.isEmpty())
    {
        showErrorMessage("该组成员为空！不能打印！");
        return;
    }


    //获取要打印的信息
    QString teamname = list[0].teamname();
    QString groupname = list[0].groupname();
    QString leader = list[0].leadername();
    QString guider = list[0].guider();
    int count = list.count();
    QString number = QString::number(count, 10);

    QString p_info;

    for(int i=0; i<count; i++)
    {
        QString cardsn = list[i].cardsn();
        QString username = list[i].username();
        QString telephone = list[i].telephone();

        p_info.append(QString(cardsn+"\t"+username+"\t"+telephone+"\n"));
    }

    realPrint(teamname, groupname, leader, guider, number, p_info );

}

// 右键批量退卡
void MainWindow::onTravelerListWidgetBatBackcardClicked()
{
    qDebug() <<"onTravelerListWidgetBatBackcardClicked";

    int r = showQueryMessage("你确定要将该组的成员全部退卡吗？");

    if(r == QMessageBox::No) return;


    qDebug() << "current row" << ui->m_travlerInfoListWidget->currentRow();

    if(ui->m_travlerInfoListWidget->currentRow() == -1)
    {
        showErrorMessage("无效操作！");
        return;
    }

    QString key = ui->m_travlerInfoListWidget->currentItem()->text();
    QList<TravlerInfo> list = m_traverInfoMap.value(key);

    if(list.isEmpty())
    {
        showErrorMessage("该组成员为空！不能退卡！");
        return;
    }

    for(int i=0; i<list.count(); i++)
    {
        //获取所在行的信息
         TravlerInfo info(m_traverInfoModel.getItem(i));
         Http* pHttpFun = new Http();
         QString strUrl = dest_ip_and_port+"/user/backcard?cardid="+info.cardid()+"&comid=255";
         connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                 this,SLOT(slot_travelerBackCardResult(bool,const QString&)));
         qDebug() <<"Send http: "<< strUrl;
         pHttpFun->get(strUrl);
    }
}

/***********************************************
 *
 *              设置队长 相关
 *
************************************************/

// 设置队长
void MainWindow::setCaptain(QString account, QString leadername,QString teamid)
{
    QJsonObject obj;
    obj.insert("account", account);
    obj.insert("leadername", leadername);
    obj.insert("teamid", teamid);
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    QString body = json;
    qDebug() << "postbody:" << body;

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/leader/assign";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_setCaptainResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->post(strUrl,body);
}

// 设置队长结果解析
void MainWindow::slot_setCaptainResult(bool success, const QString& strResult)
{
    qDebug() << "slot_setCaptainResult:"<<strResult;

    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }
    QString rslt;
    QString reason;

    if( jsonParse(strResult, rslt, reason))
    {
        if( rslt == "success" )
        {
            setTip("设置队长成功！");
            showInformationMessage("设置队长成功！");
            on_m_queryInfoBtn1_clicked();
        }
        else if(rslt == "failed")
        {
            setTip("设置队长失败！"+reason);
            showErrorMessage("设置队长失败！\n"+reason);
        }
    }
}
