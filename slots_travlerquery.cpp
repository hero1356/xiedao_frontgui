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
    QString postBody = json;

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/traveler/get";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getTravelerResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    postBody = m_et.httpPostGenerateSign(postBody);
    qDebug() << "postBody:"<< postBody;
    pHttpFun->post(strUrl,postBody);
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
    obj.insert("value", hex2dec(value));
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    QString postBody = json;

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/traveler/get";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getTravelerResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    postBody = m_et.httpPostGenerateSign(postBody);
    qDebug() << "postBody:"<< postBody;
    pHttpFun->post(strUrl,postBody);
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
    int r = showQueryMessage("您确定要退卡吗？");
    if( r == QMessageBox::No )  return;

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
     QString strUrl = dest_ip_and_port+"/user/backcard?cardid="+hex2dec(info.cardid())+"&comid=255";
     strUrl = m_et.httpGetGenerateSign(strUrl);
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
     QString strUrl = dest_ip_and_port+"/user/score?cardid="+hex2dec(info.cardid())+"&comid=255";
     strUrl = m_et.httpGetGenerateSign(strUrl);
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

    QString current_date =QDateTime::currentDateTime().toString("yyyy年MM月dd日");
    ShowTravlerScoreDlg showScore(strResult, current_date);
    showScore.exec();
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

// 右键菜单————批量退卡
void MainWindow::onTravelerListWidgetBatBackcardClicked()
{
    qDebug() <<"onTravelerListWidgetBatBackcardClicked";

    int r = showQueryMessage("你确定要将该组的成员全部退卡吗？");

    if(r == QMessageBox::No) return;

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

    //获取key
    QString key = info.teamname() + " - " + info.groupname();

    //获取key对应的list
    QList<TravlerInfo> list = m_traverInfoMap.value(key);

    m_backCardList = m_traverInfoMap.value(key);

    if(list.isEmpty())
    {
        showErrorMessage("该组成员为空！不能退卡！");
        return;
    }

    batBackCard(m_backCardList);


}


//右键菜单————打印人员对照单
void MainWindow::onTravelerListWidgetPrintClicked()
{
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


//右键菜单————编辑组信息
void MainWindow::onTravelerGroupEditClicked()
{
    qDebug() << "current row :" << ui->m_travlerInfoListWidget->currentRow();

    if( ui->m_travlerInfoListWidget->currentRow() == -1)
    {
        showErrorMessage("无效操作！");
        return;
    }

    //获取退伍信息——teamname， groupname， teamid，groupid等
    QString key = ui->m_travlerInfoListWidget->currentItem()->text();
    QStringList list = key.split('-');
    QString old_teamname = list.at(0);
    QString old_groupname = list.at(1);

    QList<TravlerInfo> infoList = m_traverInfoMap.value(key);
    if(infoList.isEmpty())
    {
        showErrorMessage("该组成员为空！");
        return;
    }
    QString teamid = infoList[0].teamid();
    QString groupid = infoList[0].groupid();

    qDebug() << "old teamname:" << old_teamname;
    qDebug() << "old groupname:" << old_groupname;
    qDebug() << "teamid:" << teamid;
    qDebug() << "groupid:" << groupid;


    ModGroupInfoDlg dlg;
    dlg.setGroupname(old_groupname.trimmed());
    dlg.setTeamname(old_teamname.trimmed());
    if( dlg.exec() == QDialog::Accepted )
    {
        QString new_teamname = dlg.getTeamname();
        QString new_groupname = dlg.getGroupname();

        qDebug() << "new_teamname : "<<new_teamname;
        qDebug() << "new_groupname : "<<new_groupname;

        QJsonObject obj;
        obj.insert("teamname", new_teamname);
        obj.insert("groupname", new_groupname);
        obj.insert("teamid", teamid);
        obj.insert("groupid", groupid);
        QJsonDocument document;
        document.setObject(obj);
        QString json(document.toJson(QJsonDocument::Compact));
        QString postBody = json;

        Http* pHttpFun = new Http();
        QString strUrl = dest_ip_and_port+"/user/group/mod";
        connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                this,SLOT(slot_modGroupResult(bool,const QString&)));
        qDebug() <<"Send http: "<< strUrl;
        postBody = m_et.httpPostGenerateSign(postBody);
        qDebug() << "postBody:"<< postBody;
        pHttpFun->post(strUrl,postBody);

    }

}
//槽函数——修改队伍信息
void MainWindow::slot_modGroupResult(bool success, const QString& strResult)
{
    qDebug() << "slot_modGroupResult :" << strResult;
    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }
    QString rslt, reason;
    if( jsonParse(strResult, rslt, reason))
    {
        if( rslt == "success")
        {
            setTip("信息修改成功！");
            on_m_queryInfoBtn1_clicked();
        }
        else if( rslt == "failed")
        {
            setTip("信息修改失败！"+reason);
            showErrorMessage("信息修改失败！\n"+reason);
        }
    }
}

//右键菜单————编辑人员信息
void MainWindow::onTravelerPersonEditClicked()
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
    ModPersonInfoDlg dlg;
    dlg.set_username(info.username());
    dlg.set_age(info.age());
    dlg.set_telephone(info.telephone());
    dlg.set_cardid(info.cardid());
    connect(this, SIGNAL(signal_sendCardID(QString)), &dlg, SLOT(slot_cardid(QString)));
    if( dlg.exec() == QDialog::Accepted )
    {
        QString username = dlg.username();
        QString age = dlg.age();
        QString telephone = dlg.telephone();
        QString cardid = dlg.cardid();
        QString ismodcard = dlg.ismodcard();

        qDebug() << "new username: " << username;
        qDebug() << "new age: " << age;
        qDebug() << "new telephone: " << telephone;
        qDebug() << "new cardid: " << cardid;


        QJsonObject obj;
        obj.insert("account", info.account());
        obj.insert("username", username);
        obj.insert("age", age);
        obj.insert("telephone", telephone);
        obj.insert("cardid", hex2dec(cardid));
        obj.insert("ismodcard",ismodcard);
        obj.insert("oldcardid",info.cardid());
        QJsonDocument document;
        document.setObject(obj);
        QString json(document.toJson(QJsonDocument::Compact));
        QString postBody = json;

        Http* pHttpFun = new Http();
        QString strUrl = dest_ip_and_port+"/user/mod";
        connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                this,SLOT(slot_modGroupResult(bool,const QString&)));
        qDebug() <<"Send http: "<< strUrl;
        postBody = m_et.httpPostGenerateSign(postBody);
        qDebug() << "postBody:"<< postBody;
        pHttpFun->post(strUrl,postBody);

    }
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
                                            info.set_cardid(dec2hex(value.toString()));

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


//批量退卡
void MainWindow::batBackCard(QList<TravlerInfo>& list)
{
    if(!list.isEmpty())
    {
        //获取所在行的信息
         TravlerInfo info = list[0];
         Http* pHttpFun = new Http();
         QString strUrl = dest_ip_and_port+"/user/backcard?cardid="+hex2dec(info.cardid())+"&comid=255";
         strUrl = m_et.httpGetGenerateSign(strUrl);
         connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                 this,SLOT(slot_travelerBackCardResult(bool,const QString&)));
         qDebug() <<"Send http: "<< strUrl;
         pHttpFun->get(strUrl);
    }
    else
    {
        setTip("全部退卡完毕！");
    }
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
                    cardid = dec2hex(value.toString());
                    str = "ok";
                }
            }

        }else{
            str = "json is not object";
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


        //从m_backCardList中移除
        if(!m_backCardList.isEmpty())
        {
            for(int i=0; i<m_backCardList.count(); i++)
            {
                if(m_backCardList[i].cardid() == cardid)
                {
                    m_backCardList.removeAt(i);
                    break;
                }
            }
            batBackCard(m_backCardList);
        }

        showInformationMessage(name+"退卡成功！");
    }
    else
    {
        showErrorMessage("退卡失败！");
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
    QString postBody = json;

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/leader/assign";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_setCaptainResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    postBody = m_et.httpPostGenerateSign(postBody);
    qDebug() << "postBody:"<< postBody;
    pHttpFun->post(strUrl,postBody);
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
