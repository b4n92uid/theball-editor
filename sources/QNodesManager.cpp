/* 
 * File:   QNodesManager.cpp
 * Author: b4n92uid
 * 
 * Created on 4 décembre 2010, 00:04
 */

#include "QNodesManager.h"

#define ContentType (Qt::UserRole + 2)

enum NodeType
{
    IsUnknown, IsMesh, IsParticles, IsWater, IsLight
};

Q_DECLARE_METATYPE(tbe::scene::Node*)
Q_DECLARE_METATYPE(tbe::scene::Mesh*)
Q_DECLARE_METATYPE(tbe::scene::Water*)
Q_DECLARE_METATYPE(tbe::scene::ParticlesEmiter*)
Q_DECLARE_METATYPE(tbe::scene::Light*)
Q_DECLARE_METATYPE(NodeType)

typedef QList<QStandardItem*> QItemsList;

QNodesManager::QNodesManager(QObject* parent, Ui_mainWindow* uinterface) : QObject(parent)
{
    m_qnodebind = new QNodeBinders(this);

    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Mesh*)), this, SIGNAL(notifyMeshSelect(tbe::scene::Mesh*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Light*)), this, SIGNAL(notifyLightSelect(tbe::scene::Light*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Node*)), this, SLOT(updateList()));

    // Node --------------------------------------------------------------------

    NodeTab.name = uinterface->node_name;
    NodeTab.pos = new QVectorBox(this, uinterface->node_pos_x, uinterface->node_pos_y, uinterface->node_pos_z);

    connect(NodeTab.name, SIGNAL(textChanged(const QString&)), m_qnodebind, SLOT(nodeSetName(const QString&)));
    connect(NodeTab.pos, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(nodeSetPos(const tbe::Vector3f&)));

    // Mesh --------------------------------------------------------------------

    MeshTab.add = uinterface->node_mesh_add;
    MeshTab.clone = uinterface->node_mesh_clone;
    MeshTab.del = uinterface->node_mesh_del;

    connect(MeshTab.add, SIGNAL(clicked()), this, SLOT(guiMeshNew()));
    connect(MeshTab.clone, SIGNAL(clicked()), this, SLOT(guiMeshClone()));
    connect(MeshTab.del, SIGNAL(clicked()), this, SLOT(guiMeshDelete()));

    // Water -------------------------------------------------------------------

    WaterTab.deform = uinterface->node_water_deform;
    WaterTab.size = new QVector2Box(this, uinterface->node_water_size_x, uinterface->node_water_size_y);
    WaterTab.uvrepeat = new QVector2Box(this, uinterface->node_water_uvrepeat_x, uinterface->node_water_uvrepeat_y);
    WaterTab.speed = uinterface->node_water_speed;
    WaterTab.blend = uinterface->node_water_blend;
    WaterTab.add = uinterface->node_water_add;
    WaterTab.clone = uinterface->node_water_clone;
    WaterTab.del = uinterface->node_water_del;

    connect(WaterTab.deform, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(waterSetDeform(double)));
    connect(WaterTab.size, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qnodebind, SLOT(waterSetSize(const tbe::Vector2f&)));
    connect(WaterTab.uvrepeat, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qnodebind, SLOT(waterSetUvRepeat(const tbe::Vector2f&)));
    connect(WaterTab.speed, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(waterSetSpeed(double)));
    connect(WaterTab.blend, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(waterSetBlend(double)));

    // Particles ---------------------------------------------------------------

    ParticlesTab.gravity = new QVectorBox(this, uinterface->node_particles_gravity_x, uinterface->node_particles_gravity_y, uinterface->node_particles_gravity_z);
    ParticlesTab.freemove = uinterface->node_particles_freemove;
    ParticlesTab.lifeinit = uinterface->node_particles_lifeinit;
    ParticlesTab.lifedown = uinterface->node_particles_lifedown;
    ParticlesTab.number = uinterface->node_particles_number;
    ParticlesTab.texture = new QBrowsEdit(this, uinterface->node_particles_texture, uinterface->node_particles_texture_browse);
    ParticlesTab.continiousmode = uinterface->node_particles_continousmide;
    ParticlesTab.add = uinterface->node_particles_add;
    ParticlesTab.clone = uinterface->node_particles_clone;
    ParticlesTab.del = uinterface->node_particles_del;

    connect(ParticlesTab.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(particleSetGravity(const tbe::Vector3f&)));
    connect(ParticlesTab.freemove, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(particleSetFreemove(double)));
    connect(ParticlesTab.lifeinit, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(particleSetLifeinit(double)));
    connect(ParticlesTab.lifedown, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(particleSetLifedown(double)));
    connect(ParticlesTab.number, SIGNAL(valueChanged(int)), m_qnodebind, SLOT(particleSetNumber(int)));
    connect(ParticlesTab.texture, SIGNAL(textChanged(const QString&)), m_qnodebind, SLOT(particleSetTexture(const QString&)));
    connect(ParticlesTab.continiousmode, SIGNAL(stateChanged(int)), m_qnodebind, SLOT(particleSetContinousMode(int)));

    // Lights ------------------------------------------------------------------

    LighTab.type = uinterface->node_light_type;

    LighTab.ambiant = new QVectorBox(this, uinterface->node_light_ambiant_x, uinterface->node_light_ambiant_y, uinterface->node_light_ambiant_z);
    LighTab.diffuse = new QVectorBox(this, uinterface->node_light_diffuse_x, uinterface->node_light_diffuse_y, uinterface->node_light_diffuse_z);
    LighTab.specular = new QVectorBox(this, uinterface->node_light_specular_x, uinterface->node_light_specular_y, uinterface->node_light_specular_z);

    LighTab.radius = uinterface->node_light_radius;

    LighTab.add = uinterface->node_light_add;
    LighTab.clone = uinterface->node_light_clone;
    LighTab.del = uinterface->node_light_del;

    connect(LighTab.type, SIGNAL(activated(int)), m_qnodebind, SLOT(lightSetType(int)));
    connect(LighTab.ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(lightSetAmbiant(const tbe::Vector3f&)));
    connect(LighTab.diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(lightSetDiffuse(const tbe::Vector3f&)));
    connect(LighTab.specular, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(lightSetSpecular(const tbe::Vector3f&)));
    connect(LighTab.radius, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(lightSetRadius(double)));

    connect(LighTab.add, SIGNAL(clicked()), this, SLOT(guiLightNew()));
    connect(LighTab.clone, SIGNAL(clicked()), this, SLOT(guiLightClone()));
    connect(LighTab.del, SIGNAL(clicked()), this, SLOT(guiLightDelete()));

    // Nodes liste -------------------------------------------------------------

    QStringList headerLabels;
    headerLabels << "Type" << "Nom";

    m_nodesListModel = new QStandardItemModel(this);
    m_nodesListModel->setHorizontalHeaderLabels(headerLabels);

    m_nodesListView = uinterface->node_list;
    m_nodesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_nodesListView->setModel(m_nodesListModel);

    connect(m_nodesListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(guiSelect(const QModelIndex&)));
}

QNodesManager::~QNodesManager()
{
}

void QNodesManager::guiSelect(const QModelIndex& index)
{
    using namespace tbe::scene;

    QStandardItem* item = m_nodesListModel->itemFromIndex(index);

    // NOTE Conversion to NodeType fail (return 0)
    int type = item->data(ContentType).toInt();

    switch(type)
    {
        case IsMesh:
        {
            Mesh* mesh = item->data().value<Mesh*>();
            meshSelect(mesh, false);
            emit notifyMeshSelect(mesh);
        }
            break;
        case IsParticles:
        {
            // ParticlesEmiter* pemiter = item->data().value<ParticlesEmiter*>();
        }
            break;
        case IsWater:
        {
            // Water* water = item->data().value<Water*>();
        }
            break;
        case IsLight:
        {
            Light* light = item->data().value<Light*>();
            lightSelect(light, false);
            emit notifyLightSelect(light);
        }
            break;
    }
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

        meshAdd(mesh);
        meshSelect(mesh);

        emit notifyMeshNew(mesh);
    }

    emit resumeRendring();
}

void QNodesManager::guiMeshClone()
{
    if(m_qnodebind->getCurmesh())
        emit notifyMeshClone(m_qnodebind->getCurmesh());
}

void QNodesManager::guiMeshDelete()
{
    if(tbe::scene::Mesh * mesh = m_qnodebind->getCurmesh())
    {
        QStandardItem* item = mesh->GetUserData().GetValue<QStandardItem*> ();
        QStandardItem* parent = item->parent();

        if(parent)
            parent->removeRow(item->row());
        else
            m_nodesListModel->removeRow(item->row());

        emit notifyMeshDelete(mesh);
    }
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
    itid->setData(IsMesh, ContentType);

    QStandardItem* itname = new QStandardItem(mesh->GetName().c_str());
    itname->setData(userdata);
    itname->setData(IsMesh, ContentType);

    QItemsList items;
    items << itid << itname;

    if(parent)
        parent->appendRow(items);
    else
        m_nodesListModel->appendRow(items);

    mesh->SetUserData(itid);

    m_nodesListView->resizeColumnToContents(0);
    m_nodesListView->resizeColumnToContents(1);
}

void QNodesManager::meshSelect(tbe::scene::Mesh* mesh, bool upList)
{
    m_qnodebind->setCurmesh(mesh);

    if(!mesh)
        return;

    NodeTab.name->setText(mesh->GetName().c_str());
    NodeTab.pos->setValue(mesh->GetPos());

    if(upList)
    {
        QStandardItem* item = mesh->GetUserData().GetValue<QStandardItem*> ();
        m_nodesListView->setCurrentIndex(m_nodesListModel->indexFromItem(item));
    }
}

void QNodesManager::guiLightNew()
{
    using namespace tbe::scene;

    Light* light = new Light;

    lightAdd(light);
    lightSelect(light);

    emit notifyLightNew(light);
}

void QNodesManager::guiLightClone()
{
    if(m_qnodebind->getCurlight())
        emit notifyLightClone(m_qnodebind->getCurlight());
}

void QNodesManager::guiLightDelete()
{
    if(tbe::scene::Light * light = m_qnodebind->getCurlight())
    {
        QStandardItem* item = light->GetUserData().GetValue<QStandardItem*> ();
        QStandardItem* parent = item->parent();

        if(parent)
            parent->removeRow(item->row());
        else
            m_nodesListModel->removeRow(item->row());

        emit notifyLightDelete(m_qnodebind->getCurlight());
    }
}

void QNodesManager::lightAdd(tbe::scene::Light* light)
{
    using namespace tbe::scene;

    QVariant userData;
    userData.setValue(light);

    QStandardItem* itemType = new QStandardItem("Light");
    itemType->setData(userData);
    itemType->setData(IsLight, ContentType);

    QStandardItem* itemName = new QStandardItem(light->GetName().c_str());
    itemName->setData(userData);
    itemName->setData(IsLight, ContentType);

    QItemsList items;
    items << itemType << itemName;

    m_nodesListModel->appendRow(items);

    light->SetUserData(itemType);

    m_nodesListView->resizeColumnToContents(0);
    m_nodesListView->resizeColumnToContents(1);
}

void QNodesManager::lightSelect(tbe::scene::Light* light, bool upList)
{
    m_qnodebind->setCurlight(light);

    if(!light)
        return;

    NodeTab.name->setText(light->GetName().c_str());
    NodeTab.pos->setValue(light->GetPos());

    LighTab.type->setCurrentIndex((int)light->GetType());
    LighTab.ambiant->setValue(vec43(light->GetAmbient()));
    LighTab.diffuse->setValue(vec43(light->GetDiffuse()));
    LighTab.specular->setValue(vec43(light->GetSpecular()));
    LighTab.radius->setValue(light->GetRadius());

    if(upList)
    {
        QStandardItem* item = light->GetUserData().GetValue<QStandardItem*> ();
        m_nodesListView->setCurrentIndex(m_nodesListModel->indexFromItem(item));
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

        Node* node = NULL;

        if(item->data().canConvert<Light*>())
            node = item->data().value<Light*>();

        else if(item->data().canConvert<Mesh*>())
            node = item->data().value<Mesh*>();

        else if(item->data().canConvert<ParticlesEmiter*>())
            node = item->data().value<ParticlesEmiter*>();

        else if(item->data().canConvert<Water*>())
            node = item->data().value<Water*>();

        itemName->setText(node->GetName().c_str());
    }
}

