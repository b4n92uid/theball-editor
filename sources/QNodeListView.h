/* 
 * File:   QNodeListView.h
 * Author: b4n92uid
 *
 * Created on 29 novembre 2011, 13:04
 */

#ifndef QNODELISTVIEW_H
#define	QNODELISTVIEW_H

#include <QtGui/QtGui>

#include "Metatype.h"
#include "QNodeInteractor.h"
#include "QMeshInteractor.h"

class QNodeListView : public QTreeView
{
    Q_OBJECT

public:
    QNodeListView(QWidget* parent);
    virtual ~QNodeListView();

public slots:
    void mousePressEvent(QMouseEvent * event);

    void onEnableNode();
    void onLockNode();

signals:
    void assignParent();
    void promoteChild();

    void removeNode();
    void enableNode();
    void lockNode();
    void setOnFloorNode();
    void pastMaterials();
    void pastPosition();
    void pastScale();
    void pastRotation();
    void pastFields();

    void select(QNodeInteractor*);
    void deselect(QNodeInteractor*);

protected:
    void selectionChanged(const QItemSelection&, const QItemSelection&);

private:
    QItemsList selection();

private:
    QMenu* m_itemedit;
    QAction* m_promote;
    QAction* m_assign;
};

#endif	/* QNODELISTVIEW_H */

