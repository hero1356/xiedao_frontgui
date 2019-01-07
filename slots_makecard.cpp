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
#include <QPrintDialog>
#include <QFont>
#include <QDir>

#include <QMessageBox>
#include <QMap>
#include <QStringList>
#include "OrderInfo.h"
#include <QDebug>


//搜索未制卡的索引
int MainWindow::searchUnsettledOrderIndex()
{
   int ret = 0;
   int i=0;

   int count = m_orderInfoModel.count();
   if(count > 0)
   {
       for(i=0; i<count; i++)
       {
            OrderInfo info = m_orderInfoModel.getItem(i);

            qDebug() << "info:"<< info.username() << info.ismakecard();
            if(info.ismakecard() == false)
            {
                ret = i;
                break;
            }
       }
   }
   if(i == count)
   {
       ret = -1;
   }
   return ret;
}

//button slot : 制卡结束打印
void MainWindow::on_m_makeCardEndPrintBtn_clicked()
{

    //获取要打印的信息
    if(m_orderInfoModel.count() == 0)
    {
        showErrorMessage("无效操作！");
        return;
    }

    QString teamname = m_groupInfo.teamname();
    QString groupname = m_groupInfo.groupname();
    QString leader = m_orderInfoModel.index(0, 6).data(Qt::DisplayRole).toString();;
    QString guider = m_guider;
    int count = m_orderInfoModel.count();
    QString number = QString::number(m_orderInfoModel.count(), 10);

    QString p_info;

    for(int i=0; i<count; i++)
    {
        QModelIndex index_cardsn = m_orderInfoModel.index(i, 11);
        QModelIndex index_username = m_orderInfoModel.index(i, 0);
        QModelIndex index_telephone = m_orderInfoModel.index(i, 2);

        QString cardsn = index_cardsn.data(Qt::DisplayRole).toString();
        QString username = index_username.data(Qt::DisplayRole).toString();
        QString telephone = index_telephone.data(Qt::DisplayRole).toString();

        p_info.append(QString(cardsn+"\t\t"+username+"\t\t"+telephone+"\n"));
    }

    localPrint(teamname, groupname, leader, guider, number, p_info );
    realPrint(teamname, groupname, leader, guider, number, p_info );
}


/***********************************************
 *
 *              制卡 相关
 *
************************************************/


//button slot : 开始批量制卡
void MainWindow::on_m_beginMarkCardBatchBtn_clicked()
{
    if(!isSerialPortConnected())
    {
        showErrorMessage("串口未连接！请先连接串口...");
        serialOpen();
        return;
    }
    if( m_isStartMarkCard == false)
    {
        m_isStartMarkCard = true;
        ui->m_beginMarkCardBatchBtn->setVisible(false);
        ui->m_StopMarkCardBatchBtn->setVisible(true);
        connect(this, SIGNAL(signal_sendCardID(uint)), this, SLOT(slot_startMakeCard(uint)));
        qDebug() <<"connect(this, SIGNAL(signal_sendCardID(uint)), this, SLOT(slot_startMakeCard(uint)));";
    }
}
//button slot : 停止批量制卡
void MainWindow::on_m_StopMarkCardBatchBtn_clicked()
{
    if( m_isStartMarkCard == true)
    {
        m_isStartMarkCard = false;
        ui->m_beginMarkCardBatchBtn->setVisible(true);
        ui->m_StopMarkCardBatchBtn->setVisible(false);
        disconnect(this, SIGNAL(signal_sendCardID(uint)), this, SLOT(slot_startMakeCard(uint)));
        qDebug()<<"disconnect(this, SIGNAL(signal_sendCardID(uint)), this, SLOT(slot_startMakeCard(uint)));";
    }
}

// 开始制卡：
void MainWindow::slot_startMakeCard(unsigned int cardid)
{
    qDebug() << "slot_startMakeCard:"<< cardid;


    //获取刷卡卡号
    QString cardID(QString::number(cardid,10));

    //获取当前行需要的数据
    OrderInfo info(m_orderInfoModel.getItem(currentIndex));
    QString teamname  = info.team();
    QString groupname = info.group();

    //发送http查询cardID是否有效
     QString postBody = makeCardidQueryBody(cardID, teamname, groupname);
     Http* pHttpFun = new Http();
     QString strUrl = dest_ip_and_port + "/user/makegroup";
     connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
             this,SLOT(slot_makeCardQueryResult(bool,const QString&)));
     qDebug() <<"Send http: "<< strUrl;
     postBody = httpPostGenerateSign(postBody);
     qDebug() << "postBody:"<< postBody;
     pHttpFun->post(strUrl,postBody);
}
 //查询cardID是否有效HTTP body
QString MainWindow::makeCardidQueryBody(QString cardid, QString teamname, QString groupname)
{
    QString postBody;
    QJsonObject obj;

    obj.insert("cardid", cardid);
    obj.insert("teamname", teamname);
    obj.insert("groupname", groupname);

    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));

    postBody = json;
    qDebug() << "postbody:"<<postBody;
    return postBody;
}

//http slot: 卡号查询结果
void MainWindow::slot_makeCardQueryResult(bool success,const QString& strResult)
{
    qDebug() << "slot_makeCardQueryResult :" << strResult;
    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }
    QString rslt, reason;
    if( makeCardQueryResultParse(strResult, rslt, reason, m_groupInfo) )
    {
        if( rslt == "success")
        {
         // 讲键值对存储到QMap
//            m_groupCache.insert(groupinfo.key(),groupinfo);
         // 发送制卡请求
            setTip("查询成功!正在制卡...");
            m_groupid = m_groupInfo.groupid();
            sendTransferRequest(m_groupInfo.cardid(), m_groupInfo.cardsn(), m_groupInfo.teamid(), m_groupInfo.groupid());
        }
        else if( rslt == "failed")
        {
            setTip("制卡失败！"+reason);
            showErrorMessage("制卡失败！\n"+reason);
        }
    }
}

//发送制卡请求
void MainWindow::sendTransferRequest(QString cardid, QString cardsn, QString teamid, QString groupid)
{
    OrderInfo orderInfo = m_orderInfoModel.getItem(currentIndex);

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyyMMddhhmmss");

    QString postBody;
    QJsonObject obj;
    obj.insert("ordertime", orderInfo.orderdate());
    obj.insert("orderno", orderInfo.orderno());
    obj.insert("userno", orderInfo.userno());
    obj.insert("identitycard", orderInfo.identitycard());
    obj.insert("team", orderInfo.team());
    obj.insert("group", orderInfo.group());
    obj.insert("operator", currentOperator);
    obj.insert("opertime", current_date);

    obj.insert("account", orderInfo.account());
    obj.insert("cardid", cardid);
    obj.insert("cardsn", cardsn);
    obj.insert("username", orderInfo.username());
    obj.insert("age", orderInfo.age());
    obj.insert("workunit", orderInfo.workunit());
    obj.insert("email", "email");
    obj.insert("telephone", orderInfo.telephone());
    obj.insert("teamleader", orderInfo.teamleader());
    obj.insert("leadername",orderInfo.leadername());
    obj.insert("groupid", groupid);
    obj.insert("teamid", teamid);
    obj.insert("guider", "");
    obj.insert("role", "traveler");

    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));

    postBody = json;
    qDebug() << "postBody" << postBody;

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/makecard";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_transferResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    postBody = httpPostGenerateSign(postBody);
    qDebug() << "postBody:"<< postBody;
    pHttpFun->post(strUrl, postBody);
}

// 制卡结果
void MainWindow::slot_transferResult(bool success,const QString& strResult)
{
    qDebug() << "slot_transferResult:"<<strResult;

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
            setTip("制卡成功！");
            qDebug() <<m_orderInfoModel.getItem(currentIndex).username()+"-"+m_groupInfo.cardsn()+"-制卡成功！";
            qDebug() <<"currentIndex = "<< currentIndex;

            // 1. 设置视图显示，制卡状态+cardSN
            QModelIndex index_ismakecard = m_orderInfoModel.index(currentIndex, 10);
            m_orderInfoModel.setData(index_ismakecard, true);
            QModelIndex index_cardsn = m_orderInfoModel.index(currentIndex, 11);
            m_orderInfoModel.setData(index_cardsn, m_groupInfo.cardsn());

            // 2. 设置m_map中的数据
            qDebug() << "list widget index:" << ui->m_listWidget->currentItem()->text();

            QString key = ui->m_listWidget->currentItem()->text();

            qDebug() << "key:" << key;

            QList<OrderInfo> orderList = m_map.value(key);

            OrderInfo info = orderList.at(currentIndex);
            info.set_ismakecard(true);
            info.set_cardsn(m_groupInfo.cardsn());
            orderList.replace(currentIndex, info);

            m_map.insert(key,orderList);



            // 3. 制卡未全部结束currentIndex++，
            if(currentIndex < m_orderInfoModel.count())
            {
                currentIndex++;
            }

            // 4. 若制卡结束，清空currentIndex, 提示用户设置导游，打印对照表等
            if(currentIndex == m_orderInfoModel.count())
            {
                currentIndex = 0;
                qDebug() <<"全部制卡完成！";
                qDebug() <<"currentIndex = "<< currentIndex;
                on_m_StopMarkCardBatchBtn_clicked();
                int r = showQueryMessage("已全部制卡完毕！\n请设置导游！");
                if(r == QMessageBox::Yes)
                {
                //查询空闲导游并设置队伍导游
                    getGuider("idle");
                }

                //打印人员对照单
//                on_m_makeCardEndPrintBtn_clicked();
            }
        }
        else if(rslt == "failed")
        {
            setTip("制卡失败！"+reason);
            showErrorMessage("制卡失败！\n"+reason);
        }
    }
}

//卡号查询结果解析。
bool MainWindow::makeCardQueryResultParse(const QString& strResult, QString &rslt, QString &reason, GroupInfo& groupinfo)
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
                //判断是否是string类型
                if (value.isString())
                {
                    rslt = value.toString();
                }
            }


            //是否含有key  data
            if(rootObj.contains("data"))
            {
                QJsonValue value = rootObj.value("data");
                if (value.isObject())
                {
                    QJsonObject obj = value.toObject();
                    if(obj.contains("cardid"))
                    {
                        QJsonValue value = obj.value("cardid");
                        if (value.isString())
                        {
                            groupinfo.set_cardid(value.toString());
                            str = "ok";
                        }
                    }

                    if(obj.contains("cardsn"))
                    {
                        QJsonValue value = obj.value("cardsn");
                        if (value.isString())
                        {
                            groupinfo.set_cardsn(value.toString());
                        }
                    }

                    if(obj.contains("teamname"))
                    {
                        QJsonValue value = obj.value("teamname");
                        if (value.isString())
                        {
                            groupinfo.set_teamname(value.toString());
                        }
                    }

                    if(obj.contains("teamid"))
                    {
                        QJsonValue value = obj.value("teamid");
                        if (value.isString())
                        {
                            groupinfo.set_teamid(value.toString());
                        }
                    }

                    if(obj.contains("groupname"))
                    {
                        QJsonValue value = obj.value("groupname");
                        if (value.isString())
                        {
                            groupinfo.set_groupname(value.toString());
                        }
                    }

                    if(obj.contains("groupid"))
                    {
                        QJsonValue value = obj.value("groupid");
                        if (value.isString())
                        {
                            groupinfo.set_groupid(value.toString());
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



/***********************************************
 *
 *              设置导游 相关
 *
************************************************/

// 查询导游
void MainWindow::getGuider(QString state)
{
    QJsonObject obj;
    obj.insert("state", state);
    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));
    QString postBody = json;

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/guider/get";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_getGuiderResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    postBody = httpPostGenerateSign(postBody);
    qDebug() << "postBody:"<< postBody;
    pHttpFun->post(strUrl,postBody);
}

// 查询导游结果
void MainWindow::slot_getGuiderResult(bool success, const QString& strResult)
{
    qDebug() << "slot_getGuiderResult:"<<strResult;
    if( !success )
    {
        setTip("请求失败！"+strResult);
        showErrorMessage("请求失败！\n"+strResult);
        return;
    }
    QString rslt, reason;
    QStringList guiderList;
    if(getGuiderResultParse(strResult, rslt, reason, guiderList))
    {
        if(rslt == "success")
        {
            if(guiderList.isEmpty())
            {
                showErrorMessage("没有可用的导游！");
                //打印
//                on_m_makeCardEndPrintBtn_clicked();
                return;
            }

            //弹窗显示导游表
            SetLeaderAndGuiderDlg  dlg(guiderList);
            int ret = dlg.exec();
            if (ret == QDialog::Accepted)
            {
                qDebug() <<"已选择的导游："<< dlg.guider();
                //设置导游信息
                m_guider = dlg.guider();
                setGuider(m_groupid, m_guider);
            }
            else
            {
                //打印
//                on_m_makeCardEndPrintBtn_clicked();
            }

        }
        else if(rslt == "failed")
        {
            setTip("导游信息查询失败！"+reason);
            showErrorMessage("导游信息查询失败！\n"+reason);
        }

    }
}

// 设置导游
void MainWindow::setGuider(QString groupid, QString guider)
{
    QString postBody;
    QJsonObject obj;

    obj.insert("groupid", groupid);
    obj.insert("account", guider);

    QJsonDocument document;
    document.setObject(obj);
    QString json(document.toJson(QJsonDocument::Compact));

    postBody = json;
    qDebug() << "postbody:"<<postBody;

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/user/guider/assign";
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_setGuiderResult(bool,const QString&)));
    qDebug() <<"Send http: "<< strUrl;
    postBody = httpPostGenerateSign(postBody);
    qDebug() << "postBody:"<< postBody;
    pHttpFun->post(strUrl, postBody);

}

// 设置导游结果
void MainWindow::slot_setGuiderResult(bool success, const QString& strResult)
{
    qDebug() << "slot_setGuiderResult:"<<strResult;

    if( !success )
    {
        m_guider = "";
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
            setTip("设置导游成功！");
            showInformationMessage("设置导游成功！");

            if(ui->stackedWidget->currentIndex() == 2)
            {
                on_m_queryInfoBtn1_clicked();
            }
            emit signal_setGuider(true);
        }
        else if(rslt == "failed")
        {
            m_guider = "";
            setTip("设置导游失败！"+reason);
            showErrorMessage("设置导游失败！\n"+reason);
        }

        //打印
//        on_m_makeCardEndPrintBtn_clicked();
    }

}

//查询导游信息结果解析
bool MainWindow::getGuiderResultParse(const QString& strResult, QString &rslt, QString &reason, QStringList& list)
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
                //判断是否是string类型
                if (value.isString())
                {
                    rslt = value.toString();
                    str = "ok";
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

            //是否含有key  data
            if(rootObj.contains("data"))
            {
                QJsonValue value = rootObj.value("data");
                if (value.isArray())
                {
                    QJsonArray array = value.toArray();
                    list.clear();
                    for(int i=0; i<array.count(); i++)
                    {
                        QJsonValue arrayValue = array[i];
                        if(arrayValue.isObject())
                        {
                            QJsonObject arrayObj = arrayValue.toObject();
                            if(arrayObj.contains("account"))
                            {
                                QJsonValue value = arrayObj.value("account");
                                if(value.isString())
                                {
                                    list.append(value.toString());
                                    str = "ok";
                                }
                            }
                        }
                    }
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


void MainWindow::localPrint(QString teamname, QString groupname, QString leader, QString guider, QString number, QString info)
{

    QString p_teamname = "队名：" + teamname;
    QString p_groupname = "组名："+ groupname;
    QString p_leader = "队长：" + leader + "\t导游："+guider+"\t人数：" + number + "\n";
    QString p_info = info;

    QFont font_title(QFont("隶书", 24));
    QFont font_teamname(QFont("隶书", 14));
    QFont font_groupname(QFont("隶书", 14));
    QFont font_leader(QFont("隶书", 14));
    QFont font_info(QFont("楷书", 12));

    //开始排版
    QTextEdit* edit = new QTextEdit;
    QTextDocument* doc = edit->document();
    QTextCursor cursor = edit->textCursor();
    QTextFrame *root_frame = doc->rootFrame();
    QTextFrameFormat root_frame_format = root_frame->frameFormat();//创建框架格式
    root_frame_format.setMargin(250);
    root_frame->setFrameFormat(root_frame_format); //给框架使用格式

    QTextBlockFormat textBlockFormat;
//    textBlockFormat.setTopMargin(15);//设置段前行距：15
//    textBlockFormat.setBottomMargin(15);//设置段后行距：15
//    textBlockFormat.setLeftMargin(20);//设置段左行距：20
//    textBlockFormat.setRightMargin(20);//设置段右行距：20
//    textBlockFormat.setLineHeight(100,QTextBlockFormat::ProportionalHeight);//设置行间距，单倍行距

    // 显示标题段落
    {
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_title);
        //段落格式

        textBlockFormat.setAlignment(Qt::AlignHCenter);//对其方式：居中对其
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText("人员对照表",char_format);
    }

    // 显示队名
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_teamname);
        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);//对其方式：左对齐
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_teamname,char_format);
    }

    // 显示组名
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_groupname);
        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);//对其方式：居中对齐
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_groupname,char_format);
    }

    // 显示队长
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_leader);

        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_leader,char_format);
    }

    // 显示名单
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_info);
        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_info,char_format);
    }


    // 打印到本地文件夹"D:\\TeamInfo\"
    qDebug() << "==================  开始缓存 =============";
    QPrinter p;
    qDebug() << p.printerName();
    p.setOrientation(QPrinter::Portrait);   //设置为竖向
    p.setPaperSize(QPrinter::A4);            //设置纸张为A4
    QTextDocument *td = edit->document();
    QString current_date =QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString path = QString("D:\\TeamInfo\\%1 - %2 - %3.pdf").arg(current_date).arg(teamname).arg(groupname);
    p.setOutputFileName(path);
    td->print(&p);
    qDebug() << "================== 本地缓存成功 "+ path+"============";



}
void MainWindow::realPrint(QString teamname, QString groupname, QString leader, QString guider, QString number, QString info)
{

    QString p_teamname = "队名：" + teamname;
    QString p_groupname = "组名："+ groupname;
    QString p_leader = "队长：" + leader + "\t导游："+guider+"\t人数：" + number + "\n";
    QString p_info = info;

    QFont font_title(QFont("隶书", 24));
    QFont font_teamname(QFont("隶书", 14));
    QFont font_groupname(QFont("隶书", 14));
    QFont font_leader(QFont("隶书", 14));
    QFont font_info(QFont("楷书", 12));

    //开始排版
    QTextEdit* edit = new QTextEdit;
    QTextDocument* doc = edit->document();
    QTextCursor cursor = edit->textCursor();
    QTextFrame *root_frame = doc->rootFrame();
    QTextFrameFormat root_frame_format = root_frame->frameFormat();//创建框架格式
    root_frame_format.setMargin(250);
    root_frame->setFrameFormat(root_frame_format); //给框架使用格式

    QTextBlockFormat textBlockFormat;
//    textBlockFormat.setTopMargin(15);//设置段前行距：15
//    textBlockFormat.setBottomMargin(15);//设置段后行距：15
//    textBlockFormat.setLeftMargin(20);//设置段左行距：20
//    textBlockFormat.setRightMargin(20);//设置段右行距：20
//    textBlockFormat.setLineHeight(100,QTextBlockFormat::ProportionalHeight);//设置行间距，单倍行距

    // 显示标题段落
    {
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_title);
        //段落格式

        textBlockFormat.setAlignment(Qt::AlignHCenter);//对其方式：居中对其
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText("人员对照表",char_format);
    }

    // 显示队名
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_teamname);
        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);//对其方式：左对齐
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_teamname,char_format);
    }

    // 显示组名
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_groupname);
        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);//对其方式：居中对齐
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_groupname,char_format);
    }

    // 显示队长
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_leader);

        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_leader,char_format);
    }

    // 显示名单
    {
        cursor.insertBlock();
        // 字体格式
        QTextCharFormat char_format;
        char_format.setFont(font_info);
        // 段落格式
        textBlockFormat.setAlignment(Qt::AlignLeft);
        cursor.setBlockFormat(textBlockFormat);
        // 插入内容
        cursor.insertText(p_info,char_format);
    }


    // 调用本地打印机打印
    qDebug() << "==================  开始打印 =============";
    QPrintDialog dlg(this);
    dlg.setWindowTitle("打印");
    if(dlg.exec() == QPrintDialog::Accepted )
    {
        QPrinter* p1 = dlg.printer();
        p1->setOrientation(QPrinter::Portrait);   //设置为竖向
        p1->setPaperSize(QPrinter::A4);            //设置纸张为A4
        QTextDocument *td = edit->document();
        td->print(p1);
    }
    qDebug() << "==================  打印成功 =============";
}

