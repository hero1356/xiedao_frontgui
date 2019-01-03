#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QEvent>
#include <QDateTime>
#include <QTimer>
#include <QIcon>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow), m_pOpenSerialDlg(new OpenSerialDlg)
{
    ui->setupUi(this);

    //系统初始化
    sysInit();

    //状态栏初始化
    statusBarInit();

    //视图初始化
    cardInfoViewInit();
    personInfoViewInit();
    orderInfoViewInit();
    listWidgetInit();
    travlerInfoListWidgetInit();
    travlerInfoViewInit();
    queryHistoryViewInit();
}

// init the system param
void MainWindow::sysInit()
{
    //读取配置文件，设置IP和Port
    AppConfig config;
    dest_ip_and_port = "http://"+config.ip()+":"+config.port();
    qDebug() << "目标地址" <<dest_ip_and_port;

    //参数初始化
    currentOperator = "admin";
    m_isSerialPortConnected = false;
    currentIndex = 0;
    m_isStartMarkCard = false;
    ui->m_StopMarkCardBatchBtn->setVisible(false);

    //设置ui上日期栏
    QDate date = QDate::currentDate();
    ui->m_orderPlayTimeEdit->setDate(date);
    ui->m_orderBeginTimeEdit->setDate(date);
    ui->m_orderEndTimeEdit->setDate(date.addDays(3));
    ui->m_queryHistoryEdit2->setDate(date.addDays(-7));
    ui->m_queryHistoryEdit3->setDate(date);

    //系统初始页面
    ui->stackedWidget->setCurrentIndex(0);
    ui->m_inportExcelBtn->setProperty("switchpage", true);

    //隐藏按日期查询未处理订单
    ui->m_unsettledOrderQueryBtn->setVisible(false);
    ui->m_orderPlayTimeEdit->setVisible(false);
    ui->m_orderPlayTimeLbl->setVisible(false);
}

//init the status bar
void MainWindow::statusBarInit()
{
    m_operatInfoLbl.setText("");
    m_operatInfoLbl.setMinimumWidth(200);
    m_operatInfoLbl.setAlignment(Qt::AlignCenter);
    ui->statusBar->addWidget(&m_operatInfoLbl);

    ui->statusBar->addPermanentWidget(new QLabel(""));

    m_serialPortLbl.setStyleSheet("color:red; font-size: 15px;");
    m_serialPortLbl.setText("串口未连接！");
    m_serialPortLbl.setMinimumWidth(200);
    m_serialPortLbl.setAlignment(Qt::AlignCenter);
    ui->statusBar->addPermanentWidget(&m_serialPortLbl);
    m_serialPortLbl.installEventFilter(this);

    m_OperatorLbl.setText("当前用户："+currentOperator);
    m_OperatorLbl.setMinimumWidth(200);
    m_OperatorLbl.setAlignment(Qt::AlignCenter);
    ui->statusBar->addPermanentWidget(&m_OperatorLbl);
}

//import excel
void MainWindow::personInfoViewInit()
{
    m_personInfoModel.setView(*ui->m_personInfoView);

    ui->m_personInfoView->setColumnWidth(2,150);
    ui->m_personInfoView->setColumnWidth(3,200);
    ui->m_personInfoView->setColumnWidth(5,200);
}

//bat makecard-unsettled order right view
void MainWindow::orderInfoViewInit()
{
    m_orderInfoModel.setView(*ui->m_orderInfoView);
    ui->m_orderInfoView->setItemDelegate(&m_orderInfoDelegate);

    ui->m_orderInfoView->setColumnWidth(1, 50);
    ui->m_orderInfoView->setColumnWidth(3, 150);
    ui->m_orderInfoView->setColumnWidth(4, 150);
    ui->m_orderInfoView->setColumnWidth(8, 150);
}

//bat makecard-unsettled order left widget
void MainWindow::listWidgetInit()
{
    ui->m_listWidget->setViewMode(QListWidget::ListMode);
    ui->m_listWidget->installEventFilter(this);

    connect(ui->m_listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(onListWidgetClicked(QListWidgetItem*)));

    m_orderInfoMenu.addAction(QIcon(":/icons/icons/delete.png"),"删除该组");
    connect(m_orderInfoMenu.actions()[0], SIGNAL(triggered(bool)),this, SLOT(onOrderInfoDelClicked()));
}

//travler query - left widget
void MainWindow::travlerInfoListWidgetInit()
{
    ui->m_travlerInfoListWidget->setViewMode(QListWidget::ListMode);
    ui->m_travlerInfoListWidget->installEventFilter(this);

    connect(ui->m_travlerInfoListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(onTravelerListWidgetClicked(QListWidgetItem*)));

    m_traverListWidgetMenu.addAction(QIcon(":/icons/icons/print.png"),"打印人员对照单");
//    m_traverListWidgetMenu.addAction("批量退卡");
    connect(m_traverListWidgetMenu.actions()[0], SIGNAL(triggered(bool)), this, SLOT(onTravelerListWidgetPrintClicked()));
//    connect(m_traverListWidgetMenu.actions()[1], SIGNAL(triggered(bool)), this, SLOT(onTravelerListWidgetBatBackcardClicked()));

    ui->m_travlerInfoView->setColumnWidth(5, 150);
}

//travler query - right view
void MainWindow::travlerInfoViewInit()
{
    m_traverInfoModel.setView(*ui->m_travlerInfoView);
    ui->m_travlerInfoView->installEventFilter(this);

    //menu
    m_traverInfoMenu.addAction(QIcon(":/icons/icons/card.png"),"退卡");
    m_traverInfoMenu.addAction(QIcon(":/icons/icons/score.png"),"成绩");
    m_traverInfoMenu.addSeparator();
    m_traverInfoMenu.addAction(QIcon(":/icons/icons/leader.png"),"设为队长");
    m_traverInfoMenu.addAction(QIcon(":/icons/icons/leader.png"),"设置导游");
    m_traverInfoMenu.addSeparator();
    m_traverInfoMenu.addAction(QIcon(":/icons/icons/card.png"),"批量退卡");

    connect(m_traverInfoMenu.actions()[0], SIGNAL(triggered(bool)), this, SLOT(onTravelerBackCardClicked()));
    connect(m_traverInfoMenu.actions()[1], SIGNAL(triggered(bool)), this, SLOT(onTravelerScoreClicked()));
    // m_traverInfoMenu.actions()[2]   ---- 空格
    connect(m_traverInfoMenu.actions()[3], SIGNAL(triggered(bool)), this, SLOT(onTravelerSetCaptainClicked()));
    connect(m_traverInfoMenu.actions()[4], SIGNAL(triggered(bool)), this, SLOT(onTravelerSetGuiderClicked()));
    // m_traverInfoMenu.actions()[5]   ---- 空格
    connect(m_traverInfoMenu.actions()[6], SIGNAL(triggered(bool)), this, SLOT(onTravelerListWidgetBatBackcardClicked()));

}

//card management
void MainWindow::cardInfoViewInit()
{
    m_cardInfomodel.setView(*ui->m_cardInfoView);
    ui->m_cardInfoView->installEventFilter(this);

    m_cardInfoMenu.addAction(QIcon(":/icons/icons/edit.png"),"编辑");
    m_cardInfoMenu.addAction(QIcon(":/icons/icons/delete.png"),"删除");
    m_cardInfoMenu.addSeparator();
    m_cardInfoMenu.addAction(QIcon(":/icons/icons/delete.png"),"删除全部");

    connect(m_cardInfoMenu.actions()[0], SIGNAL(triggered(bool)), this, SLOT(onCardInfoMenuEditClicked()));
    connect(m_cardInfoMenu.actions()[1], SIGNAL(triggered(bool)), this, SLOT(onCardInfoMenuDeleteClicked()));
    connect(m_cardInfoMenu.actions()[3], SIGNAL(triggered(bool)), this, SLOT(onCardInfoMenuDeleteAllClicked()));
}

//history query
void MainWindow::queryHistoryViewInit()
{
    m_historyInfoModel.setView(*ui->m_queryHistoryView);
    ui->m_queryHistoryView->setColumnWidth(3, 200);
    ui->m_queryHistoryView->setColumnWidth(4, 200);

    ui->m_queryHistoryView->installEventFilter(this);

    m_historyInfoMenu.addAction(QIcon(":/icons/icons/score.png"), "成绩");
    connect(m_historyInfoMenu.actions()[0], SIGNAL(triggered(bool)), this, SLOT(onHistoryScoreClicked()));
}

//设置提示信息
void MainWindow::setTip(QString msg)
{
    m_operatInfoLbl.setText(msg);
    m_tipTimer.start(3000);
    connect(&m_tipTimer,SIGNAL(timeout()),this,SLOT(tipTimerOut()));
}

//提示信息定时器
void MainWindow::tipTimerOut()
{
    m_operatInfoLbl.setText("");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    //未处理订单右键菜单 -- 删除该组
    if( (obj == ui->m_listWidget) && (evt->type() == QEvent::ContextMenu) )
    {
        m_orderInfoMenu.exec(cursor().pos());
    }

    // 在馆游客信息右键菜单 -- 退卡，成绩，设置队长，设置导游
    else if( (obj == ui->m_travlerInfoView) && (evt->type() == QEvent::ContextMenu) )
    {
        m_traverInfoMenu.exec(cursor().pos());
    }

    // 在馆游客信息列表右键菜单 -- 打印人员对照表
    else if( (obj == ui->m_travlerInfoListWidget) && (evt->type() == QEvent::ContextMenu) )
    {
        m_traverListWidgetMenu.exec(cursor().pos());
    }
    //卡片管理右键菜单 -- 编辑，删除，删除全部
    else if( (obj == ui->m_cardInfoView) && (evt->type() == QEvent::ContextMenu) )
    {
        m_cardInfoMenu.exec(cursor().pos());
    }

    //历史人员右键菜单 -- 成绩
    else if( (obj == ui->m_queryHistoryView) && (evt->type() == QEvent::ContextMenu) )
    {
        m_historyInfoMenu.exec(cursor().pos());
    }

    //连接串口双击
    else if( (obj == &m_serialPortLbl) && (evt->type() == QEvent::MouseButtonDblClick) )
    {
        serialOpen();
    }

    return QWidget::eventFilter(obj, evt);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
//        qDebug() << "Return pressed!";
        if( ui->stackedWidget->currentIndex() == index_query )
        {
            on_m_queryInfoBtn1_clicked();
        }

        if( ui->stackedWidget->currentIndex() == index_cardManage )
        {
            on_m_cardQueryBtn_clicked();
        }

        if( ui->stackedWidget->currentIndex() == index_history )
        {
            on_m_queryHistoryBtn1_clicked();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
