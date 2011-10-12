/* 
 * File:   NodeListProxyModel.cpp
 * Author: b4n92uid
 * 
 * Created on 11 octobre 2011, 21:03
 */

#include "NodeListProxyModel.h"

#include <Tbe.h>

#include "MainWindow.h"
#include "Metatype.h"

NodeListProxyModel::NodeListProxyModel(QObject* parent) : QSortFilterProxyModel(parent)
{
    m_filter = MESH | LIGHT | PARTICLES | MARK;
    m_sort = NAME;
}

void NodeListProxyModel::toggleMesh(bool state)
{
    state ? m_filter |= MESH : m_filter &= ~MESH;
    invalidateFilter();
}

void NodeListProxyModel::toggleLight(bool state)
{
    state ? m_filter |= LIGHT : m_filter &= ~LIGHT;
    invalidateFilter();
}

void NodeListProxyModel::toggleParticles(bool state)
{
    state ? m_filter |= PARTICLES : m_filter &= ~PARTICLES;
    invalidateFilter();
}

void NodeListProxyModel::toggleMapMark(bool state)
{
    state ? m_filter |= MARK : m_filter &= ~MARK;
    invalidateFilter();
}

void NodeListProxyModel::sortByName()
{
    m_sort = NAME;
    sort(0);
}

void NodeListProxyModel::sortByType()
{
    m_sort = TYPE;
    sort(0);
}

void NodeListProxyModel::sortFromCamera()
{
    m_sort = CAMERA;
    sort(0);
}

void NodeListProxyModel::sortFromSelection()
{
    m_sort = SELECTION;
    sort(0);
}

bool NodeListProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    QString type = sourceModel()->data(index, ITEM_ROLE_TYPE).toString();

    if(type == "Mesh" && !(m_filter & MESH))
        return false;

    else if(type == "Light" && !(m_filter & LIGHT))
        return false;

    else if(type == "Particles" && !(m_filter & PARTICLES))
        return false;

    else if(type == "Mark" && !(m_filter & MARK))
        return false;

    else
        return true;
}

bool NodeListProxyModel::lessThan(const QModelIndex & left, const QModelIndex & right) const
{
    if(m_sort == NAME)
    {
        return sourceModel()->data(left, ITEM_ROLE_NAME).toString() < sourceModel()->data(right, ITEM_ROLE_NAME).toString();
    }

    else if(m_sort == TYPE)
    {
        return sourceModel()->data(left, ITEM_ROLE_TYPE).toString() < sourceModel()->data(right, ITEM_ROLE_TYPE).toString();
    }

    else if(m_sort == CAMERA)
    {
        using namespace tbe;
        using namespace scene;

        QNodeInteractor* nleft = sourceModel()->data(left, ITEM_ROLE_NODE).value<QNodeInteractor*>();
        QNodeInteractor* nright = sourceModel()->data(right, ITEM_ROLE_NODE).value<QNodeInteractor*>();

        Vector3f campos = nleft->getMainwin()->tbeWidget()->camera()->getPos();

        return nleft->getTarget()->getPos() - campos < nright->getTarget()->getPos() - campos;
    }

    else if(m_sort == SELECTION)
    {
        using namespace tbe;
        using namespace scene;

        QNodeInteractor* nleft = sourceModel()->data(left, ITEM_ROLE_NODE).value<QNodeInteractor*>();
        QNodeInteractor* nright = sourceModel()->data(right, ITEM_ROLE_NODE).value<QNodeInteractor*>();

        Vector3f selpos = nleft->getMainwin()->tbeWidget()->axe()->getPos();

        return nleft->getTarget()->getPos() - selpos < nright->getTarget()->getPos() - selpos;
    }

    else
        return true;
}
