/* 
 * File:   QNodesManager.cpp
 * Author: b4n92uid
 * 
 * Created on 4 décembre 2010, 00:04
 */

#include "QNodesManager.h"

Q_DECLARE_METATYPE(tbe::scene::Mesh*)

QNodesManager::QNodesManager(QObject* parent, Ui_mainWindow* uinterface) : QObject(parent)
{
    NodeTab.name = uinterface->node_name;
    NodeTab.pos = new QVectorBox(this, uinterface->node_pos_x, uinterface->node_pos_y, uinterface->node_pos_z);

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

    QStringList headerLabels;
    headerLabels << "ID" << "Nom" << "Position";

    m_nodeModel = new QStandardItemModel(this);
    m_nodeModel->setHorizontalHeaderLabels(headerLabels);

    m_nodeList = uinterface->node_list;
    m_nodeList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_nodeList->setModel(m_nodeModel);

    m_qnode = new QNode(this);
    m_qmesh = new QMesh(this);
    m_qparticles = new QParticles(this);
    m_qwater = new QWater(this);

    m_selectedNode = NULL;

    connect(m_nodeList, SIGNAL(clicked(const QModelIndex&)), this, SLOT(guiMeshSelect(const QModelIndex&)));

    // Node
    connect(NodeTab.name, SIGNAL(textChanged(const QString&)), m_qnode, SLOT(SetName(const QString&)));
    connect(NodeTab.pos, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnode, SLOT(SetPos(const tbe::Vector3f&)));

    // Mesh
    connect(MeshTab.add, SIGNAL(clicked()), this, SLOT(guiMeshNew()));

    // Water
    connect(WaterTab.deform, SIGNAL(valueChanged(double)), m_qwater, SLOT(SetDeform(double)));
    connect(WaterTab.size, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qwater, SLOT(SetSize(const tbe::Vector2f&)));
    connect(WaterTab.uvrepeat, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qwater, SLOT(SetUvRepeat(const tbe::Vector2f&)));
    connect(WaterTab.speed, SIGNAL(valueChanged(double)), m_qwater, SLOT(SetSpeed(double)));
    connect(WaterTab.blend, SIGNAL(valueChanged(double)), m_qwater, SLOT(SetBlend(double)));

    // Particles
    connect(ParticlesTab.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qparticles, SLOT(SetGravity(const tbe::Vector3f&)));
    connect(ParticlesTab.freemove, SIGNAL(valueChanged(double)), m_qparticles, SLOT(SetFreemove(double)));
    connect(ParticlesTab.lifeinit, SIGNAL(valueChanged(double)), m_qparticles, SLOT(SetLifeinit(double)));
    connect(ParticlesTab.lifedown, SIGNAL(valueChanged(double)), m_qparticles, SLOT(SetLifedown(double)));
    connect(ParticlesTab.number, SIGNAL(valueChanged(int)), m_qparticles, SLOT(SetNumber(int)));
    connect(ParticlesTab.texture, SIGNAL(textChanged(const QString&)), m_qparticles, SLOT(SetTexture(const QString&)));
    connect(ParticlesTab.continiousmode, SIGNAL(stateChanged(int)), m_qparticles, SLOT(SetContinousMode(int)));
}

QNodesManager::~QNodesManager()
{
}

void QNodesManager::guiMeshNew()
{
    QString filename = QFileDialog::getOpenFileName(qobject_cast<QWidget*>(parent()));

    using namespace tbe;
    using namespace scene;

    Mesh* mesh = NULL;

    if(filename.endsWith("ball3d"))
        mesh = new Ball3DMesh(filename.toStdString());
    else if(filename.endsWith("obj"))
        mesh = new OBJMesh(filename.toStdString());

    meshAdd(mesh);

    emit notifyMeshAdd(mesh);
}

void QNodesManager::guiMeshSelect(const QModelIndex& index)
{
    using namespace tbe::scene;

    Mesh* mesh = m_nodeModel->itemFromIndex(index)->data().value<Mesh*>();

    meshSelect(mesh);

    emit notifyMeshSelect(mesh);
}

void QNodesManager::meshAdd(tbe::scene::Mesh* mesh)
{
    int id = m_nodeModel->rowCount() + 1;
    tbe::Vector3f pos = mesh->GetPos();

    QVariant userdata;
    userdata.setValue<tbe::scene::Mesh*>(mesh);

    QStandardItem* itid = new QStandardItem(QString::number(id));
    itid->setData(userdata);

    QStandardItem* itname = new QStandardItem(mesh->GetName().c_str());
    itname->setData(userdata);

    QStandardItem* itpos = new QStandardItem(QString("%1 %2 %3").arg(pos.x).arg(pos.y).arg(pos.z));
    itpos->setData(userdata);

    QList<QStandardItem*> items;
    items << itid << itname << itpos;

    m_nodeModel->appendRow(items);
}

void QNodesManager::meshSelect(tbe::scene::Mesh* mesh)
{
    m_selectedNode = mesh;

    m_qmesh->setCurmesh(mesh);
    m_qnode->setCurNode(mesh);

    NodeTab.name->setText(mesh->GetName().c_str());
    NodeTab.pos->SetVectorValue(mesh->GetPos());
}
