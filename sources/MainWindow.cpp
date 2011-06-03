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
Q_DECLARE_METATYPE(tbe::scene::Material*)
Q_DECLARE_METATYPE(NodeType)

MainWindow::MainWindow()
{
    notifyChanges(false);
}

MainWindow::~MainWindow()
{
}

void MainWindow::buildFileHistory()
{
    QMenu* filehistory = m_uinterface.actionDernier_fichiers->menu();

    if(!filehistory)
    {
        filehistory = new QMenu(this);
        m_uinterface.actionDernier_fichiers->setMenu(filehistory);
    }

    filehistory->clear();

    QStringList history = m_config->value("history").toStringList();

    if(history.isEmpty())
    {
        filehistory->setEnabled(false);
    }
    else
    {

        foreach(QString filepath, history)
        {
            QAction* act = filehistory->addAction(filepath);

            connect(act, SIGNAL(triggered()), m_historyMapping, SLOT(map()));
            m_historyMapping->setMapping(act, filepath);
        }

        connect(m_historyMapping, SIGNAL(mapped(const QString&)), this, SLOT(openScene(const QString&)));
    }
}

void MainWindow::pushFileHistory(const QString& filepath)
{
    QStringList history = m_config->value("history").toStringList();

    if(history.count(filepath))
        history.removeAll(filepath);

    history << filepath;

    if(history.count() > 16)
        history.pop_front();

    m_config->setValue("history", history);

    buildFileHistory();
}

void MainWindow::initWidgets()
{
    m_uinterface.setupUi(this);

    m_tbeWidget = m_uinterface.glwidget;

    m_infoText = m_uinterface.infoText;

    nodesGui.attribTab = m_uinterface.attribTab;

    m_config = new QSettings(this);

    m_historyMapping = new QSignalMapper(this);

    buildFileHistory();

    // Générale ----------------------------------------------------------------

    genGui.title = m_uinterface.gen_title;
    genGui.author = m_uinterface.gen_author;

    genGui.additionalModel = new QStandardItemModel(this);
    genGui.additionalModel->setHorizontalHeaderLabels(QStringList() << "Clé" << "Valeur");

    genGui.additionalView = m_uinterface.gen_additional;
    genGui.additionalView->setModel(genGui.additionalModel);
    genGui.additionalView->setRootIsDecorated(false);
    genGui.additionalView->header()->setResizeMode(QHeaderView::Stretch);

    genGui.addField = m_uinterface.gen_addfield;
    genGui.delField = m_uinterface.gen_delfield;
    genGui.clearFields = m_uinterface.gen_clearfields;

    // Node --------------------------------------------------------------------

    nodesGui.additionalModel = new QStandardItemModel(this);
    nodesGui.additionalModel->setHorizontalHeaderLabels(QStringList() << "Clé" << "Valeur");

    nodesGui.additionalView = m_uinterface.node_additional;
    nodesGui.additionalView->setModel(nodesGui.additionalModel);
    nodesGui.additionalView->setRootIsDecorated(false);
    nodesGui.additionalView->header()->setResizeMode(QHeaderView::Stretch);

    nodesGui.addField = m_uinterface.node_addfield;
    nodesGui.delField = m_uinterface.node_delfield;
    nodesGui.clearFields = m_uinterface.node_clearfields;

    m_qnodebind = new QNodeBinders(this);

    nodesGui.name = m_uinterface.node_name;
    nodesGui.pos = new QVectorBox(this, m_uinterface.node_pos_x, m_uinterface.node_pos_y, m_uinterface.node_pos_z);
    nodesGui.rot = new QVectorBox(this, m_uinterface.node_rot_x, m_uinterface.node_rot_y, m_uinterface.node_rot_z);
    nodesGui.scl = new QVectorBox(this, m_uinterface.node_scl_x, m_uinterface.node_scl_y, m_uinterface.node_scl_z);

    nodesGui.nodeUp = m_uinterface.node_list_up;
    nodesGui.nodeDown = m_uinterface.node_list_down;
    nodesGui.nodeRight = m_uinterface.node_list_makechild;
    nodesGui.nodeLeft = m_uinterface.node_list_makeparent;

    // -------- Mesh

    nodesGui.meshTab.add = m_uinterface.node_mesh_add;
    nodesGui.meshTab.clone = m_uinterface.node_mesh_clone;
    nodesGui.meshTab.del = m_uinterface.node_mesh_del;

    nodesGui.meshTab.texture = new QBrowsEdit(this, m_uinterface.node_mesh_texture, m_uinterface.node_mesh_texture_browse);

    nodesGui.meshTab.opacity = m_uinterface.node_mesh_opacity;

    nodesGui.meshTab.textured = m_uinterface.node_mesh_textured;
    nodesGui.meshTab.lighted = m_uinterface.node_mesh_lighted;
    nodesGui.meshTab.alphablend = m_uinterface.node_mesh_alphablend;

    nodesGui.meshTab.materialsModel = new QStandardItemModel(this);
    nodesGui.meshTab.materialsModel->setHorizontalHeaderLabels(QStringList() << "Materiaux");

    nodesGui.meshTab.materialsView = m_uinterface.node_mesh_materials;
    nodesGui.meshTab.materialsView->setModel(nodesGui.meshTab.materialsModel);

    // -------- Water

    nodesGui.waterTab.deform = m_uinterface.node_water_deform;
    nodesGui.waterTab.size = new QVector2Box(this, m_uinterface.node_water_size_x, m_uinterface.node_water_size_y);
    nodesGui.waterTab.uvrepeat = new QVector2Box(this, m_uinterface.node_water_uvrepeat_x, m_uinterface.node_water_uvrepeat_y);
    nodesGui.waterTab.speed = m_uinterface.node_water_speed;
    nodesGui.waterTab.blend = m_uinterface.node_water_blend;
    nodesGui.waterTab.add = m_uinterface.node_water_add;
    nodesGui.waterTab.clone = m_uinterface.node_water_clone;
    nodesGui.waterTab.del = m_uinterface.node_water_del;

    // -------- Particles

    nodesGui.particlesTab.gravity = new QVectorBox(this, m_uinterface.node_particles_gravity_x, m_uinterface.node_particles_gravity_y, m_uinterface.node_particles_gravity_z);
    nodesGui.particlesTab.boxsize = new QVectorBox(this, m_uinterface.node_particles_boxsize_x, m_uinterface.node_particles_boxsize_y, m_uinterface.node_particles_boxsize_z);
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

    // -------- Lights

    nodesGui.lighTab.type = m_uinterface.node_light_type;

    nodesGui.lighTab.ambiant = new QVectorBox(this, m_uinterface.node_light_ambiant_x, m_uinterface.node_light_ambiant_y, m_uinterface.node_light_ambiant_z);
    nodesGui.lighTab.diffuse = new QVectorBox(this, m_uinterface.node_light_diffuse_x, m_uinterface.node_light_diffuse_y, m_uinterface.node_light_diffuse_z);
    nodesGui.lighTab.specular = new QVectorBox(this, m_uinterface.node_light_specular_x, m_uinterface.node_light_specular_y, m_uinterface.node_light_specular_z);

    nodesGui.lighTab.radius = m_uinterface.node_light_radius;

    nodesGui.lighTab.add = m_uinterface.node_light_add;
    nodesGui.lighTab.clone = m_uinterface.node_light_clone;
    nodesGui.lighTab.del = m_uinterface.node_light_del;

    // Nodes liste -------------------------------------------------------------

    QStringList headerLabels;
    headerLabels << "Type" << "Nom";

    nodesGui.nodesListModel = new QStandardItemModel(this);
    nodesGui.nodesListModel->setHorizontalHeaderLabels(headerLabels);

    nodesGui.nodesListView = m_uinterface.node_list;
    nodesGui.nodesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    nodesGui.nodesListView->header()->setResizeMode(QHeaderView::Stretch);
    nodesGui.nodesListView->setModel(nodesGui.nodesListModel);

    // Environment -------------------------------------------------------------

    envGui.sceneAmbiant = new QVectorBox(this, m_uinterface.env_ambient_x, m_uinterface.env_ambient_y, m_uinterface.env_ambient_z);

    envGui.skybox.apply = m_uinterface.skybox_apply;
    envGui.skybox.enable = m_uinterface.skybox_enable;

    envGui.skybox.textures[0] = new QBrowsEdit(this, m_uinterface.skybox_front, m_uinterface.skybox_front_browse);
    envGui.skybox.textures[1] = new QBrowsEdit(this, m_uinterface.skybox_back, m_uinterface.skybox_back_browse);
    envGui.skybox.textures[2] = new QBrowsEdit(this, m_uinterface.skybox_top, m_uinterface.skybox_top_browse);
    envGui.skybox.textures[3] = new QBrowsEdit(this, m_uinterface.skybox_bottom, m_uinterface.skybox_bottom_browse);
    envGui.skybox.textures[4] = new QBrowsEdit(this, m_uinterface.skybox_left, m_uinterface.skybox_left_browse);
    envGui.skybox.textures[5] = new QBrowsEdit(this, m_uinterface.skybox_right, m_uinterface.skybox_right_browse);

    envGui.fog.color = new QVectorBox(this, m_uinterface.fog_x, m_uinterface.fog_y, m_uinterface.fog_z);
    envGui.fog.start = m_uinterface.fog_start;
    envGui.fog.end = m_uinterface.fog_end;
    envGui.fog.enable = m_uinterface.fog_enable;
}

void MainWindow::initConnections()
{
    /*
     * Modification
     * ------------
     * Gui mod -> Node binder -> Engine
     * Engine mod -> Gui display
     *
     * Allocation
     * ----------
     * Engine new -> Gui register -> Engine register
     * Gui new -> Gui register -> Engine register
     *
     */

    connect(genGui.addField, SIGNAL(clicked()), this, SLOT(guiAddSceneField()));
    connect(genGui.delField, SIGNAL(clicked()), this, SLOT(guiDelSceneField()));
    connect(genGui.clearFields, SIGNAL(clicked()), this, SLOT(guiClearSceneField()));

    connect(m_uinterface.actionNouvelle_scene, SIGNAL(triggered()), this, SLOT(newScene()));
    connect(m_uinterface.actionOuvrire, SIGNAL(triggered()), this, SLOT(openSceneDialog()));
    connect(m_uinterface.actionEnregistrer, SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(m_uinterface.actionEnregistrer_sous, SIGNAL(triggered()), this, SLOT(saveSceneDialog()));
    connect(m_uinterface.actionA_Propos, SIGNAL(triggered()), this, SLOT(about()));
    connect(m_uinterface.actionRendue_plein_fenete, SIGNAL(triggered(bool)), this, SLOT(toggleFullWidget(bool)));
    connect(m_uinterface.actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(this, SIGNAL(pauseRendring()), m_tbeWidget, SLOT(pauseRendring()));
    connect(this, SIGNAL(resumeRendring()), m_tbeWidget, SLOT(resumeRendring()));

    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Mesh*)), this, SLOT(meshUpdate(tbe::scene::Mesh*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Light*)), this, SLOT(lightUpdate(tbe::scene::Light*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesUpdate(tbe::scene::ParticlesEmiter*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Node*)), this, SLOT(updateNodeInfo(tbe::scene::Node*)));
    connect(m_qnodebind, SIGNAL(notifyUpdate(tbe::scene::Node*)), m_tbeWidget, SLOT(placeSelection()));

    QSignalMapper* nodeMoveBind = new QSignalMapper(this);
    nodeMoveBind->setMapping(nodesGui.nodeUp, 1);
    nodeMoveBind->setMapping(nodesGui.nodeDown, 2);
    nodeMoveBind->setMapping(nodesGui.nodeLeft, 3);
    nodeMoveBind->setMapping(nodesGui.nodeRight, 4);

    connect(nodesGui.additionalModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(guiChangeNodeField(QStandardItem*)));

    connect(nodesGui.addField, SIGNAL(clicked()), this, SLOT(guiAddNodeField()));
    connect(nodesGui.delField, SIGNAL(clicked()), this, SLOT(guiDelNodeField()));
    connect(nodesGui.clearFields, SIGNAL(clicked()), this, SLOT(guiClearNodeField()));

    connect(nodesGui.nodeUp, SIGNAL(clicked()), nodeMoveBind, SLOT(map()));
    connect(nodesGui.nodeDown, SIGNAL(clicked()), nodeMoveBind, SLOT(map()));
    connect(nodesGui.nodeLeft, SIGNAL(clicked()), nodeMoveBind, SLOT(map()));
    connect(nodesGui.nodeRight, SIGNAL(clicked()), nodeMoveBind, SLOT(map()));

    connect(nodeMoveBind, SIGNAL(mapped(int)), this, SLOT(scopeNode(int)));

    connect(nodesGui.name, SIGNAL(textChanged(const QString&)), m_qnodebind, SLOT(nodeSetName(const QString&)));
    connect(nodesGui.pos, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(nodeSetPos(const tbe::Vector3f&)));
    connect(nodesGui.rot, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(nodeSetRotation(const tbe::Vector3f&)));
    connect(nodesGui.scl, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(nodeSetScale(const tbe::Vector3f&)));

    connect(nodesGui.meshTab.add, SIGNAL(clicked()), this, SLOT(guiMeshNew()));
    connect(nodesGui.meshTab.clone, SIGNAL(clicked()), this, SLOT(guiMeshClone()));
    connect(nodesGui.meshTab.del, SIGNAL(clicked()), this, SLOT(guiMeshDelete()));

    connect(nodesGui.meshTab.textured, SIGNAL(clicked(bool)), this, SLOT(guiMeshSetTextured(bool)));
    connect(nodesGui.meshTab.lighted, SIGNAL(clicked(bool)), this, SLOT(guiMeshSetLighted(bool)));
    connect(nodesGui.meshTab.alphablend, SIGNAL(clicked(bool)), this, SLOT(guiMeshSetAlphaBlend(bool)));

    connect(nodesGui.meshTab.opacity, SIGNAL(valueChanged(double)), this, SLOT(guiMeshSetOpacity(double)));

    connect(nodesGui.meshTab.texture, SIGNAL(textChanged(const QString&)), this, SLOT(guiMeshSetTexture(const QString&)));

    connect(nodesGui.meshTab.materialsView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(guiMeshMaterialSelected(const QModelIndex &)));

    connect(nodesGui.waterTab.deform, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(waterSetDeform(double)));
    connect(nodesGui.waterTab.size, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qnodebind, SLOT(waterSetSize(const tbe::Vector2f&)));
    connect(nodesGui.waterTab.uvrepeat, SIGNAL(valueChanged(const tbe::Vector2f&)), m_qnodebind, SLOT(waterSetUvRepeat(const tbe::Vector2f&)));
    connect(nodesGui.waterTab.speed, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(waterSetSpeed(double)));
    connect(nodesGui.waterTab.blend, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(waterSetBlend(double)));

    connect(nodesGui.particlesTab.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(particleSetGravity(const tbe::Vector3f&)));
    connect(nodesGui.particlesTab.boxsize, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(particleSetBoxsize(const tbe::Vector3f&)));
    connect(nodesGui.particlesTab.freemove, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(particleSetFreemove(double)));
    connect(nodesGui.particlesTab.lifeinit, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(particleSetLifeinit(double)));
    connect(nodesGui.particlesTab.lifedown, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(particleSetLifedown(double)));
    connect(nodesGui.particlesTab.number, SIGNAL(valueChanged(int)), m_qnodebind, SLOT(particleSetNumber(int)));
    connect(nodesGui.particlesTab.texture, SIGNAL(textChanged(const QString&)), m_qnodebind, SLOT(particleSetTexture(const QString&)));
    connect(nodesGui.particlesTab.continiousmode, SIGNAL(stateChanged(int)), m_qnodebind, SLOT(particleSetContinousMode(int)));
    connect(nodesGui.particlesTab.build, SIGNAL(clicked()), m_qnodebind, SLOT(particleBuild()));

    connect(nodesGui.particlesTab.add, SIGNAL(clicked()), this, SLOT(guiParticlesNew()));
    connect(nodesGui.particlesTab.clone, SIGNAL(clicked()), this, SLOT(guiParticlesClone()));
    connect(nodesGui.particlesTab.del, SIGNAL(clicked()), this, SLOT(guiParticlesDelete()));

    connect(nodesGui.lighTab.type, SIGNAL(activated(int)), m_qnodebind, SLOT(lightSetType(int)));
    connect(nodesGui.lighTab.ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(lightSetAmbiant(const tbe::Vector3f&)));
    connect(nodesGui.lighTab.diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(lightSetDiffuse(const tbe::Vector3f&)));
    connect(nodesGui.lighTab.specular, SIGNAL(valueChanged(const tbe::Vector3f&)), m_qnodebind, SLOT(lightSetSpecular(const tbe::Vector3f&)));
    connect(nodesGui.lighTab.radius, SIGNAL(valueChanged(double)), m_qnodebind, SLOT(lightSetRadius(double)));

    connect(nodesGui.lighTab.add, SIGNAL(clicked()), this, SLOT(guiLightNew()));
    connect(nodesGui.lighTab.clone, SIGNAL(clicked()), this, SLOT(guiLightClone()));
    connect(nodesGui.lighTab.del, SIGNAL(clicked()), this, SLOT(guiLightDelete()));

    connect(nodesGui.nodesListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(guiSelect(const QModelIndex&)));

    connect(m_tbeWidget, SIGNAL(notifyMeshAdd(tbe::scene::Mesh*)), this, SLOT(meshRegister(tbe::scene::Mesh*)));
    connect(m_tbeWidget, SIGNAL(notifyMeshSelect(tbe::scene::Mesh*)), this, SLOT(meshSelect(tbe::scene::Mesh*)));
    connect(m_tbeWidget, SIGNAL(notifyMeshUpdate(tbe::scene::Mesh*)), this, SLOT(meshUpdate(tbe::scene::Mesh*)));

    connect(m_tbeWidget, SIGNAL(notifyLightAdd(tbe::scene::Light*)), this, SLOT(lightRegister(tbe::scene::Light*)));
    connect(m_tbeWidget, SIGNAL(notifyLightSelect(tbe::scene::Light*)), this, SLOT(lightSelect(tbe::scene::Light*)));
    connect(m_tbeWidget, SIGNAL(notifyLightUpdate(tbe::scene::Light*)), this, SLOT(lightUpdate(tbe::scene::Light*)));

    connect(m_tbeWidget, SIGNAL(notifyParticlesAdd(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesRegister(tbe::scene::ParticlesEmiter*)));
    connect(m_tbeWidget, SIGNAL(notifyParticlesSelect(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesSelect(tbe::scene::ParticlesEmiter*)));
    connect(m_tbeWidget, SIGNAL(notifyParticlesUpdate(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesUpdate(tbe::scene::ParticlesEmiter*)));

    connect(envGui.sceneAmbiant, SIGNAL(valueChanged(const tbe::Vector3f&)), m_tbeWidget, SLOT(setSceneAmbiant(const tbe::Vector3f&)));

    connect(m_tbeWidget, SIGNAL(notifyInitFog(tbe::scene::Fog*)), this, SLOT(fogRegister(tbe::scene::Fog*)));
    connect(m_tbeWidget, SIGNAL(notifyInitSkybox(tbe::scene::SkyBox*)), this, SLOT(skyboxRegister(tbe::scene::SkyBox*)));
    connect(m_tbeWidget, SIGNAL(notifyInitAmbiant(const tbe::Vector3f&)), this, SLOT(sceneAmbiantUpdate(const tbe::Vector3f&)));

    connect(m_tbeWidget, SIGNAL(notifyListRebuild()), this, SLOT(clearNodeList()));

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateInfoBox()));
    m_timer->start(16);

    // Change notify

    connect(m_tbeWidget, SIGNAL(notifyMeshUpdate(tbe::scene::Mesh*)), this, SLOT(notifyChanges()));
    connect(m_tbeWidget, SIGNAL(notifyLightUpdate(tbe::scene::Light*)), this, SLOT(notifyChanges()));
    connect(m_tbeWidget, SIGNAL(notifyParticlesUpdate(tbe::scene::ParticlesEmiter*)), this, SLOT(notifyChanges()));
}

bool MainWindow::leaveSafely()
{
    if(m_somethingChange)
    {
        QMessageBox::StandardButton answer =
                QMessageBox::warning(this, "Enregistrer ?",
                                     "La scene a été modifier\nVous-les vous enregistrer avant de quitter ?",
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if(answer == QMessageBox::Yes)
        {
            saveScene();
            return true;
        }

        else if(answer == QMessageBox::No)
            return true;

        else if(answer == QMessageBox::Cancel)
            return false;

        else
            return false;
    }
    else
        return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(leaveSafely())
        event->accept();

    else
        event->ignore();
}

void MainWindow::newScene()
{
    if(!leaveSafely())
        return;

    m_tbeWidget->clearScene();

    m_filename.clear();

    notifyChanges(false);

    nodesGui.nodesListModel->removeRows(0, nodesGui.nodesListModel->rowCount());
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
    using namespace tbe::scene;

    if(!leaveSafely())
        return;

    try
    {
        m_tbeWidget->clearScene();

        m_filename.clear();

        nodesGui.nodesListModel->removeRows(0, nodesGui.nodesListModel->rowCount());

        notifyChanges(false);

        m_tbeWidget->loadScene(filename);

        QDir::setCurrent(QFileInfo(filename).path());

        pushFileHistory(filename);

        SceneParser* sceneParser = m_tbeWidget->getSceneParser();

        genGui.title->setText(sceneParser->getSceneName().c_str());
        genGui.author->setText(sceneParser->getAuthorName().c_str());

        genGui.additionalModel->removeRows(0, genGui.additionalModel->rowCount());

        const SceneParser::AttribMap addfields = sceneParser->additionalFields();

        for(SceneParser::AttribMap::const_iterator it = addfields.begin(); it != addfields.end(); it++)
        {
            QStandardItem* key = new QStandardItem;
            key->setText(it->first.c_str());

            QStandardItem* value = new QStandardItem;
            value->setText(it->second.c_str());

            genGui.additionalModel->appendRow(QList<QStandardItem*> () << key << value);
        }

        QVariant rootUserData;
        rootUserData.setValue<tbe::scene::Node*>(m_tbeWidget->rootNode());

        nodesGui.nodesListModel->invisibleRootItem()->setData(rootUserData);
        nodesGui.nodesListModel->invisibleRootItem()->setData(IsUnknown, ContentType);

        m_filename = filename;

        notifyChanges(false);
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(this, "Erreur de chargement", e.what());
    }
}

void MainWindow::saveSceneDialog()
{
    QString filename = QFileDialog::getSaveFileName(this);

    if(!filename.isNull())
    {
        saveScene(filename);
        openScene(filename);
    }
}

void MainWindow::saveScene()
{
    if(m_filename.isEmpty())
        saveSceneDialog();
    else
        saveScene(m_filename);
}

void MainWindow::saveScene(const QString& filename)
{
    tbe::scene::SceneParser* sceneParser = m_tbeWidget->getSceneParser();

    sceneParser->setSceneName(genGui.title->text().toStdString());
    sceneParser->setAuthorName(genGui.author->text().toStdString());

    sceneParser->clearAdditional();

    int count = genGui.additionalModel->rowCount();
    for(int i = 0; i < count; i++)
    {
        std::string key = genGui.additionalModel->item(i, 0)->text().toStdString();
        std::string value = genGui.additionalModel->item(i, 1)->text().toStdString();

        sceneParser->setAdditional(key, value);
    }

    m_tbeWidget->saveScene(filename);

    notifyChanges(false);

    statusBar()->showMessage("Scene enregistrer...", 2000);
}

void MainWindow::notifyChanges(bool stat)
{
    m_somethingChange = stat;

    if(!m_filename.isEmpty())
    {
        if(m_somethingChange)
            setWindowTitle(QString("%1 - * [%2]").arg(QApplication::applicationName()).arg(m_filename));
        else
            setWindowTitle(QString("%1 - [%2]").arg(QApplication::applicationName()).arg(m_filename));
    }
    else
    {
        if(m_somethingChange)
            setWindowTitle(QString("%1 - * [Nouvelle scene]").arg(QApplication::applicationName()));
        else
            setWindowTitle(QString("%1 - [Nouvelle scene]").arg(QApplication::applicationName()));
    }
}

void MainWindow::about()
{
    QDialog* aboutdlg = new QDialog(this);

    Ui::About ctor;
    ctor.setupUi(aboutdlg);

    aboutdlg->open();
    aboutdlg->raise();
}

void MainWindow::updateInfoBox()
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

    QString filename = QFileDialog::getOpenFileName(parentWidget());

    if(!filename.isNull())
    {
        try
        {
            tbe::scene::Mesh* mesh = m_tbeWidget->meshNew(filename);

            meshRegister(mesh);
            meshSelect(mesh);
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(parentWidget(), "Errur d'ouverture", e.what());
        }
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
        QStandardItem* item = nodesGui.nodeItemBinder[mesh];
        QStandardItem* parent = item->parent();

        if(parent)
            parent->removeRow(item->row());
        else
            nodesGui.nodesListModel->removeRow(item->row());

        m_tbeWidget->meshDelete(mesh);

        notifyChanges(true);
    }
}

void MainWindow::guiLightNew()
{
    using namespace tbe::scene;

    Light* light = m_tbeWidget->lightNew();

    lightRegister(light);
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
        QStandardItem* item = nodesGui.nodeItemBinder[light];
        QStandardItem* parent = item->parent();

        if(parent)
            parent->removeRow(item->row());
        else
            nodesGui.nodesListModel->removeRow(item->row());

        m_tbeWidget->lightDelete(light);

        notifyChanges(true);
    }
}

void MainWindow::guiParticlesNew()
{
    using namespace tbe::scene;

    try
    {
        ParticlesEmiter* particles = m_tbeWidget->particlesNew();

        particlesRegister(particles);
        particlesSelect(particles);
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(parentWidget(), "Errur de création", e.what());
    }
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
        QStandardItem* item = nodesGui.nodeItemBinder[particles];
        QStandardItem* parent = item->parent();

        if(parent)
            parent->removeRow(item->row());
        else
            nodesGui.nodesListModel->removeRow(item->row());

        m_tbeWidget->particlesDelete(particles);

        notifyChanges(true);
    }
}

void MainWindow::nodeUpdate(tbe::scene::Node* node)
{
    if(!node)
        return;

    nodesGui.name->blockSignals(true);
    nodesGui.pos->blockSignals(true);
    nodesGui.scl->blockSignals(true);
    nodesGui.rot->blockSignals(true);

    nodesGui.name->setText(node->getName().c_str());
    nodesGui.pos->setValue(node->getPos());
    nodesGui.scl->setValue(node->getMatrix().getScale());
    nodesGui.rot->setValue(node->getMatrix().getRotate().getEuler());

    nodesGui.name->blockSignals(false);
    nodesGui.pos->blockSignals(false);
    nodesGui.scl->blockSignals(false);
    nodesGui.rot->blockSignals(false);

    using namespace tbe;

    const Any::Map userData = node->getUserDatas();

    nodesGui.additionalModel->removeRows(0, nodesGui.additionalModel->rowCount());

    for(Any::Map::const_iterator it = userData.begin(); it != userData.end(); it++)
    {
        QList<QStandardItem*> newfield;

        newfield
                << new QStandardItem(it->first.c_str())
                << new QStandardItem(it->second.getValue<std::string > ().c_str());

        nodesGui.additionalModel->appendRow(newfield);
    }
}

void MainWindow::meshRegister(tbe::scene::Mesh* mesh)
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
    itid->setIcon(QIcon(":/Icons/icons/mesh.png"));
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

    nodesGui.nodeItemBinder[mesh] = itid;

    m_tbeWidget->meshRegister(mesh);

    nodesGui.nodesListView->resizeColumnToContents(0);
    nodesGui.nodesListView->resizeColumnToContents(1);

    notifyChanges(true);
}

void MainWindow::meshUpdate(tbe::scene::Mesh* mesh)
{
    nodeUpdate(mesh);

    using namespace tbe::scene;

    nodesGui.meshTab.materialsModel->
            removeRows(0, nodesGui.meshTab.materialsModel->rowCount());

    Material::Array matarr = mesh->getAllMaterial();

    foreach(Material* mat, matarr)
    {
        QVariant data;
        data.setValue<Material*>(mat);

        QStandardItem* item = new QStandardItem(mat->getName().c_str());
        item->setData(data);

        nodesGui.meshTab.materialsModel->appendRow(item);
    }

    nodesGui.meshTab.opacity->setValue(mesh->getOpacity());

    QModelIndex index = nodesGui.meshTab.materialsModel->index(0, 0);

    nodesGui.meshTab.materialsView->setCurrentIndex(index);
    guiMeshMaterialSelected(index);
}

void MainWindow::meshSelect(tbe::scene::Mesh* mesh, bool upList)
{
    m_qnodebind->setCurmesh(mesh);

    if(!mesh)
        return;

    if(upList)
    {
        QStandardItem* item = nodesGui.nodeItemBinder[mesh];
        nodesGui.nodesListView->
                setCurrentIndex(nodesGui.nodesListModel->indexFromItem(item));
    }

    m_tbeWidget->meshSelect(mesh);

    nodesGui.attribTab->setCurrentIndex(1);

    meshUpdate(mesh);
}

void MainWindow::lightRegister(tbe::scene::Light* light)
{
    using namespace tbe::scene;

    QVariant userData;
    userData.setValue(light);

    QStandardItem* itemType = new QStandardItem("Light");
    itemType->setData(userData);
    itemType->setData(IsLight, ContentType);
    itemType->setIcon(QIcon(":/Icons/icons/light.png"));

    QStandardItem* itemName = new QStandardItem(light->getName().c_str());
    itemName->setData(userData);
    itemName->setData(IsLight, ContentType);

    QItemsList items;
    items << itemType << itemName;

    nodesGui.nodesListModel->appendRow(items);

    nodesGui.nodeItemBinder[light] = itemType;

    m_tbeWidget->lightRegister(light);

    nodesGui.nodesListView->resizeColumnToContents(0);
    nodesGui.nodesListView->resizeColumnToContents(1);

    notifyChanges(true);
}

void MainWindow::lightUpdate(tbe::scene::Light* light)
{
    nodeUpdate(light);

    nodesGui.lighTab.type->setCurrentIndex((int)light->getType());
    nodesGui.lighTab.ambiant->setValue(vec43(light->getAmbient()));
    nodesGui.lighTab.diffuse->setValue(vec43(light->getDiffuse()));
    nodesGui.lighTab.specular->setValue(vec43(light->getSpecular()));
    nodesGui.lighTab.radius->setValue(light->getRadius());
}

void MainWindow::lightSelect(tbe::scene::Light* light, bool upList)
{
    m_qnodebind->setCurlight(light);

    if(!light)
        return;

    if(upList)
    {
        QStandardItem* item = nodesGui.nodeItemBinder[light];
        nodesGui.nodesListView->setCurrentIndex(nodesGui.nodesListModel->indexFromItem(item));
    }

    m_tbeWidget->lightSelect(light);

    nodesGui.attribTab->setCurrentIndex(0);

    lightUpdate(light);
}

void MainWindow::particlesRegister(tbe::scene::ParticlesEmiter* particles)
{
    using namespace tbe::scene;

    QVariant userData;
    userData.setValue(particles);

    QStandardItem* itemType = new QStandardItem("Particles");
    itemType->setData(userData);
    itemType->setData(IsParticles, ContentType);
    itemType->setIcon(QIcon(":/Icons/icons/particles.png"));

    QStandardItem* itemName = new QStandardItem(particles->getName().c_str());
    itemName->setData(userData);
    itemName->setData(IsParticles, ContentType);

    QItemsList items;
    items << itemType << itemName;

    if(nodesGui.nodeItemBinder.count(particles->getParent()))
        nodesGui.nodeItemBinder[particles->getParent()]->appendRow(items);
    else
        nodesGui.nodesListModel->appendRow(items);

    nodesGui.nodeItemBinder[particles] = itemType;

    m_tbeWidget->particlesRegister(particles);

    nodesGui.nodesListView->resizeColumnToContents(0);
    nodesGui.nodesListView->resizeColumnToContents(1);

    notifyChanges(true);
}

void MainWindow::particlesUpdate(tbe::scene::ParticlesEmiter* particles)
{
    nodeUpdate(particles);

    nodesGui.particlesTab.gravity->setValue(particles->getGravity());
    nodesGui.particlesTab.boxsize->setValue(particles->getBoxSize());
    nodesGui.particlesTab.freemove->setValue(particles->getFreeMove());
    nodesGui.particlesTab.lifeinit->setValue(particles->getLifeInit());
    nodesGui.particlesTab.lifedown->setValue(particles->getLifeDown());
    nodesGui.particlesTab.number->setValue(particles->getNumber());
    nodesGui.particlesTab.texture->setOpenFileName(particles->getTexture().getFilename().c_str());
    nodesGui.particlesTab.continiousmode->setChecked(particles->isContinousMode());
}

void MainWindow::particlesSelect(tbe::scene::ParticlesEmiter *particles, bool upList)
{
    m_qnodebind->setCurparticles(particles);

    if(!particles)
        return;

    if(upList)
    {
        QStandardItem* item = nodesGui.nodeItemBinder[particles];
        nodesGui.nodesListView->setCurrentIndex(nodesGui.nodesListModel->indexFromItem(item));
    }

    m_tbeWidget->particlesSelect(particles);

    nodesGui.attribTab->setCurrentIndex(3);

    particlesUpdate(particles);
}

void MainWindow::scopeNode(int move)
{
    QStandardItem* item = nodesGui.nodesListModel->itemFromIndex(nodesGui.nodesListView->currentIndex());
    QStandardItem* parent = item->parent() ? item->parent() : nodesGui.nodesListModel->invisibleRootItem();

    int currRow = item->row();

    QList<QStandardItem*> row = parent->takeRow(currRow);

    if(move == 1) // Up
    {
        currRow = std::max(currRow - 1, 0);
        parent->insertRow(currRow, row);

        nodesGui.nodesListView->setCurrentIndex(item->index());
    }
    else if(move == 2) // Down
    {
        currRow = std::min(currRow + 1, parent->rowCount());
        parent->insertRow(currRow, row);

        nodesGui.nodesListView->setCurrentIndex(item->index());
    }
    else if(move == 3) // Left
    {
        if(parent != nodesGui.nodesListModel->invisibleRootItem())
        {
            QStandardItem* host = parent->parent()
                    ? parent->parent()
                    : nodesGui.nodesListModel->invisibleRootItem();

            host->insertRow(parent->row() + 1, row);

            tbe::scene::Node* parentNode = itemNode(host);
            tbe::scene::Node* currNode = itemNode(item);

            currNode->setParent(parentNode);
        }
        else
            parent->insertRow(currRow, row);

        nodesGui.nodesListView->setCurrentIndex(item->index());
    }
    else if(move == 4) // Right
    {
        if(currRow > 0)
        {
            QStandardItem* host = parent->child(currRow - 1);

            host->appendRow(row);

            tbe::scene::Node* parentNode = itemNode(host);
            tbe::scene::Node* currNode = itemNode(item);

            currNode->setParent(parentNode);
        }
        else
            parent->insertRow(currRow, row);

        nodesGui.nodesListView->setCurrentIndex(item->index());
    }

    notifyChanges(true);
}

void MainWindow::updateNodeInfo(tbe::scene::Node* node)
{
    QStandardItem* itemType = nodesGui.nodeItemBinder[node];
    QStandardItem* itemName = itemType->parent()
            ? itemType->parent()->child(itemType->row(), 1)
            : nodesGui.nodesListModel->item(itemType->row(), 1);

    itemName->setText(node->getName().c_str());
}

void MainWindow::guiSkyboxApply(bool enable)
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

void MainWindow::guiFogApply(bool enable)
{
    if(enable)
        m_tbeWidget->fogApply(vec34(envGui.fog.color->value()),
                              envGui.fog.start->value(),
                              envGui.fog.end->value());
    else
        m_tbeWidget->fogClear();
}

void MainWindow::sceneAmbiantUpdate(const tbe::Vector3f& value)
{
    envGui.sceneAmbiant->setValue(value);
}

void MainWindow::fogRegister(tbe::scene::Fog* fog)
{
    envGui.fog.enable->setChecked(fog->isEnable());
    envGui.fog.color->setValue(vec43(fog->getColor()));
    envGui.fog.start->setValue(fog->getStart());
    envGui.fog.end->setValue(fog->getEnd());

    connect(envGui.fog.enable, SIGNAL(clicked(bool)), this, SLOT(guiFogApply(bool)));
    connect(envGui.fog.color, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(guiFogApply()));
    connect(envGui.fog.start, SIGNAL(valueChanged(double)), this, SLOT(guiFogApply()));
    connect(envGui.fog.end, SIGNAL(valueChanged(double)), this, SLOT(guiFogApply()));
}

void MainWindow::skyboxRegister(tbe::scene::SkyBox* sky)
{
    tbe::Texture* texs = sky->getTextures();

    for(unsigned i = 0; i < 6; i++)
        this->envGui.skybox.textures[i]->setOpenFileName(texs[i].getFilename().c_str());

    envGui.skybox.enable->setChecked(sky->isEnable());

    connect(envGui.skybox.enable, SIGNAL(clicked(bool)), this, SLOT(guiSkyboxApply(bool)));
    connect(envGui.skybox.apply, SIGNAL(clicked()), this, SLOT(guiSkyboxApply()));
}

void MainWindow::toggleFullWidget(bool full)
{
    m_uinterface.propertyTab->setVisible(full);
    m_uinterface.infoBox->setVisible(full);
}

tbe::scene::Node* MainWindow::itemNode(QStandardItem* item)
{
    int type = item->data(ContentType).toInt();

    switch(type)
    {
        case IsLight:
            return item->data().value<tbe::scene::Light*>();

        case IsMesh:
            return item->data().value<tbe::scene::Mesh*>();

        case IsWater:
            return item->data().value<tbe::scene::Water*>();

        case IsParticles:
            return item->data().value<tbe::scene::ParticlesEmiter*>();

        case IsUnknown:
            return item->data().value<tbe::scene::Node*>();

        default: return NULL;
    }
}

void MainWindow::guiAddSceneField()
{
    QList<QStandardItem*> newfield;

    newfield
            << new QStandardItem("[nouvelle clé]")
            << new QStandardItem("[nouvelle valeur]");

    genGui.additionalModel->appendRow(newfield);

    notifyChanges(true);
}

void MainWindow::guiDelSceneField()
{
    QModelIndex i = genGui.additionalView->currentIndex();

    if(i.isValid())
        genGui.additionalModel->removeRow(i.row());

    notifyChanges(true);
}

void MainWindow::guiClearSceneField()
{
    if(genGui.additionalModel->rowCount() <= 0)
        return;


    int re = QMessageBox::warning(this, "Question ?", "Effacer tout les champs ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if(re == QMessageBox::Yes)
    {
        genGui.additionalModel->removeRows(0, genGui.additionalModel->rowCount());
        notifyChanges(true);
    }
}

void MainWindow::guiAddNodeField()
{
    QList<QStandardItem*> newfield;

    newfield
            << new QStandardItem("[nouvelle clé]")
            << new QStandardItem("[nouvelle valeur]");

    nodesGui.additionalModel->appendRow(newfield);

    notifyChanges(true);
}

void MainWindow::guiDelNodeField()
{
    QModelIndex i = nodesGui.additionalView->currentIndex();

    if(i.isValid())
    {
        QString key = nodesGui.additionalModel->item(i.row(), 0)->text();

        m_qnodebind->getCurNode()->delUserData(key.toStdString());

        nodesGui.additionalModel->removeRow(i.row());

        notifyChanges(true);
    }
}

void MainWindow::guiClearNodeField()
{
    if(nodesGui.additionalModel->rowCount() <= 0)
        return;

    int re = QMessageBox::warning(this, "Question ?", "Effacer tout les champs ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if(re == QMessageBox::Yes)
    {
        m_qnodebind->getCurNode()->clearUserData();
        nodesGui.additionalModel->removeRows(0, nodesGui.additionalModel->rowCount());
        notifyChanges(true);
    }
}

void MainWindow::guiChangeNodeField(QStandardItem* item)
{
    if(item->column() == 0)
    {
        QStandardItem* value = nodesGui.additionalModel->item(item->row(), 1);

        m_qnodebind->getCurNode()->delUserData(item->text().toStdString());
        m_qnodebind->getCurNode()->setUserData(item->text().toStdString(), value->text().toStdString());
    }
    else if(item->column() == 1)
    {
        QStandardItem* key = nodesGui.additionalModel->item(item->row(), 0);
        m_qnodebind->getCurNode()->setUserData(key->text().toStdString(), item->text().toStdString());

    }
}

void MainWindow::clearNodeList()
{
    nodesGui.nodesListModel->removeRows(0, nodesGui.nodesListModel->rowCount());
}

void MainWindow::guiMeshSetTexture(const QString& path)
{
    using namespace tbe::scene;

    QModelIndex index = nodesGui.meshTab.materialsView->currentIndex();
    QStandardItem* item = nodesGui.meshTab.materialsModel->itemFromIndex(index);
    Material* mat = item->data().value<Material*>();

    mat->setTexture(path.toStdString());
}

void MainWindow::guiMeshSetOpacity(double value)
{
    m_qnodebind->getCurmesh()->setOpacity((float)value);
}

void MainWindow::guiMeshMaterialSelected(const QModelIndex& index)
{
    using namespace tbe::scene;

    QStandardItem* item = nodesGui.meshTab.materialsModel->itemFromIndex(index);

    Material* mat = item->data().value<Material*>();

    nodesGui.meshTab.texture->setOpenFileName(mat->getTexture().getFilename().c_str());

    nodesGui.meshTab.textured->setChecked(mat->isEnable(Material::TEXTURE));
    nodesGui.meshTab.lighted->setChecked(mat->isEnable(Material::LIGHT));
    nodesGui.meshTab.alphablend->setChecked(mat->isEnable(Material::BLEND_MOD));
}

void MainWindow::guiMeshSetTextured(bool stat)
{
    using namespace tbe::scene;

    QModelIndex index = nodesGui.meshTab.materialsView->currentIndex();
    QStandardItem* item = nodesGui.meshTab.materialsModel->itemFromIndex(index);
    Material* mat = item->data().value<Material*>();

    if(stat)
        mat->enable(Material::TEXTURE);
    else
        mat->disable(Material::TEXTURE);
}

void MainWindow::guiMeshSetLighted(bool stat)
{
    using namespace tbe::scene;

    QModelIndex index = nodesGui.meshTab.materialsView->currentIndex();
    QStandardItem* item = nodesGui.meshTab.materialsModel->itemFromIndex(index);
    Material* mat = item->data().value<Material*>();

    if(stat)
        mat->enable(Material::LIGHT);
    else
        mat->disable(Material::LIGHT);
}

void MainWindow::guiMeshSetAlphaBlend(bool stat)
{
    using namespace tbe::scene;

    QModelIndex index = nodesGui.meshTab.materialsView->currentIndex();
    QStandardItem* item = nodesGui.meshTab.materialsModel->itemFromIndex(index);
    Material* mat = item->data().value<Material*>();

    if(stat)
        mat->enable(Material::BLEND_MOD | Material::VERTEX_SORT_CULL_TRICK);
    else
        mat->disable(Material::BLEND_MOD | Material::VERTEX_SORT_CULL_TRICK);
}
