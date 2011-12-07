/* 
 * File:   QNodeListView.h
 * Author: b4n92uid
 *
 * Created on 29 novembre 2011, 13:04
 */

#ifndef QNODELISTVIEW_H
#define	QNODELISTVIEW_H

#include <QtGui/QtGui>

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

signals:
    void assignParent(QStandardItem*, QStandardItem*);
    void promoteChild(QStandardItem*);

private:
    QMenu* m_itemedit;
    QAction* m_promote;
    QAction* m_assign;
};

#endif	/* QNODELISTVIEW_H */

