#include "HistoryInfoModel.h"

HistoryInfoModel::HistoryInfoModel(QObject *parent) : QObject(parent)
{
    setHeader();
}

bool HistoryInfoModel::add(HistoryInfo info)
{
    QStandardItem* root = m_model.invisibleRootItem();

    QStandardItem* item_username = new QStandardItem();
    QStandardItem* item_telephone = new QStandardItem();
    QStandardItem* item_score = new QStandardItem();

    QStandardItem* item_ordertime = new QStandardItem();
    QStandardItem* item_scoretime = new QStandardItem();
    QStandardItem* item_openid = new QStandardItem();

    bool ret = true;

    if( m_model.rowCount() == 0)
    {
        setHeader();
    }

    if( (root != NULL) && (item_username != NULL) && (item_telephone != NULL) &&
            (item_score != NULL) && (item_ordertime != NULL) && (item_scoretime != NULL) &&
            (item_openid != NULL))
    {

        item_username->setData(info.username(), Qt::DisplayRole);
        item_telephone->setData(info.telephone(), Qt::DisplayRole);
        item_score->setData(info.score(), Qt::DisplayRole);

        item_ordertime->setData(info.ordertime(), Qt::DisplayRole);
        item_scoretime->setData(info.scoretime(), Qt::DisplayRole);
        item_openid->setData(info.openid(), Qt::DisplayRole);

        item_score->setData(info.scorejson(), Qt::WhatsThisRole);



        item_username->setEditable(false);
        item_telephone->setEditable(false);
        item_score->setEditable(false);

        item_ordertime->setEditable(false);
        item_scoretime->setEditable(false);
        item_openid->setEditable(false);

        int newRow = count();

        root->setChild(newRow, 0, item_username);
        root->setChild(newRow, 1, item_telephone);
        root->setChild(newRow, 2, item_score);

        root->setChild(newRow, 3, item_ordertime);
        root->setChild(newRow, 4, item_scoretime);
        root->setChild(newRow, 5, item_openid);
    }
    else
    {
        ret = false;
    }

    return ret;
}

HistoryInfo HistoryInfoModel::getItem(int i)
{
    HistoryInfo ret;

    if( (0 <= i) && (i<count()) )
    {
        QModelIndex index_username = m_model.index(i, 0, QModelIndex());
        QModelIndex index_telephone = m_model.index(i, 1, QModelIndex());
        QModelIndex index_socre = m_model.index(i, 2, QModelIndex());

        QModelIndex index_ordertime = m_model.index(i, 3, QModelIndex());
        QModelIndex index_socertime = m_model.index(i, 4, QModelIndex());
        QModelIndex index_openid = m_model.index(i, 5, QModelIndex());

        QString username = index_username.data(Qt::DisplayRole).toString();
        QString telephone = index_telephone.data(Qt::DisplayRole).toString();
        QString score = index_socre.data(Qt::DisplayRole).toString();

        QString ordertime = index_ordertime.data(Qt::DisplayRole).toString();
        QString scoretime = index_socertime.data(Qt::DisplayRole).toString();
        QString openid = index_openid.data(Qt::DisplayRole).toString();

        QString scorejson = index_socre.data(Qt::WhatsThisRole).toString();


        ret = HistoryInfo(username, telephone,score, ordertime, scoretime, openid, scorejson);
    }

    return ret;
}
bool HistoryInfoModel::add(QList<HistoryInfo> list)
{
    bool ret = true;

    for(int i=0; i<list.count(); i++)
    {
        ret = ret && add(list[i]);
    }

    return ret;
}

bool HistoryInfoModel::remove(int i)
{
    bool ret = true;

    if( (0 <= i) && (i<count()) )
    {
        m_model.removeRow(i);
    }

    return ret;
}


int HistoryInfoModel::count()
{
    return m_model.rowCount();
}

void HistoryInfoModel::clear()
{
    m_model.clear();
    setHeader();
}

void HistoryInfoModel::setView(QTableView& view)
{
    view.setModel(&m_model);
}

void HistoryInfoModel::setHeader()
{
    QStringList list;

    list.append("姓名");
    list.append("手机号");
    list.append("分数");
    list.append("订单时间");
    list.append("退卡时间");
    list.append("微信号");

    m_model.setHorizontalHeaderLabels(list);

}
