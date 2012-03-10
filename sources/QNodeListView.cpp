/* 
 * File:   QNodeListView.cpp
 * Author: b4n92uid
 * 
 * Created on 29 novembre 2011, 13:04
 */

#include "QNodeListView.h"

#define nodeInterface() data(ITEM_ROLE_NODE).value<QNodeInteractor*>()

QNodeListView::QNodeListView(QWidget* parent) : QTreeView(parent)
{
    m_itemedit = new QMenu(this);

    m_promote = m_itemedit->addAction("Promouvoir le(s) noeud(s)");
    m_promote->setIcon(QIcon(":/Medias/medias/promot.png"));

    m_assign = m_itemedit->addAction("Attacher au noeud");
    m_assign->setIcon(QIcon(":/Medias/medias/assign.png"));

    QAction* remove = m_itemedit->addAction("Supprimer");
    remove->setIcon(QIcon(":/Medias/medias/cross.png"));

    QAction* enable = m_itemedit->addAction("Activer/Désactiver");
    enable->setIcon(QIcon(":/Medias/medias/accept.png"));

    QAction* lock = m_itemedit->addAction("Vérouiller/Dévérouiller");
    lock->setIcon(QIcon(":/Medias/medias/selection_select.png"));

    QAction* onfloor = m_itemedit->addAction("Mise au sol");
    onfloor->setIcon(QIcon(":/Medias/medias/shape_align_bottom.png"));

    QAction* pastScale = m_itemedit->addAction("Appliquer le scale");
    pastScale->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    QAction* pastPosition = m_itemedit->addAction("Appliquer la position");
    pastPosition->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    QAction* pastRotation = m_itemedit->addAction("Appliquer la rotation");
    pastRotation->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    QAction* pastFields = m_itemedit->addAction("Appliquer la champs");
    pastFields->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    QAction* pastMaterials = m_itemedit->addAction("Appliquer les matériaux");
    pastMaterials->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    connect(m_promote, SIGNAL(triggered()), this, SLOT(onPromoteChild()));
    connect(m_assign, SIGNAL(triggered()), this, SLOT(onAssignParent()));
    connect(remove, SIGNAL(triggered()), this, SLOT(onRemoveNode()));
    connect(enable, SIGNAL(triggered()), this, SLOT(onEnableNode()));
    connect(lock, SIGNAL(triggered()), this, SLOT(onLockNode()));
    connect(onfloor, SIGNAL(triggered()), this, SLOT(onSetOnFloorNode()));
    connect(pastPosition, SIGNAL(triggered()), this, SLOT(onPastPosition()));
    connect(pastScale, SIGNAL(triggered()), this, SLOT(onPastScale()));
    connect(pastRotation, SIGNAL(triggered()), this, SLOT(onPastRotation()));
    connect(pastFields, SIGNAL(triggered()), this, SLOT(onPastFields()));
    connect(pastMaterials, SIGNAL(triggered()), this, SLOT(onPastMaterials()));
}

QNodeListView::~QNodeListView()
{
}

QItemsList QNodeListView::selection()
{
    QItemsList items;
    QModelIndexList indexes = selectionModel()->selectedRows();

    if(indexes.empty())
        return items;

    foreach(QModelIndex index, indexes)
    {
        items << model()->itemData(index)[ITEM_ROLE_NODE].value<QNodeInteractor*>()->item();

    }

    clearSelection();

    return items;
}

void QNodeListView::onPromoteChild()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        if(item->parent())
            emit promoteChild(item);
    }
}

void QNodeListView::onAssignParent()
{
    QItemsList items = selection();

    foreach(QStandardItem* child, items)
    {
        emit assignParent(child);
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

void QNodeListView::onRemoveNode()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        emit removeNode(item->nodeInterface());
    }
}

void QNodeListView::onEnableNode()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        QNodeInteractor* interface = item->nodeInterface();

        interface->setEnable(!interface->isEnable());
    }
}

void QNodeListView::onLockNode()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        QNodeInteractor* interface = item->nodeInterface();

        interface->setLocked(!interface->isLocked());
    }
}

void QNodeListView::onSetOnFloorNode()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        emit setOnFloorNode(item->nodeInterface());
    }
}

void QNodeListView::onPastPosition()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        emit pastPosition(item->nodeInterface());
    }
}

void QNodeListView::onPastScale()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        emit pastScale(item->nodeInterface());
    }
}

void QNodeListView::onPastRotation()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        emit pastRotation(item->nodeInterface());
    }
}

void QNodeListView::onPastFields()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        emit pastFields(item->nodeInterface());
    }
}

void QNodeListView::onPastMaterials()
{
    QItemsList items = selection();

    foreach(QStandardItem* item, items)
    {
        emit pastMaterials(dynamic_cast<QMeshInteractor*>(item->nodeInterface()));
    }
}
