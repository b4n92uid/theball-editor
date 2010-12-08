/* 
 * File:   QNodeBinders.h
 * Author: b4n92uid
 *
 * Created on 7 décembre 2010, 21:00
 */

#ifndef QNODEBINDERS_H
#define	QNODEBINDERS_H

#include <Tbe.h>
#include <QObject>

class QWater : public QObject
{
    Q_OBJECT

public:
    QWater(QObject* parent, tbe::scene::Water* water);

    void setCurwater(tbe::scene::Water* curwater);
    tbe::scene::Water* getCurwater() const;

public slots:
    void SetDeform(double v);
    void SetSize(const tbe::Vector2f& v);
    void SetUvRepeat(const tbe::Vector2f& v);
    void SetSpeed(double v);
    void SetBlend(double v);

private:
    tbe::scene::Water* m_curwater;
};

class QParticles : public QObject
{
    Q_OBJECT

public:
    QParticles(QObject* parent, tbe::scene::ParticlesEmiter* particles);

    void setCurparticles(tbe::scene::ParticlesEmiter* curparticles);
    tbe::scene::ParticlesEmiter* getCurparticles() const;

public slots:
    void SetGravity(const tbe::Vector3f& v);
    void SetFreemove(double v);
    void SetLifeinit(double v);
    void SetLifedown(double v);
    void SetNumber(int v);
    void SetTexture(const QString& v);
    void SetContinousMode(int v);

private:
    tbe::scene::ParticlesEmiter* m_curparticles;
};

class QMesh : public QObject
{
    Q_OBJECT

public:
    QMesh(QObject* parent, tbe::scene::Mesh* mesh);

    void setCurmesh(tbe::scene::Mesh* curmesh);
    tbe::scene::Mesh* getCurmesh() const;

private:
    tbe::scene::Mesh* m_curmesh;
};


#endif	/* QNODEBINDERS_H */

