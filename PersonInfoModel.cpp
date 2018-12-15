#include "PersonInfoModel.h"
#include <QModelIndex>
#include <QVariant>

PersonInfoModel::PersonInfoModel(QObject *parent) : QObject(parent)
{
    setHeader();
}

bool PersonInfoModel::add(PersonInfo info)
{
    QStandardItem* root = m_model.invisibleRootItem();
    QStandardItem* item_name = new QStandardItem();
    QStandardItem* item_age = new QStandardItem();
    QStandardItem* item_phoneNum = new QStandardItem();
    QStandardItem* item_id = new QStandardItem();
    QStandardItem* item_playTime = new QStandardItem();
    QStandardItem* item_workUnit = new QStandardItem();
    QStandardItem* item_groupNum = new QStandardItem();
    bool ret = true;

    if( m_model.rowCount() == 0)
    {
        setHeader();
    }

    if( (root != NULL) && (item_name != NULL) && (item_age != NULL) && (item_phoneNum != NULL) && (item_id != NULL) && (item_playTime != NULL) && (item_workUnit != NULL) && (item_groupNum != NULL) )
    {
        item_name->setData(info.name(), Qt::DisplayRole);
        item_age->setData(info.age(), Qt::DisplayRole);
        item_phoneNum->setData(info.phoneNum(),Qt::DisplayRole);
        item_id->setData(info.id(), Qt::DisplayRole);
        item_playTime->setData(info.playTime(), Qt::DisplayRole);
        item_workUnit->setData(info.workUnit(), Qt::DisplayRole);
        item_groupNum->setData(info.groupNum(), Qt::DisplayRole);

        item_name->setEditable(false);
        item_age->setEditable(false);
        item_phoneNum->setEditable(false);
        item_id->setEditable(false);
        item_playTime->setEditable(false);
        item_workUnit->setEditable(false);
        item_groupNum->setEditable(false);

        int newRow = count();

        root->setChild(newRow, 0, item_name);
        root->setChild(newRow, 1, item_age);
        root->setChild(newRow, 2, item_phoneNum);
        root->setChild(newRow, 3, item_id);
        root->setChild(newRow, 4, item_playTime);
        root->setChild(newRow, 5, item_workUnit);
        root->setChild(newRow, 6, item_groupNum);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool PersonInfoModel::add(QList<PersonInfo> list)
{
    bool ret = true;

    for(int i=0; i<list.count(); i++)
    {
        ret = ret && add(list[i]);
    }

    return ret;
}

bool PersonInfoModel::remove(int i)
{
    bool ret = true;

    if( (0 <= i) && (i < count()) )
    {
        m_model.removeRow(i);
    }

    return ret;
}

PersonInfo PersonInfoModel::getItem(int i)
{
    PersonInfo ret;

    if( (0 <= i) && (i < count()) )
    {
        QModelIndex index_name = m_model.index(i, 0, QModelIndex());
        QModelIndex index_age = m_model.index(i, 1, QModelIndex());
        QModelIndex index_phoneNum = m_model.index(i, 2, QModelIndex());
        QModelIndex index_id = m_model.index(i, 3, QModelIndex());
        QModelIndex index_playTime = m_model.index(i, 4, QModelIndex());
        QModelIndex index_workUnit = m_model.index(i, 5, QModelIndex());
        QModelIndex index_groupNum = m_model.index(i, 6, QModelIndex());


        QVariant name = index_name.data(Qt::DisplayRole);
        QVariant age = index_age.data(Qt::DisplayRole);
        QVariant phoneNum = index_phoneNum.data(Qt::DisplayRole);
        QVariant id = index_id.data(Qt::DisplayRole);
        QVariant playTime = index_playTime.data(Qt::DisplayRole);
        QVariant workUnit = index_workUnit.data(Qt::DisplayRole);
        QVariant groupNum = index_groupNum.data(Qt::DisplayRole);

        ret = PersonInfo(name.toString(), age.toString(), phoneNum.toString(), id.toString(), playTime.toString(), workUnit.toString(), groupNum.toString());
    }

    return ret;
}

int PersonInfoModel::count()
{
    return m_model.rowCount();
}

void PersonInfoModel::clear()
{
    m_model.clear();
    setHeader();
}

void PersonInfoModel::setView(QTableView& view)
{
    view.setModel(&m_model);
}

void PersonInfoModel::setHeader()
{
    QStringList list;

    list.append("姓名");
    list.append("年龄");
    list.append("联系方式");
    list.append("身份证号");
    list.append("参观时间");
    list.append("班级");
    list.append("分组");

    m_model.setHorizontalHeaderLabels(list);
}
