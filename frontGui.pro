#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T16:46:55
#
#-------------------------------------------------

QT  += core
QT  += gui
QT  += network
QT  += serialport
QT  += axcontainer
QT  += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = frontGui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
            MainWindow.cpp \
            Http.cpp \
            OpenSerialDlg.cpp \
            MainWindowSlot.cpp \
            CardInfo.cpp \
            CardInfoModel.cpp \
            ModCardInfoDlg.cpp \
            AddCardDlg.cpp \
            PersonInfo.cpp \
            PersonInfoModel.cpp \
            ReadPersonInfo.cpp \
            slots_cardManage.cpp \
            slots_importExcel.cpp \
            OrderInfo.cpp \
            slots_makeCard.cpp \
            AppConfig.cpp \
            OrderInfoModel.cpp \
            GroupInfo.cpp \
            OrderInfoDelegate.cpp \
            SetLeaderAndGuiderDlg.cpp \
            slots_queryUnsettledOrder.cpp \
            slots_travlerQuery.cpp \
            TravlerInfo.cpp \
            TravlerInfoModel.cpp \
            ShowTravlerScoreDlg.cpp \
            UserInputDlg.cpp \
            slots_queryHistory.cpp \
            HistoryInfo.cpp \
            HistoryInfoModel.cpp \
            LoginDlg.cpp \
    EncryptionTransmission.cpp \
    ModGroupInfoDlg.cpp \
    ModPersonInfoDlg.cpp

HEADERS  += MainWindow.h \
            Http.h \
            OpenSerialDlg.h \
            CardInfo.h \
            CardInfoModel.h \
            ModCardInfoDlg.h \
            AddCardDlg.h \
            PersonInfo.h \
            PersonInfoModel.h \
            ReadPersonInfo.h \
            OrderInfo.h \
            AppConfig.h \
            OrderInfoModel.h \
            GroupInfo.h \
            OrderInfoDelegate.h \
            SetLeaderAndGuiderDlg.h \
            TravlerInfo.h \
            TravlerInfoModel.h \
            ShowTravlerScoreDlg.h \
            UserInputDlg.h \
            HistoryInfo.h \
            HistoryInfoModel.h \
            LoginDlg.h \
    EncryptionTransmission.h \
    ModGroupInfoDlg.h \
    ModPersonInfoDlg.h

FORMS    += MainWindow.ui \
            ModCardInfoDlg.ui \
            AddCardDlg.ui \
            SetLeaderAndGuiderDlg.ui \
            ShowTravlerScoreDlg.ui \
            UserInputDlg.ui \
            LoginDlg.ui \
    ModGroupInfoDlg.ui \
    ModPersonInfoDlg.ui

RC_ICONS = icon.ico

RESOURCES += \
    resource.qrc \
    res.qrc \

