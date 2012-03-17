/* 
 * File:   QMapMark.h
 * Author: b4n92uid
 *
 * Created on 17 mars 2012, 18:46
 */

#ifndef QMAPMARK_H
#define	QMAPMARK_H

#include <Tbe.h>
#include "QMapMarkInteractor.h"

class MainWindow;

class QMapMark : public tbe::scene::MapMark, public QMapMarkInteractor
{
public:
    QMapMark(MainWindow* mainwin);
    QMapMark(MainWindow* mainwin, const tbe::scene::MapMark& copy);
    QMapMark(const QMapMark& copy);
    virtual ~QMapMark();

    QMapMark* clone();

    void setup();
};

#endif	/* QMAPMARK_H */
