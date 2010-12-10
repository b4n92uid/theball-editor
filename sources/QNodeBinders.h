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

class QNode : public QObject
{
    Q_OBJECT

public:
    QNode(QObject* parent);

    void setCurNode(tbe::scene::Node* curNode);
    tbe::scene::Node* getCurNode() const;

public slots:
    void SetName(const QString& s);
    void SetPos(const tbe::Vector3f& v);
    void SetMatrix(const tbe::Matrix4f& m);

private:
    tbe::scene::Node* m_curNode;
};

class QWater : public QObject
{
    Q_OBJECT

public:
    QWater(QObject* parent);

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
    QParticles(QObject* parent);

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
    QMesh(QObject* parent);

    void setCurmesh(tbe::scene::Mesh* curmesh);
    tbe::scene::Mesh* getCurmesh() const;

private:
    tbe::scene::Mesh* m_curmesh;
};


#endif	/* QNODEBINDERS_H */

