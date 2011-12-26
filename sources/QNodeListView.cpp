/* 
 * File:   QNodeListView.cpp
 * Author: b4n92uid
 * 
 * Created on 29 novembre 2011, 13:04
 */

#include "QNodeListView.h"
#include "QNodeInteractor.h"

QNodeListView::QNodeListView(QWidget* parent) : QTreeView(parent)
{
    m_itemedit = new QMenu(this);

    m_promote = m_itemedit->addAction("Promouvoir le(s) noeud(s)");
    m_promote->setIcon(QIcon(":/Medias/medias/promot.png"));
    connect(m_promote, SIGNAL(triggered()), this, SLOT(onPromoteChild()));

    m_assign = m_itemedit->addAction("Attacher la sélection");
    m_assign->setIcon(QIcon(":/Medias/medias/assign.png"));
    connect(m_assign, SIGNAL(triggered()), this, SLOT(onAssignParent()));
}

QNodeListView::~QNodeListView()
{
}

void QNodeListView::onPromoteChild()
{
    QModelIndexList indexes = selectionModel()->selectedRows();
    QItemsList items;

    if(indexes.empty())
        return;

    foreach(QModelIndex index, indexes)
    {
        items << model()->itemData(index)[ITEM_ROLE_NODE].value<QNodeInteractor*>()->item();

    }

    clearSelection();

    foreach(QStandardItem* item, items)
    {
        if(item->parent())
            emit promoteChild(item);
    }
}

void QNodeListView::onAssignParent()
{
    QModelIndex curIndex = currentIndex();

    QStandardItem* parent = model()->itemData(curIndex)[ITEM_ROLE_NODE].value<QNodeInteractor*>()->item();

    QModelIndexList indexes = selectionModel()->selectedRows();
    QItemsList items;

    if(indexes.empty())
        return;

    foreach(QModelIndex index, indexes)
    {
        items << model()->itemData(index)[ITEM_ROLE_NODE].value<QNodeInteractor*>()->item();
    }

    clearSelection();

    foreach(QStandardItem* child, items)
    {
        if(child != parent)
            emit assignParent(parent, child);
    }
}

void QNodeListView::mousePressEvent(QMouseEvent * event)
{
    QTreeView::mousePressEvent(event);

    if(event->button() == Qt::RightButton)
    {
        m_promote->setEnabled(selectionModel()->selectedRows().front().data(ITEM_ROLE_NODE)
                              .value<QNodeInteractor*>()->item()->parent());

        m_assign->setEnabled(selectionModel()->selectedRows().count() > 1);

        m_itemedit->popup(QCursor::pos());

        event->accept();
    }

    else
    {
        event->ignore();
    }
}
