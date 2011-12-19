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

QString QMapMarkInteractor::typeName() const
{
    return "MapMark";
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

    m_mainwin->m_tbeWidget->selBox()->setAround(m_target, tbe::Vector4f(1, 1, 0, 0.25));
}
