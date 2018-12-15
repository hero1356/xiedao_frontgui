#include "OrderInfoDelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QCheckBox>
#include <QDebug>

OrderInfoDelegate::OrderInfoDelegate(QObject *parent)
{

}
void OrderInfoDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void OrderInfoDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if( index.data().type() == QVariant::Bool )
    {
        QCheckBox* cb = dynamic_cast<QCheckBox*>(editor);

        if( cb != NULL )
        {
            model->setData(index, cb->isChecked(), Qt::DisplayRole);
        }
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void OrderInfoDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if( index.data().type() == QVariant::Bool )
    {
        QCheckBox* cb = dynamic_cast<QCheckBox*>(editor);

        if( cb != NULL )
        {
            cb->setChecked(index.data().toBool());
        }
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
    }
}

void OrderInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( index.data().type() == QVariant::Bool )
    {
        bool data = index.data(Qt::DisplayRole).toBool();
        QStyleOptionButton checkBoxStyle;

        checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
        checkBoxStyle.state |= QStyle::State_Enabled;
        checkBoxStyle.rect = option.rect;
        checkBoxStyle.rect.setX(option.rect.x() + option.rect.width() / 2 - 6);

        QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxStyle, painter);
    }
    else
    {
        QItemDelegate::paint(painter, option, index);
    }
}

bool OrderInfoDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    bool ret = true;

        if( index.data().type() == QVariant::Bool )
        {
            QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);

            if( (event->type() == QEvent::MouseButtonPress) && option.rect.contains(mouseEvent->pos()) )
            {
//                bool data = index.data(Qt::DisplayRole).toBool();

//                qDebug() << "set model data: " << !data;
//                qDebug() << model->setData(index, !data, Qt::DisplayRole);
            }
        }
        else
        {
            ret = QItemDelegate::editorEvent(event, model, option, index);
        }

        return ret;
}
