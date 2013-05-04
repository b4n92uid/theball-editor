/* 
 * File:   QParticlesInteractor.h
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 07:48
 */

#ifndef QPARTICLESINTERACTOR_H
#define	QPARTICLESINTERACTOR_H

#include "QNodeInteractor.h"

class QParticlesInteractor : public QNodeInteractor
{
    Q_OBJECT

public:
    QParticlesInteractor(MainWindow* mainwin, tbe::scene::ParticlesEmiter* target);
    virtual ~QParticlesInteractor();
    
    void setup();

    virtual QParticlesInteractor* clone();

    virtual QString typeName() const;

public slots:
    void bindWithGui();
    void unbindFromGui();
    void updateGui();

public slots:
    void setGravity(const tbe::Vector3f& v);
    void setBoxsize(const tbe::Vector3f& v);
    void setBulletsize(const tbe::Vector2f& v);
    void setFreemove(double v);
    void setLifeinit(double v);
    void setLifedown(double v);
    void setNumber(int v);
    void setTexture(const QString& v);
    void setContinousMode(bool stat);
    void setPointSprite(bool stat);
    void build();

private:
    tbe::scene::ParticlesEmiter* m_target;
};

#endif	/* QPARTICLESINTERACTOR_H */

