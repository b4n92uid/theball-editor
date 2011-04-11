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

    void setCurlight(tbe::scene::Light* curlight);
    tbe::scene::Light* getCurlight() const;

    void setCurmesh(tbe::scene::Mesh* curmesh);
    tbe::scene::Mesh* getCurmesh() const;

    void setCurparticles(tbe::scene::ParticlesEmiter* curparticles);
    tbe::scene::ParticlesEmiter* getCurparticles() const;

    void setCurwater(tbe::scene::Water* curwater);
    tbe::scene::Water* getCurwater() const;

signals:
    void notifyUpdate(tbe::scene::Node*);
    void notifyUpdate(tbe::scene::Light*);
    void notifyUpdate(tbe::scene::Mesh*);
    void notifyUpdate(tbe::scene::ParticlesEmiter*);
    void notifyUpdate(tbe::scene::Water*);

public slots:
    void nodeSetName(const QString& s);
    void nodeSetPos(const tbe::Vector3f& v);
    void nodeSetMatrix(const tbe::Matrix4f& m);

    void waterSetDeform(double v);
    void waterSetSize(const tbe::Vector2f& v);
    void waterSetUvRepeat(const tbe::Vector2f& v);
    void waterSetSpeed(double v);
    void waterSetBlend(double v);

    void particleSetGravity(const tbe::Vector3f& v);
    void particleSetFreemove(double v);
    void particleSetLifeinit(double v);
    void particleSetLifedown(double v);
    void particleSetNumber(int v);
    void particleSetTexture(const QString& v);
    void particleSetContinousMode(int v);
    void particleBuild();

    void lightSetType(int type);
    void lightSetAmbiant(const tbe::Vector3f& value);
    void lightSetDiffuse(const tbe::Vector3f& value);
    void lightSetSpecular(const tbe::Vector3f& value);
    void lightSetRadius(double value);


private:
    tbe::scene::Node* m_curNode;
    tbe::scene::Mesh* m_curmesh;
    tbe::scene::ParticlesEmiter* m_curparticles;
    tbe::scene::Light* m_curlight;
    tbe::scene::Water* m_curwater;
};

#endif	/* QNODEBINDERS_H */

