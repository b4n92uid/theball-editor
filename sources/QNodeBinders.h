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

class QNodeBinders : public QObject
{
    Q_OBJECT

public:
    QNodeBinders(QObject* parent);

    void setCurNode(tbe::scene::Node* curNode);
    tbe::scene::Node* getCurNode() const;

    void setCurmesh(tbe::scene::Mesh* curmesh);
    tbe::scene::Mesh* getCurmesh() const;

    void setCurparticles(tbe::scene::ParticlesEmiter* curparticles);
    tbe::scene::ParticlesEmiter* getCurparticles() const;

    void setCurwater(tbe::scene::Water* curwater);
    tbe::scene::Water* getCurwater() const;

signals:
    void notifyUpdate(tbe::scene::Node*);
    void notifyUpdate(tbe::scene::Mesh*);
    void notifyUpdate(tbe::scene::ParticlesEmiter*);
    void notifyUpdate(tbe::scene::Water*);

public slots:
    void SetName(const QString& s);
    void SetPos(const tbe::Vector3f& v);
    void SetMatrix(const tbe::Matrix4f& m);

    void SetDeform(double v);
    void SetSize(const tbe::Vector2f& v);
    void SetUvRepeat(const tbe::Vector2f& v);
    void SetSpeed(double v);
    void SetBlend(double v);

    void SetGravity(const tbe::Vector3f& v);
    void SetFreemove(double v);
    void SetLifeinit(double v);
    void SetLifedown(double v);
    void SetNumber(int v);
    void SetTexture(const QString& v);
    void SetContinousMode(int v);


private:
    tbe::scene::Node* m_curNode;
    tbe::scene::Mesh* m_curmesh;
    tbe::scene::ParticlesEmiter* m_curparticles;
    tbe::scene::Water* m_curwater;
};

#endif	/* QNODEBINDERS_H */

