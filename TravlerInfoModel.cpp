#include "TravlerInfoModel.h"

TravlerInfoModel::TravlerInfoModel(QObject *parent) : QObject(parent)
{
    setHeader();

}
bool TravlerInfoModel::add(TravlerInfo info)
{
    QStandardItem* root = m_model.invisibleRootItem();

    QStandardItem* item_username = new QStandardItem();
    QStandardItem* item_age = new QStandardItem();
    QStandardItem* item_telephone = new QStandardItem();
    QStandardItem* item_cardid = new QStandardItem();

    QStandardItem* item_cardsn = new QStandardItem();

    QStandardItem* item_teamname = new QStandardItem();
    QStandardItem* item_groupname = new QStandardItem();
    QStandardItem* item_leadername = new QStandardItem();
    QStandardItem* item_guider = new QStandardItem();
    bool ret = true;

    if( m_model.rowCount() == 0)
    {
        setHeader();
    }

    if( (root != NULL) && (item_username != NULL) &&
        (item_age != NULL) && (item_telephone != NULL) && ( item_cardsn!=NULL) &&
        (item_cardid != NULL) && (item_teamname != NULL) &&
        (item_groupname != NULL) && (item_leadername != NULL) &&
        (item_guider != NULL))
    {
        //DisplayRole  显示数据
        item_username->setData(info.username(), Qt::DisplayRole);
        item_age->setData(info.age(), Qt::DisplayRole);
        item_telephone->setData(info.telephone(),Qt::DisplayRole);
        item_cardid->setData(info.cardid(), Qt::DisplayRole);

        item_cardsn->setData(info.cardsn(), Qt::DisplayRole);

        item_teamname->setData(info.teamname(), Qt::DisplayRole);
        item_groupname->setData(info.groupname(), Qt::DisplayRole);
        item_leadername->setData(info.leadername(), Qt::DisplayRole);
        item_guider->setData(info.guider(), Qt::DisplayRole);


        //WhatsThisRole  非显示数据
        item_username->setData(info.account(), Qt::WhatsThisRole);
        item_telephone->setData(info.email(), Qt::WhatsThisRole);
//        item_cardid->setData(info.cardsn(), Qt::WhatsThisRole);
        item_teamname->setData(info.teamid(), Qt::WhatsThisRole);
        item_groupname->setData(info.groupid(), Qt::WhatsThisRole);
        item_leadername->setData(info.teamleader(), Qt::WhatsThisRole);


        item_username->setEditable(false);
        item_age->setEditable(false);
        item_telephone->setEditable(false);
        item_cardid->setEditable(false);
        item_cardsn->setEditable(false);

        item_teamname->setEditable(false);
        item_groupname->setEditable(false);
        item_leadername->setEditable(false);
        item_guider->setEditable(false);

        int newRow = count();

        root->setChild(newRow, 0, item_username);
        root->setChild(newRow, 1, item_age);
        root->setChild(newRow, 2, item_telephone);
        root->setChild(newRow, 3, item_cardid);
        root->setChild(newRow, 4, item_cardsn);

        root->setChild(newRow, 5, item_teamname);
        root->setChild(newRow, 6, item_groupname);
        root->setChild(newRow, 7, item_leadername);
        root->setChild(newRow, 8, item_guider);
    }
    else
    {
        ret = false;
    }

    return ret;
}

TravlerInfo TravlerInfoModel::getItem(int i)
{
    TravlerInfo ret;
    if( (0 <= i) && (i < count()) )
    {
        QModelIndex index_username = m_model.index(i, 0, QModelIndex());
        QModelIndex index_age = m_model.index(i, 1, QModelIndex());
        QModelIndex index_telephone = m_model.index(i, 2, QModelIndex());
        QModelIndex index_cardid = m_model.index(i, 3, QModelIndex());
        QModelIndex index_cardsn = m_model.index(i, 4, QModelIndex());
        
        QModelIndex index_teamname = m_model.index(i, 5, QModelIndex());
        QModelIndex index_groupname = m_model.index(i, 6, QModelIndex());
        QModelIndex index_leadername = m_model.index(i, 7, QModelIndex());
        QModelIndex index_guider = m_model.index(i, 8, QModelIndex());


        //DisplayRole 数据
        QVariant username = index_username.data(Qt::DisplayRole);
        QVariant age = index_age.data(Qt::DisplayRole);
        QVariant telephone = index_telephone.data(Qt::DisplayRole);
        QVariant cardid = index_cardid.data(Qt::DisplayRole);
        QVariant cardsn = index_cardsn.data(Qt::DisplayRole);
        
        QVariant teamname = index_teamname.data(Qt::DisplayRole);
        QVariant groupname = index_groupname.data(Qt::DisplayRole);
        QVariant leadername = index_leadername.data(Qt::DisplayRole);
        QVariant guider = index_guider.data(Qt::DisplayRole);

        //WhatsThisRole 数据
        QVariant account = index_username.data(Qt::WhatsThisRole);
        QVariant email = index_telephone.data(Qt::WhatsThisRole);
//        QVariant cardsn = index_cardid.data(Qt::WhatsThisRole);
        QVariant teamid = index_teamname.data(Qt::WhatsThisRole);

        QVariant groupid = index_groupname.data(Qt::WhatsThisRole);
        QVariant teamleader = index_leadername.data(Qt::WhatsThisRole);

        ret.set_username(username.toString());
        ret.set_age(age.toString());
        ret.set_telephone(telephone.toString());
        ret.set_cardid(cardid.toString());
        ret.set_teamname(teamname.toString());
        ret.set_groupname(groupname.toString());
        ret.set_leadername(leadername.toString());
        ret.set_guider(guider.toString());

        ret.set_account(account.toString());
        ret.set_email(email.toString());
        ret.set_cardsn(cardsn.toString());
        ret.set_teamid(teamid.toString());
        ret.set_teamleader(teamleader.toString());
        ret.set_groupid(groupid.toString());

    }
    return ret;
}

void TravlerInfoModel::setData(QModelIndex index, QVariant data)
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


bool TravlerInfoModel::add(QList<TravlerInfo> list)
{
    bool ret = true;

    for(int i=0; i<list.count(); i++)
    {
        ret = ret && add(list[i]);
    }

    return ret;
}

bool TravlerInfoModel::remove(int i)
{
    bool ret = true;

    if( (0 <= i) && (i < count()) )
    {
        m_model.removeRow(i);
    }

    return ret;
}



int TravlerInfoModel::count()
{
    return m_model.rowCount();
}

void TravlerInfoModel::clear()
{
    m_model.clear();
    setHeader();
}

void TravlerInfoModel::setView(QTableView& view)
{
    view.setModel(&m_model);
}

QModelIndex TravlerInfoModel::index(int row, int column)
{
    return m_model.index(row, column, QModelIndex());
}
void TravlerInfoModel::setHeader()
{
    QStringList list;

    list.append("姓名");
    list.append("年龄");
    list.append("联系方式");
    list.append("卡号");
    list.append("编号");

    list.append("队伍名称");
    list.append("组名");
    list.append("队长");
    list.append("导游");

    m_model.setHorizontalHeaderLabels(list);
}
