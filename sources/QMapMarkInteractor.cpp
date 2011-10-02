/* 
 * File:   QMapMarkInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:51
 */

#include "QMapMarkInteractor.h"
#include "MainWindow.h"

QMapMarkInteractor::QMapMarkInteractor(MainWindow* mainwin, tbe::scene::MapMark* target)
: QNodeInteractor(mainwin, target), m_target(target)
{

}


QMapMarkInteractor::~QMapMarkInteractor()
{
}

void QMapMarkInteractor::select()
{
    deselect();

    QNodeInteractor::select();

    update();

    QStandardItem* item = m_mainwin->nodesGui.nodeItemBinder[this];

    m_mainwin->nodesGui.nodesListView->blockSignals(true);
    m_mainwin->nodesGui.nodesListView->setCurrentIndex(m_mainwin->nodesGui.nodesListModel->indexFromItem(item));
    m_mainwin->nodesGui.nodesListView->blockSignals(false);

    m_mainwin->nodesGui.attribTab->setCurrentIndex(4);
}

void QMapMarkInteractor::deselect()
{
    QNodeInteractor::deselect();
}

void QMapMarkInteractor::update()
{
    QNodeInteractor::update();

    using namespace tbe;
    using namespace scene;

    Grid* grid = m_mainwin->m_tbeWidget->getGrid();
    Box* axe = m_mainwin->m_tbeWidget->getAxe();

    AABB selAabb = m_target->getAabb();

    axe->setEnable(true);
    axe->setMatrix(m_target->getAbsoluteMatrix());
    axe->setPos(m_target->getAbsoluteMatrix() * selAabb.getCenter());
    axe->setSize(0.25);
    axe->setColor(Vector4f(1, 1, 0, 0.25));

    grid->setPos(Vector3f(0, axe->getPos().y, 0));
}
