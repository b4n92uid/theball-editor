/* 
 * File:   QNodesManager.cpp
 * Author: b4n92uid
 * 
 * Created on 4 décembre 2010, 00:04
 */

#include "QNodesManager.h"

QNodesManager::QNodesManager(QObject* parent, Ui_mainWindow* uinterface) : QObject(parent)
{
    connect(m_nodeName, SIGNAL(textChanged(const QString&)), this, SLOT(OnNodeNameChanged(const QString&)));
    connect(m_nodePos, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(OnNodePosChanged(const tbe::Vector3f&)));
    connect(m_nodeRot, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(OnNodeRotChanged(const tbe::Vector3f&)));

    connect(MeshTab.add, SIGNAL(clicked()), this, SLOT(OnNodeMeshAlloc()));
    connect(MeshTab.del, SIGNAL(clicked()), this, SLOT(OnNodeMeshAlloc()));

    connect(WaterTab.deform, SIGNAL(valueChanged(double)), this, SLOT(OnNodeMeshSet()));
    connect(WaterTab.size, SIGNAL(valueChanged(double)), this, SLOT(OnNodeMeshSet()));
    connect(WaterTab.uvrepeat, SIGNAL(valueChanged(double)), this, SLOT(OnNodeMeshSet()));
    connect(WaterTab.speed, SIGNAL(valueChanged(double)), this, SLOT(OnNodeMeshSet()));
    connect(WaterTab.blend, SIGNAL(valueChanged(double)), this, SLOT(OnNodeMeshSet()));

    connect(WaterTab.add, SIGNAL(clicked()), this, SLOT(OnNodeMeshAlloc()));
    connect(WaterTab.del, SIGNAL(clicked()), this, SLOT(OnNodeMeshAlloc()));

    connect(ParticlesTab.gravity, SIGNAL(valueChanged(double)), this, SLOT(OnNodeParticleSet()));
    connect(ParticlesTab.freemove, SIGNAL(valueChanged(double)), this, SLOT(OnNodeParticleSet()));
    connect(ParticlesTab.lifeinit, SIGNAL(valueChanged(double)), this, SLOT(OnNodeParticleSet()));
    connect(ParticlesTab.lifedown, SIGNAL(valueChanged(double)), this, SLOT(OnNodeParticleSet()));
    connect(ParticlesTab.number, SIGNAL(valueChanged(double)), this, SLOT(OnNodeParticleSet()));
    connect(ParticlesTab.texture, SIGNAL(textChanged(const QString&)), this, SLOT(OnNodeParticleSet()));
    connect(ParticlesTab.continiousmode, SIGNAL(stateChanged(int)), this, SLOT(OnNodeParticleSet()));

    connect(ParticlesTab.add, SIGNAL(clicked()), this, SLOT(OnNodeMeshAlloc()));
    connect(ParticlesTab.del, SIGNAL(clicked()), this, SLOT(OnNodeMeshAlloc()));
}

QNodesManager::~QNodesManager()
{
}

void QNodesManager::OnNodeNameChanged(const QString& str)
{
    m_curNode->SetName(str.toStdString());
}

void QNodesManager::OnNodePosChanged(const tbe::Vector3f& vec)
{
    m_curNode->SetPos(vec);
}

void QNodesManager::OnNodeRotChanged(const tbe::Vector3f& vec)
{
    m_curNode->GetMatrix().SetRotate(vec);
}

void QNodesManager::OnNodeMeshSet()
{
}

void QNodesManager::OnNodeMeshAlloc()
{
    QString filename = QFileDialog::getOpenFileName(qobject_cast<QWidget*>(parent()));

    using namespace tbe;
    using namespace scene;

    Mesh* mesh = NULL;

    if(filename.endsWith("ball3d"))
        mesh = new Ball3DMesh(filename.toStdString());
    else if(filename.endsWith("obj"))
        mesh = new OBJMesh(filename.toStdString());

    emit RegisterMesh(mesh);
}

void QNodesManager::OnNodeWaterSet()
{
    if(sender() == WaterTab.deform)
        m_curWater->SetDeform(WaterTab.deform->value());
    else if(sender() == WaterTab.size)
        m_curWater->SetSize(WaterTab.size->value());
    else if(sender() == WaterTab.uvrepeat)
        m_curWater->SetUvRepeat(WaterTab.uvrepeat->value());
    else if(sender() == WaterTab.speed)
        m_curWater->SetSpeed(WaterTab.speed->value());
    else if(sender() == WaterTab.blend)
        m_curWater->SetBlend(WaterTab.blend->value());

}

void QNodesManager::OnNodeWaterAlloc()
{
    using namespace tbe;
    using namespace scene;

    Water* waterplane = new Water;

    emit RegisterWater(waterplane);
}

void QNodesManager::OnNodeParticleSet()
{
    if(sender() == ParticlesTab.gravity)
        m_curParticles->SetGravity(ParticlesTab.gravity->value());
    else if(sender() == ParticlesTab.freemove)
        m_curParticles->SetFreeMove(ParticlesTab.freemove->value());
    else if(sender() == ParticlesTab.lifeinit)
        m_curParticles->SetLifeInit(ParticlesTab.lifeinit->value());
    else if(sender() == ParticlesTab.lifedown)
        m_curParticles->SetLifeDown(ParticlesTab.lifedown->value());
    else if(sender() == ParticlesTab.number)
        m_curParticles->SetNumber(ParticlesTab.number->value());
    else if(sender() == ParticlesTab.texture)
        m_curParticles->SetTexture(ParticlesTab.texture->GetOpenFileName().toStdString());
    else if(sender() == ParticlesTab.continiousmode)
        m_curParticles->SetContinousMode(ParticlesTab.continiousmode->isChecked());

}

void QNodesManager::OnNodeParticleAlloc()
{
    using namespace tbe;
    using namespace scene;

    ParticlesEmiter* pemitter = new ParticlesEmiter;

    emit RegisterParticles(pemitter);
}

void QNodesManager::SetCurrentMesh(tbe::scene::Mesh* mesh)
{
    m_curMesh = mesh;
    m_curNode = mesh;
}

void QNodesManager::SetCurrentParticle(tbe::scene::ParticlesEmiter* particles)
{
    m_curParticles = particles;
    m_curNode = particles;
}

void QNodesManager::SetCurrentWater(tbe::scene::Water* water)
{
    m_curWater = water;
    m_curNode = water;
}