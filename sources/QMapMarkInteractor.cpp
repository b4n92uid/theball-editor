/* 
 * File:   QMapMarkInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:51
 */

#include "QMapMarkInteractor.h"
#include "MainWindow.h"
#include "QTBEngine.h"

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

void QMapMarkInteractor::bindWithGui()
{
    QNodeInteractor::bindWithGui();

    updateGui();

    m_mainwin->nodesGui.attribTab->setCurrentIndex(4);
}

void QMapMarkInteractor::unbindFromGui()
{
    QNodeInteractor::unbindFromGui();
}

void QMapMarkInteractor::updateGui()
{
    QNodeInteractor::updateGui();

    m_mainwin->m_tbeWidget->highlight(this);
}
