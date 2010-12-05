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

    void OnNodeMeshSet();
    void OnNodeMeshAlloc();

    void OnNodeWaterSet();
    void OnNodeWaterAlloc();

    void OnNodeParticleSet();
    void OnNodeParticleAlloc();

    void SetCurrentMesh(tbe::scene::Mesh* mesh);
    void SetCurrentParticle(tbe::scene::ParticlesEmiter* particles);
    void SetCurrentWater(tbe::scene::Water* water);

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
        QDoubleSpinBox* size;
        QDoubleSpinBox* uvrepeat;
        QDoubleSpinBox* speed;
        QDoubleSpinBox* blend;

        QPushButton* add;
        QPushButton* del;

    } WaterTab;

    struct
    {
        QDoubleSpinBox* gravity;
        QDoubleSpinBox* freemove;
        QDoubleSpinBox* lifeinit;
        QDoubleSpinBox* lifedown;
        QSpinBox* number;
        QBrowsEdit* texture;
        QCheckBox* continiousmode;

        QPushButton* add;
        QPushButton* del;

    } ParticlesTab;

    QTreeView* m_nodeList;

    tbe::scene::Node* m_curNode;
    tbe::scene::Mesh* m_curMesh;
    tbe::scene::ParticlesEmiter* m_curParticles;
    tbe::scene::Water* m_curWater;
};

#endif	/* QNODESMANAGER_H */

