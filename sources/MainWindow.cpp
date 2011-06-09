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
Q_DECLARE_METATYPE(tbe::Texture)
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
        history.sort();

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

    history.push_front(filepath);

    if(history.count() > 16)
        history.pop_back();

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

    m_selectedNode = new QNodeBinders(this);

    nodesGui.name = m_uinterface.node_name;
    nodesGui.position = new QVectorBox(this, m_uinterface.node_pos_x, m_uinterface.node_pos_y, m_uinterface.node_pos_z);
    nodesGui.rotation = new QVectorBox(this, m_uinterface.node_rot_x, m_uinterface.node_rot_y, m_uinterface.node_rot_z);
    nodesGui.scale = new QVectorBox(this, m_uinterface.node_scl_x, m_uinterface.node_scl_y, m_uinterface.node_scl_z);

    nodesGui.enable = m_uinterface.node_enable;

    nodesGui.nodeUp = m_uinterface.node_list_up;
    nodesGui.nodeDown = m_uinterface.node_list_down;
    nodesGui.nodeRight = m_uinterface.node_list_makechild;
    nodesGui.nodeLeft = m_uinterface.node_list_makeparent;

    // -------- Mark

    nodesGui.markTab.type = m_uinterface.node_mark_type;
    nodesGui.markTab.color = m_uinterface.node_mark_color;
    nodesGui.markTab.size = m_uinterface.node_mark_size;

    nodesGui.markTab.add = m_uinterface.node_mark_add;
    nodesGui.markTab.del = m_uinterface.node_mark_del;
    nodesGui.markTab.clone = m_uinterface.node_mark_clone;

    // -------- Mesh

    nodesGui.meshTab.add = m_uinterface.node_mesh_add;
    nodesGui.meshTab.clone = m_uinterface.node_mesh_clone;
    nodesGui.meshTab.del = m_uinterface.node_mesh_del;

    nodesGui.meshTab.openmatedit = m_uinterface.node_mesh_editmat;

    nodesGui.meshTab.textureModel = new QStandardItemModel(this);

    nodesGui.meshTab.opacity = m_uinterface.node_mesh_opacity;

    nodesGui.meshTab.matedit = new MaterialEditDialog(this);

    nodesGui.meshTab.matedit->textureView->setModel(nodesGui.meshTab.textureModel);

    nodesGui.meshTab.materialsModel = new QStandardItemModel(this);

    nodesGui.meshTab.materialsView = m_uinterface.node_mesh_materials;
    nodesGui.meshTab.materialsView->setModel(nodesGui.meshTab.materialsModel);

    nodesGui.meshTab.saveMaterials = m_uinterface.node_mesh_savemat;

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

    connect(m_selectedNode, SIGNAL(notifyUpdate(tbe::scene::Mesh*)), this, SLOT(meshUpdate(tbe::scene::Mesh*)));
    connect(m_selectedNode, SIGNAL(notifyUpdate(tbe::scene::Light*)), this, SLOT(lightUpdate(tbe::scene::Light*)));
    connect(m_selectedNode, SIGNAL(notifyUpdate(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesUpdate(tbe::scene::ParticlesEmiter*)));
    connect(m_selectedNode, SIGNAL(notifyUpdate(tbe::scene::Node*)), this, SLOT(updateNodeInfo(tbe::scene::Node*)));
    connect(m_selectedNode, SIGNAL(notifyUpdate(tbe::scene::Node*)), m_tbeWidget, SLOT(placeSelection()));

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

    connect(nodesGui.name, SIGNAL(textChanged(const QString&)), m_selectedNode, SLOT(nodeSetName(const QString&)));
    connect(nodesGui.position, SIGNAL(valueChanged(const tbe::Vector3f&)), m_selectedNode, SLOT(nodeSetPos(const tbe::Vector3f&)));
    connect(nodesGui.rotation, SIGNAL(valueChanged(const tbe::Vector3f&)), m_selectedNode, SLOT(nodeSetRotation(const tbe::Vector3f&)));
    connect(nodesGui.scale, SIGNAL(valueChanged(const tbe::Vector3f&)), m_selectedNode, SLOT(nodeSetScale(const tbe::Vector3f&)));
    connect(nodesGui.enable, SIGNAL(clicked(bool)), m_selectedNode, SLOT(nodeSetEnalbe(bool)));

    connect(nodesGui.meshTab.add, SIGNAL(clicked()), this, SLOT(guiMeshNew()));
    connect(nodesGui.meshTab.clone, SIGNAL(clicked()), this, SLOT(guiMeshClone()));
    connect(nodesGui.meshTab.del, SIGNAL(clicked()), this, SLOT(guiMeshDelete()));

    connect(nodesGui.meshTab.saveMaterials, SIGNAL(clicked(bool)), this, SLOT(guiMeshSetSaveMaterial(bool)));
    connect(nodesGui.meshTab.openmatedit, SIGNAL(clicked()), nodesGui.meshTab.matedit, SLOT(show()));

    connect(nodesGui.meshTab.matedit->textured, SIGNAL(clicked(bool)), this, SLOT(guiMeshSetTextured(bool)));
    connect(nodesGui.meshTab.matedit->lighted, SIGNAL(clicked(bool)), this, SLOT(guiMeshSetLighted(bool)));
    connect(nodesGui.meshTab.matedit->alpha, SIGNAL(clicked(bool)), this, SLOT(guiMeshSetAlpha(bool)));
    connect(nodesGui.meshTab.matedit->blending, SIGNAL(clicked(bool)), this, SLOT(guiMeshSetBlend(bool)));

    connect(nodesGui.meshTab.opacity, SIGNAL(valueChanged(double)), this, SLOT(guiMeshSetOpacity(double)));

    connect(nodesGui.meshTab.matedit->alphathreshold, SIGNAL(valueChanged(double)), this, SLOT(guiMeshSetAlphaThreshold(double)));

    connect(nodesGui.meshTab.matedit->add, SIGNAL(clicked()), this, SLOT(guiMeshAddTexture()));
    connect(nodesGui.meshTab.matedit->del, SIGNAL(clicked()), this, SLOT(guiMeshDelTexture()));
    connect(nodesGui.meshTab.matedit->up, SIGNAL(clicked()), this, SLOT(guiMeshTextureUp()));
    connect(nodesGui.meshTab.matedit->down, SIGNAL(clicked()), this, SLOT(guiMeshTextureDown()));

    connect(nodesGui.meshTab.matedit->blend_additive, SIGNAL(clicked()), this, SLOT(guiMeshMaterialSetBlendMode()));
    connect(nodesGui.meshTab.matedit->blend_modulate, SIGNAL(clicked()), this, SLOT(guiMeshMaterialSetBlendMode()));
    connect(nodesGui.meshTab.matedit->blend_mul, SIGNAL(clicked()), this, SLOT(guiMeshMaterialSetBlendMode()));

    connect(nodesGui.meshTab.matedit->texture_additive, SIGNAL(clicked()), this, SLOT(guiMeshTextureSetBlendMode()));
    connect(nodesGui.meshTab.matedit->texture_modulate, SIGNAL(clicked()), this, SLOT(guiMeshTextureSetBlendMode()));
    connect(nodesGui.meshTab.matedit->texture_replace, SIGNAL(clicked()), this, SLOT(guiMeshTextureSetBlendMode()));

    connect(nodesGui.meshTab.matedit->textureView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(guiMeshTextureSelected(const QModelIndex &)));

    connect(nodesGui.meshTab.materialsView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(guiMeshMaterialSelected(const QModelIndex &)));

    connect(nodesGui.waterTab.deform, SIGNAL(valueChanged(double)), m_selectedNode, SLOT(waterSetDeform(double)));
    connect(nodesGui.waterTab.size, SIGNAL(valueChanged(const tbe::Vector2f&)), m_selectedNode, SLOT(waterSetSize(const tbe::Vector2f&)));
    connect(nodesGui.waterTab.uvrepeat, SIGNAL(valueChanged(const tbe::Vector2f&)), m_selectedNode, SLOT(waterSetUvRepeat(const tbe::Vector2f&)));
    connect(nodesGui.waterTab.speed, SIGNAL(valueChanged(double)), m_selectedNode, SLOT(waterSetSpeed(double)));
    connect(nodesGui.waterTab.blend, SIGNAL(valueChanged(double)), m_selectedNode, SLOT(waterSetBlend(double)));

    connect(nodesGui.particlesTab.gravity, SIGNAL(valueChanged(const tbe::Vector3f&)), m_selectedNode, SLOT(particleSetGravity(const tbe::Vector3f&)));
    connect(nodesGui.particlesTab.boxsize, SIGNAL(valueChanged(const tbe::Vector3f&)), m_selectedNode, SLOT(particleSetBoxsize(const tbe::Vector3f&)));
    connect(nodesGui.particlesTab.freemove, SIGNAL(valueChanged(double)), m_selectedNode, SLOT(particleSetFreemove(double)));
    connect(nodesGui.particlesTab.lifeinit, SIGNAL(valueChanged(double)), m_selectedNode, SLOT(particleSetLifeinit(double)));
    connect(nodesGui.particlesTab.lifedown, SIGNAL(valueChanged(double)), m_selectedNode, SLOT(particleSetLifedown(double)));
    connect(nodesGui.particlesTab.number, SIGNAL(valueChanged(int)), m_selectedNode, SLOT(particleSetNumber(int)));
    connect(nodesGui.particlesTab.texture, SIGNAL(textChanged(const QString&)), m_selectedNode, SLOT(particleSetTexture(const QString&)));
    connect(nodesGui.particlesTab.continiousmode, SIGNAL(stateChanged(int)), m_selectedNode, SLOT(particleSetContinousMode(int)));
    connect(nodesGui.particlesTab.build, SIGNAL(clicked()), m_selectedNode, SLOT(particleBuild()));

    connect(nodesGui.particlesTab.add, SIGNAL(clicked()), this, SLOT(guiParticlesNew()));
    connect(nodesGui.particlesTab.clone, SIGNAL(clicked()), this, SLOT(guiParticlesClone()));
    connect(nodesGui.particlesTab.del, SIGNAL(clicked()), this, SLOT(guiParticlesDelete()));

    connect(nodesGui.lighTab.type, SIGNAL(activated(int)), m_selectedNode, SLOT(lightSetType(int)));
    connect(nodesGui.lighTab.ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), m_selectedNode, SLOT(lightSetAmbiant(const tbe::Vector3f&)));
    connect(nodesGui.lighTab.diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), m_selectedNode, SLOT(lightSetDiffuse(const tbe::Vector3f&)));
    connect(nodesGui.lighTab.specular, SIGNAL(valueChanged(const tbe::Vector3f&)), m_selectedNode, SLOT(lightSetSpecular(const tbe::Vector3f&)));
    connect(nodesGui.lighTab.radius, SIGNAL(valueChanged(double)), m_selectedNode, SLOT(lightSetRadius(double)));

    connect(nodesGui.lighTab.add, SIGNAL(clicked()), this, SLOT(guiLightNew()));
    connect(nodesGui.lighTab.clone, SIGNAL(clicked()), this, SLOT(guiLightClone()));
    connect(nodesGui.lighTab.del, SIGNAL(clicked()), this, SLOT(guiLightDelete()));

    connect(nodesGui.nodesListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(guiSelect(const QModelIndex&)));

    connect(m_tbeWidget, SIGNAL(notifyMeshAdd(tbe::scene::Mesh*)), this, SLOT(meshRegister(tbe::scene::Mesh*)));
    connect(m_tbeWidget, SIGNAL(notifyMeshSelect(tbe::scene::Mesh*)), this, SLOT(meshSelect(tbe::scene::Mesh*)));
    connect(m_tbeWidget, SIGNAL(notifyMeshUpdate(tbe::scene::Mesh*)), this, SLOT(meshUpdate(tbe::scene::Mesh*)));
    connect(m_tbeWidget, SIGNAL(notifyMeshDelete(tbe::scene::Mesh*)), this, SLOT(meshDelete(tbe::scene::Mesh*)));

    connect(m_tbeWidget, SIGNAL(notifyLightAdd(tbe::scene::Light*)), this, SLOT(lightRegister(tbe::scene::Light*)));
    connect(m_tbeWidget, SIGNAL(notifyLightSelect(tbe::scene::Light*)), this, SLOT(lightSelect(tbe::scene::Light*)));
    connect(m_tbeWidget, SIGNAL(notifyLightUpdate(tbe::scene::Light*)), this, SLOT(lightUpdate(tbe::scene::Light*)));
    connect(m_tbeWidget, SIGNAL(notifyLightDelete(tbe::scene::Light*)), this, SLOT(lightDelete(tbe::scene::Light*)));

    connect(m_tbeWidget, SIGNAL(notifyParticlesAdd(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesRegister(tbe::scene::ParticlesEmiter*)));
    connect(m_tbeWidget, SIGNAL(notifyParticlesSelect(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesSelect(tbe::scene::ParticlesEmiter*)));
    connect(m_tbeWidget, SIGNAL(notifyParticlesUpdate(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesUpdate(tbe::scene::ParticlesEmiter*)));
    connect(m_tbeWidget, SIGNAL(notifyParticlesDelete(tbe::scene::ParticlesEmiter*)), this, SLOT(particlesDelete(tbe::scene::ParticlesEmiter*)));

    connect(envGui.sceneAmbiant, SIGNAL(valueChanged(const tbe::Vector3f&)), m_tbeWidget, SLOT(setSceneAmbiant(const tbe::Vector3f&)));

    connect(m_tbeWidget, SIGNAL(notifyInitFog(tbe::scene::Fog*)), this, SLOT(fogRegister(tbe::scene::Fog*)));
    connect(m_tbeWidget, SIGNAL(notifyInitSkybox(tbe::scene::SkyBox*)), this, SLOT(skyboxRegister(tbe::scene::SkyBox*)));
    connect(m_tbeWidget, SIGNAL(notifyInitAmbiant(const tbe::Vector3f&)), this, SLOT(sceneAmbiantUpdate(const tbe::Vector3f&)));

    connect(m_tbeWidget, SIGNAL(notifyListRebuild()), this, SLOT(clearNodeList()));

    connect(m_tbeWidget, SIGNAL(notifyDeselect()), this, SLOT(unselect()));

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

    unselect();
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

            m_tbeWidget->meshRegister(mesh);

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
    if(m_selectedNode->mesh())
        m_tbeWidget->meshClone(m_selectedNode->mesh());
}

void MainWindow::guiMeshDelete()
{
    if(tbe::scene::Mesh * mesh = m_selectedNode->mesh())
    {
        m_tbeWidget->meshDelete(mesh);

        meshDelete(mesh);

        unselect();

        m_tbeWidget->deselect();
    }
}

void MainWindow::guiLightNew()
{
    using namespace tbe::scene;

    Light* light = m_tbeWidget->lightNew();

    m_tbeWidget->lightRegister(light);

    lightRegister(light);
    lightSelect(light);
}

void MainWindow::guiLightClone()
{
    if(m_selectedNode->light())
        m_tbeWidget->lightClone(m_selectedNode->light());
}

void MainWindow::guiLightDelete()
{
    if(tbe::scene::Light * light = m_selectedNode->light())
    {
        m_tbeWidget->lightDelete(light);

        lightDelete(light);

        unselect();

        m_tbeWidget->deselect();

        notifyChanges(true);
    }
}

void MainWindow::guiParticlesNew()
{
    using namespace tbe::scene;

    ParticlesEmiter* particles = m_tbeWidget->particlesNew();

    m_tbeWidget->particlesRegister(particles);

    particlesRegister(particles);
    particlesSelect(particles);
}

void MainWindow::guiParticlesClone()
{
    if(m_selectedNode->particles())
    {
        m_tbeWidget->particlesClone(m_selectedNode->particles());
    }
}

void MainWindow::guiParticlesDelete()
{
    if(tbe::scene::ParticlesEmiter * particles = m_selectedNode->particles())
    {
        m_tbeWidget->particlesDelete(particles);

        particlesDelete(particles);

        unselect();

        m_tbeWidget->deselect();

        notifyChanges(true);
    }
}

void MainWindow::nodeUpdate(tbe::scene::Node* node)
{
    if(!node)
        return;

    nodesGui.name->blockSignals(true);
    nodesGui.position->blockSignals(true);
    nodesGui.scale->blockSignals(true);
    nodesGui.rotation->blockSignals(true);
    nodesGui.enable->blockSignals(true);

    nodesGui.name->setText(node->getName().c_str());
    nodesGui.position->setValue(node->getPos());
    nodesGui.scale->setValue(node->getMatrix().getScale());
    nodesGui.rotation->setValue(node->getMatrix().getRotate().getEuler());
    nodesGui.enable->setChecked(node->isEnable());

    nodesGui.name->blockSignals(false);
    nodesGui.position->blockSignals(false);
    nodesGui.scale->blockSignals(false);
    nodesGui.rotation->blockSignals(false);
    nodesGui.enable->blockSignals(false);

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

void MainWindow::unselect()
{
    m_selectedNode->mesh(NULL);
    m_selectedNode->light(NULL);
    m_selectedNode->particles(NULL);
    m_selectedNode->node(NULL);

    nodesGui.nodesListView->clearSelection();
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

    nodesGui.nodesListView->resizeColumnToContents(0);
    nodesGui.nodesListView->resizeColumnToContents(1);

    notifyChanges(true);
}

void MainWindow::meshDelete(tbe::scene::Mesh* mesh)
{
    QStandardItem* item = nodesGui.nodeItemBinder[mesh];
    QStandardItem* parent = item->parent();

    if(parent)
        parent->removeRow(item->row());
    else
        nodesGui.nodesListModel->removeRow(item->row());

    nodesGui.nodeItemBinder.remove(mesh);

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

    bool matset = m_selectedNode->mesh()->isOutputMaterial();

    nodesGui.meshTab.saveMaterials->setEnabled(true);

    nodesGui.meshTab.saveMaterials->setChecked(matset);
    nodesGui.meshTab.openmatedit->setEnabled(matset);
    nodesGui.meshTab.opacity->setEnabled(matset);
}

void MainWindow::meshSelect(tbe::scene::Mesh* mesh, bool upList)
{
    if(upList)
    {
        QStandardItem* item = nodesGui.nodeItemBinder[mesh];
        nodesGui.nodesListView->
                setCurrentIndex(nodesGui.nodesListModel->indexFromItem(item));
    }

    m_selectedNode->mesh(mesh);

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

void MainWindow::lightDelete(tbe::scene::Light* light)
{
    QStandardItem* item = nodesGui.nodeItemBinder[light];
    QStandardItem* parent = item->parent();

    if(parent)
        parent->removeRow(item->row());
    else
        nodesGui.nodesListModel->removeRow(item->row());
}

void MainWindow::lightSelect(tbe::scene::Light* light, bool upList)
{
    if(upList)
    {
        QStandardItem* item = nodesGui.nodeItemBinder[light];
        nodesGui.nodesListView->setCurrentIndex(nodesGui.nodesListModel->indexFromItem(item));
    }

    m_selectedNode->light(light);

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

void MainWindow::particlesDelete(tbe::scene::ParticlesEmiter* particles)
{
    QStandardItem* item = nodesGui.nodeItemBinder[particles];
    QStandardItem* parent = item->parent();

    if(parent)
        parent->removeRow(item->row());
    else
        nodesGui.nodesListModel->removeRow(item->row());
}

void MainWindow::particlesSelect(tbe::scene::ParticlesEmiter *particles, bool upList)
{
    if(upList)
    {
        QStandardItem* item = nodesGui.nodeItemBinder[particles];
        nodesGui.nodesListView->setCurrentIndex(nodesGui.nodesListModel->indexFromItem(item));
    }

    m_selectedNode->particles(particles);

    m_tbeWidget->particlesSelect(particles);

    nodesGui.attribTab->setCurrentIndex(3);

    particlesUpdate(particles);
}

void MainWindow::scopeNode(int move)
{
    if(!m_selectedNode->node())
        return;

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
    m_uinterface.propertyTab->setVisible(!full);
    m_uinterface.infoBox->setVisible(!full);
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
    if(!m_selectedNode->node())
        return;

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
    if(!m_selectedNode->node())
        return;

    QList<QStandardItem*> newfield;

    newfield
            << new QStandardItem("[nouvelle clé]")
            << new QStandardItem("[nouvelle valeur]");

    nodesGui.additionalModel->appendRow(newfield);

    notifyChanges(true);
}

void MainWindow::guiDelNodeField()
{
    if(!m_selectedNode->node())
        return;

    QModelIndex i = nodesGui.additionalView->currentIndex();

    if(i.isValid())
    {
        QString key = nodesGui.additionalModel->item(i.row(), 0)->text();

        m_selectedNode->node()->delUserData(key.toStdString());

        nodesGui.additionalModel->removeRow(i.row());

        notifyChanges(true);
    }
}

void MainWindow::guiClearNodeField()
{
    if(!m_selectedNode->node())
        return;

    if(nodesGui.additionalModel->rowCount() <= 0)
        return;

    int re = QMessageBox::warning(this, "Question ?", "Effacer tout les champs ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if(re == QMessageBox::Yes)
    {
        m_selectedNode->node()->clearUserData();
        nodesGui.additionalModel->removeRows(0, nodesGui.additionalModel->rowCount());
        notifyChanges(true);
    }
}

void MainWindow::guiChangeNodeField(QStandardItem* item)
{
    if(!m_selectedNode->node())
        return;

    if(item->column() == 0)
    {
        QStandardItem* value = nodesGui.additionalModel->item(item->row(), 1);

        m_selectedNode->node()->delUserData(item->text().toStdString());
        m_selectedNode->node()->setUserData(item->text().toStdString(), value->text().toStdString());
    }
    else if(item->column() == 1)
    {
        QStandardItem* key = nodesGui.additionalModel->item(item->row(), 0);
        m_selectedNode->node()->setUserData(key->text().toStdString(), item->text().toStdString());

    }
}

void MainWindow::clearNodeList()
{
    nodesGui.nodesListModel->removeRows(0, nodesGui.nodesListModel->rowCount());
}

void MainWindow::setSelectedTexture(tbe::Texture& tex)
{
    using namespace tbe::scene;

    QModelIndex index = nodesGui.meshTab.matedit->textureView->currentIndex();

    if(!index.isValid())
        return;

    Material* mat = getSelectedMaterial();

    mat->setTexture(tex, index.row());

    QVariant data;
    data.setValue(tex);

    nodesGui.meshTab.textureModel->itemFromIndex(index)->setData(data);
}

tbe::Texture MainWindow::getSelectedTexture()
{
    using namespace tbe;

    QModelIndex index = nodesGui.meshTab.matedit->textureView->currentIndex();

    if(index.isValid())
        return nodesGui.meshTab.textureModel
            ->itemFromIndex(index)->data().value<Texture > ();
    else
        return Texture();
}

tbe::scene::Material* MainWindow::getSelectedMaterial()
{
    using namespace tbe::scene;

    QModelIndex index = nodesGui.meshTab.materialsView->currentIndex();

    if(index.isValid())
        return nodesGui.meshTab.materialsModel
            ->itemFromIndex(index)->data().value<Material*>();
    else
        return NULL;
}

void MainWindow::guiMeshSetSaveMaterial(bool stat)
{
    if(!m_selectedNode->mesh())
        return;

    m_selectedNode->mesh()->setOutputMaterial(stat);

    nodesGui.meshTab.openmatedit->setEnabled(stat);
    nodesGui.meshTab.opacity->setEnabled(stat);
}

void MainWindow::guiMeshMaterialSelected(const QModelIndex& index)
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe;
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    // Textures

    nodesGui.meshTab.matedit->textured->setChecked(mat->isEnable(Material::TEXTURE));

    nodesGui.meshTab.textureModel->
            removeRows(0, nodesGui.meshTab.textureModel->rowCount());

    unsigned count = mat->getTexturesCount();
    for(unsigned i = 0; i < count; i++)
    {
        QString path = mat->getTexture(i).getFilename().c_str();

        Texture tex(path.toStdString(), true);

        QVariant data;
        data.setValue(tex);

        QStandardItem* item = new QStandardItem(QFileInfo(path).baseName());
        item->setData(data);

        nodesGui.meshTab.textureModel->appendRow(item);
    }

    QModelIndex first = nodesGui.meshTab.textureModel->index(0, 0);

    nodesGui.meshTab.matedit->textureView->setCurrentIndex(first);
    guiMeshTextureSelected(first);

    // Material stat
    nodesGui.meshTab.matedit->lighted->setChecked(mat->isEnable(Material::LIGHT));

    // Blending stat
    bool blending = mat->isEnable(Material::BLEND_MOD)
            || mat->isEnable(Material::BLEND_ADD)
            || mat->isEnable(Material::BLEND_MUL);

    nodesGui.meshTab.matedit->blending->setChecked(blending);

    if(blending)
    {
        if(mat->isEnable(Material::BLEND_MOD))
            nodesGui.meshTab.matedit->blend_modulate->setChecked(true);

        else if(mat->isEnable(Material::BLEND_ADD))
            nodesGui.meshTab.matedit->blend_additive->setChecked(true);

        else if(mat->isEnable(Material::BLEND_MUL))
            nodesGui.meshTab.matedit->blend_mul->setChecked(true);
    }

    else
        nodesGui.meshTab.matedit->blend_modulate->setChecked(true);

    nodesGui.meshTab.opacity->setValue(m_selectedNode->mesh()->getOpacity());

    // Alpha
    bool alpha = mat->isEnable(Material::ALPHA);

    nodesGui.meshTab.matedit->alpha->setChecked(alpha);
    nodesGui.meshTab.matedit->alphathreshold->setValue(mat->getAlphaThershold());
}

void MainWindow::guiMeshSetTextured(bool stat)
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
        mat->enable(Material::TEXTURE);
    else
        mat->disable(Material::TEXTURE);
}

void MainWindow::guiMeshTextureSelected(const QModelIndex& index)
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe;

    Texture tex = getSelectedTexture();

    if(tex.getMulTexBlend() == Texture::MODULATE)
        nodesGui.meshTab.matedit->texture_modulate->setChecked(true);

    else if(tex.getMulTexBlend() == Texture::ADDITIVE)
        nodesGui.meshTab.matedit->texture_additive->setChecked(true);

    else if(tex.getMulTexBlend() == Texture::REPLACE)
        nodesGui.meshTab.matedit->texture_replace->setChecked(true);
}

void MainWindow::guiMeshAddTexture()
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe;
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    QStringList paths = QFileDialog::getOpenFileNames(this);

    int offset = mat->getTexturesCount();

    for(int i = 0; i < paths.size(); i++)
    {
        try
        {
            Texture tex(paths[i].toStdString(), true);

            QVariant data;
            data.setValue(tex);

            QStandardItem* item = new QStandardItem(QFileInfo(paths[i]).baseName());
            item->setData(data);

            nodesGui.meshTab.textureModel->appendRow(item);
            mat->setTexture(tex, offset + i);
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(this, "Erreur de chargement", e.what());
        }
    }
}

void MainWindow::guiMeshDelTexture()
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    QModelIndex index = nodesGui.meshTab.matedit->textureView->currentIndex();
    mat->dropTexture(index.row());

    nodesGui.meshTab.textureModel->removeRow(index.row());
}

void MainWindow::guiMeshTextureUp()
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe;
    using namespace scene;

    int srcindex = nodesGui.meshTab.matedit->textureView->currentIndex().row();

    if(srcindex > 0)
    {
        int dstindex = srcindex - 1;

        Material* mat = getSelectedMaterial();

        Texture src = mat->getTexture(srcindex);
        Texture dst = mat->getTexture(dstindex);

        mat->setTexture(src, dstindex);
        mat->setTexture(dst, srcindex);

        QList<QStandardItem*> items = nodesGui.meshTab.textureModel->takeRow(srcindex);
        nodesGui.meshTab.textureModel->insertRow(dstindex, items);

        nodesGui.meshTab.matedit->textureView
                ->setCurrentIndex(nodesGui.meshTab.textureModel->index(dstindex, 0));
    }
}

void MainWindow::guiMeshTextureDown()
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe;
    using namespace scene;

    int srcindex = nodesGui.meshTab.matedit->textureView->currentIndex().row();

    if(srcindex < nodesGui.meshTab.textureModel->rowCount() - 1)
    {
        int dstindex = srcindex + 1;

        Material* mat = getSelectedMaterial();

        Texture src = mat->getTexture(srcindex);
        Texture dst = mat->getTexture(dstindex);

        mat->setTexture(src, dstindex);
        mat->setTexture(dst, srcindex);

        QList<QStandardItem*> items = nodesGui.meshTab.textureModel->takeRow(srcindex);
        nodesGui.meshTab.textureModel->insertRow(dstindex, items);

        nodesGui.meshTab.matedit->textureView
                ->setCurrentIndex(nodesGui.meshTab.textureModel->index(dstindex, 0));
    }
}

void MainWindow::guiMeshTextureSetBlendMode()
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe;
    using namespace scene;

    Texture tex = getSelectedTexture();

    if(nodesGui.meshTab.matedit->texture_modulate->isChecked())
        tex.setMulTexBlend(Texture::MODULATE);

    else if(nodesGui.meshTab.matedit->texture_additive->isChecked())
        tex.setMulTexBlend(Texture::ADDITIVE);

    else if(nodesGui.meshTab.matedit->texture_replace->isChecked())
        tex.setMulTexBlend(Texture::REPLACE);

    setSelectedTexture(tex);
}

void MainWindow::guiMeshSetBlend(bool stat)
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
    {
        mat->enable(Material::COLOR);

        guiMeshMaterialSetBlendMode();
    }

    else
    {
        mat->disable(Material::BLEND_ADD | Material::BLEND_MOD
                     | Material::BLEND_MUL | Material::COLOR);
    }
}

void MainWindow::guiMeshMaterialSetBlendMode()
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe;
    using namespace scene;

    Material* mat = getSelectedMaterial();

    mat->disable(Material::BLEND_MOD | Material::BLEND_ADD | Material::BLEND_MUL);

    if(nodesGui.meshTab.matedit->blend_modulate->isChecked())
        mat->enable(Material::BLEND_MOD);

    else if(nodesGui.meshTab.matedit->blend_additive->isChecked())
        mat->enable(Material::BLEND_ADD);

    else if(nodesGui.meshTab.matedit->blend_mul->isChecked())
        mat->enable(Material::BLEND_MUL);
}

void MainWindow::guiMeshSetOpacity(double value)
{
    if(!m_selectedNode->mesh())
        return;

    m_selectedNode->mesh()->setOpacity((float)value);
}

void MainWindow::guiMeshSetAlpha(bool stat)
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
    {
        mat->enable(Material::ALPHA);
    }
    else
    {
        mat->disable(Material::ALPHA);
    }
}

void MainWindow::guiMeshSetAlphaThreshold(double value)
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setAlphaThershold((float)value);
}

void MainWindow::guiMeshSetLighted(bool stat)
{
    if(!m_selectedNode->mesh())
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
        mat->enable(Material::LIGHT);
    else
        mat->disable(Material::LIGHT);
}
