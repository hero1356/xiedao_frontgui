#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QModelIndex>
#include <QMessageBox>
#include <QTextCodec>
#include <QJsonObject>
#include <QDebug>

/***********************************************
 *
 *              卡片管理相关
 *
************************************************/


//button slot: 查询卡片编号
void MainWindow::on_m_cardQueryBtn_clicked()
{
    QString cardSN = ui->m_cardSNInputEdit->text().trimmed();

    if(cardSN == "")
    {
        Http* pHttpFun = new Http();
        QString strUrl = dest_ip_and_port+"/card/get";
        connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                this,SLOT(slot_cardGetResult(bool,const QString&)));
        qDebug() <<"Send http: "<< strUrl;
        pHttpFun->get(strUrl);
    }else
    {
        Http* pHttpFun = new Http();
        QString strUrl  = dest_ip_and_port+"/card/get?cardsn="+cardSN;
        connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                this,SLOT(slot_cardGetResult(bool,const QString&)));
        qDebug() <<"Send http: "<< strUrl;
        pHttpFun->get(strUrl);
    }
}

//button slot: 查询卡片ID
void MainWindow::on_m_cardIDQueryBtn_clicked()
{
    QString cardID = ui->m_cardIDInputEdit->text().trimmed();

    if(cardID == "")
    {
        Http* pHttpFun = new Http();
        QString strUrl = dest_ip_and_port+"/card/get";
        connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                this,SLOT(slot_cardGetResult(bool,const QString&)));
        qDebug() <<"Send http: "<< strUrl;
        pHttpFun->get(strUrl);
    }else
    {
        Http* pHttpFun = new Http();
        QString strUrl = dest_ip_and_port+"/card/get?cardid="+cardID;
        connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                this,SLOT(slot_cardGetResult(bool,const QString&)));
        qDebug() <<"Send http: "<< strUrl;
        pHttpFun->get(strUrl);
    }
}


//button slot: 添加卡片
void MainWindow::on_m_cardAddBtn_clicked()
{
    AddCardDlg dlg;
    connect(&dlg,SIGNAL(signal_addCardSuccessed(QString,QString)),this,SLOT(slot_cardAddSuccessed(QString,QString)));
    connect(this,SIGNAL(signal_sendCardID(uint)),&dlg,SLOT(slot_receCardID(uint)));
    dlg.exec();
}


//button slot: 右键编辑
void MainWindow::onCardInfoMenuEditClicked()
{
    qDebug() << "onCardInfoMenuEditClicked";

    //获取右键位置的 index
    QModelIndex index = ui->m_cardInfoView->currentIndex();
    //获取 index 对应的数据
    QString ret = index.data(Qt::DisplayRole).toString();
    qDebug() << "ret = " << ret;

    if(ret.isEmpty())
    {
        showErrorMessage("无效操作!");
        return;
    }
    //获取对应的卡片信息
    CardInfo info(m_cardInfomodel.getItem(index.row()));
    //保存已经编辑的行
    m_modRow = index.row();


    QString strUrl;
    // 根据SN修改ID
    if(ret.length() == 6)
    {
        QString newCardID;

        ModCardInfoDlg dlg;
        dlg.setCardSN(info.cardSN());
        connect(this,SIGNAL(signal_sendCardID(uint)), &dlg, SLOT(slot_receiveCardID(uint)));
        if( dlg.exec() == QDialog::Accepted )
        {
            newCardID = dlg.cardID();
            strUrl = dest_ip_and_port+"/card/mod?direction=sn2id&cardid="+newCardID+"&cardsn="+info.cardSN();
            Http* pHttpFun = new Http();
        //    QString strUrl = "http://10.55.206.28:8687/card/del?cardid="+cardID;
            connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                    this,SLOT(slot_cardModResult(bool,const QString&)));
            qDebug() <<"Send http: "<< strUrl;
            pHttpFun->get(strUrl);

            //保存修改后的cardID 和cardSN
            m_modCardID = newCardID;
            m_modCardSN = info.cardSN();
        }
    }
    // 根据ID修改SN
    else
    {
        QString newCardSN;

        ModCardInfoDlg dlg;
        dlg.setCardID(info.cardID());
        if( dlg.exec() == QDialog::Accepted )
        {
            newCardSN = dlg.cardSN();            
            Http* pHttpFun = new Http();
            strUrl = dest_ip_and_port+"/card/mod?direction=id2sn&cardid="+info.cardID()+"&cardsn="+newCardSN;
            connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                    this,SLOT(slot_cardModResult(bool,const QString&)));
            qDebug() <<"Send http: "<< strUrl;
            pHttpFun->get(strUrl);

            //保存修改后的cardID 和cardSN
            m_modCardID = info.cardID();
            m_modCardSN = newCardSN;
        }
    }
}

//button slot: 右键删除
void MainWindow::onCardInfoMenuDeleteClicked()
{
    int r = showQueryMessage("你确定要删除选中的卡片吗？");
    if( r == QMessageBox::No )
        return;

    QModelIndex index = ui->m_cardInfoView->currentIndex();
    QString ret = index.data(Qt::DisplayRole).toString();
    qDebug() << "ret = " <<ret;
    if(ret.isEmpty())
    {
        showErrorMessage("无效操作!");
        return;
    }

    CardInfo info(m_model.getItem(index.row()));
    QString cardID = info.cardID();
    QString cardSN = info.cardSN();

    QString strUrl;
    if(ret.length() == 6)
    {
        strUrl = dest_ip_and_port+"/card/del?cardid="+ret;
    }
    else
    {
        strUrl = dest_ip_and_port+"/card/del?cardsn="+ret;
    }

    Http* pHttpFun = new Http();
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_cardDeleteResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->get(strUrl);

    //加入删除链表
    removeList.append(index.row());
}

//button slot: 右键删除全部
void MainWindow::onCardInfoMenuDeleteAllClicked()
{
    int r = showQueryMessage("你确定要删除所有的卡片吗？");
    if( r == QMessageBox::No )
        return;

    QModelIndex index = ui->m_cardInfoView->currentIndex();
    QString ret = index.data(Qt::DisplayRole).toString();
    qDebug() << "ret = " <<ret;
    if(ret.isEmpty())
    {
        showErrorMessage("无效操作!");
        return;
    }

    QString strUrl = dest_ip_and_port+"/card/del";
    Http* pHttpFun = new Http();
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_cardDeleteResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->get(strUrl);

}



//http slot: 查询卡片结果
void MainWindow::slot_cardGetResult(bool success, const QString &strResult)
{

    qDebug() <<"slot_cardGetResult: receive data:"<< strResult;

    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }

    m_cardInfomodel.clear();
    QString rslt;
    QString reason;

    QString ret = "default";
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
                    QString data1 = value.toString();
                    if(data1=="success")
                    {
                        ret = "ok";
                        setTip("卡片查询成功！");
                    }
                }
            }
            //是否含有key  reason
            if(rootObj.contains("reason"))
            {
                QJsonValue value = rootObj.value("reason");
                //判断是否是string类型
                if (value.isString())
                {
                    reason = value.toString();
                }
            }

            //是否含有key  data
            if(rootObj.contains("data"))
            {
                QJsonValue value = rootObj.value("data");
                if(value.isArray())
                {
                    QJsonArray dataArray = value.toArray();
                    for(int i=0; i<dataArray.size(); i++)
                    {
                        QJsonValue dataVal = dataArray.at(i);
                        if(dataVal.isObject())
                        {
                            QJsonObject dataObj = dataVal.toObject();
                            QString cardID = dataObj.value("cardid").toString();
                            QString cardSN = dataObj.value("cardsn").toString();
                            CardInfo info(cardID, cardSN);
                            m_cardInfomodel.add(info);
                        }
                    }
                }
            }
        }else{
            ret = "json is not object";
        }
    }

    qDebug() << ret;
    if( rslt == "failed" )
    {
        setTip("查询失败"+reason);
        showErrorMessage("查询失败！\n"+reason);
    }
}

//http slot: 删除卡片结果
void MainWindow::slot_cardDeleteResult(bool success, const QString& strResult)
{
    qDebug() << "slot_cardDeleteResult:"<<strResult;
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
        //如果删除成功，则从m_cardInfomodel中移除
        if( rslt == "success" )
        {
            for(int i=0; i<removeList.count(); i++)
            {
                m_cardInfomodel.remove(removeList[i]);
            }
            setTip("卡片删除成功！");
            showInformationMessage("卡片删除成功！");
        }
        else if(rslt == "failed")
        {
            setTip("卡片删除失败！"+reason);
            showErrorMessage("卡片删除失败！\n"+reason);
        }
    }
}

//http slot: 修改卡片结果
void MainWindow::slot_cardModResult(bool success, const QString &strResult)
{
    qDebug() << "slot_cardDeleteResult:"<<strResult;
    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }
    QString rslt;
    QString reason;

    if( jsonParse(strResult, rslt, reason ))
    {
        if( rslt == "success" )
        {
            //移除旧行
            m_cardInfomodel.remove(modRow());
            CardInfo info(modCardID(),modCardSN());
            m_cardInfomodel.add(info);
            setTip("卡片修改成功！");
            showInformationMessage("卡片修改成功！");
        }
        else if (rslt == "failed")
        {
            setTip("卡片修改失败！"+reason);
            showErrorMessage("卡片修改失败！\n"+reason);
        }
    }
}


//http slot: 添加卡片成功数
void MainWindow::slot_cardAddSuccessed(QString cardID, QString cardSN)
{
    setTip("添加卡片成功！");
    CardInfo info(cardID,cardSN);
    m_cardInfomodel.add(info);
}


//QString MainWindow::transCoding(const QString& source)
//{
//    QTextCodec* pCodec = QTextCodec::codecForName("gbk");
//    if(!pCodec) return false;
//    QString des = pCodec->toUnicode(source.toLocal8Bit());
//    return des;
//}


