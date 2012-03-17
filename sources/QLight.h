/* 
 * File:   QLight.h
 * Author: b4n92uid
 *
 * Created on 17 mars 2012, 18:43
 */

#ifndef QLIGHT_H
#define	QLIGHT_H

#include <Tbe.h>
#include "QLightInteractor.h"

class MainWindow;

class QLight : public tbe::scene::Light, public QLightInteractor
{
public:
    QLight(MainWindow* mainwin);
    QLight(MainWindow* mainwin, const tbe::scene::Light& copy);
    QLight(const QLight& copy);
    virtual ~QLight();

    QLight* clone();

    void setup();
};
#endif	/* QLIGHT_H */
