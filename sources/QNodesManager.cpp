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
    m_qnodebind = new QNodeBinders(this);

    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Mesh*)), this, SIGNAL(notifyMeshSelect(tbe::scene::Mesh*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Node*)), this, SLOT(updateList()));

    // Node --------------------------------------------------------------------

    NodeTab.name = uinterface->node_name;
    NodeTab.pos = new QVectorBox(this, uinterface->node_pos_x, uinterface->node_pos_y, uinterface->node_pos_z);

    connect(NodeTab.name, SIGNAL(textChanged(const QString&)), m_qnodebind, SLOT(SetName(const QString&)));
    connect(NodeTab.pos, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(SetPos(const tbe::Vector3f&)));

    // Mesh --------------------------------------------------------------------

    MeshTab.add = uinterface->node_mesh_add;
    MeshTab.clone = uinterface->node_mesh_clone;
    MeshTab.del = uinterface->node_mesh_del;

    connect(MeshTab.add, SIGNAL(clicked()), this, SLOT(guiMeshNew()));
    connect(MeshTab.clone, SIGNAL(clicked()), this, SLOT(guiMeshClone()));

    // Water -------------------------------------------------------------------

    WaterTab.deform = uinterface->node_water_deform;
    WaterTab.size = new QVector2Box(this, uinterface->node_water_size_x, uinterface->node_water_size_y);
    WaterTab.uvrepeat = new QVector2Box(this, uinterface->node_water_uvrepeat_x, uinterface->node_water_uvrepeat_y);
    WaterTab.speed = uinterface->node_water_speed;
    WaterTab.blend = uinterface->node_water_blend;
    WaterTab.add = uinterface->node_water_add;
    WaterTab.del = uinterface->node_water_del;

    connect(WaterTab.deform, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(SetDeform(double)));
    connect(WaterTab.size, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qnodebind, SLOT(SetSize(const tbe::Vector2f&)));
    connect(WaterTab.uvrepeat, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qnodebind, SLOT(SetUvRepeat(const tbe::Vector2f&)));
    connect(WaterTab.speed, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(SetSpeed(double)));
    connect(WaterTab.blend, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(SetBlend(double)));

    // Particles ---------------------------------------------------------------

    ParticlesTab.gravity = new QVectorBox(this, uinterface->node_particles_gravity_x, uinterface->node_particles_gravity_y, uinterface->node_particles_gravity_z);
    ParticlesTab.freemove = uinterface->node_particles_freemove;
    ParticlesTab.lifeinit = uinterface->node_particles_lifeinit;
    ParticlesTab.lifedown = uinterface->node_particles_lifedown;
    ParticlesTab.number = uinterface->node_particles_number;
    ParticlesTab.texture = new QBrowsEdit(this, uinterface->node_particles_texture, uinterface->node_particles_texture_browse);
    ParticlesTab.continiousmode = uinterface->node_particles_continousmide;
    ParticlesTab.add = uinterface->node_particles_add;
    ParticlesTab.del = uinterface->node_particles_del;

    connect(ParticlesTab.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(SetGravity(const tbe::Vector3f&)));
    connect(ParticlesTab.freemove, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(SetFreemove(double)));
    connect(ParticlesTab.lifeinit, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(SetLifeinit(double)));
    connect(ParticlesTab.lifedown, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(SetLifedown(double)));
    connect(ParticlesTab.number, SIGNAL(valueChanged(int)), m_qnodebind, SLOT(SetNumber(int)));
    connect(ParticlesTab.texture, SIGNAL(textChanged(const QString&)), m_qnodebind, SLOT(SetTexture(const QString&)));
    connect(ParticlesTab.continiousmode, SIGNAL(stateChanged(int)), m_qnodebind, SLOT(SetContinousMode(int)));

    // Nodes liste -------------------------------------------------------------

    QStringList headerLabels;
    headerLabels << "Type" << "Nom";

    m_nodesListModel = new QStandardItemModel(this);
    m_nodesListModel->setHorizontalHeaderLabels(headerLabels);

    m_nodesListView = uinterface->node_list;
    m_nodesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_nodesListView->setModel(m_nodesListModel);

    connect(m_nodesListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(guiMeshSelect(const QModelIndex&)));

    // -------------------------------------------------------------------------

    m_selectedNode = NULL;
}

QNodesManager::~QNodesManager()
{
}

void QNodesManager::guiMeshNew()
{
    emit pauseRendring();

    QString filename = QFileDialog::getOpenFileName(qobject_cast<QWidget*>(parent()));

    if(!filename.isNull())
    {
        using namespace tbe;
        using namespace scene;

        Mesh* mesh = NULL;

        if(filename.endsWith("ball3d"))
            mesh = new Ball3DMesh(filename.toStdString());

        else if(filename.endsWith("obj"))
            mesh = new OBJMesh(filename.toStdString());

        emit notifyMeshAdd(mesh);

        meshAdd(mesh);
        meshSelect(mesh);
    }

    emit resumeRendring();
}

void QNodesManager::guiMeshClone()
{
    emit notifyMeshClone(m_qnodebind->getCurmesh());
}

void QNodesManager::guiMeshSelect(const QModelIndex& index)
{
    using namespace tbe::scene;

    Mesh* mesh = m_nodesListModel->itemFromIndex(index)->data().value<Mesh*>();

    meshSelect(mesh);

    emit notifyMeshSelect(mesh);
}

void QNodesManager::meshAdd(tbe::scene::Mesh* mesh)
{
    using namespace tbe::scene;

    QStandardItem* parent = NULL;

    int count = m_nodesListModel->rowCount();

    if(mesh->HasParent())
        for(int i = 0; i < count; i++)
        {
            QStandardItem* item = m_nodesListModel->item(i);

            if(item && mesh->GetParent() == item->data().value<Mesh*>())
            {
                parent = item;
                break;
            }
        }

    QVariant userdata;
    userdata.setValue<Mesh*>(mesh);

    QStandardItem* itid = new QStandardItem(QString("Mesh"));
    itid->setData(userdata);

    QStandardItem* itname = new QStandardItem(mesh->GetName().c_str());
    itname->setData(userdata);

    QList<QStandardItem*> items;
    items << itid << itname;

    if(parent)
        parent->appendRow(items);
    else
        m_nodesListModel->appendRow(items);

    m_nodesListView->resizeColumnToContents(0);
    m_nodesListView->resizeColumnToContents(1);
}

void QNodesManager::meshSelect(tbe::scene::Mesh* mesh)
{
    m_selectedNode = mesh;

    m_qnodebind->setCurmesh(mesh);

    NodeTab.name->setText(mesh->GetName().c_str());
    NodeTab.pos->setValue(mesh->GetPos());

    int count = m_nodesListModel->rowCount();

    for(int i = 0; i < count; i++)
    {
        using namespace tbe::scene;

        QStandardItem* item = m_nodesListModel->item(i);

        if(mesh == item->data().value<Mesh*>())
        {
            while(item->parent())
                item = item->parent();

            m_nodesListView->setCurrentIndex(m_nodesListModel->indexFromItem(item));
            break;
        }
    }

}

void QNodesManager::updateList()
{
    int count = m_nodesListModel->rowCount();

    for(int i = 0; i < count; i++)
    {
        using namespace tbe::scene;

        QStandardItem* item = m_nodesListModel->item(i);
        QStandardItem* itemName = m_nodesListModel->item(i, 1);

        Mesh* mesh = item->data().value<Mesh*>();
        itemName->setText(mesh->GetName().c_str());
    }
}
