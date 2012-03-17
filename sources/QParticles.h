/* 
 * File:   QParticles.h
 * Author: b4n92uid
 *
 * Created on 17 mars 2012, 18:44
 */

#ifndef QPARTICLES_H
#define	QPARTICLES_H

#include <Tbe.h>
#include "QParticlesInteractor.h"

class MainWindow;

class QParticles : public tbe::scene::ParticlesEmiter, public QParticlesInteractor
{
public:
    QParticles(MainWindow* mainwin);
    QParticles(MainWindow* mainwin, const tbe::scene::ParticlesEmiter& copy);
    QParticles(const QParticles& copy);
    virtual ~QParticles();

    QParticles* clone();

    void setup();
};

#endif	/* QPARTICLES_H */
