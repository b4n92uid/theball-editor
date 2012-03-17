/* 
 * File:   QNodeListView.cpp
 * Author: b4n92uid
 * 
 * Created on 29 novembre 2011, 13:04
 */

#include "QNodeListView.h"

#include "QNodeInteractor.h"
#include "QMeshInteractor.h"

#define nodeInterface() data(ITEM_ROLE_NODE).value<QNodeInteractor*>()

QNodeListView::QNodeListView(QWidget* parent) : QTreeView(parent)
{
    m_proxy = NULL;

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

    m_itemedit->addSeparator();

    QAction* pastScale = m_itemedit->addAction("Appliquer le scale");
    pastScale->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    QAction* pastPosition = m_itemedit->addAction("Appliquer la position");
    pastPosition->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    QAction* pastRotation = m_itemedit->addAction("Appliquer la rotation");
    pastRotation->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    QAction* pastFields = m_itemedit->addAction("Appliquer les champs");
    pastFields->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    QAction* pastMaterials = m_itemedit->addAction("Appliquer les matériaux");
    pastMaterials->setIcon(QIcon(":/Medias/medias/page_white_paste.png"));

    connect(m_promote, SIGNAL(triggered()), this, SIGNAL(promoteChild()));
    connect(m_assign, SIGNAL(triggered()), this, SIGNAL(assignParent()));

    connect(enable, SIGNAL(triggered()), this, SLOT(onEnableNode()));
    connect(lock, SIGNAL(triggered()), this, SLOT(onLockNode()));

    connect(remove, SIGNAL(triggered()), this, SIGNAL(removeNode()));
    connect(onfloor, SIGNAL(triggered()), this, SIGNAL(setOnFloorNode()));
    connect(pastPosition, SIGNAL(triggered()), this, SIGNAL(pastPosition()));
    connect(pastScale, SIGNAL(triggered()), this, SIGNAL(pastScale()));
    connect(pastRotation, SIGNAL(triggered()), this, SIGNAL(pastRotation()));
    connect(pastFields, SIGNAL(triggered()), this, SIGNAL(pastFields()));
    connect(pastMaterials, SIGNAL(triggered()), this, SIGNAL(pastMaterials()));
}

QNodeListView::~QNodeListView()
{
}

void QNodeListView::setModel(QSortFilterProxyModel* model)
{
    m_proxy = model;
    QTreeView::setModel(m_proxy);
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

    return items;
}

void QNodeListView::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::RightButton)
    {
        m_promote->setEnabled(selectionModel()->selectedRows().front().data(ITEM_ROLE_NODE)
                              .value<QNodeInteractor*>()->item()->parent());

        m_assign->setEnabled(selectionModel()->selectedRows().count() > 1);

        m_itemedit->popup(QCursor::pos());

        event->accept();
    }

    QTreeView::mousePressEvent(event);
}

void QNodeListView::highlightItem(QNodeInteractor* qnode)
{
    QModelIndex curindex = m_proxy->mapFromSource(qnode->item()->index());

    if(!selectionModel()->isSelected(curindex))
    {
        QItemSelection indexes = m_proxy->mapSelectionFromSource(qnode->itemSelection());
        selectionModel()->select(indexes, QItemSelectionModel::Select);
    }
}

void QNodeListView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    QTreeView::selectionChanged(selected, deselected);

    foreach(QModelIndex index, deselected.indexes())
    {
        emit deselect(index.data(ITEM_ROLE_NODE).value<QNodeInteractor*>());
    }

    foreach(QModelIndex index, selected.indexes())
    {
        emit select(index.data(ITEM_ROLE_NODE).value<QNodeInteractor*>());
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
