#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMenu>
#include <QList>
#include <QLabel>
#include <QMap>
#include <QUuid>
#include <QListWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QCryptographicHash>
#include "AppConfig.h"
#include "AddCardDlg.h"
#include "OpenSerialDlg.h"
#include "CardInfoModel.h"
#include "ModCardInfoDlg.h"
#include "PersonInfoModel.h"
#include "ReadPersonInfo.h"
#include "OrderInfo.h"
#include "OrderInfoModel.h"
#include "OrderInfoDelegate.h"
#include "GroupInfo.h"
#include "TravlerInfo.h"
#include "TravlerInfoModel.h"
#include "SetLeaderAndGuiderDlg.h"
#include "ShowTravlerScoreDlg.h"
#include "UserInputDlg.h"
#include "HistoryInfoModel.h"
#include "LoginDlg.h"


#define index_importExcel  0
#define index_makeCard  1
#define index_query  2
#define index_cardManage  3
#define index_history  4

#define ENCRYPTION_TRANSMISSION 1

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

    LoginDlg login;

    QMap<QString, GroupInfo> m_groupCache;
    int currentIndex;
    bool m_isStartMarkCard;

 //当前登录用户名和密码
    QString currentOperator;
    QString userPwd;

// IP 和 port
    QString dest_ip_and_port;

//串口
    QSerialPort* m_serialPort;
    bool m_isSerialPortConnected;
    void serialOpen();

// cardInfo 相关
    CardInfoModel m_model;
    CardInfoModel m_cardInfomodel;
    QMenu m_cardInfoMenu;
    QList<int> removeList;

    QString m_modCardID;
    QString m_modCardSN;
    int m_modRow;

 // personInfo 相关
    PersonInfoModel m_personInfoModel;
    QString makeHttpPostBody(QList<PersonInfo> infoList);
    void clearPersonInfoModel();


  // OrderInfo 相关
    OrderInfoDelegate m_orderInfoDelegate;
    OrderInfoModel m_orderInfoModel;
    GroupInfo m_groupInfo;
    QString m_guider;
    QString m_groupid;

    QString makeCardidQueryBody(QString cardid, QString teamname, QString groupname);
    void setCaptain(QString account, QString leadername, QString teamid);
    void getGuider(QString state);
    void setGuider(QString groupid, QString guider);

    QMap<QString, QList<OrderInfo>> m_map;
    void makeListWidget(QList<OrderInfo> orderList);

    void clearOrderInfoModel();
    int searchUnsettledOrderIndex();

    //orderinfo 菜单栏
    QString m_mapDeleteKey;
    QMenu m_orderInfoMenu;


    //游客查询相关
    QMenu m_traverInfoMenu;
    QMenu m_traverListWidgetMenu;
    TravlerInfoModel m_traverInfoModel;
    QMap<QString, QList<TravlerInfo>> m_traverInfoMap;
    void makeTravlerInfoListWidget(QList<TravlerInfo> list);
    void clearTravlerInfoModel();

    QList<TravlerInfo> m_backCardList;
    void batBackCard(QList<TravlerInfo> &list);

    //历史查询相关
    QMenu m_historyInfoMenu;
    HistoryInfoModel m_historyInfoModel;
    bool historyResultParse(const QString& strResult, QString& rslt, QString& reason, QList<HistoryInfo>& list);
    void clearHistoryInfoModel();
    QString scoreParse(QString Json);

// 对话框指针
    QSharedPointer<OpenSerialDlg> m_pOpenSerialDlg;

 // 状态栏信息
    QLabel m_operatInfoLbl; //当前操作状态
    QLabel m_serialPortLbl; //串口连接状态
    QLabel m_OperatorLbl;   //当前登录人员

 // 初始化函数
    void sysInit();
    void statusBarInit();
    void cardInfoViewInit();
    void personInfoViewInit();
    void orderInfoViewInit();
    void listWidgetInit();

    void travlerInfoListWidgetInit();
    void travlerInfoViewInit();

    void queryHistoryViewInit();


    // 打印函数
    void localPrint(QString teamname, QString groupname,
                    QString leader,   QString guider,
                    QString number,        QString info);
    void realPrint(QString teamname, QString groupname,
                   QString leader,   QString guider,
                   QString number,        QString info);

    //加密相关
    QString generateSign(QString input);
    QString httpGetGenerateSign(QString input);
    QString httpPostGenerateSign(QString input);


public:
    explicit MainWindow(QWidget *parent = 0);
    bool eventFilter(QObject *obj, QEvent *evt);
    ~MainWindow();

    QString modCardID() {return m_modCardID;}
    QString modCardSN() {return m_modCardSN;}
    int modRow() {return m_modRow;}


    int  showQueryMessage(QString message);
    void showErrorMessage(QString message);
    void showInformationMessage(QString message);
    bool jsonParse(const QString &strResult, QString& rslt, QString& reason);
    bool unsettledOrderResultParse(const QString& strResult, QString &rslt, QString &reason, QList<OrderInfo>& list);
    bool makeCardQueryResultParse(const QString& strResult, QString &rslt, QString &reason,  GroupInfo &groupinfo);
    bool travelerResultParse(const QString& strResult, QString& rslt, QString& reason, QList<TravlerInfo>& list);

    bool isSerialPortConnected(){return m_isSerialPortConnected;}
//    QString transCoding(const QString& source);
    bool getGuiderResultParse(const QString& strResult, QString &rslt, QString &reason, QStringList& list);

private slots:

    void on_m_cardAddBtn_clicked();
    void on_m_cardQueryBtn_clicked();
    void on_m_cardIDQueryBtn_clicked();
    void on_m_inportExcelBtn_clicked();
    void on_m_openExcelBtn_clicked();
    void on_m_importOrderBtn_clicked();
    void on_m_importOrderCancelBtn_clicked();
    void on_m_makeCardBtn_clicked();
    void on_m_unsettledOrderQueryBtn_clicked();
    void on_m_beginMarkCardBatchBtn_clicked();
    void on_m_StopMarkCardBatchBtn_clicked();
    void on_m_makeCardEndPrintBtn_clicked();
    void on_m_unsettledOrderQueryAllBtn_clicked();
    void on_m_unsettledOrderQuerySectionBtn_clicked();
    void on_m_travelerQueryBtn_clicked();
    void on_m_queryInfoBtn1_clicked();
    void on_m_queryInfoBtn2_clicked();
    void on_m_userInfoInputBtn_clicked();
    void on_m_queryHistoryBtn1_clicked();
    void on_m_queryHistoryaBtn2_clicked();
//    void on_m_cardManageBtn_clicked();
    void on_m_queryHistoryBtn_clicked();
//    void on_m_serialManageBtn_clicked();
    void on_m_cardMagBtn_clicked();

    void on_stackedWidget_currentChanged(int index);


protected slots:

    //串口相关槽函数
    void read_SerialPort();
    void slot_serialData(QString portName, QString baudRate);
    void slot_closeSerial(QString portName, QString baudRate);

    // 卡片管理相关槽函数
    void onCardInfoMenuEditClicked();
    void onCardInfoMenuDeleteClicked();
    void onCardInfoMenuDeleteAllClicked();


    void slot_cardAddSuccessed(QString cardID, QString cardSN);
    void slot_cardGetResult(bool success, const QString& strResult);
    void slot_cardDeleteResult(bool success, const QString& strResult);
    void slot_cardModResult(bool success, const QString& strResult);

    //订单处理相关槽函数
    void slot_addUnsettledOrderResult(bool success,const QString& strResult);
    void slot_getUnsettledOrderResult(bool success,const QString& strResult);

    void onOrderInfoDelClicked();
    void slot_orderInfoDelResult(bool success, const QString& strResult);

    void slot_receivePersonInfo(PersonInfo info);

    //制卡相关槽函数
    void slot_makeCardQueryResult(bool success,const QString& strResult);
    void slot_transferResult(bool success,const QString& strResult);
    void sendTransferRequest(QString cardid, QString cardsn, QString teamid, QString groupid);

    void slot_startMakeCard(unsigned int cardid);
    void tipTimerOut();

    void slot_setCaptainResult(bool success, const QString& strResult);
    void slot_setGuiderResult(bool success, const QString& strResult);
    void slot_getGuiderResult(bool success, const QString& strResult);

    void onListWidgetClicked(QListWidgetItem* item);

    //游客查询槽函数
    void onTravelerListWidgetClicked(QListWidgetItem* item);
    void slot_getTravelerResult(bool success,const QString& strResult);
    void onTravelerBackCardClicked();
    void onTravelerScoreClicked();
    void onTravelerSetCaptainClicked();
    void onTravelerSetGuiderClicked();
    void slot_getTravelerScoreResult(bool success, const QString& strResult);
    void slot_travelerBackCardResult(bool success, const QString& strResult);

    void onTravelerListWidgetPrintClicked();
    void onTravelerListWidgetBatBackcardClicked();


    //历史查询
    void slot_getHistoryResult(bool success, const QString& strResult);
    void onHistoryScoreClicked();



private:
    Ui::MainWindow *ui;

    void setTip(QString msg);
    QTimer m_tipTimer;

protected:
    void keyPressEvent(QKeyEvent *event);
signals:
    void signal_sendCardID(unsigned int cardID);
    void signal_setGuider(bool success);
};

#endif // MAINWINDOW_H
