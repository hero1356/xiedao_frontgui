#include "CardInfoModel.h"
#include <QStandardItem>
#include <QModelIndex>
#include <QVariant>

CardInfoModel::CardInfoModel(QObject *parent) : QObject(parent)
{
    setHeader();
}
bool CardInfoModel::add(CardInfo info)
{
    QStandardItem* root = m_model.invisibleRootItem();
    QStandardItem* item0 = new QStandardItem();
    QStandardItem* item1 = new QStandardItem();
    bool ret = true;

    if( m_model.rowCount() == 0)
    {
        setHeader();
    }

    if( (root != NULL) && (item0 != NULL) && (item1 != NULL) )
    {

        item0->setData(info.cardSN(), Qt::DisplayRole);
        item1->setData(info.cardID(), Qt::DisplayRole);

        item0->setEditable(false);
        item1->setEditable(false);

        int newRow = count();

        root->setChild(newRow, 0, item0);
        root->setChild(newRow, 1, item1);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool CardInfoModel::add(QList<CardInfo> list)
{
    bool ret = true;

    for(int i=0; i<list.count(); i++)
    {
        ret = ret && add(list[i]);
    }

    return ret;
}

bool CardInfoModel::remove(int i)
{
    bool ret = true;

    if( (0 <= i) && (i<count()) )
    {
        m_model.removeRow(i);
    }

    return ret;
}

CardInfo CardInfoModel::getItem(int i)
{
    CardInfo ret;

    if( (0 <= i) && (i<count()) )
    {
        QModelIndex index0 = m_model.index(i, 0, QModelIndex());
        QModelIndex index1 = m_model.index(i, 1, QModelIndex());

        QVariant v0 = index0.data(Qt::DisplayRole);
        QVariant v1 = index1.data(Qt::DisplayRole);

        ret = CardInfo(v1.toString(), v0.toString());
    }

    return ret;
}

int CardInfoModel::count()
{
    return m_model.rowCount();
}

void CardInfoModel::clear()
{
    m_model.clear();
    setHeader();
}

void CardInfoModel::setView(QTableView& view)
{
    view.setModel(&m_model);
}

void CardInfoModel::setHeader()
{
    QStringList list;

    list.append("卡片编号");
    list.append("卡片ID");

    m_model.setHorizontalHeaderLabels(list);
}
