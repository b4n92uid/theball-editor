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

class QNodeListView : public QTreeView
{
    Q_OBJECT

public:
    QNodeListView(QWidget* parent);
    virtual ~QNodeListView();

public slots:
    void mousePressEvent(QMouseEvent * event);

    void onPromoteChild();
    void onAssignParent();

    void onRemoveNode();
    void onEnableNode();
    void onLockNode();
    void onSetOnFloorNode();
    void onPastPosition();
    void onPastScale();
    void onPastRotation();
    void onPastFields();

signals:
    void assignParent(QStandardItem*, QStandardItem*);
    void promoteChild(QStandardItem*);

    void removeNode(QNodeInteractor*);
    void enableNode(QNodeInteractor*);
    void lockNode(QNodeInteractor*);
    void setOnFloorNode(QNodeInteractor*);
    void pastPosition(QNodeInteractor*);
    void pastScale(QNodeInteractor*);
    void pastRotation(QNodeInteractor*);
    void pastFields(QNodeInteractor*);

private:
    QItemsList selection();

private:
    QMenu* m_itemedit;
    QAction* m_promote;
    QAction* m_assign;
};

#endif	/* QNODELISTVIEW_H */

