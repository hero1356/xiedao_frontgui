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

#include <QMessageBox>
#include <QMap>
#include <QStringList>
#include "OrderInfo.h"
#include <QDebug>


/***********************************************
 *
 *              查询未处理订单 相关
 *
************************************************/


//清空订单查询界面的所有内容
void MainWindow::clearOrderInfoModel()
{
    m_map.clear();
    currentIndex = 0;
    m_orderInfoModel.clear();
    ui->m_listWidget->clear();

    ui->m_orderInfoView->setColumnWidth(1, 50);
    ui->m_orderInfoView->setColumnWidth(3, 150);
    ui->m_orderInfoView->setColumnWidth(4, 150);
    ui->m_orderInfoView->setColumnWidth(8, 150);
}



//button slot : 查询未处理订单
void MainWindow::on_m_unsettledOrderQueryBtn_clicked()
{

    ui->m_unsettledOrderQueryBtn->setEnabled(false);
    ui->m_unsettledOrderQuerySectionBtn->setEnabled(false);
    ui->m_unsettledOrderQueryAllBtn->setEnabled(false);

    clearOrderInfoModel();

    QString playTime = ui->m_orderPlayTimeEdit->text();

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port + "/order/unsettled/get?ordertime='" + playTime + "'";
    strUrl = m_et.httpGetGenerateSign(strUrl);
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getUnsettledOrderResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->get(strUrl);
}

//button slot : 查询未处理订单--查询全部
void MainWindow::on_m_unsettledOrderQueryAllBtn_clicked()
{
    ui->m_unsettledOrderQueryBtn->setEnabled(false);
    ui->m_unsettledOrderQuerySectionBtn->setEnabled(false);
    ui->m_unsettledOrderQueryAllBtn->setEnabled(false);

    clearOrderInfoModel();

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port + "/order/unsettled/get";
    strUrl = m_et.httpGetGenerateSign(strUrl);
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getUnsettledOrderResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->get(strUrl);
}

//button slot : 查询未处理订单--区间全部
void MainWindow::on_m_unsettledOrderQuerySectionBtn_clicked()
{
    ui->m_unsettledOrderQueryBtn->setEnabled(false);
    ui->m_unsettledOrderQuerySectionBtn->setEnabled(false);
    ui->m_unsettledOrderQueryAllBtn->setEnabled(false);

    clearOrderInfoModel();

    QString begin = ui->m_orderBeginTimeEdit->text();
    QString end = ui->m_orderEndTimeEdit->text();

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port + "/order/unsettled/get?begintime='" + begin +"'&endtime='" + end + "'";
    strUrl = m_et.httpGetGenerateSign(strUrl);
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getUnsettledOrderResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->get(strUrl);
}
//http slot: 查询未处理订单
void MainWindow::slot_getUnsettledOrderResult(bool success,const QString& strResult)
{
    ui->m_unsettledOrderQueryBtn->setEnabled(true);
    ui->m_unsettledOrderQuerySectionBtn->setEnabled(true);
    ui->m_unsettledOrderQueryAllBtn->setEnabled(true);

    qDebug() << "slot_getUnsettledOrderResult :" << strResult;
    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }
    QString rslt, reason;
    QList<OrderInfo> orderList;
    if( unsettledOrderResultParse(strResult, rslt, reason, orderList))
    {
        if( rslt == "success")
        {
//            m_orderInfoModel.add(orderList);
            makeListWidget(orderList);
            setTip("查询成功！");
//            showInformationMessage("查询成功！");
        }
        else if( rslt == "failed")
        {
            setTip("查询失败！\n"+reason);
            showErrorMessage("查询失败！\n"+reason);
        }
    }
}

//将orderInfo解析为list
void MainWindow::makeListWidget(QList<OrderInfo> orderList)
{
    for(int i=0; i<orderList.count(); i++)
    {
        QString teamname = orderList[i].team();
        QString groupname = orderList[i].group();
        QString key = teamname + " - " + groupname;

        QList<OrderInfo> list = m_map.value(key);
        list.append(orderList[i]);
        m_map.insert(key, list);
    }

    QStringList keyList = m_map.keys();
    ui->m_listWidget->addItems(keyList);
    ui->m_listWidget->setIconSize(QSize(20,20));

    for(int i=0; i<keyList.count(); i++)
    {
        QListWidgetItem *item = ui->m_listWidget->item(i);
        item->setIcon(QIcon(":/icons/icons/team.png"));
    }

}



//未处理订单查询结果解析
bool MainWindow::unsettledOrderResultParse(const QString& strResult, QString& rslt, QString& reason, QList<OrderInfo>& list)
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
                                    OrderInfo info;

                                    QJsonObject arrayObj = array.at(i).toObject();

                                    if(arrayObj.contains("ordertime"))
                                    {
                                        QJsonValue value = arrayObj.value("ordertime");
                                        if(value.isString())
                                        {
                                            info.set_orderdate(value.toString());

                                            str = "ok";
                                        }
                                    }
                                    if(arrayObj.contains("orderno"))
                                    {
                                        QJsonValue value = arrayObj.value("orderno");
                                        if(value.isString())
                                        {
                                            info.set_orderno(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("userno"))
                                    {
                                        QJsonValue value = arrayObj.value("userno");
                                        if(value.isString())
                                        {
                                            info.set_userno(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("username"))
                                    {
                                        QJsonValue value = arrayObj.value("username");
                                        if(value.isString())
                                        {
                                            info.set_username(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("age"))
                                    {
                                        QJsonValue value = arrayObj.value("age");
                                        if(value.isString())
                                        {
                                            info.set_age(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("identitycard"))
                                    {
                                        QJsonValue value = arrayObj.value("identitycard");
                                        if(value.isString())
                                        {
                                            info.set_identitycard(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("telephone"))
                                    {
                                        QJsonValue value = arrayObj.value("telephone");
                                        if(value.isString())
                                        {
                                            info.set_telephone(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("email"))
                                    {
                                        QJsonValue value = arrayObj.value("email");
                                        if(value.isString())
                                        {
                                            info.set_email(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("workunit"))
                                    {
                                        QJsonValue value = arrayObj.value("workunit");
                                        if(value.isString())
                                        {
                                            info.set_workunit(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("team"))
                                    {
                                        QJsonValue value = arrayObj.value("team");
                                        if(value.isString())
                                        {
                                            info.set_team(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("group"))
                                    {
                                        QJsonValue value = arrayObj.value("group");
                                        if(value.isString())
                                        {
                                            info.set_group(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("teamleader"))
                                    {
                                        QJsonValue value = arrayObj.value("teamleader");
                                        if(value.isString())
                                        {
                                            info.set_teamleader(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("account"))
                                    {
                                        QJsonValue value = arrayObj.value("account");
                                        if(value.isString())
                                        {
                                            info.set_account(value.toString());
                                        }
                                    }
                                    if(arrayObj.contains("leadername"))
                                    {
                                        QJsonValue value = arrayObj.value("leadername");
                                        if(value.isString())
                                        {
                                            info.set_leadername(value.toString());
                                        }
                                    }
                                    info.set_ismakecard(false);
                                    info.set_cardsn("");
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

//m_listWidget的点击槽函数
void MainWindow::onListWidgetClicked(QListWidgetItem* item)
{

    QString key = item->text();
    QList<OrderInfo> list = m_map.value(key);
    m_orderInfoModel.clear();
    m_orderInfoModel.add(list);

    currentIndex = searchUnsettledOrderIndex();
    qDebug() << "searchUnsettledOrderIndex:" << currentIndex;
    if(currentIndex == -1)
    {
        showInformationMessage("该组成员已经全部制卡完毕！");
    }

    ui->m_orderInfoView->setColumnWidth(1, 50);
    ui->m_orderInfoView->setColumnWidth(3, 150);
    ui->m_orderInfoView->setColumnWidth(4, 150);
    ui->m_orderInfoView->setColumnWidth(8, 150);
}


//删除订单-----根据队名和组名
void MainWindow::onOrderInfoDelClicked()
{
    qDebug() << "onOrderInfoDelClicked";

    int r = showQueryMessage("您确定要删除该组成员的订单信息吗？");\
    if(r == QMessageBox::No)
    {
        return;
    }

    //获取当前的索引
    QModelIndex index = ui->m_listWidget->currentIndex();
    qDebug() << "current index:" << index.row();

    if(index.row() == -1)
    {
        showErrorMessage("无效操作！");
        return;
    }

    m_mapDeleteKey = ui->m_listWidget->currentItem()->text();
    qDebug() << "m_mapDeleteKey = " << m_mapDeleteKey;
    QStringList list = m_mapDeleteKey.split('-');
    if( list.isEmpty() )
    {
        showErrorMessage("队伍名称为空！");
        return;
    }

    //将键值对拆分为队名和组名
    QString teamname = list.at(0);
    QString groupname = list.at(1);

    QJsonObject obj;
    obj.insert("teamname", teamname.trimmed());
    obj.insert("groupname", groupname.trimmed());
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    QString postBody = json;

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/order/unsettled/del";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_orderInfoDelResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    postBody = m_et.httpPostGenerateSign(postBody);
    qDebug() << "postBody:"<< postBody;
    pHttpFun->post(strUrl,postBody);
}

//订单删除结果
void MainWindow::slot_orderInfoDelResult(bool success, const QString& strResult)
{
    qDebug() << "slot_orderInfoDelResult :" << strResult;
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
            //订单删除成功后从m_map中移除选中的项
            setTip("订单删除成功！");
            //从m_map中删除
            m_map.remove(m_mapDeleteKey);
            //从listWidet中删除
            int index = ui->m_listWidget->currentRow();
            qDebug() << "current index" << index;
            ui->m_listWidget->takeItem(index);
            //从orderinfoview中删除
            m_orderInfoModel.clear();
            ui->m_orderInfoView->setColumnWidth(1, 50);
            ui->m_orderInfoView->setColumnWidth(3, 150);
            ui->m_orderInfoView->setColumnWidth(4, 150);
            ui->m_orderInfoView->setColumnWidth(8, 150);
        }
        else if( rslt == "failed")
        {
            setTip("订单删除失败！"+reason);
            showErrorMessage("订单删除失败！\n"+reason);
        }
    }
}

