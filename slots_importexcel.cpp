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
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>


/***********************************************
 *
 *              import execl
 *
************************************************/
void MainWindow::clearPersonInfoModel()
{
    m_personInfoModel.clear();
    ui->m_personInfoView->setColumnWidth(2,150);
    ui->m_personInfoView->setColumnWidth(3,200);
    ui->m_personInfoView->setColumnWidth(4,200);
    ui->m_personInfoView->setColumnWidth(5,200);
}
//button slot: open Excel
void MainWindow::on_m_openExcelBtn_clicked()
{
    clearPersonInfoModel();
    ReadPersonInfo read;
    QString path = read.getData();
    m_personInfoModel.add(read.fetchData());
    ui->m_excelPathEdit->setText(path);
}

//button slot: cancel import
void MainWindow::on_m_importOrderCancelBtn_clicked()
{
    // 清空模型与视图
    clearPersonInfoModel();
    // 清空路径
    ui->m_excelPathEdit->clear();
}

//button slot: import order
void MainWindow::on_m_importOrderBtn_clicked()
{
    ui->m_importOrderBtn->setEnabled(false);

    // 2.获取所有人员的信息
    int row = m_personInfoModel.count();

    if(row == 0)
    {
        showErrorMessage("无效操作！");
        ui->m_importOrderBtn->setEnabled(true);
        return;
    }

    QList<PersonInfo> infoList;

    for(int i = 0; i < row; i++)
    {
        infoList.append(m_personInfoModel.getItem(i));
    }

    // 3.生成http post body
    QString postBody = makeHttpPostBody(infoList);


    // 4.发送http post方法
    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/order/unsettled/add";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_addUnsettledOrderResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    postBody = httpPostGenerateSign(postBody);
    qDebug() << "postBody:"<< postBody;
    pHttpFun->post(strUrl, postBody);
}

// 导入订单，生成http post body
QString MainWindow::makeHttpPostBody( QList<PersonInfo> infoList)
{
    QString postBody;
    QString leader;
    QString leadername = infoList[0].name();

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyyMMddhhmmsszz");

    for(int i=0; i<infoList.count(); i++)
    {
        QString uuid = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
        if( i==0 )
        {
            leader = uuid;
        }

        PersonInfo info = infoList[i];
        QJsonObject obj;

        obj.insert("ordertime", info.playTime());
        obj.insert("orderno", current_date);
        obj.insert("userno", QString::number(i+1, 10));

        obj.insert("username", info.name());
        obj.insert("age", info.age());
        obj.insert("identitycard", info.id());
        obj.insert("telephone", info.phoneNum());
        obj.insert("email", "Email");

        obj.insert("account", uuid);

        obj.insert("workunit", info.workUnit());
        obj.insert("team", info.workUnit());
        obj.insert("group", info.groupNum());
        obj.insert("teamleader", leader);
        obj.insert("leadername", leadername);

        QJsonDocument document;
        document.setObject(obj);
        QString json(document.toJson(QJsonDocument::Compact));

        postBody.append(json);
        postBody.append(",");
    }
    int i = postBody.lastIndexOf(",");
    postBody.remove(i,1);
    postBody.insert(0,'[');
    postBody.insert(postBody.count(),']');

    qDebug() << "导入未处理订单 post body :" << postBody;
    return postBody;
}



//http slot: 添加未处理订单
void MainWindow::slot_addUnsettledOrderResult(bool success,const QString& strResult)
{
    ui->m_importOrderBtn->setEnabled(true);
    qDebug() << "ui->m_importOrderBtn->setEnabled(true);";

    qDebug() << "slot_addUnsettledOrderResult :" << strResult;
    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }
    QString rslt, reason;
    if( jsonParse(strResult, rslt, reason) )
    {
        if( "success" == rslt )
        {
            setTip("订单添加成功！");
            showInformationMessage("订单添加成功！");
            on_m_importOrderCancelBtn_clicked();
        }
        else if( "failed" == rslt )
        {
            setTip("订单添加失败！"+reason);
            showErrorMessage("订单添加失败！\n"+reason);
        }
    }
}

//散客信息录入
void MainWindow::on_m_userInfoInputBtn_clicked()
{
    UserInputDlg inputDlg;
    connect(&inputDlg, SIGNAL(signal_sendPersonInfo(PersonInfo)), this, SLOT(slot_receivePersonInfo(PersonInfo)));
    inputDlg.exec();
}

void MainWindow::slot_receivePersonInfo(PersonInfo info)
{
    m_personInfoModel.add(info);
}
