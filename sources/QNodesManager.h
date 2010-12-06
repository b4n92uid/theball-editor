/* 
 * File:   QNodesManager.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 00:04
 */

#ifndef QNODESMANAGER_H
#define	QNODESMANAGER_H

#include "ui_interface.h"
#include "QVectorBox.h"
#include "QBrowsEdit.h"
#include "QVector2Box.h"

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

class QNodesManager : public QObject
{
    Q_OBJECT

public:
    QNodesManager(QObject* parent, Ui_mainWindow* uinterface);
    virtual ~QNodesManager();

public slots:
    void OnNodeNameChanged(const QString& str);
    void OnNodePosChanged(const tbe::Vector3f& vec);
    void OnNodeRotChanged(const tbe::Vector3f& vec);

    void OnGuiMeshAlloc();
    void OnGuiParticleAlloc();
    void OnGuiWaterAlloc();

signals:
    void RegisterMesh(tbe::scene::Mesh*);
    void RegisterParticles(tbe::scene::ParticlesEmiter*);
    void RegisterWater(tbe::scene::Water*);

private:
    QLineEdit* m_nodeName;
    QVectorBox* m_nodePos;
    QVectorBox* m_nodeRot;

    struct
    {
        QPushButton* add;
        QPushButton* del;

    } MeshTab;

    struct
    {
        QDoubleSpinBox* deform;
        QVector2Box* size;
        QVector2Box* uvrepeat;
        QDoubleSpinBox* speed;
        QDoubleSpinBox* blend;

        QPushButton* add;
        QPushButton* del;

        QMap<QObject*, QString> sourcMap;

    } WaterTab;

    struct
    {
        QVectorBox* gravity;
        QDoubleSpinBox* freemove;
        QDoubleSpinBox* lifeinit;
        QDoubleSpinBox* lifedown;
        QSpinBox* number;
        QBrowsEdit* texture;
        QCheckBox* continiousmode;

        QPushButton* add;
        QPushButton* del;

        QMap<QObject*, QString> sourcMap;

    } ParticlesTab;

    QTreeView* m_nodeList;

    tbe::scene::Node* m_curNode;
    tbe::scene::Mesh* m_curMesh;
    tbe::scene::ParticlesEmiter* m_curParticles;
    tbe::scene::Water* m_curWater;

    QWater* m_qwater;
    QMesh* m_qmesh;
    QParticles* m_qparticles;
};

#endif	/* QNODESMANAGER_H */

