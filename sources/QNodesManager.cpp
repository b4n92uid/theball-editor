/* 
 * File:   QNodesManager.cpp
 * Author: b4n92uid
 * 
 * Created on 4 décembre 2010, 00:04
 */

#include "QNodesManager.h"

QNodesManager::QNodesManager(QObject* parent, Ui_mainWindow* uinterface) : QObject(parent)
{
    m_nodeName = uinterface->node_name;
    m_nodePos = new QVectorBox(this, uinterface->node_pos_x, uinterface->node_pos_y, uinterface->node_pos_z);
    m_nodeRot = new QVectorBox(this, uinterface->node_rotation_x, uinterface->node_rotation_y, uinterface->node_rotation_z);

    MeshTab.add = uinterface->node_add;
    MeshTab.del = uinterface->node_del;

    WaterTab.deform = uinterface->node_water_deform;
    WaterTab.size = new QVector2Box(this, uinterface->node_water_size_x, uinterface->node_water_size_y);
    WaterTab.uvrepeat = new QVector2Box(this, uinterface->node_water_uvrepeat_x, uinterface->node_water_uvrepeat_y);
    WaterTab.speed = uinterface->node_water_speed;
    WaterTab.blend = uinterface->node_water_blend;
    WaterTab.add = uinterface->node_water_add;
    WaterTab.del = uinterface->node_water_del;

    ParticlesTab.gravity = new QVectorBox(this, uinterface->node_particles_gravity_x, uinterface->node_particles_gravity_y, uinterface->node_particles_gravity_z);
    ParticlesTab.freemove = uinterface->node_particles_freemove;
    ParticlesTab.lifeinit = uinterface->node_particles_lifeinit;
    ParticlesTab.lifedown = uinterface->node_particles_lifedown;
    ParticlesTab.number = uinterface->node_particles_number;
    ParticlesTab.texture = new QBrowsEdit(this, uinterface->node_particles_texture, uinterface->node_particles_texture_browse);
    ParticlesTab.continiousmode = uinterface->node_particles_continousmide;
    ParticlesTab.add = uinterface->node_particles_add;
    ParticlesTab.del = uinterface->node_particles_del;

    m_nodeList = uinterface->node_list;

    m_qwater = new QWater(this, NULL);
    m_qparticles = new QParticles(this, NULL);

    connect(m_nodeName, SIGNAL(textChanged(const QString&)), this, SLOT(OnNodeNameChanged(const QString&)));
    connect(m_nodePos, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(OnNodePosChanged(const tbe::Vector3f&)));
    connect(m_nodeRot, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(OnNodeRotChanged(const tbe::Vector3f&)));

    connect(MeshTab.add, SIGNAL(clicked()), this, SLOT(OnGuiMeshAlloc()));
    connect(MeshTab.del, SIGNAL(clicked()), this, SLOT(OnGuiMeshAlloc()));


    connect(WaterTab.deform, SIGNAL(valueChanged(double)), m_qwater, SLOT(SetDeform(double)));
    connect(WaterTab.size, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qwater, SLOT(SetSize(const tbe::Vector2f&)));
    connect(WaterTab.uvrepeat, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qwater, SLOT(SetUvRepeat(const tbe::Vector2f&)));
    connect(WaterTab.speed, SIGNAL(valueChanged(double)), m_qwater, SLOT(SetSpeed(double)));
    connect(WaterTab.blend, SIGNAL(valueChanged(double)), m_qwater, SLOT(SetBlend(double)));

    connect(WaterTab.add, SIGNAL(clicked()), this, SLOT(OnGuiMeshAlloc()));
    connect(WaterTab.del, SIGNAL(clicked()), this, SLOT(OnGuiMeshAlloc()));


    connect(ParticlesTab.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qparticles, SLOT(SetGravity(const tbe::Vector3f&)));
    connect(ParticlesTab.freemove, SIGNAL(valueChanged(double)), m_qparticles, SLOT(SetFreemove(double)));
    connect(ParticlesTab.lifeinit, SIGNAL(valueChanged(double)), m_qparticles, SLOT(SetLifeinit(double)));
    connect(ParticlesTab.lifedown, SIGNAL(valueChanged(double)), m_qparticles, SLOT(SetLifedown(double)));
    connect(ParticlesTab.number, SIGNAL(valueChanged(int)), m_qparticles, SLOT(SetNumber(int)));
    connect(ParticlesTab.texture, SIGNAL(textChanged(const QString&)), m_qparticles, SLOT(SetTexture(const QString&)));
    connect(ParticlesTab.continiousmode, SIGNAL(stateChanged(int)), m_qparticles, SLOT(SetContinousMode(int)));

    connect(ParticlesTab.add, SIGNAL(clicked()), this, SLOT(OnGuiMeshAlloc()));
    connect(ParticlesTab.del, SIGNAL(clicked()), this, SLOT(OnGuiMeshAlloc()));
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

void QNodesManager::OnGuiMeshAlloc()
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

void QNodesManager::OnGuiWaterAlloc()
{
    using namespace tbe;
    using namespace scene;

    Water* waterplane = new Water;

    emit RegisterWater(waterplane);
}

void QNodesManager::OnGuiParticleAlloc()
{
    using namespace tbe;
    using namespace scene;

    ParticlesEmiter* pemitter = new ParticlesEmiter;

    emit RegisterParticles(pemitter);
}

QWater::QWater(QObject* parent, tbe::scene::Water* water) : QObject(parent)
{
    m_curwater = water;
}

void QWater::SetDeform(double v)
{
    m_curwater->SetDeform(v);
}

void QWater::SetSize(const tbe::Vector2f& v)
{
    m_curwater->SetSize(v);
}

void QWater::SetUvRepeat(const tbe::Vector2f& v)
{
    m_curwater->SetUvRepeat(v);
}

void QWater::SetSpeed(double v)
{
    m_curwater->SetSpeed(v);
}

void QWater::SetBlend(double v)
{
    m_curwater->SetBlend(v);
}

void QWater::setCurwater(tbe::scene::Water* curwater)
{
    this->m_curwater = curwater;
}

tbe::scene::Water* QWater::getCurwater() const
{
    return m_curwater;
}

QParticles::QParticles(QObject* parent, tbe::scene::ParticlesEmiter* particles) : QObject(parent)
{
    m_curparticles = particles;
}

void QParticles::setCurparticles(tbe::scene::ParticlesEmiter* curparticles)
{
    this->m_curparticles = curparticles;
}

tbe::scene::ParticlesEmiter* QParticles::getCurparticles() const
{
    return m_curparticles;
}

void QParticles::SetGravity(const tbe::Vector3f& v)
{
    m_curparticles->SetGravity(v);
}

void QParticles::SetFreemove(double v)
{
    m_curparticles->SetFreeMove(v);
}

void QParticles::SetLifeinit(double v)
{
    m_curparticles->SetLifeInit(v);
}

void QParticles::SetLifedown(double v)
{
    m_curparticles->SetLifeDown(v);
}

void QParticles::SetNumber(int v)
{
    m_curparticles->SetNumber(v);
}

void QParticles::SetTexture(const QString& v)
{
    m_curparticles->SetTexture(v.toStdString());
}

void QParticles::SetContinousMode(int v)
{
    m_curparticles->SetContinousMode(v);
}

QMesh::QMesh(QObject* parent, tbe::scene::Mesh* mesh)
{
    m_curmesh = mesh;
}

void QMesh::setCurmesh(tbe::scene::Mesh* curmesh)
{
    this->m_curmesh = curmesh;
}

tbe::scene::Mesh* QMesh::getCurmesh() const
{
    return m_curmesh;
}
