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

    AABB selAabb = m_target->getAabb();

    Box* axe = m_mainwin->m_tbeWidget->selbox();
    axe->setMatrix(m_target->getAbsoluteMatrix());
    axe->setPos(m_target->getAbsoluteMatrix() * selAabb.getCenter());
    axe->setSize(0.25);
    axe->getMaterial("main")->setColor(Vector4f(1, 1, 0, 0.25));
}
