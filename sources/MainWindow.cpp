/*
 * File:   MainWindow.cpp
 * Author: b4n92uid
 *
 * Created on 3 décembre 2010, 17:20
 */

#include "MainWindow.h"
#include "HistoryState.h"

#include "QMeshInteractor.h"
#include "QLightInteractor.h"
#include "QParticlesInteractor.h"
#include "QMapMarkInteractor.h"

#include "QTBEngine.h"

#include "ui_interface.h"

#define backupOf(filename) \
        QString(filename).replace(QRegExp("\\.(.+)$"), ".bak")

MainWindow::MainWindow()
{
    notifyChange(false);

    m_uinterface = new Ui_mainWindow;
    m_rootNode = NULL;
}

MainWindow::~MainWindow()
{
    delete m_uinterface;
    delete m_rootNode;

    if(!m_filename.isEmpty())
        QFile::remove(backupOf(m_filename));
}

QString MainWindow::openFileName() const
{
    return m_filename;
}

Ui_mainWindow* MainWindow::ui()
{
    return m_uinterface;
}

void MainWindow::registerInteractor(QNodeInteractor* node, QItemsList& items)
{
    QNodeInteractor* parent = node->getParent();

    if(nodeItemBinder.count(parent))
        nodeItemBinder[parent]->appendRow(items);
    else
        nodesGui.nodesListModel->appendRow(items);

    nodeItemBinder[node] = items[0];

    m_tbeWidget->registerInteractor(node);
}

void unregisterNodeRecursive(QInterfaceItemsMap& nodeItemBinder, QStandardItem* item)
{
    int count = item->rowCount();

    for(int i = 0; i < count; i++)
    {
        QStandardItem* subitem = item->child(i);

        unregisterNodeRecursive(nodeItemBinder, subitem);

        QNodeInteractor* interface = subitem->data(ITEM_ROLE_NODE).value<QNodeInteractor*>();
        nodeItemBinder.remove(interface);
    }
}

void MainWindow::unregisterInteractor(QNodeInteractor* node)
{
    if(nodeItemBinder.count(node))
    {
        QStandardItem* item = nodeItemBinder[node];

        unregisterNodeRecursive(nodeItemBinder, item);

        QModelIndex sindex = nodesGui.nodesListModel->indexFromItem(item);

        nodesGui.nodesListProxyModel->blockSignals(true);

        if(sindex.parent().isValid())
            nodesGui.nodesListModel->removeRow(sindex.row(), sindex.parent());
        else
            nodesGui.nodesListModel->removeRow(sindex.row());

        nodesGui.nodesListProxyModel->blockSignals(false);

        nodeItemBinder.remove(node);
    }

    m_tbeWidget->unregisterInteractor(node);
}

void MainWindow::openSceneFromHistory()
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
            QAction* act = filehistory->addAction("..." + filepath.section(QDir::separator(), -2),
                                                  this, SLOT(openSceneFromHistory()));
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

    {
        QFile ss("style.css");

        if(ss.open(QIODevice::ReadOnly))
            setStyleSheet(ss.readAll());

        ss.close();
    }

    m_tbeWidget = m_uinterface->glwidget;

    nodesGui.attribTab = m_uinterface->attribTab;

    m_config = new QSettings(this);

    m_packerDialog = new PackerDialog(this);

    buildFileHistory();

    m_uinterface->actionUndo->setEnabled(false);

    m_uinterface->baseAttribTab->setEnabled(false);
    m_uinterface->attribTab->setEnabled(false);

    m_uinterface->node_list_sort->setMenu(m_uinterface->menuTrier);

    QToolBar* toptoolbar = new QToolBar;
    toptoolbar->setFloatable(false);
    toptoolbar->setMovable(false);
    toptoolbar->setIconSize(QSize(16, 16));
    toptoolbar->addAction(m_uinterface->actionNewScene);
    toptoolbar->addAction(m_uinterface->actionOpen);
    toptoolbar->addAction(m_uinterface->actionSave);
    toptoolbar->addSeparator();
    toptoolbar->addAction(m_uinterface->actionToggleSelBox);
    toptoolbar->addAction(m_uinterface->actionToggleStaticView);
    toptoolbar->addSeparator();
    toptoolbar->addAction(m_uinterface->actionNewMesh);
    toptoolbar->addAction(m_uinterface->actionNewLight);
    toptoolbar->addAction(m_uinterface->actionNewParticles);
    toptoolbar->addAction(m_uinterface->actionNewMapMark);
    toptoolbar->addSeparator();
    toptoolbar->addAction(m_uinterface->actionCloneNode);
    toptoolbar->addAction(m_uinterface->actionDeleteNode);
    toptoolbar->addSeparator();
    toptoolbar->addAction(m_uinterface->actionBaseOnFloor);
    toptoolbar->addAction(m_uinterface->actionCenterOnFloor);
    toptoolbar->addAction(m_uinterface->actionToggleGrid);
    toptoolbar->addSeparator();
    toptoolbar->addAction(m_uinterface->actionSelectionTool);
    toptoolbar->addAction(m_uinterface->actionRotateTool);
    toptoolbar->addAction(m_uinterface->actionScaleTool);

    addToolBar(Qt::TopToolBarArea, toptoolbar);

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
    genGui.information = m_uinterface->gen_information;

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

    nodesGui.name = m_uinterface->node_name;
    nodesGui.position = new QDoubleVector3Box(this, m_uinterface->node_pos_x, m_uinterface->node_pos_y, m_uinterface->node_pos_z);
    nodesGui.rotation = new QDoubleVector3Box(this, m_uinterface->node_rot_x, m_uinterface->node_rot_y, m_uinterface->node_rot_z);
    nodesGui.scale = new QDoubleVector3Box(this, m_uinterface->node_scl_x, m_uinterface->node_scl_y, m_uinterface->node_scl_z);

    nodesGui.lock = m_uinterface->node_lock;
    nodesGui.enable = m_uinterface->node_enable;

    nodesGui.displayMesh = m_uinterface->node_display_meshs;
    nodesGui.displayLights = m_uinterface->node_display_lights;
    nodesGui.displayParticles = m_uinterface->node_display_particules;
    nodesGui.displayMarks = m_uinterface->node_display_marks;

    // -------- Mesh

    nodesGui.mesh.editMaterial = m_uinterface->node_mesh_editmat;
    nodesGui.mesh.reloadMaterial = m_uinterface->node_mesh_reloadmat;
    nodesGui.mesh.attachMaterial = m_uinterface->node_mesh_addmat;
    nodesGui.mesh.releaseMaterial = m_uinterface->node_mesh_relmat;
    nodesGui.mesh.matinfo = m_uinterface->node_mesh_matinfo;

    nodesGui.mesh.billboardX = m_uinterface->node_mesh_billboard_x;
    nodesGui.mesh.billboardY = m_uinterface->node_mesh_billboard_y;

    nodesGui.mesh.castshadow = m_uinterface->node_mesh_castshadow;
    nodesGui.mesh.receiveshadow = m_uinterface->node_mesh_receiveshadow;
    nodesGui.mesh.computeNormal = m_uinterface->node_mesh_cnormal;
    nodesGui.mesh.computeTangent = m_uinterface->node_mesh_ctangent;

    // -------- Particles

    nodesGui.particles.gravity = new QDoubleVector3Box(this, m_uinterface->node_particles_gravity_x, m_uinterface->node_particles_gravity_y, m_uinterface->node_particles_gravity_z);
    nodesGui.particles.boxsize = new QDoubleVector3Box(this, m_uinterface->node_particles_boxsize_x, m_uinterface->node_particles_boxsize_y, m_uinterface->node_particles_boxsize_z);
    nodesGui.particles.bulletsize = new QDoubleVector2Box(this, m_uinterface->node_particles_bulletsize_x, m_uinterface->node_particles_bulletsize_y);
    nodesGui.particles.freemove = m_uinterface->node_particles_freemove;
    nodesGui.particles.lifeinit = m_uinterface->node_particles_lifeinit;
    nodesGui.particles.lifedown = m_uinterface->node_particles_lifedown;
    nodesGui.particles.number = m_uinterface->node_particles_number;
    nodesGui.particles.texture = new QBrowsEdit(this, m_uinterface->node_particles_texture, m_uinterface->node_particles_texture_browse);
    nodesGui.particles.continiousmode = m_uinterface->node_particles_continousmode;
    nodesGui.particles.pointsprite = m_uinterface->node_particles_pointsprite;
    nodesGui.particles.build = m_uinterface->node_particles_build;

    // -------- Lights

    nodesGui.light.type = m_uinterface->node_light_type;

    nodesGui.light.ambiant = new QDoubleVector3Box(this, m_uinterface->node_light_ambiant_x, m_uinterface->node_light_ambiant_y, m_uinterface->node_light_ambiant_z);
    nodesGui.light.diffuse = new QDoubleVector3Box(this, m_uinterface->node_light_diffuse_x, m_uinterface->node_light_diffuse_y, m_uinterface->node_light_diffuse_z);
    nodesGui.light.specular = new QDoubleVector3Box(this, m_uinterface->node_light_specular_x, m_uinterface->node_light_specular_y, m_uinterface->node_light_specular_z);
    nodesGui.light.castshadow = m_uinterface->node_light_castshadow;

    nodesGui.light.radius = m_uinterface->node_light_radius;

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
    nodesGui.nodesListView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Environment -------------------------------------------------------------

    envGui.sceneAmbiant = new QDoubleVector3Box(this, m_uinterface->env_ambient_x, m_uinterface->env_ambient_y, m_uinterface->env_ambient_z);

    envGui.skybox.apply = m_uinterface->skybox_apply;
    envGui.skybox.enable = m_uinterface->skybox_enable;
    envGui.skybox.browse = m_uinterface->skybox_browse;
    envGui.skybox.list = m_uinterface->skybox_list;
    envGui.skybox.up = m_uinterface->skybox_up;
    envGui.skybox.down = m_uinterface->skybox_down;

    envGui.fog.color = new QDoubleVector3Box(this, m_uinterface->fog_x, m_uinterface->fog_y, m_uinterface->fog_z);
    envGui.fog.start = m_uinterface->fog_start;
    envGui.fog.end = m_uinterface->fog_end;
    envGui.fog.enable = m_uinterface->fog_enable;

    envGui.shadow.enable = m_uinterface->env_shadow_enable;
    envGui.shadow.size = m_uinterface->env_shadow_size;
    envGui.shadow.blur = m_uinterface->env_shadow_blur;
    envGui.shadow.intentsity = m_uinterface->env_shadow_intensity;
    envGui.shadow.shader = m_uinterface->env_shadow_shader;

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
    connect(m_uinterface->actionAbout, SIGNAL(triggered()), this, SLOT(aboutDialog()));
    connect(m_uinterface->actionToggleFullScreen, SIGNAL(triggered(bool)), this, SLOT(toggleFullWidget(bool)));
    connect(m_uinterface->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(m_uinterface->actionScreenShot, SIGNAL(triggered()), this, SLOT(takeScreenshot()));

    // Edit Menu

    connect(m_uinterface->actionBaseOnFloor, SIGNAL(triggered()), m_tbeWidget, SLOT(baseOnFloor()));
    connect(m_uinterface->actionCenterOnFloor, SIGNAL(triggered()), m_tbeWidget, SLOT(centerOnFloor()));
    connect(m_uinterface->actionCloneNode, SIGNAL(triggered()), m_tbeWidget, SLOT(cloneSelected()));
    connect(m_uinterface->actionDeleteNode, SIGNAL(triggered()), m_tbeWidget, SLOT(deleteSelected()));

    connect(m_uinterface->actionToggleGrid, SIGNAL(toggled(bool)), m_tbeWidget, SLOT(toggleGrid(bool)));
    connect(m_uinterface->actionToggleSelBox, SIGNAL(toggled(bool)), m_tbeWidget, SLOT(toggleSelBox(bool)));
    connect(m_uinterface->actionToggleStaticView, SIGNAL(toggled(bool)), m_tbeWidget, SLOT(toggleStaticView(bool)));

    connect(m_uinterface->actionUndo, SIGNAL(triggered()), m_tbeWidget, SLOT(popHistoryStat()));

    connect(m_uinterface->actionNewMesh, SIGNAL(triggered()), this, SLOT(guiMeshNew()));
    connect(m_uinterface->actionNewLight, SIGNAL(triggered()), this, SLOT(guiLightNew()));
    connect(m_uinterface->actionNewParticles, SIGNAL(triggered()), this, SLOT(guiParticlesNew()));
    connect(m_uinterface->actionNewMapMark, SIGNAL(triggered()), this, SLOT(guiMarkNew()));

    connect(m_uinterface->actionOpenPacker, SIGNAL(triggered()), m_packerDialog, SLOT(exec()));

    connect(this, SIGNAL(pauseRendring()), m_tbeWidget, SLOT(pauseRendring()));
    connect(this, SIGNAL(resumeRendring()), m_tbeWidget, SLOT(resumeRendring()));

    connect(nodesGui.displayMesh, SIGNAL(clicked(bool)), nodesGui.nodesListProxyModel, SLOT(toggleMesh(bool)));
    connect(nodesGui.displayLights, SIGNAL(clicked(bool)), nodesGui.nodesListProxyModel, SLOT(toggleLight(bool)));
    connect(nodesGui.displayParticles, SIGNAL(clicked(bool)), nodesGui.nodesListProxyModel, SLOT(toggleParticles(bool)));
    connect(nodesGui.displayMarks, SIGNAL(clicked(bool)), nodesGui.nodesListProxyModel, SLOT(toggleMapMark(bool)));

    connect(m_uinterface->actionSortByName, SIGNAL(triggered()), nodesGui.nodesListProxyModel, SLOT(sortByName()));
    connect(m_uinterface->actionSortByType, SIGNAL(triggered()), nodesGui.nodesListProxyModel, SLOT(sortByType()));
    connect(m_uinterface->actionSortFromCamera, SIGNAL(triggered()), nodesGui.nodesListProxyModel, SLOT(sortFromCamera()));
    connect(m_uinterface->actionSortFromSelection, SIGNAL(triggered()), nodesGui.nodesListProxyModel, SLOT(sortFromSelection()));

    connect(m_tbeWidget, SIGNAL(selection(QNodeInteractor*)), this, SLOT(select(QNodeInteractor*)));
    connect(m_tbeWidget, SIGNAL(deselection(QNodeInteractor*)), this, SLOT(deselect(QNodeInteractor*)));
    connect(m_tbeWidget, SIGNAL(deselectionAll()), this, SLOT(deselectAll()));
    connect(m_tbeWidget, SIGNAL(notifyChange()), this, SLOT(notifyChange()));

    connect(nodesGui.nodesListView, SIGNAL(select(QNodeInteractor*)), this, SLOT(select(QNodeInteractor*)));
    connect(nodesGui.nodesListView, SIGNAL(deselect(QNodeInteractor*)), this, SLOT(deselect(QNodeInteractor*)));
    connect(nodesGui.nodesListView, SIGNAL(deselectAll()), this, SLOT(deselectAll()));

    connect(nodesGui.nodesListView, SIGNAL(assignParent()), this, SLOT(assignParent()));
    connect(nodesGui.nodesListView, SIGNAL(promoteChild()), this, SLOT(promoteChild()));
    connect(nodesGui.nodesListView, SIGNAL(pastPosition()), this, SLOT(pastPosition()));
    connect(nodesGui.nodesListView, SIGNAL(pastScale()), this, SLOT(pastScale()));
    connect(nodesGui.nodesListView, SIGNAL(pastRotation()), this, SLOT(pastRotation()));
    connect(nodesGui.nodesListView, SIGNAL(pastFields()), this, SLOT(pastFields()));
    connect(nodesGui.nodesListView, SIGNAL(pastMaterials()), this, SLOT(pastMaterials()));
    connect(nodesGui.nodesListView, SIGNAL(removeNode()), m_tbeWidget, SLOT(deleteSelected()));
    connect(nodesGui.nodesListView, SIGNAL(setOnFloorNode()), m_tbeWidget, SLOT(baseOnFloor()));

    connect(envGui.sceneAmbiant, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(guiAmbient(const tbe::Vector3f&)));

    connect(envGui.znear, SIGNAL(valueChanged(double)), this, SLOT(guiZNear(double)));
    connect(envGui.zfar, SIGNAL(valueChanged(double)), this, SLOT(guiZFar(double)));

    connect(envGui.fog.enable, SIGNAL(clicked(bool)), this, SLOT(guiFogEnable(bool)));
    connect(envGui.fog.color, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(guiFogChange()));
    connect(envGui.fog.start, SIGNAL(valueChanged(double)), this, SLOT(guiFogChange()));
    connect(envGui.fog.end, SIGNAL(valueChanged(double)), this, SLOT(guiFogChange()));

    connect(envGui.skybox.enable, SIGNAL(clicked(bool)), this, SLOT(guiSkyboxEnable(bool)));
    connect(envGui.skybox.apply, SIGNAL(clicked()), this, SLOT(guiSkyboxChange()));
    connect(envGui.skybox.browse, SIGNAL(clicked()), this, SLOT(guiSkyboxBrowse()));
    connect(envGui.skybox.up, SIGNAL(clicked()), this, SLOT(guiSkyboxShift()));
    connect(envGui.skybox.down, SIGNAL(clicked()), this, SLOT(guiSkyboxShift()));

    connect(envGui.shadow.enable, SIGNAL(clicked(bool)), m_tbeWidget, SLOT(setShadowEnable(bool)));
    connect(envGui.shadow.size, SIGNAL(valueChanged(int)), m_tbeWidget, SLOT(setShadowSize(int)));
    connect(envGui.shadow.blur, SIGNAL(valueChanged(int)), m_tbeWidget, SLOT(setShadowBlur(int)));
    connect(envGui.shadow.intentsity, SIGNAL(valueChanged(double)), m_tbeWidget, SLOT(setShadowIntensity(double)));
    connect(envGui.shadow.shader, SIGNAL(clicked(bool)), m_tbeWidget, SLOT(setShadowShader(bool)));

    // Tools Menu

    QSignalMapper* toolsigmap = new QSignalMapper(this);
    toolsigmap->setMapping(m_uinterface->actionSelectionTool, SELECTION_TOOL);
    toolsigmap->setMapping(m_uinterface->actionRotateTool, ROTATE_TOOL);
    toolsigmap->setMapping(m_uinterface->actionScaleTool, SCALE_TOOL);

    connect(toolsigmap, SIGNAL(mapped(int)), this, SLOT(setCurrentTool(int)));

    connect(m_uinterface->actionSelectionTool, SIGNAL(triggered()), toolsigmap, SLOT(map()));
    connect(m_uinterface->actionRotateTool, SIGNAL(triggered()), toolsigmap, SLOT(map()));
    connect(m_uinterface->actionScaleTool, SIGNAL(triggered()), toolsigmap, SLOT(map()));
}

bool MainWindow::leaveSafely()
{
    if(m_somethingChange)
    {
        QMessageBox::StandardButton answer =
                QMessageBox::warning(this, "Enregistrer ?",
                                     "La scene à été modifier\nVous-les vous enregistrer avant de quitter ?",
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

void MainWindow::updateGui()
{
    nodesGui.particles.texture->setWorkDir(m_filename);

    // -------------------------------------------------------------------------

    using namespace tbe;
    using namespace scene;

    SceneManager* sceneManager = m_tbeWidget->rootNode()->getSceneManager();

    envGui.zfar->setValue(sceneManager->getZFar());
    envGui.znear->setValue(sceneManager->getZNear());

    ShadowMap* smap = sceneManager->getShadowMap();

    if(smap->isEnabled())
    {
        QSignalBlocker blocker;
        blocker << envGui.shadow.enable << envGui.shadow.size << envGui.shadow.blur;
        blocker.block();

        envGui.shadow.enable->setChecked(smap->isEnabled());
        envGui.shadow.size->setValue(smap->getFrameSize().x);
        envGui.shadow.blur->setValue(smap->getBlurPass());
        envGui.shadow.intentsity->setValue(smap->getIntensity());
        envGui.shadow.shader->setChecked(smap->isShaderHandled());

        blocker.unblock();
    }

    Fog* fog = sceneManager->getFog();

    if(fog->isEnable())
    {
        QSignalBlocker blocker;
        blocker << envGui.fog.enable << envGui.fog.color << envGui.fog.start << envGui.fog.end;
        blocker.block();

        envGui.fog.enable->setChecked(fog->isEnable());
        envGui.fog.color->setValue(math::vec43(fog->getColor()));
        envGui.fog.start->setValue(fog->getStart());
        envGui.fog.end->setValue(fog->getEnd());

        blocker.unblock();
    }

    SkyBox* sky = sceneManager->getSkybox();

    if(sky->isEnable())
    {
        QSignalBlocker blocker;
        blocker << envGui.skybox.apply << envGui.skybox.enable;
        blocker.block();

        blocker.block();

        tbe::Texture* texs = sky->getTextures();

        envGui.skybox.list->clear();

        QMap<QString, QString> skymap;
        skymap["1:Devant"] = QString::fromStdString(texs[0].getFilename());
        skymap["2:Dèrrier"] = QString::fromStdString(texs[1].getFilename());
        skymap["3:Haut"] = QString::fromStdString(texs[2].getFilename());
        skymap["4:Bas"] = QString::fromStdString(texs[3].getFilename());
        skymap["5:Gauche"] = QString::fromStdString(texs[4].getFilename());
        skymap["6:Droite"] = QString::fromStdString(texs[5].getFilename());

        foreach(QString k, skymap.keys())
        {
            QString path = skymap.value(k);

            QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << k << QFileInfo(path).baseName());
            item->setData(1, Qt::UserRole, path);

            envGui.skybox.list->addTopLevelItem(item);
        }

        envGui.skybox.enable->setChecked(sky->isEnable());

        blocker.unblock();
    }

    {
        envGui.sceneAmbiant->blockSignals(true);
        envGui.sceneAmbiant->setValue(math::vec43(sceneManager->getAmbientLight()));
        envGui.sceneAmbiant->blockSignals(false);
    }

    SceneParser* sceneParser = m_tbeWidget->sceneParser();

    genGui.title->setText(QString::fromStdString(sceneParser->getSceneName()));
    genGui.author->setText(QString::fromStdString(sceneParser->getAuthorName()));

    genGui.additionalModel->removeRows(0, genGui.additionalModel->rowCount());

    tbe::rtree& attributes = sceneParser->attributes();

    BOOST_FOREACH(tbe::rtree::value_type& it, attributes)
    {
        QStandardItem* key = new QStandardItem;
        key->setText(QString::fromStdString(it.first));

        QStandardItem* value = new QStandardItem;
        value->setText(QString::fromStdString(it.second.data()));

        genGui.additionalModel->appendRow(QList<QStandardItem*> () << key << value);
    }
}

void MainWindow::newScene()
{
    if(!leaveSafely())
        return;

    if(!m_filename.isEmpty() && QFile::exists(backupOf(m_filename)))
        QFile::remove(backupOf(m_filename));

    deselectAll();

    m_tbeWidget->clearScene();

    m_filename.clear();

    nodesGui.nodesListModel->removeRows(0, nodesGui.nodesListModel->rowCount());

    genGui.title->clear();
    genGui.author->clear();
    genGui.additionalModel->removeRows(0, genGui.additionalModel->rowCount());

    if(!m_rootNode)
    {
        m_rootNode = new QRootInteractor(this, m_tbeWidget->rootNode());

        QVariant rootData;
        rootData.setValue(m_rootNode);

        nodesGui.nodesListModel->invisibleRootItem()->setData(rootData, ITEM_ROLE_NODE);
        nodesGui.nodesListModel->invisibleRootItem()->setData("Root", ITEM_ROLE_NAME);
    }

    setCurrentTool(SELECTION_TOOL);

    updateGui();

    notifyChange(false);
}

void MainWindow::openSceneDialog()
{
    m_tbeWidget->pauseRendring();

    QString filename = QFileDialog::getOpenFileName(this, "Editer une scene");

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
        if(!m_filename.isEmpty() && QFile::exists(backupOf(m_filename)))
            QFile::remove(backupOf(m_filename));

        m_tbeWidget->clearScene();

        m_filename.clear();

        nodesGui.nodesListModel->removeRows(0, nodesGui.nodesListModel->rowCount());

        QString backupfile = backupOf(filename);

        if(QFile::exists(backupfile))
        {
            QMessageBox::StandardButton response =
                    QMessageBox::question(this, "Ficher de savegarde trouvé",
                                          "Un fichier de savegarde a été trouvé probablement du à un plantage de l'application\n"
                                          "vous-les vous le charger ?", QMessageBox::Yes | QMessageBox::No);

            if(response == QMessageBox::Yes)
                m_tbeWidget->loadScene(backupfile);
            else
            {
                QFile::remove(backupfile);
                m_tbeWidget->loadScene(filename);
            }
        }

        else
            m_tbeWidget->loadScene(filename);

        pushFileHistory(filename);

        m_filename = filename;

        m_uinterface->actionToggleSelBox->setChecked(true);
        m_uinterface->actionToggleStaticView->setChecked(true);

        updateGui();

        notifyChange(false);
    }

    catch(std::exception& e)
    {
        QMessageBox::critical(this, "Erreur de chargement", e.what());

        notifyChange(false);
        newScene();
    }
}

void MainWindow::saveSceneDialog()
{
    QString filename = QFileDialog::getSaveFileName(this, "", m_filename);

    if(!filename.isNull())
    {
        saveScene(filename);
        pushFileHistory(filename);
        notifyChange(false);

        m_filename = filename;

        nodesGui.particles.texture->setWorkDir(m_filename);
    }
}

void MainWindow::saveScene()
{
    if(m_filename.isEmpty())
        saveSceneDialog();
    else
        saveScene(m_filename);
}

void MainWindow::outputScene(const QString& filename)
{
    tbe::scene::SceneParser* sceneParser = m_tbeWidget->sceneParser();

    sceneParser->setSceneName(genGui.title->text().toStdString());
    sceneParser->setAuthorName(genGui.author->text().toStdString());

    sceneParser->attributes().clear();

    int count = genGui.additionalModel->rowCount();
    for(int i = 0; i < count; i++)
    {
        std::string key = genGui.additionalModel->item(i, 0)->text().toStdString();
        std::string value = genGui.additionalModel->item(i, 1)->text().toStdString();

        sceneParser->attributes().put(key, value);
    }

    m_tbeWidget->saveScene(filename);
}

void MainWindow::saveScene(const QString& filename)
{
    outputScene(filename);

    notifyChange(false);

    statusBar()->showMessage("Scene enregistrer...", 2000);
}

void MainWindow::saveBackup()
{
    if(!m_filename.isEmpty())
        outputScene(backupOf(m_filename));
}

void MainWindow::setCurrentTool(int type)
{
    m_uinterface->actionSelectionTool->setChecked(false);
    m_uinterface->actionRotateTool->setChecked(false);
    m_uinterface->actionScaleTool->setChecked(false);

    switch(type)
    {
        case SELECTION_TOOL:
            m_uinterface->actionSelectionTool->setChecked(true);
            m_tbeWidget->selectSelectionTool();
            break;

        case SCALE_TOOL:
            m_uinterface->actionScaleTool->setChecked(true);
            m_tbeWidget->selectScaleTool();
            m_tbeWidget->setFocus();
            break;

        case ROTATE_TOOL:
            m_uinterface->actionRotateTool->setChecked(true);
            m_tbeWidget->selectRotateTool();
            m_tbeWidget->setFocus();
            break;
    }
}

void MainWindow::notifyChange(bool stat)
{
    m_somethingChange = stat;

    if(m_somethingChange)
        saveBackup();

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

void MainWindow::aboutDialog()
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

void MainWindow::guiMeshNew()
{
    m_tbeWidget->pauseRendring();

    QString filename = QFileDialog::getOpenFileName(parentWidget(), QString(), m_filename, "Wavefrot Obj (*.obj);;Tout les fichiers (*.*)");

    if(!filename.isNull())
    {
        try
        {
            QMeshInteractor* mesh = m_tbeWidget->newMesh(filename);

            deselectAll();
            select(mesh);

            notifyChange(true);
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
        QLightInteractor* light = m_tbeWidget->newLight();

        deselectAll();
        select(light);

        notifyChange(true);
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
        QParticlesInteractor* particles = m_tbeWidget->newParticles();

        deselectAll();
        select(particles);

        notifyChange(true);
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
        QMapMarkInteractor* mark = m_tbeWidget->newMark();

        deselectAll();
        select(mark);

        notifyChange(true);
    }
    catch(std::exception& e)
    {
        QMessageBox::warning(parentWidget(), "Erreur", e.what());
    }
}

void MainWindow::select(QNodeInteractor* qnode)
{
    m_tbeWidget->selectNode(qnode);

    if(m_tbeWidget->selection().length() > 0)
    {
        nodesGui.name->setEnabled(true);
        nodesGui.position->setEnabled(true);
        nodesGui.rotation->setEnabled(true);
        nodesGui.scale->setEnabled(true);
        nodesGui.addField->setEnabled(true);
        nodesGui.delField->setEnabled(true);
        nodesGui.clearFields->setEnabled(true);
    }

    m_uinterface->baseAttribTab->setEnabled(true);
    m_uinterface->attribTab->setEnabled(true);

    m_uinterface->actionCloneNode->setEnabled(true);
    m_uinterface->actionDeleteNode->setEnabled(true);
}

void MainWindow::deselect(QNodeInteractor* qnode)
{
    m_tbeWidget->deselectNode(qnode);

    if(m_tbeWidget->selection().empty() == 0)
    {
        nodesGui.name->setEnabled(false);
        nodesGui.position->setEnabled(false);
        nodesGui.rotation->setEnabled(false);
        nodesGui.scale->setEnabled(false);
        nodesGui.addField->setEnabled(false);
        nodesGui.delField->setEnabled(false);
        nodesGui.clearFields->setEnabled(false);
    }
}

void MainWindow::deselectAll()
{
    nodesGui.nodesListView->clearSelection();

    m_tbeWidget->deselectAllNode();

    m_uinterface->baseAttribTab->setEnabled(false);
    m_uinterface->attribTab->setEnabled(false);

    m_uinterface->actionCloneNode->setEnabled(false);
    m_uinterface->actionDeleteNode->setEnabled(false);

    nodesGui.name->setEnabled(false);
    nodesGui.position->setEnabled(false);
    nodesGui.rotation->setEnabled(false);
    nodesGui.scale->setEnabled(false);
    nodesGui.addField->setEnabled(false);
    nodesGui.delField->setEnabled(false);
    nodesGui.clearFields->setEnabled(false);
}

void MainWindow::promoteChild()
{
    using namespace tbe;
    using namespace scene;

    QList<QNodeInteractor*> childs = m_tbeWidget->selection();

    deselectAll();

    foreach(QNodeInteractor* child, childs)
    {
        if(child->target()->getParent() == m_rootNode->target())
        {
            statusBar()->showMessage(QString("Le noeud %1 ne peut Ãªtre promue")
                                     .arg(child->target()->getName().c_str()), 2000);
            continue;
        }

        QStandardItem* ichild = child->item();

        QStandardItem* ihost = ichild->parent()->parent() ? ichild->parent()->parent() : nodesGui.nodesListModel->invisibleRootItem();

        QStandardItem* iparent = ichild->parent() ? ichild->parent() : nodesGui.nodesListModel->invisibleRootItem();

        if(iparent != ihost)
        {
            QItemsList row = ichild->parent()->takeRow(ichild->row());

            ihost->insertRow(iparent->row() + 1, row);
        }

        Node* hostNode = ihost->data(ITEM_ROLE_NODE).value<QNodeInteractor*>()->target();
        Node* currNode = ichild->data(ITEM_ROLE_NODE).value<QNodeInteractor*>()->target();

        currNode->setPos(currNode->getParent()->getAbsoluteMatrix().getPos() + currNode->getPos());
        currNode->setParent(hostNode);

        select(m_tbeWidget->getInteractor(currNode));
    }

    m_tbeWidget->placeCamera();

    notifyChange(true);

    statusBar()->showMessage("Enfants promue", 2000);
}

void MainWindow::assignParent()
{
    using namespace tbe;
    using namespace scene;

    QList<QNodeInteractor*> childs = m_tbeWidget->selection();
    QNodeInteractor* parent = childs.back();
    childs.pop_back();

    QStandardItem* iparent = parent->item();
    Node* nparent = parent->target();

    foreach(QNodeInteractor* child, childs)
    {
        QStandardItem* ichild = child->item();

        QItemsList row = (ichild->parent() ? ichild->parent() : nodesGui.nodesListModel->invisibleRootItem())->takeRow(ichild->row());
        iparent->appendRow(row);

        Node* nchild = child->target();

        nchild->setParent(nparent);
        nchild->setPos(nchild->getPos() - nparent->getPos());
    }

    m_tbeWidget->placeCamera();

    notifyChange(true);

    statusBar()->showMessage("Parent assigné", 2000);
}

void MainWindow::guiSkyboxEnable(bool enable)
{
    m_tbeWidget->setSkybox(enable);

    notifyChange(true);
}

void MainWindow::guiSkyboxBrowse()
{
    QStringList files = QFileDialog::getOpenFileNames(this);

    if(files.size() != 6)
    {
        QMessageBox::warning(this, "Skybox", "Vous devez ajouter 6 fichiers !");
        return;
    }

    envGui.skybox.list->clear();

    QTreeWidgetItem* item;

    item = new QTreeWidgetItem(QStringList() << "1:Devant" << QFileInfo(files[0]).baseName());
    item->setData(1, Qt::UserRole, files[0]);
    envGui.skybox.list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "2:Dèrriere" << QFileInfo(files[1]).baseName());
    item->setData(1, Qt::UserRole, files[1]);
    envGui.skybox.list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "3:Haut" << QFileInfo(files[2]).baseName());
    item->setData(1, Qt::UserRole, files[2]);
    envGui.skybox.list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "4:Bas" << QFileInfo(files[3]).baseName());
    item->setData(1, Qt::UserRole, files[3]);
    envGui.skybox.list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "5:Gauche" << QFileInfo(files[4]).baseName());
    item->setData(1, Qt::UserRole, files[4]);
    envGui.skybox.list->addTopLevelItem(item);

    item = new QTreeWidgetItem(QStringList() << "6:Droite" << QFileInfo(files[5]).baseName());
    item->setData(1, Qt::UserRole, files[5]);
    envGui.skybox.list->addTopLevelItem(item);
}

void MainWindow::guiSkyboxShift()
{
    if(envGui.skybox.list->selectedItems().empty())
        return;

    QTreeWidgetItem *itemSrc, *itemDst;

    itemSrc = envGui.skybox.list->selectedItems().front();

    if(sender() == envGui.skybox.up)
        itemDst = envGui.skybox.list->itemAbove(itemSrc);

    else if(sender() == envGui.skybox.down)
        itemDst = envGui.skybox.list->itemBelow(itemSrc);

    QString src = itemSrc->data(1, Qt::UserRole).toString();
    QString dst = itemDst->data(1, Qt::UserRole).toString();

    itemSrc->setData(1, Qt::UserRole, dst);
    itemSrc->setText(1, QFileInfo(dst).baseName());

    itemDst->setData(1, Qt::UserRole, src);
    itemDst->setText(1, QFileInfo(src).baseName());

    envGui.skybox.list->setCurrentItem(itemDst);
}

void MainWindow::guiSkyboxChange()
{
    if(envGui.skybox.list->topLevelItemCount() != 6)
        return;

    QStringList texs;

    for(unsigned i = 0; i < 6; i++)
        texs << envGui.skybox.list->topLevelItem(i)->data(1, Qt::UserRole).toString();

    try
    {
        m_tbeWidget->setSkybox(texs);
        m_tbeWidget->setSkybox(true);
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(this, "Erreur: Skybox", e.what());
    }

    notifyChange(true);
}

void MainWindow::guiFogEnable(bool enable)
{
    m_tbeWidget->setFog(enable);


    notifyChange(true);
}

void MainWindow::guiFogChange()
{
    m_tbeWidget->setFog(tbe::math::vec34(envGui.fog.color->value()), envGui.fog.start->value(), envGui.fog.end->value());

    notifyChange(true);
}

void MainWindow::guiZNear(double value)
{
    m_tbeWidget->setZNear(value);
}

void MainWindow::guiZFar(double value)
{
    m_tbeWidget->setZFar(value);
}

QTBEngine* MainWindow::tbeWidget() const
{
    return m_tbeWidget;
}

void MainWindow::guiAmbient(const tbe::Vector3f& value)
{
    m_tbeWidget->setSceneAmbiant(value);

    notifyChange(true);
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

    notifyChange(true);
}

void MainWindow::guiDelSceneField()
{
    QModelIndex i = genGui.additionalView->currentIndex();

    if(i.isValid())
        genGui.additionalModel->removeRow(i.row());

    notifyChange(true);
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
        notifyChange(true);
    }
}

void MainWindow::takeScreenshot()
{
    QImage shot = m_tbeWidget->grabFrameBuffer(false);

    QString name = "map";

    if(!genGui.title->text().isEmpty())
        name = genGui.title->text();

    else if(!m_filename.isEmpty())
        name = QFileInfo(m_filename).baseName();

    QString title = QString("%1-%2.png")
            .arg(QString::fromStdString(tbe::tools::unixName(name.toStdString())))
            .arg(QDate::currentDate().toString("yyyyMMdd"));

    QString defname = QFileInfo(m_filename).dir().filePath(title);

    QString filename = QFileDialog::getSaveFileName(this, "Enregistrer la capture d'écran", defname);

    if(!filename.isEmpty())
    {
        if(!filename.endsWith(".png"))
            filename += ".png";

        QFile output(filename);
        output.open(QIODevice::WriteOnly);
        shot.save(&output, "PNG");
    }
}

void MainWindow::pastMaterials()
{
    QList<QNodeInteractor*> childs = m_tbeWidget->selection();
    QNodeInteractor* selectedNode = childs.first();
    childs.pop_front();

    if(!selectedNode || selectedNode->typeName() != "Mesh")
        return;

    tbe::scene::Mesh* sourceMesh = dynamic_cast<tbe::scene::Mesh*> (selectedNode->target());

    foreach(QNodeInteractor* child, childs)
    {
        m_tbeWidget->pushHistoryStat(new ModificationState(child));

        if(child->typeName() == "Mesh")
        {
            tbe::scene::Mesh* mesh = dynamic_cast<tbe::scene::Mesh*> (child->target());
            mesh->fetchMaterials(*sourceMesh);
        }
    }

    statusBar()->showMessage("Materieaux coller...", 2000);
}

void MainWindow::pastFields()
{
    QList<QNodeInteractor*> childs = m_tbeWidget->selection();
    QNodeInteractor* selectedNode = childs.first();
    childs.pop_front();

    if(!selectedNode)
        return;

    const tbe::Any::Map& ud = selectedNode->target()->getUserDatas();

    foreach(QNodeInteractor* child, childs)
    {
        m_tbeWidget->pushHistoryStat(new ModificationState(child));

        tbe::scene::Node* nchild = child->target();

        nchild->clearUserData();

        foreach(tbe::Any::Map::value_type i, ud)
        {
            nchild->setUserData(i.first, i.second);
        }
    }

    statusBar()->showMessage("Champs coller...", 2000);
}

void MainWindow::pastPosition()
{
    QList<QNodeInteractor*> childs = m_tbeWidget->selection();
    QNodeInteractor* selectedNode = childs.first();
    childs.pop_front();

    if(!selectedNode)
        return;

    foreach(QNodeInteractor* child, childs)
    {
        m_tbeWidget->pushHistoryStat(new ModificationState(child));

        child->setPos(selectedNode->target()->getPos());
    }

    statusBar()->showMessage("Position coller...", 2000);
}

void MainWindow::pastScale()
{
    QList<QNodeInteractor*> childs = m_tbeWidget->selection();
    QNodeInteractor* selectedNode = childs.first();
    childs.pop_front();

    if(!selectedNode)
        return;

    tbe::Vector3f position, scale;
    tbe::Quaternion rotation;

    selectedNode->target()->getMatrix().decompose(position, rotation, scale);

    foreach(QNodeInteractor* child, childs)
    {
        m_tbeWidget->pushHistoryStat(new ModificationState(child));

        child->setScale(scale);
    }

    statusBar()->showMessage("Scale coller...", 2000);
}

void MainWindow::pastRotation()
{
    QList<QNodeInteractor*> childs = m_tbeWidget->selection();
    QNodeInteractor* selectedNode = childs.first();
    childs.pop_front();

    if(!selectedNode)
        return;

    tbe::Vector3f position, scale;
    tbe::Quaternion rotation;

    selectedNode->target()->getMatrix().decompose(position, rotation, scale);

    foreach(QNodeInteractor* child, childs)
    {
        m_tbeWidget->pushHistoryStat(new ModificationState(child));

        child->setRotation(rotation.getEuler());
    }

    statusBar()->showMessage("Rotation coller...", 2000);
}
