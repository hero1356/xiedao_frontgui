#include "OrderInfoModel.h"

OrderInfoModel::OrderInfoModel(QObject *parent) : QObject(parent)
{
    setHeader();

}
bool OrderInfoModel::add(OrderInfo info)
{
    QStandardItem* root = m_model.invisibleRootItem();

    QStandardItem* item_username = new QStandardItem();
    QStandardItem* item_age = new QStandardItem();
    QStandardItem* item_telephone = new QStandardItem();
    QStandardItem* item_identitycard = new QStandardItem();

    QStandardItem* item_workunit = new QStandardItem();
    QStandardItem* item_group = new QStandardItem();
//    QStandardItem* item_teamleader = new QStandardItem();

    QStandardItem* item_leadername = new QStandardItem();


    QStandardItem* item_orderdata = new QStandardItem();

    QStandardItem* item_orderno = new QStandardItem();
    QStandardItem* item_userno = new QStandardItem();
    QStandardItem* item_ismakecard = new QStandardItem();
    QStandardItem* item_cardsn = new QStandardItem();


    bool ret = true;

    if( m_model.rowCount() == 0)
    {
        setHeader();
    }

    if( (root != NULL) && (item_username != NULL) &&
        (item_age != NULL) && (item_telephone != NULL) &&
        (item_identitycard != NULL)  && (item_workunit != NULL) &&
        (item_group != NULL) &&
        (item_leadername != NULL) && (item_orderdata != NULL) &&
        (item_orderno != NULL) && (item_userno != NULL ) &&
        (item_ismakecard != NULL) && (item_cardsn != NULL))
    {
        item_username->setData(info.account(), Qt::WhatsThisRole);   //用户的account
        item_leadername->setData(info.teamleader(), Qt::WhatsThisRole); //队长的account

        item_username->setData(info.username(), Qt::DisplayRole);
        item_age->setData(info.age(), Qt::DisplayRole);
        item_telephone->setData(info.telephone(),Qt::DisplayRole);
        item_identitycard->setData(info.identitycard(), Qt::DisplayRole);


        item_workunit->setData(info.workunit(), Qt::DisplayRole);
        item_group->setData(info.group(), Qt::DisplayRole);
        item_leadername->setData(info.leadername(), Qt::DisplayRole);
        item_orderdata->setData(info.orderdate(),Qt::DisplayRole);

        item_orderno->setData(info.orderno(), Qt::DisplayRole);
        item_userno->setData(info.userno(), Qt::DisplayRole);

        item_ismakecard->setData(info.ismakecard(), Qt::DisplayRole);
        item_cardsn->setData(info.cardsn(),Qt::DisplayRole);



        item_username->setEditable(false);
        item_age->setEditable(false);
        item_telephone->setEditable(false);
        item_identitycard->setEditable(false);

        item_workunit->setEditable(false);
        item_group->setEditable(false);
        item_leadername->setEditable(false);
        item_orderdata->setEditable(false);

        item_orderno->setEditable(false);
        item_userno->setEditable(false);

        item_ismakecard->setEditable(false);
        item_cardsn->setEditable(false);

        int newRow = count();

        root->setChild(newRow, 0, item_username);
        root->setChild(newRow, 1, item_age);
        root->setChild(newRow, 2, item_telephone);
        root->setChild(newRow, 3, item_identitycard);

        root->setChild(newRow, 4, item_workunit);
        root->setChild(newRow, 5, item_group);
        root->setChild(newRow, 6, item_leadername);

        root->setChild(newRow, 7, item_orderdata);
        root->setChild(newRow, 8, item_orderno);
        root->setChild(newRow, 9, item_userno);

        root->setChild(newRow, 10, item_ismakecard);
        root->setChild(newRow, 11, item_cardsn);
    }
    else
    {
        ret = false;
    }

    return ret;
}
void OrderInfoModel::setData(QModelIndex index, QVariant data)
{
    if( data.type() == QVariant::Bool )
    {
        bool temp = data.toBool();
        m_model.setData(index, temp, Qt::DisplayRole);
    }
    else if (data.type() == QVariant::String)
    {
        QString temp = data.toString();
        m_model.setData(index, temp, Qt::DisplayRole);
    }

}


bool OrderInfoModel::add(QList<OrderInfo> list)
{
    bool ret = true;

    for(int i=0; i<list.count(); i++)
    {
        ret = ret && add(list[i]);
    }

    return ret;
}

bool OrderInfoModel::remove(int i)
{
    bool ret = true;

    if( (0 <= i) && (i < count()) )
    {
        m_model.removeRow(i);
    }

    return ret;
}

OrderInfo OrderInfoModel::getItem(int i)
{
    OrderInfo ret;

    if( (0 <= i) && (i < count()) )
    {
        QModelIndex index_username = m_model.index(i, 0, QModelIndex());
        QModelIndex index_age = m_model.index(i, 1, QModelIndex());
        QModelIndex index_telephone = m_model.index(i, 2, QModelIndex());
        QModelIndex index_identitycard = m_model.index(i, 3, QModelIndex());

        QModelIndex index_workunit = m_model.index(i, 4, QModelIndex());
        QModelIndex index_group = m_model.index(i, 5, QModelIndex());
        QModelIndex index_leadername = m_model.index(i, 6, QModelIndex());
        QModelIndex index_orderdata = m_model.index(i, 7, QModelIndex());

        QModelIndex index_orderno = m_model.index(i, 8, QModelIndex());
        QModelIndex index_userno = m_model.index(i, 9, QModelIndex());
        QModelIndex index_ismakecard = m_model.index(i, 10, QModelIndex());
        QModelIndex index_cardsn = m_model.index(i, 11, QModelIndex());


        QVariant username = index_username.data(Qt::DisplayRole);
        QVariant age = index_age.data(Qt::DisplayRole);
        QVariant telephone = index_telephone.data(Qt::DisplayRole);
        QVariant identitycard = index_identitycard.data(Qt::DisplayRole);

        QVariant workunit = index_workunit.data(Qt::DisplayRole);
        QVariant group = index_group.data(Qt::DisplayRole);
        QVariant leadername = index_leadername.data(Qt::DisplayRole);
        QVariant orderdata = index_orderdata.data(Qt::DisplayRole);

        QVariant teamleader = index_leadername.data(Qt::WhatsThisRole);

        QVariant orderno = index_orderno.data(Qt::DisplayRole);
        QVariant userno = index_userno.data(Qt::DisplayRole);

        QVariant account = index_username.data(Qt::WhatsThisRole);
        QVariant ismakecard = index_ismakecard.data(Qt::DisplayRole);
        QVariant cardsn = index_cardsn.data(Qt::DisplayRole);



        ret.set_username(username.toString());
        ret.set_age(age.toString());
        ret.set_telephone(telephone.toString());
        ret.set_identitycard(identitycard.toString());

        ret.set_workunit(workunit.toString());
        ret.set_group(group.toString());
        ret.set_leadername(leadername.toString());
        ret.set_teamleader(teamleader.toString());
        ret.set_orderdate(orderdata.toString());

        ret.set_orderno(orderno.toString());
        ret.set_userno(userno.toString());

        ret.set_team(workunit.toString());

        ret.set_account(account.toString());
        ret.set_ismakecard(ismakecard.toBool());
        ret.set_cardsn(cardsn.toString());

    }

    return ret;
}

int OrderInfoModel::count()
{
    return m_model.rowCount();
}

void OrderInfoModel::clear()
{
    m_model.clear();
    setHeader();
}

void OrderInfoModel::setView(QTableView& view)
{
    view.setModel(&m_model);
}

QModelIndex OrderInfoModel::index(int row, int column)
{
    return m_model.index(row, column, QModelIndex());
}
void OrderInfoModel::setHeader()
{
    QStringList list;

    list.append("姓名");
    list.append("年龄");
    list.append("联系方式");
    list.append("身份证号");

    list.append("工作单位");
    list.append("分组编号");
    list.append("领队人");

    list.append("体验时间");
    list.append("订单号");
    list.append("人员编号");

    list.append("制卡状态");
    list.append("卡号");

    m_model.setHorizontalHeaderLabels(list);
}
