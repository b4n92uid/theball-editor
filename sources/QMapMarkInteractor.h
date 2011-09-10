/* 
 * File:   QMapMarkInteractor.h
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 07:51
 */

#ifndef QMAPMARKINTERACTOR_H
#define	QMAPMARKINTERACTOR_H

#include "QNodeInteractor.h"

class QMapMarkInteractor : public QNodeInteractor
{
    Q_OBJECT

public:
    QMapMarkInteractor(MainWindow* mainwin, tbe::scene::MapMark* target);
    virtual ~QMapMarkInteractor();

public slots:
    void setup();
    void select();
    void deselect();
    void update();

private:
    tbe::scene::MapMark* m_target;
};

#endif	/* QMAPMARKINTERACTOR_H */

