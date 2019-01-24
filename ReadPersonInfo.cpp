#include "ReadPersonInfo.h"
#include <QAxObject>
#include <QDir>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>

ReadPersonInfo::ReadPersonInfo(QObject *parent) : QObject(parent)
{

}
QString ReadPersonInfo::showFileDialog(QString title)
{
    QString ret = "";
    QFileDialog fd;
    QStringList filters;

    QMap<QString, QString> map;
    const char* filterArray[][2] =
    {
        {"电子表格 (*.xlsx)", ".xlsx"},
        {"电子表格 (*.xls)", ".xls"},     
        {"所有文件 (*)"     ,  "*"},
        {NULL              ,  NULL}
    };
    for(int i=0; filterArray[i][0]!= NULL; i++)
    {
        filters.append(filterArray[i][0]);
        map.insert(filterArray[i][0], filterArray[i][1]);
    }
    fd.setWindowTitle(title);
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setNameFilters(filters);
    fd.setFileMode(QFileDialog::ExistingFile);

    if (fd.exec() == QFileDialog::Accepted)
    {
        ret = fd.selectedFiles()[0];
    }
    return ret;
}


QString ReadPersonInfo::getData()
{
    QString ret = "false";

    QString excel_file_path = showFileDialog("打开文件");
    if(excel_file_path == "") return "";

    QAxObject *excel = new QAxObject(this);//建立excel操作对象
    excel->setControl("Excel.Application");//连接Excel控件
    excel->setProperty("Visible", false);//显示窗体看效果
    excel->setProperty("DisplayAlerts", false);//显示警告看效果
    QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿(excel文件)集合
    workbooks->dynamicCall("Open(const QString&)", excel_file_path);
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)",1);
    QAxObject *usedRange = worksheet->querySubObject("UsedRange");
    if(NULL == usedRange || usedRange->isNull())
    {
        return ret;
    }
    QVariant var = usedRange->dynamicCall("Value");//这里是所有的内容
    workbook->dynamicCall( "Close(Boolean)", false );
    excel->dynamicCall( "Quit(void)" );
    delete excel;
//    qDebug() << var;

    parse(var, m_data);

    return excel_file_path;
}

bool ReadPersonInfo::parse(QVariant var, QList<PersonInfo>& list)
{
    bool ret = true;

    QList<QList<QVariant> > res;
    castVariant2ListListVariant(var,res);

    for( int i=1; i<res.count(); i++)
    {
        QVariantList temp = res.at(i);
        QString name = temp.at(0).toString();
        QString age = temp.at(1).toString();
        QString phoneNum = temp.at(2).toString();
        QString id  = temp.at(3).toString();
        QString playTime_1= temp.at(4).toString();
        QString workUnit = temp.at(5).toString();
        QString groupNum = temp.at(6).toString();

        if( name == "" || name.isEmpty() || name.isNull())
            continue;

        QDateTime dt = QDateTime::fromString(playTime_1, "yyyy-MM-ddThh:mm:ss");
        QString playTime = dt.toString("yyyy-MM-dd");

        PersonInfo info(name,age,phoneNum,id,playTime,workUnit,groupNum);

        list.append(info);
    }

    for(int i=0; i<list.count(); i++)
    {
        qDebug() << list[i].name();
        qDebug() << list[i].age();
        qDebug() << list[i].phoneNum();
        qDebug() << list[i].id();
        qDebug() << list[i].playTime();
        qDebug() << list[i].workUnit();
        qDebug() << list[i].groupNum();

        qDebug() << endl;
    }

    return ret;
}
void ReadPersonInfo::castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res)
{
    QVariantList varRows = var.toList();
    if(varRows.isEmpty())
    {
        return;
    }
    const int rowCount = varRows.size();
    QVariantList rowData;
    for(int i=0;i<rowCount;++i)
    {
        rowData = varRows[i].toList();
        res.push_back(rowData);
    }
}
QList<PersonInfo> ReadPersonInfo::fetchData()
{
    QList<PersonInfo> ret = m_data;

    m_data.clear();

    return ret;
}

int ReadPersonInfo::count()
{
    return m_data.count();
}
