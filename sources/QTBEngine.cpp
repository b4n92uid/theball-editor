/*
 * File:   QTBEngine.cpp
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 13:30
 */

#include "QTBEngine.h"
#include "Define.h"

#include "MainWindow.h"
#include "ClassFactory.h"

#include "QNodeInteractor.h"
#include "QMeshInteractor.h"
#include "QLightInteractor.h"
#include "QParticlesInteractor.h"
#include "QMapMarkInteractor.h"

#include "ui_interface.h"
#include "HistoryState.h"

#include <QDebug>

using namespace tbe;
using namespace scene;

inline void translate(int& c)
{
    switch(c)
    {
        case Qt::Key_Shift:
            c = EventManager::KEY_LSHIFT;
            break;
        case Qt::Key_Alt:
            c = EventManager::KEY_LALT;
            break;
        case Qt::Key_Control:
            c = EventManager::KEY_LCTRL;
            break;
    }
}

template< typename TC, typename T1, typename T2>
void swapcontainer(TC& c, T1& v1, T2& v2)
{
    c = (TC) v1 == c ? (TC) v2 : (TC) v1;
}

class QTextureLoader : public tbe::TextureLoader
{
    TextureData* load(std::string filename, int origin);
    void release(TextureData* tdata);
};

tbe::TextureLoader::TextureData* QTextureLoader::load(std::string filename, int origin)
{
    QImage img;
    
    if(img.load(QString::fromStdString(filename)))
    {
        QImage glimg = QGLWidget::convertToGLFormat(img);

        TextureData* tdata = new TextureData;
        tdata->width = glimg.width();
        tdata->height = glimg.height();
        tdata->pixels = new GLubyte[glimg.byteCount()];
        
        memcpy(tdata->pixels, glimg.bits(), glimg.byteCount());

        return tdata;
    }
    
    return NULL;
}

void QTextureLoader::release(TextureData* tdata)
{
    delete[] tdata->pixels;
    delete tdata;
}

QTBEngine::QTBEngine(QWidget* parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_mainwin = dynamic_cast<MainWindow*> (parent->parentWidget());

    m_device = NULL;
    m_sceneManager = NULL;

    m_moveObject = false;
    m_grabCamera = false;
    m_moveCamera = false;

    m_selectedNode = NULL;
    m_lastSelectedNode = NULL;

    m_selbox = NULL;

    m_grid = NULL;

    m_classFactory = new ClassFactory(m_mainwin);

    m_staticView = false;

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::OpenHandCursor);

    m_toolMode[SELECTION_TOOL].type = SELECTION_TOOL;
    m_toolMode[SELECTION_TOOL].cursor = Qt::OpenHandCursor;
    m_toolMode[SELECTION_TOOL].axis(1, 0, 1);

    m_toolMode[ROTATE_TOOL].type = ROTATE_TOOL;
    m_toolMode[ROTATE_TOOL].cursor = QCursor(QPixmap(":/Medias/medias/rotate.png"));
    m_toolMode[ROTATE_TOOL].axis(0, 1, 0);

    m_toolMode[SCALE_TOOL].type = SCALE_TOOL;
    m_toolMode[SCALE_TOOL].cursor = QCursor(QPixmap(":/Medias/medias/resize.png"));
    m_toolMode[SCALE_TOOL].axis(1, 1, 1);

    m_currentTool = NULL;

    m_gridset.enable = false;
    m_gridset.size = m_mainwin->ui()->grid_size->value();
    m_gridset.cuts = m_mainwin->ui()->grid_cuts->value();

    m_sensivitySet.camera = 0.1;
    m_sensivitySet.selection = m_mainwin->ui()->selection_sensitiv->value();
}

QTBEngine::~QTBEngine()
{
    clearScene();

    delete m_sceneParser;

    delete m_device;
}

void QTBEngine::initializeGL()
{
    using namespace tbe;
    using namespace tbe::scene;

    m_device = new Device;
    m_device->init();
    
    Texture::registerLoader(new QTextureLoader);

    m_sceneManager = m_device->getSceneManager();
    m_eventManager = m_device->getEventManager();

    m_meshScene = new MeshParallelScene;
    m_meshScene->setEnableFrustumTest(true);
    m_meshScene->setTransparencySort(true);
    m_particlesScene = new ParticlesParallelScene;
    m_markScene = new MapMarkParallelScene;
    m_waterScene = new WaterParallelScene;

    m_sceneManager->addParallelScene(m_meshScene);
    m_sceneManager->addParallelScene(m_waterScene);
    m_sceneManager->addParallelScene(m_particlesScene);
    m_sceneManager->addParallelScene(m_markScene);

    m_sceneParser = new scene::SceneParser(m_sceneManager);
    m_sceneParser->setClassFactory(m_classFactory);
    m_sceneParser->setMeshScene(m_meshScene);
    m_sceneParser->setParticlesScene(m_particlesScene);
    m_sceneParser->setMarkScene(m_markScene);
    m_sceneParser->setWaterScene(m_waterScene);

    m_fog = m_sceneManager->getFog();
    m_skybox = m_sceneManager->getSkybox();

    m_camera = new OrbitalCamera;

    m_sceneManager->addCamera(m_camera);

    m_rootNode = m_sceneManager->getRootNode();

    m_centerTarget = 0;

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_updateTimer->start(16);
}

tbe::scene::SceneParser* QTBEngine::sceneParser() const
{
    return m_sceneParser;
}

tbe::scene::Node* QTBEngine::rootNode() const
{
    return m_rootNode;
}

tbe::scene::Grid* QTBEngine::grid() const
{
    return m_grid;
}

tbe::scene::Camera* QTBEngine::camera() const
{
    return m_camera;
}

void QTBEngine::setupSelection()
{
    using namespace scene;

    m_selbox = new SelBox(m_meshScene);
    m_selbox->Node::setEnable(false);
    m_rootNode->addChild(m_selbox);

    m_grid = new Grid(m_meshScene, 8, 8);
    m_grid->setName("grid");
    m_grid->setEnable(false);
    m_grid->setSerialized(false);
    m_grid->setCastShadow(false);
    m_grid->setReceiveShadow(false);

    Material* gridmat = m_grid->getSubMesh(0)->getMaterial();
    gridmat->setRenderFlags(Material::PIPELINE);
    gridmat->setLineWidth(1);

    m_rootNode->addChild(m_grid);
}

void QTBEngine::placeCamera()
{
    if(!m_selectedNode)
        return;

    if(m_staticView)
        return;

    if(m_toolMode->type != SELECTION_TOOL)
        return;

    m_centerTarget = m_selectedNode->target()->getAbsoluteMatrix()
            * m_selectedNode->target()->getAabb().getCenter();
}

void QTBEngine::pauseRendring()
{
    m_updateTimer->stop();
}

void QTBEngine::resumeRendring()
{
    m_updateTimer->start();
}

void QTBEngine::resizeGL(int w, int h)
{
    m_device->setViewport(Vector2i(w, h));
}

void QTBEngine::applyTranslationEvents()
{
    if(m_eventManager->notify != EventManager::EVENT_MOUSE_MOVE || !m_moveObject)
        return;

    Vector2f mousePosRel = m_eventManager->mousePosRel;

    if(m_currentTool->type == SCALE_TOOL)
    {
        if(m_selectedNode
           && m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT]
           && m_eventManager->notify == EventManager::EVENT_MOUSE_MOVE)
        {

            foreach(QNodeInteractor* qnode, m_selection)
            {
                tbe::Vector3f scale = qnode->target()->getScale();

                if(m_currentTool->axis.x > 0 && m_currentTool->axis.y > 0 && m_currentTool->axis.z > 0)
                {
                    scale += -mousePosRel.y * m_sensivitySet.selection * scale;
                }
                else
                {
                    if(m_currentTool->axis.x > 0)
                        scale.x += mousePosRel.x * m_sensivitySet.selection * 0.1;

                    if(m_currentTool->axis.y > 0)
                        scale.y += mousePosRel.y * m_sensivitySet.selection * 0.1;

                    if(m_currentTool->axis.z > 0)
                        scale.z += -mousePosRel.y * m_sensivitySet.selection * 0.1;
                }


                qnode->target()->setScale(scale);

                qnode->QNodeInteractor::updateGui();

                highlight(qnode);

                emit notifyChange();
            }
        }
    }
    else if(m_currentTool->type == ROTATE_TOOL)
    {
        if(m_selectedNode
           && m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT]
           && m_eventManager->notify == EventManager::EVENT_MOUSE_MOVE)
        {

            foreach(QNodeInteractor* qnode, m_selection)
            {
                tbe::Quaternion rotation = qnode->target()->getRotation();

                if(m_currentTool->axis.x > 0)
                    rotation *= tbe::Quaternion(-mousePosRel.y * m_sensivitySet.selection * 0.1, Vector3f(1, 0, 0));

                if(m_currentTool->axis.y > 0)
                    rotation *= tbe::Quaternion(mousePosRel.x * m_sensivitySet.selection * 0.1, Vector3f(0, 1, 0));

                if(m_currentTool->axis.z > 0)
                    rotation *= tbe::Quaternion(mousePosRel.x * m_sensivitySet.selection * 0.1, Vector3f(0, 0, 1));

                qnode->target()->setRotation(rotation);

                qnode->QNodeInteractor::updateGui();

                highlight(qnode);

                emit notifyChange();
            }
        }
    }
    else if(m_currentTool->type == SELECTION_TOOL)
    {
        if(m_selectedNode
           && m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT]
           && m_eventManager->notify == EventManager::EVENT_MOUSE_MOVE)
        {
            Vector3f position;

            Vector3f target = m_camera->getTarget();
            target.y = 0;
            target.normalize();

            Vector3f left = m_camera->getLeft();
            left.y = 0;
            left.normalize();

            Vector3f transform;
            Quaternion rota;

            if(m_eventManager->keyState[EventManager::KEY_LALT])
            {
                transform.y -= mousePosRel.y * m_sensivitySet.selection;
            }
            else
            {
                if(m_currentTool->axis == Vector3f(1, 0, 1))
                {
                    transform = rota * (-left * mousePosRel.x * m_sensivitySet.selection);
                    transform -= rota * (target * mousePosRel.y * m_sensivitySet.selection);
                    transform.y = 0;
                }

                else if(m_currentTool->axis.x > 0)
                    transform.x -= mousePosRel.x * m_sensivitySet.selection;

                else if(m_currentTool->axis.y > 0)
                    transform.y -= mousePosRel.y * m_sensivitySet.selection;

                else if(m_currentTool->axis.z > 0)
                    transform.z -= mousePosRel.x * m_sensivitySet.selection;
            }

            position += transform;

            foreach(QNodeInteractor* qnode, m_selection)
            {
                tbe::scene::Node* selnode = qnode->target();

                selnode->setPos(selnode->getPos() + position);

                highlight(qnode);
            }

            m_selectedNode->QNodeInteractor::updateGui();

            emit notifyChange();
        }
    }

    m_eventManager->notify = EventManager::EVENT_NO_EVENT;
}

void QTBEngine::applyCameraEvents()
{
    Vector3f campos = m_camera->getCenter();

    if(m_eventManager->mouseState[EventManager::MOUSE_BUTTON_MIDDLE])
    {
        if(m_eventManager->keyState[EventManager::KEY_LALT])
        {
            m_centerTarget.y += -m_eventManager->mousePosRel.y * m_sensivitySet.camera;
        }
        else
        {
            m_centerTarget += -m_eventManager->mousePosRel.x * m_camera->getLeft().Y(0) * m_sensivitySet.camera;
            m_centerTarget += -m_eventManager->mousePosRel.y * m_camera->getTarget().Y(0) * m_sensivitySet.camera;
        }

        m_eventManager->mousePosRel = 0;

        if(m_gridset.enable)
        {
            Vector3f pos = math::round(m_centerTarget);
            m_grid->setPos(pos);
        }
    }

    if(m_centerTarget - campos > 0.01)
        m_camera->setCenter(campos + (m_centerTarget - campos) / 8.0f);
}

void QTBEngine::paintGL()
{
    applyTranslationEvents();

    applyCameraEvents();

    m_device->beginScene();
    m_sceneManager->render();
    m_curCursor3D = m_sceneManager->screenToWorld(m_curCursorPos);
    m_device->endScene();
}

void QTBEngine::toggleStaticView(bool state)
{
    m_staticView = state;

    if(!state)
        placeCamera();
}

void QTBEngine::toggleSelBox(bool state)
{
    using namespace std;
    using namespace tbe::scene;

    m_selbox->setVisible(state);

    std::for_each(m_selboxArray.begin(), m_selboxArray.end(),
                  bind2nd(mem_fun(&SelBox::setVisible), state));
}

void QTBEngine::toggleGrid(bool state)
{
    using namespace scene;

    m_gridset.enable = state;

    if(state)
    {
        m_grid->clear();

        m_grid->build(m_gridset.size, m_gridset.cuts);
        m_grid->getSubMesh(0)->getMaterial()->setRenderFlags(Material::PIPELINE);
        m_grid->getSubMesh(0)->getMaterial()->setColor(Vector4f(0, 0, 0, 0.75));

        Vector3f pos = math::round(m_centerTarget);
        m_grid->setPos(pos);
    }

    m_grid->setEnable(m_gridset.enable);
}

void QTBEngine::deleteNode(QNodeInteractor* node)
{
    pushHistoryStat(new DeletionState(node));

    node->unsetup();

    tbe::scene::Node* target = node->target();
    target->dettach();
}

QNodeInteractor* QTBEngine::cloneNode(QNodeInteractor* node)
{
    QNodeInteractor* clone = node->clone();

    node->target()->getParent()->addChild(clone->target());

    clone->setup();

    return clone;
}

void QTBEngine::cloneSelected()
{
    try
    {
        using namespace tbe::scene;

        QList<QNodeInteractor*> current = m_selection;

        emit deselectAllNode();

        foreach(QNodeInteractor* qnode, current)
        {
            QNodeInteractor* clone = cloneNode(qnode);
            emit selection(clone);
        }
    }
    catch(std::exception& e)
    {
        QMessageBox::warning(parentWidget(), "Erreur", e.what());
    }
}

void QTBEngine::deleteSelected()
{
    using namespace tbe::scene;

    QList<QNodeInteractor*> current = m_selection;

    emit deselectAllNode();

    foreach(QNodeInteractor* qnode, current)
    {
        deleteNode(qnode);
    }
}

void QTBEngine::pushHistoryStat(HistoryState* hs)
{
    if(m_history.size() > 32)
    {
        delete m_history.front();
        m_history.pop_front();
    }

    m_history.push(hs);

    m_mainwin->ui()->actionUndo->setEnabled(true);

    emit notifyChange();
}

void QTBEngine::popHistoryStat()
{
    if(m_history.empty())
        return;

    HistoryState* hs = m_history.pop();

    QNodeInteractor* node = hs->node();

    hs->restore();
    delete hs;

    emit selection(node);

    if(m_history.empty())
        m_mainwin->ui()->actionUndo->setEnabled(false);

    emit notifyChange();
}

void QTBEngine::baseOnFloor()
{
    foreach(QNodeInteractor* qnode, m_selection)
    baseOnFloor(qnode);
}

void QTBEngine::centerOnFloor()
{
    foreach(QNodeInteractor* qnode, m_selection)
    centerOnFloor(qnode);
}

void QTBEngine::baseOnFloor(QNodeInteractor* node)
{
    tbe::scene::Node* selnode = node->target();

    centerOnFloor(node);

    Vector3f adjust = selnode->getPos();
    adjust.y += -selnode->getAabb().min.y;
    selnode->setPos(adjust);

    node->QNodeInteractor::updateGui();

    emit notifyChange();
}

void QTBEngine::centerOnFloor(QNodeInteractor* node)
{
    using namespace std;
    using namespace tbe::scene;

    tbe::scene::Node* selnode = node->target();

    pushHistoryStat(new ModificationState(node));

    m_selbox->Node::setEnable(false);
    m_grid->setEnable(false);

    m_meshScene->setInFloor(selnode);

    m_selbox->Node::setEnable(true);
    m_grid->setEnable(m_gridset.enable);

    node->QNodeInteractor::updateGui();

    emit notifyChange();
}

void QTBEngine::enterEvent(QEvent * event)
{
    setFocus();
}

struct Nearest
{
    Vector3f center;

    bool operator()(scene::Mesh* mesh1, scene::Mesh * mesh2)
    {
        return (mesh1->getAbsoluteMatrix().getPos() - center <
                mesh2->getAbsoluteMatrix().getPos() - center);
    }
};

void QTBEngine::selectFromPick(QMouseEvent* ev)
{
    using namespace std;
    using namespace tbe;
    using namespace scene;

    m_selbox->Node::setEnable(false);
    m_grid->setEnable(false);

    // Disable locked node
    QMap < QNodeInteractor*, bool> initialState;

    foreach(QNodeInteractor* node, m_nodeInterface.values())
    {
        if(node->isLocked())
        {
            initialState[node] = node->target()->isEnable();
            node->target()->setEnable(false);
        }
    }

    if(m_selectedNode)
    {
        initialState[m_selectedNode] = true;
        m_selectedNode->target()->setEnable(false);
    }

    // Ray cast
    Vector3f campos = m_camera->getPos();
    Mesh::Array nodes = m_meshScene->findMeshs(campos, Vector3f::normalize(m_curCursor3D - campos));

    if(!nodes.empty())
    {
        Nearest pred = {campos};
        Mesh* nearest = *std::min_element(nodes.begin(), nodes.end(), pred);

        if(m_nodeInterface.contains(nearest))
        {
            QNodeInteractor* qnode = m_nodeInterface[nearest];

            // Selection
            if(ev->modifiers() & Qt::ControlModifier)
            {
                if(m_selection.contains(qnode))
                    emit deselection(qnode);
                else
                    emit selection(qnode);
            }
            else
            {
                emit deselectionAll();
                emit selection(qnode);
            }
        }
    }

    // Restor state

    foreach(QNodeInteractor* node, initialState.keys())
    {
        node->target()->setEnable(initialState[node]);
    }

    m_selbox->Node::setEnable(true);

    if(m_gridset.enable)
    {
        Vector3f pos = math::round(m_selbox->getPos());
        m_grid->setPos(pos);
        m_grid->setEnable(true);
    }
}

void QTBEngine::mousePressEvent(QMouseEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_MOUSE_DOWN;

    if(ev->button() == Qt::LeftButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_LEFT] = 1;

        m_grabCamera = true;

        m_lastCursorPos = qptovec(ev->pos());
        m_lastCursorPos.y = size().height() - m_lastCursorPos.y;

        setCursor(Qt::ClosedHandCursor);
    }

    else if(ev->button() == Qt::MiddleButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_MIDDLE] = 1;

        m_moveCamera = false;

        m_cursorRelativeMove = QCursor::pos();
    }

    else if(ev->button() == Qt::RightButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT] = 1;

        m_moveObject = false;

        m_cursorRelativeMove = QCursor::pos();
    }
}

void QTBEngine::mouseReleaseEvent(QMouseEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_MOUSE_UP;

    if(ev->button() == Qt::LeftButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_LEFT] = 0;

        m_grabCamera = false;
    }

    else if(ev->button() == Qt::MiddleButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_MIDDLE] = 0;

        if(!m_moveCamera)
        {
            m_centerTarget = m_curCursor3D;

            if(m_gridset.enable)
            {
                Vector3f pos = math::round(m_centerTarget);
                m_grid->setPos(pos);
            }
        }
    }

    else if(ev->button() == Qt::RightButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT] = 0;

        if(!m_moveObject)
            selectFromPick(ev);

        if(m_gridset.enable)
        {
            if(ev->modifiers() & Qt::AltModifier)
            {

                foreach(QNodeInteractor* qnode, m_selection)
                {
                    Vector3f current_position = qnode->target()->getPos();
                    Vector3f round_position(0, m_gridset.size.y / m_gridset.cuts.y, 0);

                    current_position = math::round(current_position, round_position).X(current_position.x).Z(current_position.z);

                    qnode->target()->setPos(current_position);
                    qnode->QNodeInteractor::updateGui();
                    highlight(qnode);
                }
            }
            else
            {

                foreach(QNodeInteractor* qnode, m_selection)
                {
                    Vector3f current_position = qnode->target()->getPos();
                    Vector3f round_position;
                    round_position.x = m_gridset.size.x / m_gridset.cuts.x;
                    round_position.z = m_gridset.size.y / m_gridset.cuts.y;

                    current_position = math::round(current_position, round_position).Y(current_position.y);

                    qnode->target()->setPos(current_position);
                    qnode->QNodeInteractor::updateGui();
                    highlight(qnode);
                }
            }
        }

    }

    setCursor(m_currentTool->cursor);
}

void QTBEngine::mouseMoveEvent(QMouseEvent* ev)
{
    updateInformationGui();

    // Engine input injection --------------------------------------------------

    m_curCursorPos = qptovec(ev->pos());
    m_curCursorPos.y = size().height() - m_curCursorPos.y;

    Vector2f mousePosRel = m_curCursorPos - m_lastCursorPos;

    m_eventManager->notify = EventManager::EVENT_MOUSE_MOVE;
    m_eventManager->mousePos = m_curCursorPos;
    m_eventManager->mousePosRel = mousePosRel;

    m_lastCursorPos = m_curCursorPos;

    // Input process -----------------------------------------------------------

    if(ev->buttons() & Qt::LeftButton)
    {
        m_camera->onEvent(m_eventManager);
    }

    if(ev->buttons() & Qt::MidButton)
    {
        setCursor(Qt::BlankCursor);

        m_moveCamera = true;

        QCursor::setPos(m_cursorRelativeMove);
    }

    if(ev->buttons() & Qt::RightButton && m_selectedNode)
    {
        if(mousePosRel > 1 && !m_moveObject)
        {
            foreach(QNodeInteractor* qnode, m_selection)
            pushHistoryStat(new ModificationState(qnode));

            m_moveObject = true;

            setCursor(Qt::BlankCursor);

            QCursor::setPos(m_cursorRelativeMove);
        }
        else if(m_moveObject)
        {
            QCursor::setPos(m_cursorRelativeMove);
        }
    }
}

void QTBEngine::wheelEvent(QWheelEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_MOUSE_DOWN;

    m_eventManager->mouseState[EventManager::MOUSE_BUTTON_WHEEL_UP] =
            m_eventManager->mouseState[EventManager::MOUSE_BUTTON_WHEEL_DOWN] = 0;

    if(ev->delta() > 0)
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_WHEEL_UP] = 1;
    else
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_WHEEL_DOWN] = 1;

    m_camera->onEvent(m_eventManager);

    m_eventManager->notify = EventManager::EVENT_NO_EVENT;

    m_eventManager->mouseState[EventManager::MOUSE_BUTTON_WHEEL_UP] =
            m_eventManager->mouseState[EventManager::MOUSE_BUTTON_WHEEL_DOWN] = 0;
}

void QTBEngine::keyPressEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_DOWN;

    if(ev->key() == Qt::Key_Q && m_lastSelectedNode)
    {
        emit selection(m_lastSelectedNode);
    }

    if(ev->key() == Qt::Key_Plus)
    {
        m_camera->setDistance(m_camera->getDistance() - 1);
    }

    if(ev->key() == Qt::Key_Minus)
    {
        m_camera->setDistance(m_camera->getDistance() + 1);
    }

    if(ev->key() == Qt::Key_1)
    {
        if(ev->modifiers() & Qt::CTRL)
        {
            m_camera->setRotate(Vector2f(180, 0));
            m_camera->setTarget(Vector3f(0, 0, -1));
        }
        else
        {
            m_camera->setRotate(Vector2f(0, 0));
            m_camera->setTarget(Vector3f(0, 0, 1));
        }
        m_camera->setCenter(m_centerTarget);
    }

    if(ev->key() == Qt::Key_2)
    {
        m_camera->rotate(Vector2f(0, 22.5));
        m_camera->setCenter(m_centerTarget);
    }

    if(ev->key() == Qt::Key_3)
    {
        if(ev->modifiers() & Qt::CTRL)
        {
            m_camera->setRotate(Vector2f(-90, 0));
            m_camera->setTarget(Vector3f(-1, 0, 0));
        }
        else
        {
            m_camera->setRotate(Vector2f(90, 0));
            m_camera->setTarget(Vector3f(1, 0, 0));
        }
        m_camera->setCenter(m_centerTarget);
    }

    if(ev->key() == Qt::Key_4)
    {
        m_camera->rotate(Vector2f(22.5, 0));
        m_camera->setCenter(m_centerTarget);
    }

    if(ev->key() == Qt::Key_5)
    {
        m_camera->setRotate(Vector2f(45, -45));
        m_centerTarget = 0;
        m_camera->setDistance(20);
    }

    if(ev->key() == Qt::Key_6)
    {
        m_camera->rotate(Vector2f(-22.5, 0));
        m_camera->setCenter(m_centerTarget);
    }

    if(ev->key() == Qt::Key_7)
    {
        if(ev->modifiers() & Qt::CTRL)
        {
            m_camera->setRotate(Vector2f(0, 90));
            m_camera->setTarget(Vector3f(0, 1, 0.01));
        }
        else
        {
            m_camera->setRotate(Vector2f(0, -90));
            m_camera->setTarget(Vector3f(0, -1, 0.01));
        }
        m_camera->setCenter(m_centerTarget);
    }

    if(ev->key() == Qt::Key_8)
    {
        m_camera->rotate(Vector2f(0, -22.5));
        m_camera->setCenter(m_centerTarget);
    }

    if(m_selectedNode)
    {
        if(ev->modifiers() & Qt::ShiftModifier)
        {
            float factor = ev->modifiers() & Qt::ControlModifier ? 0.01 : 1;

            foreach(QNodeInteractor* qnode, m_selection)
            {
                tbe::scene::Node* selnode = qnode->target();

                if(ev->key() == Qt::Key_Up)
                {
                    Vector3f pos = selnode->getPos();

                    if(ev->modifiers() & Qt::ALT)
                        pos += m_camera->getUp().X(0).Z(0).normalize() * factor;
                    else
                        pos += m_camera->getTarget().Y(0).normalize().pinpoint() * factor;

                    selnode->setPos(pos);
                    qnode->QNodeInteractor::updateGui();

                    emit notifyChange();
                }

                else if(ev->key() == Qt::Key_Down)
                {
                    Vector3f pos = selnode->getPos();

                    if(ev->modifiers() & Qt::ALT)
                        pos -= m_camera->getUp().X(0).Z(0).normalize() * factor;
                    else
                        pos -= m_camera->getTarget().Y(0).normalize().pinpoint() * factor;

                    selnode->setPos(pos);

                    qnode->QNodeInteractor::updateGui();

                    emit notifyChange();
                }

                else if(ev->key() == Qt::Key_Left)
                {
                    Vector3f pos = selnode->getPos();
                    pos -= m_camera->getLeft().Y(0).normalize().pinpoint() * factor;
                    selnode->setPos(pos);

                    qnode->QNodeInteractor::updateGui();

                    emit notifyChange();
                }

                else if(ev->key() == Qt::Key_Right)
                {
                    Vector3f pos = selnode->getPos();
                    pos += m_camera->getLeft().Y(0).normalize().pinpoint() * factor;
                    selnode->setPos(pos);

                    qnode->QNodeInteractor::updateGui();

                    emit notifyChange();
                }
            }
        }

        if(ev->key() == Qt::Key_P && !m_selectedNode->target()->getParent()->isRoot())
        {
            using namespace tbe::scene;

            Node* parent = m_selectedNode->target()->getParent();

            emit deselectionAll();
            emit selection(m_nodeInterface[parent]);
        }

        if(ev->key() == Qt::Key_Escape)
        {
            emit deselectionAll();
        }

    }

    int c = std::tolower(ev->key());
    translate(c);

    m_eventManager->keyState[c] = 1;
}

void QTBEngine::keyReleaseEvent(QKeyEvent* ev)
{
    using namespace tbe;
    using namespace scene;

    m_eventManager->notify = EventManager::EVENT_KEY_UP;

    int c = std::tolower(ev->key());
    translate(c);

    m_eventManager->keyState[c] = 0;
}

void QTBEngine::saveScene(const QString& filename)
{
    using namespace scene;

    m_sceneParser->setFilename(filename.toStdString());
    m_sceneParser->prepare();
    m_sceneParser->save();
}

void QTBEngine::clearScene()
{
    foreach(HistoryState* hs, m_history)
            delete hs;

    m_history.clear();

    foreach(tbe::scene::Node* n, m_nodeInterface.keys())
            delete m_nodeInterface.value(n);

    m_nodeInterface.clear();

    m_rootNode->clearAllChild();
    m_sceneManager->clearParallelScenes(false);
    m_sceneParser->clear();

    m_fog->clear();
    m_skybox->clear();

    m_selbox = NULL;
    m_grid = NULL;
    m_selectedNode = NULL;

    m_centerTarget = 0;
    m_lastCursorPos = 0;
    m_curCursorPos = 0;
    m_curCursor3D = 0;

    m_grabCamera = false;
    m_moveCamera = false;

    Texture::resetCache();

    setupSelection();
}

void QTBEngine::loadScene(const QString& filename)
{
    using namespace scene;

    clearScene();

    m_sceneParser->load(filename.toStdString());
    m_sceneParser->build();

    AABB sceneAabb = m_meshScene->getSceneAabb();

    m_sensivitySet.selection = math::round(sceneAabb.getLength() / 2 * 0.001f, 0.01f);
    m_sensivitySet.camera = math::round(sceneAabb.getLength() * 0.001f, 0.01f);

    m_camera->setDistance(sceneAabb.getLength() / 8.0f);
    m_camera->setRotate(Vector2f(45, -45));

    m_selectedNode = NULL;
    m_centerTarget = 1;

    m_selbox->setEnable(false);
}

struct RootSort
{
    scene::Node* root;

    bool operator ()(scene::Node* node1, scene::Node * node2)
    {
        if(node1->deepPosition() == node2->deepPosition())
            return node1 < node2;
        else
            return node1->deepPosition() < node2->deepPosition();
    }

};

void QTBEngine::setGridSize(double value)
{
    m_gridset.size = value;
    m_grid->build(value, m_gridset.cuts);
}

void QTBEngine::setGridCuts(int value)
{
    m_gridset.cuts = value;
    m_grid->build(m_gridset.size, value);
}

void QTBEngine::setSelectionSensitiv(double value)
{
    m_sensivitySet.selection = value;
}

void QTBEngine::setLockAxisX(bool check)
{
    m_currentTool->axis.X(check);
}

void QTBEngine::setLockAxisY(bool check)
{
    m_currentTool->axis.Y(check);
}

void QTBEngine::setLockAxisZ(bool check)
{
    m_currentTool->axis.Z(check);
}

void QTBEngine::setRestorePos()
{

    foreach(QNodeInteractor* qnode, m_selection)
    {
        qnode->target()->setPos(0);
        qnode->QNodeInteractor::updateGui();
    }
}

void QTBEngine::setRestoreRotation()
{

    foreach(QNodeInteractor* qnode, m_selection)
    {
        tbe::Quaternion rotation;
        qnode->target()->setRotation(rotation);
        qnode->QNodeInteractor::updateGui();
    }
}

void QTBEngine::setRestoreScale()
{

    foreach(QNodeInteractor* qnode, m_selection)
    {
        qnode->target()->setScale(1);
        qnode->QNodeInteractor::updateGui();
    }
}

void QTBEngine::setupNewNode(tbe::scene::Node* thenew)
{
    using namespace tbe;
    using namespace scene;

    if(m_selectedNode)
    {
        Node* taget = m_selectedNode->target()->getParent();
        taget->addChild(thenew);
        thenew->Node::setPos(m_centerTarget - m_selectedNode->target()->getParent()->getPos());
    }
    else
    {
        m_rootNode->addChild(thenew);
        thenew->Node::setPos(m_centerTarget);
    }

}

QMeshInteractor* QTBEngine::newMesh(const QString& filename)
{
    using namespace tbe;
    using namespace scene;

    Mesh* mesh = m_classFactory->newMesh(m_meshScene);

    Mesh* shared = Mesh::isSharedBuffer(filename.toStdString());

    if(shared)
    {
        mesh->shareVertexes(*shared);
    }
    else
    {
        OBJMesh loader(m_meshScene, filename.toStdString());
        mesh->fetchVertexes(loader);
        mesh->setName(QFileInfo(filename).baseName().toStdString());
    }

    Mesh::registerBuffer(mesh, filename.toStdString());

    mesh->serializing().put("class.path", filename.toStdString());
    mesh->serializing().put("class.format", "obj");

    setupNewNode(mesh);

    return m_classFactory->setupMeshGui(mesh);
}

QLightInteractor* QTBEngine::newLight()
{
    Light* light = new Light(m_meshScene);

    setupNewNode(light);

    return m_classFactory->setupLightGui(light);
}

QParticlesInteractor* QTBEngine::newParticles()
{
    ParticlesEmiter* particles = new ParticlesEmiter(m_particlesScene);

    setupNewNode(particles);

    return m_classFactory->setupParticlesGui(particles);
}

QMapMarkInteractor* QTBEngine::newMark()
{
    MapMark* mark = new MapMark(m_markScene);

    setupNewNode(mark);

    return m_classFactory->setupMapMarkGui(mark);
}

void QTBEngine::setSkybox(const QStringList& texs)
{
    Texture skytex[6];

    for(unsigned i = 0; i < 6; i++)
        if(!texs[i].isEmpty())
            skytex[i] = texs[i].toStdString();

    m_skybox->setTextures(skytex);
}

void QTBEngine::setSkybox(bool enable)
{
    m_skybox->setEnable(enable);
}

void QTBEngine::setFog(tbe::Vector4f color, float start, float end)
{
    m_fog->setColor(color);
    m_fog->setStart(start);
    m_fog->setEnd(end);
}

void QTBEngine::setFog(bool enable)
{
    m_fog->setEnable(enable);
}

void QTBEngine::setSceneAmbiant(const tbe::Vector3f& value)
{
    m_sceneManager->setAmbientLight(math::vec34(value));
}

void QTBEngine::selectNode(QNodeInteractor* qnode)
{
    qnode->bindInterface();

    if(m_selection.empty())
    {
        m_selectedNode = qnode;

        m_selbox->Node::setEnable(true);

        m_selection.push_back(qnode);
    }

    else if(!m_selection.contains(qnode))
    {
        m_selboxArray[qnode] = new SelBox(m_meshScene);

        m_rootNode->addChild(m_selboxArray[qnode]);

        m_selection.push_back(qnode);
    }

    if(m_gridset.enable)
        toggleGrid(true);

    updateInformationGui();
}

void QTBEngine::deselectNode(QNodeInteractor* qnode)
{
    qnode->unbindFromGui();

    m_selection.removeAll(qnode);

    unhighlight(qnode);

    if(m_selectedNode == qnode)
    {
        m_selectedNode = NULL;
        m_selbox->Node::setEnable(false);
    }

    updateInformationGui();
}

void QTBEngine::deselectAllNode()
{
    std::for_each(m_selection.begin(), m_selection.end(), std::mem_fun(&QNodeInteractor::unbindFromGui));

    m_selection.clear();

    foreach(SelBox* box, m_selboxArray.values())
            delete box;

    m_selboxArray.clear();

    m_selectedNode = NULL;

    if(m_selbox)
        m_selbox->Node::setEnable(false);

    updateInformationGui();
}

void QTBEngine::updateInformationGui()
{
    QString info;

    info =
            "<p>"
            "Curseur 3D : %1<br />"
            "Maillage &eacute;limin&eacute; du rendue : %2<br />"
            "Maillage afficher au rendue: %3<br />"
            "Maillage Totale: %4"
            "</p>"
            ;

    info = info.arg(QString::fromStdString(m_curCursor3D.toStr()));
    info = info.arg(m_meshScene->getFrustumCullingCount());
    info = info.arg(m_meshScene->getRenderedMeshCount());
    info = info.arg(m_meshScene->getRenderedMeshCount() + m_meshScene->getFrustumCullingCount());

    if(m_selectedNode)
    {
        info += "<p>%1 noeud(s) s&eacute;lectionner</p>";

        info += "<p>Nom: %2<br/>Type: %3</p>";

        info = info.arg(m_selection.count());

        if(!m_selectedNode->target()->getName().empty())
            info = info.arg(QString::fromStdString(m_selectedNode->target()->getName()));
        else
            info = info.arg("<i>[Aucun Nom]</i>");

        info = info.arg(m_selectedNode->typeName());

        info += "<p>Parent: %1<br />Enfants: %2</p>";

        if(m_selectedNode->target()->getParent() && !m_selectedNode->target()->getParent()->isRoot())
            info = info.arg(QString::fromStdString(m_selectedNode->target()->getParent()->getName()));
        else
            info = info.arg("[Pas de parent]");

        unsigned childcount = m_selectedNode->target()->getChildCount();

        if(childcount > 0)
            info = info.arg(childcount);
        else
            info = info.arg("[Pas d'enfants]");
    }

    m_mainwin->ui()->gen_information->setText(info);
}

QStringList QTBEngine::usedRessources()
{
    using namespace std;
    using namespace tbe;
    using namespace scene;

    QStringList ressPath;

    foreach(Node* node, m_nodeInterface.keys())
    {
        vector<string> paths = node->getUsedRessources();

        foreach(string path, paths)
        ressPath << QString::fromStdString(path);
    }

    return ressPath;
}

void QTBEngine::highlight(QNodeInteractor* node)
{
    if(m_selboxArray.contains(node))
        m_selboxArray[node]->setAround(node->target());
    else
        m_selbox->setAround(node->target());
}

void QTBEngine::unhighlight(QNodeInteractor* node)
{
    if(m_selboxArray.contains(node))
    {
        delete m_selboxArray[node];
        m_selboxArray.remove(node);
    }
    else
        m_selbox->setEnable(false);
}

void QTBEngine::unregisterInteractor(QNodeInteractor* node)
{
    m_nodeInterface.remove(node->target());
}

void QTBEngine::registerInteractor(QNodeInteractor* node)
{
    m_nodeInterface[node->target()] = node;
}

QNodeInteractor* QTBEngine::getInteractor(tbe::scene::Node* node)
{
    if(m_nodeInterface.contains(node))
        return m_nodeInterface[node];
    else
        return NULL;
}

const QList<QNodeInteractor*>& QTBEngine::selection()
{
    return m_selection;
}

void QTBEngine::setZNear(float value)
{
    m_sceneManager->setZNear(value);
    m_sceneManager->updateViewParameter();
}

void QTBEngine::setZFar(float value)
{
    m_sceneManager->setZFar(value);
    m_sceneManager->updateViewParameter();
}

void QTBEngine::selectSelectionTool()
{
    m_currentTool = &m_toolMode[SELECTION_TOOL];

    setCursor(m_currentTool->cursor);
}

void QTBEngine::selectScaleTool()
{
    m_currentTool = &m_toolMode[SCALE_TOOL];

    setCursor(m_currentTool->cursor);
}

void QTBEngine::selectRotateTool()
{
    m_currentTool = &m_toolMode[ROTATE_TOOL];

    setCursor(m_currentTool->cursor);
}

SelBoxInterface::SelBoxInterface() { }

SelBox::SelBox(tbe::scene::MeshParallelScene* parallelScene) : QuadBox(parallelScene, 1)
{
    using namespace tbe;
    using namespace scene;

    setName("selection");
    Material* mat = getSubMesh(0)->getMaterial();
    mat->setRenderFlags(Material::COLORED | Material::ADDITIVE | Material::PIPELINE);
    mat->setPolygoneMode(GL_LINE);
    mat->setColor(Vector4f(0, 0, 1, 1));
    setSerialized(false);
    setCastShadow(false);
    setReceiveShadow(false);
    setPriorityRender(-1);
}

void SelBox::setAround(tbe::scene::Node* node)
{
    AABB selAabb = node->getAabb();

    setMatrix(node->getAbsoluteMatrix());
    setPos(node->getAbsoluteMatrix() * selAabb.getCenter());

    if(selAabb.getSize() > 0)
        setSize(selAabb.getSize() / 2.0f);
    else
        setSize(1.0f);
}
