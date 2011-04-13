/* 
 * File:   MainWindow.cpp
 * Author: b4n92uid
 * 
 * Created on 3 décembre 2010, 17:20
 */

#include "MainWindow.h"

enum NodeType
{
    IsUnknown, IsMesh, IsParticles, IsWater, IsLight
};

typedef QList<QStandardItem*> QItemsList;

#define ContentType (Qt::UserRole + 2)

Q_DECLARE_METATYPE(tbe::scene::Node*)
Q_DECLARE_METATYPE(tbe::scene::Mesh*)
Q_DECLARE_METATYPE(tbe::scene::Water*)
Q_DECLARE_METATYPE(tbe::scene::ParticlesEmiter*)
Q_DECLARE_METATYPE(tbe::scene::Light*)
Q_DECLARE_METATYPE(NodeType)

MainWindow::MainWindow()
{
    m_uinterface.setupUi(this);

    connect(m_uinterface.actionOuvrire, SIGNAL(triggered()), this, SLOT(openSceneDialog()));
    connect(m_uinterface.actionEnregistrer, SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(m_uinterface.actionEnregistrer_sous, SIGNAL(triggered()), this, SLOT(saveSceneDialog()));
    connect(m_uinterface.actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_tbeWidget = m_uinterface.glwidget;

    m_infoText = m_uinterface.infoText;

    nodesGui.attribTab = m_uinterface.attribTab;

    connect(this, SIGNAL(pauseRendring()), m_tbeWidget, SLOT(pauseRendring()));
    connect(this, SIGNAL(resumeRendring()), m_tbeWidget, SLOT(resumeRendring()));

    // Node --------------------------------------------------------------------

    m_qnodebind = new QNodeBinders(this);

    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Mesh*)), this, SLOT(meshSelect(tbe::scene::Mesh*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Light*)), this, SLOT(lightSelect(tbe::scene::Light*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesSelect(tbe::scene::ParticlesEmiter*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Node*)), this, SLOT(updateList()));

    nodesGui.name = m_uinterface.node_name;
    nodesGui.pos = new QVectorBox(this, m_uinterface.node_pos_x, m_uinterface.node_pos_y, m_uinterface.node_pos_z);

    nodesGui.nodeUp = m_uinterface.node_list_up;
    nodesGui.nodeDown = m_uinterface.node_list_down;
    nodesGui.nodeRight = m_uinterface.node_list_makechild;
    nodesGui.nodeLeft = m_uinterface.node_list_makeparent;

    QSignalMapper* nodeMoveBind = new QSignalMapper(this);
    nodeMoveBind->setMapping(nodesGui.nodeUp, 1);
    nodeMoveBind->setMapping(nodesGui.nodeDown, 2);
    nodeMoveBind->setMapping(nodesGui.nodeLeft, 3);
    nodeMoveBind->setMapping(nodesGui.nodeRight, 4);

    connect(nodesGui.nodeUp, SIGNAL(clicked()), nodeMoveBind, SLOT(map()));
    connect(nodesGui.nodeDown, SIGNAL(clicked()), nodeMoveBind, SLOT(map()));
    connect(nodesGui.nodeLeft, SIGNAL(clicked()), nodeMoveBind, SLOT(map()));
    connect(nodesGui.nodeRight, SIGNAL(clicked()), nodeMoveBind, SLOT(map()));

    connect(nodeMoveBind, SIGNAL(mapped(int)), this, SLOT(scopeNode(int)));

    connect(nodesGui.name, SIGNAL(textChanged(const QString&)), m_qnodebind, SLOT(nodeSetName(const QString&)));
    connect(nodesGui.pos, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(nodeSetPos(const tbe::Vector3f&)));

    // -------- Mesh

    nodesGui.meshTab.add = m_uinterface.node_mesh_add;
    nodesGui.meshTab.clone = m_uinterface.node_mesh_clone;
    nodesGui.meshTab.del = m_uinterface.node_mesh_del;

    connect(nodesGui.meshTab.add, SIGNAL(clicked()), this, SLOT(guiMeshNew()));
    connect(nodesGui.meshTab.clone, SIGNAL(clicked()), this, SLOT(guiMeshClone()));
    connect(nodesGui.meshTab.del, SIGNAL(clicked()), this, SLOT(guiMeshDelete()));

    // -------- Water

    nodesGui.waterTab.deform = m_uinterface.node_water_deform;
    nodesGui.waterTab.size = new QVector2Box(this, m_uinterface.node_water_size_x, m_uinterface.node_water_size_y);
    nodesGui.waterTab.uvrepeat = new QVector2Box(this, m_uinterface.node_water_uvrepeat_x, m_uinterface.node_water_uvrepeat_y);
    nodesGui.waterTab.speed = m_uinterface.node_water_speed;
    nodesGui.waterTab.blend = m_uinterface.node_water_blend;
    nodesGui.waterTab.add = m_uinterface.node_water_add;
    nodesGui.waterTab.clone = m_uinterface.node_water_clone;
    nodesGui.waterTab.del = m_uinterface.node_water_del;

    connect(nodesGui.waterTab.deform, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(waterSetDeform(double)));
    connect(nodesGui.waterTab.size, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qnodebind, SLOT(waterSetSize(const tbe::Vector2f&)));
    connect(nodesGui.waterTab.uvrepeat, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qnodebind, SLOT(waterSetUvRepeat(const tbe::Vector2f&)));
    connect(nodesGui.waterTab.speed, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(waterSetSpeed(double)));
    connect(nodesGui.waterTab.blend, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(waterSetBlend(double)));

    // -------- Particles

    nodesGui.particlesTab.gravity = new QVectorBox(this, m_uinterface.node_particles_gravity_x, m_uinterface.node_particles_gravity_y, m_uinterface.node_particles_gravity_z);
    nodesGui.particlesTab.freemove = m_uinterface.node_particles_freemove;
    nodesGui.particlesTab.lifeinit = m_uinterface.node_particles_lifeinit;
    nodesGui.particlesTab.lifedown = m_uinterface.node_particles_lifedown;
    nodesGui.particlesTab.number = m_uinterface.node_particles_number;
    nodesGui.particlesTab.texture = new QBrowsEdit(this, m_uinterface.node_particles_texture, m_uinterface.node_particles_texture_browse);
    nodesGui.particlesTab.continiousmode = m_uinterface.node_particles_continousmide;
    nodesGui.particlesTab.build = m_uinterface.node_particles_build;
    nodesGui.particlesTab.add = m_uinterface.node_particles_add;
    nodesGui.particlesTab.clone = m_uinterface.node_particles_clone;
    nodesGui.particlesTab.del = m_uinterface.node_particles_del;

    connect(nodesGui.particlesTab.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(particleSetGravity(const tbe::Vector3f&)));
    connect(nodesGui.particlesTab.freemove, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(particleSetFreemove(double)));
    connect(nodesGui.particlesTab.lifeinit, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(particleSetLifeinit(double)));
    connect(nodesGui.particlesTab.lifedown, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(particleSetLifedown(double)));
    connect(nodesGui.particlesTab.number, SIGNAL(valueChanged(int)), m_qnodebind, SLOT(particleSetNumber(int)));
    connect(nodesGui.particlesTab.texture, SIGNAL(textChanged(const QString&)), m_qnodebind, SLOT(particleSetTexture(const QString&)));
    connect(nodesGui.particlesTab.continiousmode, SIGNAL(stateChanged(int)), m_qnodebind, SLOT(particleSetContinousMode(int)));
    connect(nodesGui.particlesTab.build, SIGNAL(clicked()), m_qnodebind, SLOT(particleBuild()));

    // -------- Lights

    nodesGui.lighTab.type = m_uinterface.node_light_type;

    nodesGui.lighTab.ambiant = new QVectorBox(this, m_uinterface.node_light_ambiant_x, m_uinterface.node_light_ambiant_y, m_uinterface.node_light_ambiant_z);
    nodesGui.lighTab.diffuse = new QVectorBox(this, m_uinterface.node_light_diffuse_x, m_uinterface.node_light_diffuse_y, m_uinterface.node_light_diffuse_z);
    nodesGui.lighTab.specular = new QVectorBox(this, m_uinterface.node_light_specular_x, m_uinterface.node_light_specular_y, m_uinterface.node_light_specular_z);

    nodesGui.lighTab.radius = m_uinterface.node_light_radius;

    nodesGui.lighTab.add = m_uinterface.node_light_add;
    nodesGui.lighTab.clone = m_uinterface.node_light_clone;
    nodesGui.lighTab.del = m_uinterface.node_light_del;

    connect(nodesGui.lighTab.type, SIGNAL(activated(int)), m_qnodebind, SLOT(lightSetType(int)));
    connect(nodesGui.lighTab.ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(lightSetAmbiant(const tbe::Vector3f&)));
    connect(nodesGui.lighTab.diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(lightSetDiffuse(const tbe::Vector3f&)));
    connect(nodesGui.lighTab.specular, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(lightSetSpecular(const tbe::Vector3f&)));
    connect(nodesGui.lighTab.radius, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(lightSetRadius(double)));

    connect(nodesGui.lighTab.add, SIGNAL(clicked()), this, SLOT(guiLightNew()));
    connect(nodesGui.lighTab.clone, SIGNAL(clicked()), this, SLOT(guiLightClone()));
    connect(nodesGui.lighTab.del, SIGNAL(clicked()), this, SLOT(guiLightDelete()));

    // Nodes liste -------------------------------------------------------------

    QStringList headerLabels;
    headerLabels << "Type" << "Nom";

    nodesGui.nodesListModel = new QStandardItemModel(this);
    nodesGui.nodesListModel->setHorizontalHeaderLabels(headerLabels);

    nodesGui.nodesListView = m_uinterface.node_list;
    nodesGui.nodesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    nodesGui.nodesListView->setModel(nodesGui.nodesListModel);

    connect(nodesGui.nodesListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(guiSelect(const QModelIndex&)));

    connect(m_tbeWidget, SIGNAL(notifyMeshAdd(tbe::scene::Mesh*)), this, SLOT(meshAdd(tbe::scene::Mesh*)));
    connect(m_tbeWidget, SIGNAL(notifyMeshSelect(tbe::scene::Mesh*)), this, SLOT(meshSelect(tbe::scene::Mesh*)));

    connect(m_tbeWidget, SIGNAL(notifyLightAdd(tbe::scene::Light*)), this, SLOT(lightAdd(tbe::scene::Light*)));
    connect(m_tbeWidget, SIGNAL(notifyLightSelect(tbe::scene::Light*)), this, SLOT(lightSelect(tbe::scene::Light*)));

    connect(m_tbeWidget, SIGNAL(notifyParticlesAdd(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesAdd(tbe::scene::ParticlesEmiter*)));
    connect(m_tbeWidget, SIGNAL(notifyParticlesSelect(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesSelect(tbe::scene::ParticlesEmiter*)));

    // Environment -------------------------------------------------------------

    envGui.sceneAmbiant = new QVectorBox(this, m_uinterface.env_ambient_x, m_uinterface.env_ambient_y, m_uinterface.env_ambient_z);
    connect(envGui.sceneAmbiant, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(ambiantScene(const tbe::Vector3f&)));

    envGui.skybox.apply = m_uinterface.skybox_apply;
    envGui.skybox.enable = m_uinterface.skybox_enable;

    envGui.skybox.textures[0] = new QBrowsEdit(this, m_uinterface.skybox_front, m_uinterface.skybox_front_browse);
    envGui.skybox.textures[1] = new QBrowsEdit(this, m_uinterface.skybox_back, m_uinterface.skybox_back_browse);
    envGui.skybox.textures[2] = new QBrowsEdit(this, m_uinterface.skybox_left, m_uinterface.skybox_left_browse);
    envGui.skybox.textures[3] = new QBrowsEdit(this, m_uinterface.skybox_right, m_uinterface.skybox_right_browse);
    envGui.skybox.textures[4] = new QBrowsEdit(this, m_uinterface.skybox_top, m_uinterface.skybox_top_browse);
    envGui.skybox.textures[5] = new QBrowsEdit(this, m_uinterface.skybox_bottom, m_uinterface.skybox_bottom_browse);

    envGui.fog.color = new QVectorBox(this, m_uinterface.fog_x, m_uinterface.fog_y, m_uinterface.fog_z);
    envGui.fog.start = m_uinterface.fog_start;
    envGui.fog.end = m_uinterface.fog_end;
    envGui.fog.enable = m_uinterface.fog_enable;

    connect(m_tbeWidget, SIGNAL(notifyInitFog(tbe::scene::Fog*)), this, SLOT(fogInit(tbe::scene::Fog*)));
    connect(m_tbeWidget, SIGNAL(notifyInitSkybox(tbe::scene::SkyBox*)), this, SLOT(skyboxInit(tbe::scene::SkyBox*)));
    connect(m_tbeWidget, SIGNAL(notifyInitAmbiant(const tbe::Vector3f&)), this, SLOT(ambiantScene(const tbe::Vector3f&)));

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGui()));
    m_timer->start(16);
}

MainWindow::~MainWindow()
{
}

void MainWindow::openSceneDialog()
{
    m_tbeWidget->pauseRendring();

    QString filename = QFileDialog::getOpenFileName(this);

    if(!filename.isNull())
        openScene(filename);

    m_tbeWidget->resumeRendring();
}

void MainWindow::openScene(const QString& filename)
{
    QDir::setCurrent(QFileInfo(filename).path());

    m_tbeWidget->loadScene(filename);

    m_filename = filename;
}

void MainWindow::saveSceneDialog()
{
    QString filename = QFileDialog::getSaveFileName(this);

    if(!filename.isNull())
        saveScene(filename);
}

void MainWindow::saveScene()
{
    saveScene(m_filename);
}

void MainWindow::saveScene(const QString& filename)
{
    m_tbeWidget->saveScene(filename);
}

void MainWindow::updateGui()
{
    m_tbeWidget->fillTextInfo(m_infoText);
}

void MainWindow::guiSelect(const QModelIndex& index)
{
    using namespace tbe::scene;

    QStandardItem* item = nodesGui.nodesListModel->itemFromIndex(index);

    // NOTE Conversion to NodeType fail (return 0)
    int type = item->data(ContentType).toInt();

    switch(type)
    {
        case IsLight:
        {
            Light* light = item->data().value<Light*>();
            lightSelect(light, false);
            nodesGui.attribTab->setCurrentIndex(0);
        }
            break;

        case IsMesh:
        {
            Mesh* mesh = item->data().value<Mesh*>();
            meshSelect(mesh, false);
            nodesGui.attribTab->setCurrentIndex(1);
        }
            break;

        case IsWater:
        {
            // Water* water = item->data().value<Water*>();
            nodesGui.attribTab->setCurrentIndex(2);
        }
            break;

        case IsParticles:
        {
            ParticlesEmiter* pemiter = item->data().value<ParticlesEmiter*>();
            particlesSelect(pemiter, false);
            nodesGui.attribTab->setCurrentIndex(3);
        }
            break;
    }
}

void MainWindow::guiMeshNew()
{
    m_tbeWidget->pauseRendring();

    QString filename = QFileDialog::getOpenFileName(qobject_cast<QWidget*>(parent()));

    if(!filename.isNull())
    {
        tbe::scene::Mesh* mesh = m_tbeWidget->meshNew(filename);

        meshAdd(mesh);
        meshSelect(mesh);
    }

    m_tbeWidget->resumeRendring();
}

void MainWindow::guiMeshClone()
{
    if(m_qnodebind->getCurmesh())
        m_tbeWidget->meshClone(m_qnodebind->getCurmesh());
}

void MainWindow::guiMeshDelete()
{
    if(tbe::scene::Mesh * mesh = m_qnodebind->getCurmesh())
    {
        QStandardItem* item = mesh->getUserData().getValue<QStandardItem*> ();
        QStandardItem* parent = item->parent();

        if(parent)
            parent->removeRow(item->row());
        else
            nodesGui.nodesListModel->removeRow(item->row());

        m_tbeWidget->meshDelete(mesh);
    }
}

void MainWindow::meshAdd(tbe::scene::Mesh* mesh)
{
    using namespace tbe::scene;

    QStandardItem* parent = NULL;

    int count = nodesGui.nodesListModel->rowCount();

    if(!mesh->getParent()->isRoot())
        for(int i = 0; i < count; i++)
        {
            QStandardItem* item = nodesGui.nodesListModel->item(i);

            if(item && mesh->getParent() == item->data().value<Mesh*>())
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

    QStandardItem* itname = new QStandardItem(mesh->getName().c_str());
    itname->setData(userdata);
    itname->setData(IsMesh, ContentType);

    QItemsList items;
    items << itid << itname;

    if(parent)
        parent->appendRow(items);
    else
        nodesGui.nodesListModel->appendRow(items);

    mesh->setUserData(itid);

    nodesGui.nodesListView->resizeColumnToContents(0);
    nodesGui.nodesListView->resizeColumnToContents(1);
}

void MainWindow::meshSelect(tbe::scene::Mesh* mesh, bool upList)
{
    m_qnodebind->setCurmesh(mesh);

    if(!mesh)
        return;

    nodesGui.name->setText(mesh->getName().c_str());
    nodesGui.pos->setValue(mesh->getPos());

    if(upList)
    {
        QStandardItem* item = mesh->getUserData().getValue<QStandardItem*> ();
        nodesGui.nodesListView->setCurrentIndex(nodesGui.nodesListModel->indexFromItem(item));
    }

    m_tbeWidget->meshSelect(mesh);

    nodesGui.attribTab->setCurrentIndex(1);
}

void MainWindow::guiLightNew()
{
    using namespace tbe::scene;

    Light* light = m_tbeWidget->lightNew();

    lightAdd(light);
    lightSelect(light);
}

void MainWindow::guiLightClone()
{
    if(m_qnodebind->getCurlight())
        m_tbeWidget->lightClone(m_qnodebind->getCurlight());
}

void MainWindow::guiLightDelete()
{
    if(tbe::scene::Light * light = m_qnodebind->getCurlight())
    {
        QStandardItem* item = light->getUserData().getValue<QStandardItem*> ();
        QStandardItem* parent = item->parent();

        if(parent)
            parent->removeRow(item->row());
        else
            nodesGui.nodesListModel->removeRow(item->row());

        m_tbeWidget->lightDelete(light);
    }
}

void MainWindow::guiParticlesNew()
{
    using namespace tbe::scene;

    ParticlesEmiter* particles = m_tbeWidget->particlesNew();

    particlesAdd(particles);
    particlesSelect(particles);
}

void MainWindow::guiParticlesClone()
{
    if(m_qnodebind->getCurparticles())
        m_tbeWidget->particlesClone(m_qnodebind->getCurparticles());
}

void MainWindow::guiParticlesDelete()
{
    if(tbe::scene::ParticlesEmiter * particles = m_qnodebind->getCurparticles())
    {
        QStandardItem* item = particles->getUserData().getValue<QStandardItem*> ();
        QStandardItem* parent = item->parent();

        if(parent)
            parent->removeRow(item->row());
        else
            nodesGui.nodesListModel->removeRow(item->row());

        m_tbeWidget->particlesDelete(particles);
    }
}

void MainWindow::lightAdd(tbe::scene::Light* light)
{
    using namespace tbe::scene;

    QVariant userData;
    userData.setValue(light);

    QStandardItem* itemType = new QStandardItem("Light");
    itemType->setData(userData);
    itemType->setData(IsLight, ContentType);

    QStandardItem* itemName = new QStandardItem(light->getName().c_str());
    itemName->setData(userData);
    itemName->setData(IsLight, ContentType);

    QItemsList items;
    items << itemType << itemName;

    nodesGui.nodesListModel->appendRow(items);

    light->setUserData(itemType);

    nodesGui.nodesListView->resizeColumnToContents(0);
    nodesGui.nodesListView->resizeColumnToContents(1);
}

void MainWindow::lightSelect(tbe::scene::Light* light, bool upList)
{
    m_qnodebind->setCurlight(light);

    if(!light)
        return;

    nodesGui.name->setText(light->getName().c_str());
    nodesGui.pos->setValue(light->getPos());

    nodesGui.lighTab.type->setCurrentIndex((int)light->getType());
    nodesGui.lighTab.ambiant->setValue(vec43(light->getAmbient()));
    nodesGui.lighTab.diffuse->setValue(vec43(light->getDiffuse()));
    nodesGui.lighTab.specular->setValue(vec43(light->getSpecular()));
    nodesGui.lighTab.radius->setValue(light->getRadius());

    if(upList)
    {
        QStandardItem* item = light->getUserData().getValue<QStandardItem*> ();
        nodesGui.nodesListView->setCurrentIndex(nodesGui.nodesListModel->indexFromItem(item));
    }

    m_tbeWidget->lightSelect(light);

    nodesGui.attribTab->setCurrentIndex(0);
}

void MainWindow::particlesAdd(tbe::scene::ParticlesEmiter* particles)
{
    using namespace tbe::scene;

    QVariant userData;
    userData.setValue(particles);

    QStandardItem* itemType = new QStandardItem("Particles");
    itemType->setData(userData);
    itemType->setData(IsParticles, ContentType);

    QStandardItem* itemName = new QStandardItem(particles->getName().c_str());
    itemName->setData(userData);
    itemName->setData(IsParticles, ContentType);

    QItemsList items;
    items << itemType << itemName;

    if(!particles->getParent()->getUserData().isNull())
        particles->getParent()->getUserData().getValue<QStandardItem*>()->appendRow(items);
    else
        nodesGui.nodesListModel->appendRow(items);

    particles->setUserData(itemType);

    nodesGui.nodesListView->resizeColumnToContents(0);
    nodesGui.nodesListView->resizeColumnToContents(1);
}

void MainWindow::particlesSelect(tbe::scene::ParticlesEmiter *particles, bool upList)
{
    m_qnodebind->setCurparticles(particles);

    if(!particles)
        return;

    nodesGui.name->setText(particles->getName().c_str());
    nodesGui.pos->setValue(particles->getPos());

    nodesGui.particlesTab.gravity->setValue(particles->getGravity());
    nodesGui.particlesTab.freemove->setValue(particles->getFreeMove());
    nodesGui.particlesTab.lifeinit->setValue(particles->getLifeInit());
    nodesGui.particlesTab.lifedown->setValue(particles->getLifeDown());
    nodesGui.particlesTab.number->setValue(particles->getNumber());
    nodesGui.particlesTab.texture->setOpenFileName(particles->getTexture().getFilename().c_str());
    nodesGui.particlesTab.continiousmode->setChecked(particles->isContinousMode());

    if(upList)
    {
        QStandardItem* item = particles->getUserData().getValue<QStandardItem*> ();
        nodesGui.nodesListView->setCurrentIndex(nodesGui.nodesListModel->indexFromItem(item));
    }

    m_tbeWidget->particlesSelect(particles);

    nodesGui.attribTab->setCurrentIndex(3);
}

void MainWindow::scopeNode(int move)
{
    using namespace tbe::scene;

    QStandardItem* item = nodesGui.nodesListModel->itemFromIndex(nodesGui.nodesListView->currentIndex());

    int currRow = item->row();

    QList<QStandardItem*> row = nodesGui.nodesListModel->takeRow(currRow);

    if(move == 1) // Up
    {
        currRow = std::max(currRow - 1, 0);
        nodesGui.nodesListModel->insertRow(currRow, row);

        nodesGui.nodesListView->setCurrentIndex(nodesGui.nodesListModel->index(currRow, 0));
    }
    else if(move == 2) // Down
    {
        int rowCount = nodesGui.nodesListModel->rowCount();
        currRow = std::min(currRow + 1, rowCount);
        nodesGui.nodesListModel->insertRow(currRow, row);

        nodesGui.nodesListView->setCurrentIndex(nodesGui.nodesListModel->index(currRow, 0));
    }
    else if(move == 3) // Left
    {
        Node* parent = item->parent()->data().value<Node*>();
        Node* curr = item->data().value<Node*>();
    }
    else if(move == 4) // Right
    {
        Node* prev = nodesGui.nodesListModel->item(currRow - 1)->data().value<Node*>();
        Node* curr = item->data().value<Node*>();
    }

}

void MainWindow::updateList()
{
    int count = nodesGui.nodesListModel->rowCount();

    for(int i = 0; i < count; i++)
    {
        using namespace tbe::scene;

        QStandardItem* item = nodesGui.nodesListModel->item(i);
        QStandardItem* itemName = nodesGui.nodesListModel->item(i, 1);

        Node* node = NULL;

        if(item->data().canConvert<Light*>())
            node = item->data().value<Light*>();

        else if(item->data().canConvert<Mesh*>())
            node = item->data().value<Mesh*>();

        else if(item->data().canConvert<ParticlesEmiter*>())
            node = item->data().value<ParticlesEmiter*>();

        else if(item->data().canConvert<Water*>())
            node = item->data().value<Water*>();

        itemName->setText(node->getName().c_str());
    }
}

void MainWindow::skyboxApply(bool enable)
{
    if(enable)
    {
        QStringList texs;

        for(unsigned i = 0; i < 6; i++)
            texs << envGui.skybox.textures[i]->getOpenFileName();


        m_tbeWidget->skyboxApply(texs);
    }

    else
    {
        m_tbeWidget->skyboxClear();
    }
}

void MainWindow::fogApply(bool enable)
{
    if(enable)
        m_tbeWidget->fogApply(vec34(envGui.fog.color->value()),
                              envGui.fog.start->value(),
                              envGui.fog.end->value());
    else
        m_tbeWidget->fogClear();
}

void MainWindow::ambiantScene(const tbe::Vector3f& value)
{
    envGui.sceneAmbiant->setValue(value);
}

void MainWindow::fogInit(tbe::scene::Fog* fog)
{
    envGui.fog.enable->setChecked(fog->isEnable());
    envGui.fog.color->setValue(vec43(fog->getColor()));
    envGui.fog.start->setValue(fog->getStart());
    envGui.fog.end->setValue(fog->getEnd());

    connect(envGui.fog.enable, SIGNAL(clicked(bool)), this, SLOT(fogApply(bool)));
    connect(envGui.fog.color, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(fogApply()));
    connect(envGui.fog.start, SIGNAL(valueChanged(double)), this, SLOT(fogApply()));
    connect(envGui.fog.end, SIGNAL(valueChanged(double)), this, SLOT(fogApply()));
}

void MainWindow::skyboxInit(tbe::scene::SkyBox* sky)
{
    tbe::Texture* texs = sky->getTextures();

    for(unsigned i = 0; i < 6; i++)
        this->envGui.skybox.textures[i]->setOpenFileName(texs[i].getFilename().c_str());

    envGui.skybox.enable->setChecked(sky->isEnable());

    connect(envGui.skybox.enable, SIGNAL(clicked(bool)), this, SLOT(skyboxApply(bool)));
    connect(envGui.skybox.apply, SIGNAL(clicked()), this, SLOT(skyboxApply()));
}
