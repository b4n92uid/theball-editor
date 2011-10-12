/* 
 * File:   NodeListProxyModel.h
 * Author: b4n92uid
 *
 * Created on 11 octobre 2011, 21:03
 */

#ifndef NODELISTPROXYMODEL_H
#define	NODELISTPROXYMODEL_H

#include <QSortFilterProxyModel>

class NodeListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    NodeListProxyModel(QObject* parent);

public slots:
    void toggleMesh(bool state);
    void toggleLight(bool state);
    void toggleParticles(bool state);
    void toggleMapMark(bool state);

    void sortByName();
    void sortByType();
    void sortFromCamera();
    void sortFromSelection();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
    bool lessThan(const QModelIndex& left, const QModelIndex& right) const;

protected:

    enum
    {
        MESH = 2, LIGHT = 4, PARTICLES = 8, MARK = 16
    };

    enum
    {
        NAME, TYPE, CAMERA, SELECTION
    };

    unsigned m_filter;
    unsigned m_sort;
};

#endif	/* NODELISTPROXYMODEL_H */

