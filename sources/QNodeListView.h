/* 
 * File:   QNodeListView.h
 * Author: b4n92uid
 *
 * Created on 29 novembre 2011, 13:04
 */

#ifndef QNODELISTVIEW_H
#define	QNODELISTVIEW_H

#include <QtGui/QtGui>

#include "Define.h"

class QNodeInteractor;
class QMeshInteractor;

class QNodeListView : public QTreeView
{
    Q_OBJECT

public:
    QNodeListView(QWidget* parent);
    virtual ~QNodeListView();

    void setModel(QSortFilterProxyModel* model);

    void highlightItem(QNodeInteractor* qnode);

public slots:
    void mousePressEvent(QMouseEvent * event);

    void onEnableNode();
    void onLockNode();

signals:
    void assignParent();
    void promoteChild();

    void removeNode();
    void setOnFloorNode();
    void pastMaterials();
    void pastPosition();
    void pastScale();
    void pastRotation();
    void pastFields();

    void select(QNodeInteractor*);
    void deselect(QNodeInteractor*);
    void deselectAll();

protected:
    void selectionChanged(const QItemSelection&, const QItemSelection&);

private:
    QItemsList selection();

private:
    QMenu* m_itemedit;
    QAction* m_promote;
    QAction* m_assign;
    QSortFilterProxyModel* m_proxy;
};

#endif	/* QNODELISTVIEW_H */

