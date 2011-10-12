/*
 * File:   MainWindow.cpp
 * Author: b4n92uid
 *
 * Created on 3 décembre 2010, 17:20
 */

#include "MainWindow.h"
#include "HistoryState.h"

#include "ui_interface.h"

MainWindow::MainWindow()
{
    notifyChanges(false);

    m_uinterface = new Ui_mainWindow;
    m_rootNode = NULL;
    m_sourceCopy = NULL;
}

MainWindow::~MainWindow()
{
    delete m_uinterface;
    delete m_rootNode;
}

QString MainWindow::openFileName() const
{
    return m_filename;
}

Ui_mainWindow* MainWindow::ui()
{
    return m_uinterface;
}

void MainWindow::openFileHistory()
{
    QAction* action = qobject_cast<QAction*>(sender());

    openScene(action->data().toString());
}

void MainWindow::buildFileHistory()
{
    QMenu* filehistory = m_uinterface->actionLastFiles->menu();

    if(!filehistory)
    {
        filehistory = new QMenu(this);
        m_uinterface->actionLastFiles->setMenu(filehistory);
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
            QAction* act = filehistory->addAction("..." + filepath.section('\\', -2),
                                                  this, SLOT(openFileHistory()));
            act->setData(filepath);
        }
    }
}

void MainWindow::pushFileHistory(const QString& filepath)
{
    QString nativefp = QDir::toNativeSeparators(filepath);

    QStringList history = m_config->value("history").toStringList();

    if(history.count(nativefp))
        history.removeAll(nativefp);

    history.push_front(nativefp);

    if(history.count() > 16)
        history.pop_back();

    m_config->setValue("history", history);

    buildFileHistory();
}

void MainWindow::initWidgets()
{
    m_uinterface->setupUi(this);

    m_tbeWidget = m_uinterface->glwidget;

    nodesGui.attribTab = m_uinterface->attribTab;

    m_config = new QSettings(this);

    m_packerDialog = new PackerDialog(this);

    buildFileHistory();

    m_uinterface->actionUndo->setEnabled(false);

    m_uinterface->node_list_sort->setMenu(m_uinterface->menuTrier);

    // Générale ----------------------------------------------------------------

    genGui.title = m_uinterface->gen_title;
    genGui.author = m_uinterface->gen_author;

    genGui.additionalModel = new QStandardItemModel(this);
    genGui.additionalModel->setHorizontalHeaderLabels(QStringList() << "Clé" << "Valeur");

    genGui.additionalView = m_uinterface->gen_additional;
    genGui.additionalView->setModel(genGui.additionalModel);
    genGui.additionalView->setRootIsDecorated(false);
    genGui.additionalView->header()->setResizeMode(QHeaderView::Stretch);

    genGui.addField = m_uinterface->gen_addfield;
    genGui.delField = m_uinterface->gen_delfield;
    genGui.clearFields = m_uinterface->gen_clearfields;

    // Node --------------------------------------------------------------------

    nodesGui.additionalModel = new QStandardItemModel(this);
    nodesGui.additionalModel->setHorizontalHeaderLabels(QStringList() << "Clé" << "Valeur");

    nodesGui.additionalView = m_uinterface->node_additional;
    nodesGui.additionalView->setModel(nodesGui.additionalModel);
    nodesGui.additionalView->setRootIsDecorated(false);
    nodesGui.additionalView->header()->setResizeMode(QHeaderView::Stretch);

    nodesGui.addField = m_uinterface->node_addfield;
    nodesGui.delField = m_uinterface->node_delfield;
    nodesGui.clearFields = m_uinterface->node_clearfields;

    m_selectedNode = NULL;

    nodesGui.name = m_uinterface->node_name;
    nodesGui.position = new QVectorBox(this, m_uinterface->node_pos_x, m_uinterface->node_pos_y, m_uinterface->node_pos_z);
    nodesGui.rotation = new QVectorBox(this, m_uinterface->node_rot_x, m_uinterface->node_rot_y, m_uinterface->node_rot_z);
    nodesGui.scale = new QVectorBox(this, m_uinterface->node_scl_x, m_uinterface->node_scl_y, m_uinterface->node_scl_z);

    nodesGui.enable = m_uinterface->node_enable;

    nodesGui.nodeUp = m_uinterface->node_list_up;
    nodesGui.nodeDown = m_uinterface->node_list_down;
    nodesGui.nodeRight = m_uinterface->node_list_makechild;
    nodesGui.nodeLeft = m_uinterface->node_list_makeparent;

    nodesGui.displayMesh = m_uinterface->node_display_meshs;
    nodesGui.displayLights = m_uinterface->node_display_lights;
    nodesGui.displayParticles = m_uinterface->node_display_particules;
    nodesGui.displayMarks = m_uinterface->node_display_marks;

    // -------- Mark

    nodesGui.markTab.add = m_uinterface->node_mark_add;

    // -------- Mesh

    nodesGui.meshTab.add = m_uinterface->node_mesh_add;

    nodesGui.meshTab.openmatedit = m_uinterface->node_mesh_editmat;

    nodesGui.meshTab.textureModel = new QStandardItemModel(this);

    nodesGui.meshTab.opacity = m_uinterface->node_mesh_opacity;

    nodesGui.meshTab.matedit = new MaterialEditDialog(this);

    nodesGui.meshTab.matedit->textureView->setModel(nodesGui.meshTab.textureModel);

    nodesGui.meshTab.materialsModel = new QStandardItemModel(this);

    nodesGui.meshTab.materialsView = m_uinterface->node_mesh_materials;
    nodesGui.meshTab.materialsView->setModel(nodesGui.meshTab.materialsModel);

    nodesGui.meshTab.saveMaterials = m_uinterface->node_mesh_savemat;

    nodesGui.meshTab.billboardX = m_uinterface->node_mesh_billboard_x;
    nodesGui.meshTab.billboardY = m_uinterface->node_mesh_billboard_y;

    // -------- Water

    nodesGui.waterTab.deform = m_uinterface->node_water_deform;
    nodesGui.waterTab.size = new QVector2Box(this, m_uinterface->node_water_size_x, m_uinterface->node_water_size_y);
    nodesGui.waterTab.uvrepeat = new QVector2Box(this, m_uinterface->node_water_uvrepeat_x, m_uinterface->node_water_uvrepeat_y);
    nodesGui.waterTab.speed = m_uinterface->node_water_speed;
    nodesGui.waterTab.blend = m_uinterface->node_water_blend;
    nodesGui.waterTab.add = m_uinterface->node_water_add;

    // -------- Particles

    nodesGui.particlesTab.gravity = new QVectorBox(this, m_uinterface->node_particles_gravity_x, m_uinterface->node_particles_gravity_y, m_uinterface->node_particles_gravity_z);
    nodesGui.particlesTab.boxsize = new QVectorBox(this, m_uinterface->node_particles_boxsize_x, m_uinterface->node_particles_boxsize_y, m_uinterface->node_particles_boxsize_z);
    nodesGui.particlesTab.bulletsize = new QVector2Box(this, m_uinterface->node_particles_bulletsize_x, m_uinterface->node_particles_bulletsize_y);
    nodesGui.particlesTab.freemove = m_uinterface->node_particles_freemove;
    nodesGui.particlesTab.lifeinit = m_uinterface->node_particles_lifeinit;
    nodesGui.particlesTab.lifedown = m_uinterface->node_particles_lifedown;
    nodesGui.particlesTab.number = m_uinterface->node_particles_number;
    nodesGui.particlesTab.texture = new QBrowsEdit(this, m_uinterface->node_particles_texture, m_uinterface->node_particles_texture_browse);
    nodesGui.particlesTab.continiousmode = m_uinterface->node_particles_continousmode;
    nodesGui.particlesTab.pointsprite = m_uinterface->node_particles_pointsprite;
    nodesGui.particlesTab.build = m_uinterface->node_particles_build;
    nodesGui.particlesTab.add = m_uinterface->node_particles_add;

    // -------- Lights

    nodesGui.lighTab.type = m_uinterface->node_light_type;

    nodesGui.lighTab.ambiant = new QVectorBox(this, m_uinterface->node_light_ambiant_x, m_uinterface->node_light_ambiant_y, m_uinterface->node_light_ambiant_z);
    nodesGui.lighTab.diffuse = new QVectorBox(this, m_uinterface->node_light_diffuse_x, m_uinterface->node_light_diffuse_y, m_uinterface->node_light_diffuse_z);
    nodesGui.lighTab.specular = new QVectorBox(this, m_uinterface->node_light_specular_x, m_uinterface->node_light_specular_y, m_uinterface->node_light_specular_z);

    nodesGui.lighTab.radius = m_uinterface->node_light_radius;

    nodesGui.lighTab.add = m_uinterface->node_light_add;

    // Nodes liste -------------------------------------------------------------

    QStringList headerLabels;
    headerLabels << "Type" << "Nom";

    nodesGui.nodesListModel = new QStandardItemModel(this);
    nodesGui.nodesListModel->setHorizontalHeaderLabels(headerLabels);

    nodesGui.nodesListProxyModel = new NodeListProxyModel(this);
    nodesGui.nodesListProxyModel->setSourceModel(nodesGui.nodesListModel);

    nodesGui.nodesListView = m_uinterface->node_list;
    nodesGui.nodesListView->setModel(nodesGui.nodesListProxyModel);
    nodesGui.nodesListView->setAlternatingRowColors(true);
    nodesGui.nodesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    nodesGui.nodesListView->header()->setResizeMode(QHeaderView::Stretch);

    // Environment -------------------------------------------------------------

    envGui.sceneAmbiant = new QVectorBox(this, m_uinterface->env_ambient_x, m_uinterface->env_ambient_y, m_uinterface->env_ambient_z);

    envGui.skybox.apply = m_uinterface->skybox_apply;
    envGui.skybox.enable = m_uinterface->skybox_enable;

    envGui.skybox.textures[0] = new QBrowsEdit(this, m_uinterface->skybox_front, m_uinterface->skybox_front_browse);
    envGui.skybox.textures[1] = new QBrowsEdit(this, m_uinterface->skybox_back, m_uinterface->skybox_back_browse);
    envGui.skybox.textures[2] = new QBrowsEdit(this, m_uinterface->skybox_top, m_uinterface->skybox_top_browse);
    envGui.skybox.textures[3] = new QBrowsEdit(this, m_uinterface->skybox_bottom, m_uinterface->skybox_bottom_browse);
    envGui.skybox.textures[4] = new QBrowsEdit(this, m_uinterface->skybox_left, m_uinterface->skybox_left_browse);
    envGui.skybox.textures[5] = new QBrowsEdit(this, m_uinterface->skybox_right, m_uinterface->skybox_right_browse);

    envGui.fog.color = new QVectorBox(this, m_uinterface->fog_x, m_uinterface->fog_y, m_uinterface->fog_z);
    envGui.fog.start = m_uinterface->fog_start;
    envGui.fog.end = m_uinterface->fog_end;
    envGui.fog.enable = m_uinterface->fog_enable;

    envGui.znear = m_uinterface->env_znear;
    envGui.zfar = m_uinterface->env_zfar;
}

void MainWindow::initConnections()
{
    connect(genGui.addField, SIGNAL(clicked()), this, SLOT(guiAddSceneField()));
    connect(genGui.delField, SIGNAL(clicked()), this, SLOT(guiDelSceneField()));
    connect(genGui.clearFields, SIGNAL(clicked()), this, SLOT(guiClearSceneField()));

    // File Menu

    connect(m_uinterface->actionNewScene, SIGNAL(triggered()), this, SLOT(newScene()));
    connect(m_uinterface->actionOpen, SIGNAL(triggered()), this, SLOT(openSceneDialog()));
    connect(m_uinterface->actionSave, SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(m_uinterface->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveSceneDialog()));
    connect(m_uinterface->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(m_uinterface->actionToggleFullScreen, SIGNAL(triggered(bool)), this, SLOT(toggleFullWidget(bool)));
    connect(m_uinterface->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(m_uinterface->actionScreenShot, SIGNAL(triggered()), this, SLOT(screenshot()));

    // Edit Menu

    connect(m_uinterface->actionLockNode, SIGNAL(triggered()), m_tbeWidget, SLOT(toggleLockedNode()));
    connect(m_uinterface->actionBaseOnFloor, SIGNAL(triggered()), m_tbeWidget, SLOT(baseOnFloor()));
    connect(m_uinterface->actionCenterOnFloor, SIGNAL(triggered()), m_tbeWidget, SLOT(centerOnFloor()));
    connect(m_uinterface->actionEnableGrid, SIGNAL(triggered()), m_tbeWidget, SLOT(toggleGridDisplay()));
    connect(m_uinterface->actionCloneNode, SIGNAL(triggered()), m_tbeWidget, SLOT(cloneSelected()));
    connect(m_uinterface->actionDeleteNode, SIGNAL(triggered()), m_tbeWidget, SLOT(deleteSelected()));

    connect(m_uinterface->actionUndo, SIGNAL(triggered()), m_tbeWidget, SLOT(popHistoryStat()));

    connect(m_uinterface->actionNewMesh, SIGNAL(triggered()), this, SLOT(guiMeshNew()));
    connect(m_uinterface->actionNewLight, SIGNAL(triggered()), this, SLOT(guiLightNew()));
    connect(m_uinterface->actionNewParticles, SIGNAL(triggered()), this, SLOT(guiParticlesNew()));
    connect(m_uinterface->actionNewMapMark, SIGNAL(triggered()), this, SLOT(guiMarkNew()));

    connect(m_uinterface->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(m_uinterface->actionPastFields, SIGNAL(triggered()), this, SLOT(pastField()));
    connect(m_uinterface->actionPastPosition, SIGNAL(triggered()), this, SLOT(pastPosition()));
    connect(m_uinterface->actionPastScale, SIGNAL(triggered()), this, SLOT(pastScale()));
    connect(m_uinterface->actionPastRotation, SIGNAL(triggered()), this, SLOT(pastRotation()));

    // Tools Menu

    connect(m_uinterface->actionOpenPacker, SIGNAL(triggered()), m_packerDialog, SLOT(exec()));

    connect(this, SIGNAL(pauseRendring()), m_tbeWidget, SLOT(pauseRendring()));
    connect(this, SIGNAL(resumeRendring()), m_tbeWidget, SLOT(resumeRendring()));

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

    connect(nodesGui.displayMesh, SIGNAL(clicked(bool)), nodesGui.nodesListProxyModel, SLOT(toggleMesh(bool)));
    connect(nodesGui.displayLights, SIGNAL(clicked(bool)), nodesGui.nodesListProxyModel, SLOT(toggleLight(bool)));
    connect(nodesGui.displayParticles, SIGNAL(clicked(bool)), nodesGui.nodesListProxyModel, SLOT(toggleParticles(bool)));
    connect(nodesGui.displayMarks, SIGNAL(clicked(bool)), nodesGui.nodesListProxyModel, SLOT(toggleMapMark(bool)));

    connect(m_uinterface->actionSortByName, SIGNAL(triggered()), nodesGui.nodesListProxyModel, SLOT(sortByName()));
    connect(m_uinterface->actionSortByType, SIGNAL(triggered()), nodesGui.nodesListProxyModel, SLOT(sortByType()));
    connect(m_uinterface->actionSortFromCamera, SIGNAL(triggered()), nodesGui.nodesListProxyModel, SLOT(sortFromCamera()));
    connect(m_uinterface->actionSortFromSelection, SIGNAL(triggered()), nodesGui.nodesListProxyModel, SLOT(sortFromSelection()));

    connect(nodesGui.markTab.add, SIGNAL(clicked()), this, SLOT(guiMarkNew()));
    connect(nodesGui.meshTab.add, SIGNAL(clicked()), this, SLOT(guiMeshNew()));
    connect(nodesGui.particlesTab.add, SIGNAL(clicked()), this, SLOT(guiParticlesNew()));
    connect(nodesGui.lighTab.add, SIGNAL(clicked()), this, SLOT(guiLightNew()));

    connect(m_tbeWidget, SIGNAL(selection(QNodeInteractor*)), this, SLOT(select(QNodeInteractor*)));
    connect(m_tbeWidget, SIGNAL(deselection()), this, SLOT(deselect()));

    connect(nodesGui.nodesListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(guiSelect(const QModelIndex&)));

    connect(envGui.znear, SIGNAL(valueChanged(double)), this, SLOT(guiZNear(double)));
    connect(envGui.zfar, SIGNAL(valueChanged(double)), this, SLOT(guiZFar(double)));

    connect(envGui.fog.enable, SIGNAL(clicked(bool)), this, SLOT(guiFogApply(bool)));
    connect(envGui.fog.enable, SIGNAL(clicked(bool)), this, SLOT(guiFogApply(bool)));
    connect(envGui.fog.color, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(guiFogApply()));
    connect(envGui.fog.start, SIGNAL(valueChanged(double)), this, SLOT(guiFogApply()));
    connect(envGui.fog.end, SIGNAL(valueChanged(double)), this, SLOT(guiFogApply()));

    connect(envGui.skybox.textures[0], SIGNAL(textChanged(const QString&)), this, SLOT(skyboxWorkingDir(const QString&)));
    connect(envGui.skybox.textures[1], SIGNAL(textChanged(const QString&)), this, SLOT(skyboxWorkingDir(const QString&)));
    connect(envGui.skybox.textures[2], SIGNAL(textChanged(const QString&)), this, SLOT(skyboxWorkingDir(const QString&)));
    connect(envGui.skybox.textures[3], SIGNAL(textChanged(const QString&)), this, SLOT(skyboxWorkingDir(const QString&)));
    connect(envGui.skybox.textures[4], SIGNAL(textChanged(const QString&)), this, SLOT(skyboxWorkingDir(const QString&)));
    connect(envGui.skybox.textures[5], SIGNAL(textChanged(const QString&)), this, SLOT(skyboxWorkingDir(const QString&)));

    connect(envGui.skybox.enable, SIGNAL(clicked(bool)), this, SLOT(guiSkyboxApply(bool)));
    connect(envGui.skybox.apply, SIGNAL(clicked()), this, SLOT(guiSkyboxApply()));

    connect(envGui.sceneAmbiant, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(guiAmbiantApply(const tbe::Vector3f&)));

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateInfoBox()));
    m_timer->start(16);
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

    nodesGui.nodesListModel->removeRows(0, nodesGui.nodesListModel->rowCount());

    genGui.title->clear();
    genGui.author->clear();
    genGui.additionalModel->removeRows(0, genGui.additionalModel->rowCount());

    deselect();

    notifyChanges(false);

    if(!m_rootNode)
    {
        m_rootNode = new QNodeInteractor(this, m_tbeWidget->rootNode());

        QVariant rootData;
        rootData.setValue(m_rootNode);

        nodesGui.nodesListModel->invisibleRootItem()->setData(rootData, ITEM_ROLE_NODE);
        nodesGui.nodesListModel->invisibleRootItem()->setData("Root", ITEM_ROLE_NAME);
    }
}

void MainWindow::openSceneDialog()
{
    m_tbeWidget->pauseRendring();

    QString filename = QFileDialog::getOpenFileName(this, "Editer une scene",
                                                    m_workingDir.scene);

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

        m_tbeWidget->loadScene(filename);

        m_workingDir.scene
                = m_workingDir.mesh
                = m_workingDir.meshTexture
                = QFileInfo(filename).path();

        nodesGui.particlesTab.texture->setWorkDir(m_workingDir.scene);

        for(unsigned i = 0; i < 6; i++)
            envGui.skybox.textures[i]->setWorkDir(m_workingDir.scene);

        pushFileHistory(filename);

        SceneParser* sceneParser = m_tbeWidget->sceneParser();

        genGui.title->setText(QString::fromStdString(sceneParser->getSceneName()));
        genGui.author->setText(QString::fromStdString(sceneParser->getAuthorName()));

        genGui.additionalModel->removeRows(0, genGui.additionalModel->rowCount());

        const SceneParser::AttribMap addfields = sceneParser->additionalFields();

        for(SceneParser::AttribMap::const_iterator it = addfields.begin(); it != addfields.end(); it++)
        {
            QStandardItem* key = new QStandardItem;
            key->setText(QString::fromStdString(it->first));

            QStandardItem* value = new QStandardItem;
            value->setText(QString::fromStdString(it->second));

            genGui.additionalModel->appendRow(QList<QStandardItem*> () << key << value);
        }

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

        m_workingDir.scene
                = m_workingDir.mesh
                = m_workingDir.meshTexture
                = QFileInfo(filename).path();

        nodesGui.particlesTab.texture->setWorkDir(m_workingDir.scene);

        for(unsigned i = 0; i < 6; i++)
            envGui.skybox.textures[i]->setWorkDir(m_workingDir.scene);

        pushFileHistory(filename);

        m_filename = filename;

        notifyChanges(false);
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
    tbe::scene::SceneParser* sceneParser = m_tbeWidget->sceneParser();

    sceneParser->setSceneName(genGui.title->text().toStdString());
    sceneParser->setAuthorName(genGui.author->text().toStdString());

    sceneParser->clearAdditional();

    int count = genGui.additionalModel->rowCount();
    for(int i = 0; i < count; i++)
    {
        std::string key = genGui.additionalModel->item(i, 0)->text().toStdString();
        std::string value = genGui.additionalModel->item(i, 1)->text().toStdString();

        sceneParser->setAdditionalString(key, value);
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

    QString content = ctor.text->text();

    content.replace("$APP_VERSION", "1.0");
    content.replace("$BUILD_DATE", __DATE__);
    content.replace("$QT_VERSION", qVersion());
    content.replace("$TBE_VERSION", QString::fromStdString(tbe::Device::getVersion()));

    ctor.text->setText(content);

    aboutdlg->open();
    aboutdlg->raise();
}

void MainWindow::guiSelect(const QModelIndex& index)
{
    using namespace tbe::scene;

    QNodeInteractor* interface = nodesGui.nodesListProxyModel->itemData(index)[ITEM_ROLE_NODE].value<QNodeInteractor*>();

    select(interface);
}

void MainWindow::guiMeshNew()
{
    m_tbeWidget->pauseRendring();

    QString filename = QFileDialog::getOpenFileName(parentWidget(), QString(),
                                                    m_workingDir.mesh);

    if(!filename.isNull())
    {
        try
        {
            QMesh* mesh = m_tbeWidget->meshNew(filename);

            select(mesh);

            m_workingDir.mesh
                    = m_workingDir.meshTexture
                    = QFileInfo(filename).path();

            notifyChanges(true);
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(parentWidget(), "Erreur d'ouverture", e.what());
        }
    }

    m_tbeWidget->resumeRendring();
}

void MainWindow::guiLightNew()
{
    using namespace tbe::scene;

    try
    {
        QLight* light = m_tbeWidget->lightNew();

        select(light);

        notifyChanges(true);
    }
    catch(std::exception& e)
    {
        QMessageBox::warning(parentWidget(), "Erreur d'instance", e.what());
    }
}

void MainWindow::guiParticlesNew()
{
    using namespace tbe::scene;

    try
    {
        QParticles* particles = m_tbeWidget->particlesNew();

        select(particles);

        notifyChanges(true);
    }
    catch(std::exception& e)
    {
        QMessageBox::warning(parentWidget(), "Erreur", e.what());
    }
}

void MainWindow::guiMarkNew()
{
    using namespace tbe::scene;

    try
    {
        QMapMark* mark = m_tbeWidget->markNew();

        select(mark);

        notifyChanges(true);
    }
    catch(std::exception& e)
    {
        QMessageBox::warning(parentWidget(), "Erreur", e.what());
    }
}

void MainWindow::select(QNodeInteractor* qnode)
{
    m_lastSelectedNode = m_selectedNode;
    m_selectedNode = qnode;

    m_tbeWidget->selectNode(qnode);
}

void MainWindow::deselect()
{
    if(m_selectedNode)
    {
        m_selectedNode->deselect();
        m_selectedNode = NULL;
    }

    nodesGui.nodesListView->clearSelection();

    m_tbeWidget->deselectNode();
}

void MainWindow::scopeNode(int move)
{
    using namespace tbe;
    using namespace scene;

    if(!m_selectedNode)
        return;

    QModelIndex srcindex = nodesGui.nodesListProxyModel->mapToSource(nodesGui.nodesListView->currentIndex());

    QStandardItem* item = nodesGui.nodesListModel->itemFromIndex(srcindex);
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

            Node* parentNode = host->data(ITEM_ROLE_NODE).value<QNodeInteractor*>()->getTarget();
            Node* currNode = item->data(ITEM_ROLE_NODE).value<QNodeInteractor*>()->getTarget();

            currNode->setParent(parentNode);
            currNode->setPos(parentNode->getPos() + currNode->getPos());

            m_tbeWidget->placeCamera();
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

            Node* parentNode = host->data(ITEM_ROLE_NODE).value<QNodeInteractor*>()->getTarget();
            Node* currNode = item->data(ITEM_ROLE_NODE).value<QNodeInteractor*>()->getTarget();

            currNode->setParent(parentNode);
            currNode->setPos(currNode->getPos() - parentNode->getPos());

            m_tbeWidget->placeCamera();
        }
        else
            parent->insertRow(currRow, row);

        nodesGui.nodesListView->setCurrentIndex(item->index());
    }

    notifyChanges(true);
}

void MainWindow::guiSkyboxApply(bool enable)
{
    if(enable)
    {
        QStringList texs;

        for(unsigned i = 0; i < 6; i++)
            texs << envGui.skybox.textures[i]->getOpenFileName();

        try
        {
            m_tbeWidget->skyboxApply(texs);
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(this, "Erreur: Skybox", e.what());
        }
    }

    else
    {
        m_tbeWidget->skyboxClear();
    }

    notifyChanges(true);
}

void MainWindow::guiFogApply(bool enable)
{
    if(enable)
        m_tbeWidget->fogApply(tbe::math::vec34(envGui.fog.color->value()),
                              envGui.fog.start->value(),
                              envGui.fog.end->value());
    else
        m_tbeWidget->fogClear();

    notifyChanges(true);
}

void MainWindow::ambiant(const tbe::Vector3f& value)
{
    envGui.sceneAmbiant->blockSignals(true);
    envGui.sceneAmbiant->setValue(value);
    envGui.sceneAmbiant->blockSignals(false);
}

QTBEngine* MainWindow::tbeWidget() const
{
    return m_tbeWidget;
}

void MainWindow::guiAmbiantApply(const tbe::Vector3f& value)
{
    m_tbeWidget->setSceneAmbiant(value);

    notifyChanges(true);
}

void MainWindow::fog(tbe::scene::Fog* fog)
{
    QSignalBlocker blocker;
    blocker << envGui.fog.enable << envGui.fog.color
            << envGui.fog.start << envGui.fog.end;

    blocker.block();

    envGui.fog.enable->setChecked(fog->isEnable());
    envGui.fog.color->setValue(tbe::math::vec43(fog->getColor()));
    envGui.fog.start->setValue(fog->getStart());
    envGui.fog.end->setValue(fog->getEnd());

    blocker.unblock();
}

void MainWindow::skybox(tbe::scene::SkyBox* sky)
{
    QSignalBlocker blocker;
    blocker << envGui.skybox.apply << envGui.skybox.enable;

    blocker.block();

    tbe::Texture* texs = sky->getTextures();

    for(unsigned i = 0; i < 6; i++)
        envGui.skybox.textures[i]->setOpenFileName(QString::fromStdString(texs[i].getFilename()));

    envGui.skybox.enable->setChecked(sky->isEnable());

    blocker.unblock();
}

void MainWindow::toggleFullWidget(bool full)
{
    m_uinterface->propertyTab->setVisible(!full);
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

void MainWindow::clearNodeList()
{
    nodesGui.nodesListModel->removeRows(0, nodesGui.nodesListModel->rowCount());

    nodesGui.nodeItemBinder.clear();
}

void MainWindow::skyboxWorkingDir(const QString& filename)
{
    envGui.skybox.textures[0]->setWorkDir(QFileInfo(filename).path());
    envGui.skybox.textures[1]->setWorkDir(QFileInfo(filename).path());
    envGui.skybox.textures[2]->setWorkDir(QFileInfo(filename).path());
    envGui.skybox.textures[3]->setWorkDir(QFileInfo(filename).path());
    envGui.skybox.textures[4]->setWorkDir(QFileInfo(filename).path());
    envGui.skybox.textures[5]->setWorkDir(QFileInfo(filename).path());
}

void MainWindow::screenshot()
{
    QImage shot = m_tbeWidget->grabFrameBuffer(false);

    QString title = QString("%1-%2.png")
            .arg(QString::fromStdString(tbe::tools::unixName(genGui.title->text().toStdString())))
            .arg(QDate::currentDate().toString("yyyyMMdd"));

    QString defname = QDir(m_workingDir.scene).filePath(title);

    QString filename = QFileDialog::getSaveFileName(this, "Enregistrer la capture d'écran", defname);

    if(!filename.isEmpty())
    {
        QFile output(filename);
        output.open(QIODevice::WriteOnly);
        shot.save(&output, "PNG");
    }
}

void MainWindow::pastField()
{
    if(!m_sourceCopy)
        return;

    using namespace tbe;
    using namespace scene;

    m_tbeWidget->pushHistoryStat(new ModificationState(m_selectedNode));

    Node* nearest = m_selectedNode->getTarget();
    nearest->clearUserData();

    const Any::Map& ud = m_sourceCopy->getTarget()->getUserDatas();

    foreach(Any::Map::value_type i, ud)
    {
        nearest->setUserData(i.first, i.second);
    }

    statusBar()->showMessage("Champs coller...", 2000);
}

void MainWindow::pastPosition()
{
    if(!m_sourceCopy)
        return;

    m_tbeWidget->pushHistoryStat(new ModificationState(m_selectedNode));

    m_selectedNode->setPos(m_sourceCopy->getTarget()->getPos());

    statusBar()->showMessage("Position coller...", 2000);
}

void MainWindow::pastScale()
{
    if(!m_sourceCopy)
        return;

    m_tbeWidget->pushHistoryStat(new ModificationState(m_selectedNode));

    m_selectedNode->setScale(m_sourceCopy->getTarget()->getMatrix().getScale());

    statusBar()->showMessage("Scale coller...", 2000);
}

void MainWindow::pastRotation()
{
    if(!m_sourceCopy)
        return;

    m_tbeWidget->pushHistoryStat(new ModificationState(m_selectedNode));

    m_selectedNode->setRotation(m_sourceCopy->getTarget()->getMatrix().getRotate().getEuler());

    statusBar()->showMessage("Rotation coller...", 2000);
}

void MainWindow::copy()
{
    m_sourceCopy = m_selectedNode;

    statusBar()->showMessage("Copie effectuer...", 2000);
}

void MainWindow::zNear(float value)
{
    envGui.znear->blockSignals(true);
    envGui.znear->setValue(value);
    envGui.znear->blockSignals(false);
}

void MainWindow::zFar(float value)
{
    envGui.zfar->blockSignals(true);
    envGui.zfar->setValue(value);
    envGui.zfar->blockSignals(false);
}

void MainWindow::guiZNear(double value)
{
    m_tbeWidget->setZNear(value);
}

void MainWindow::guiZFar(double value)
{
    m_tbeWidget->setZFar(value);
}
