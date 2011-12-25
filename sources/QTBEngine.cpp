/*
 * File:   QTBEngine.cpp
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 13:30
 */

#include "QTBEngine.h"
#include "Tools.h"

#include "NodeFactory.h"
#include "MainWindow.h"
#include "ClassFactory.h"

#include "QNodeInteractor.h"

#include "ui_interface.h"
#include "HistoryState.h"

#include <QDebug>

using namespace tbe;

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
    c = (TC)v1 == c ? (TC)v2 : (TC)v1;
}

QTBEngine::QTBEngine(QWidget* parent) : QGLWidget(QGLFormat(), parent)
{
    m_mainwin = dynamic_cast<MainWindow*>(parent->parentWidget());

    m_device = NULL;
    m_sceneManager = NULL;

    m_grabCamera = false;

    m_selectedNode = NULL;
    m_lastSelectedNode = NULL;

    m_selbox = NULL;
    m_penarea = NULL;

    m_grid = NULL;

    m_classFactory = new ClassFactory(m_mainwin);

    m_gridEnable = false;
    m_staticView = false;
    m_magnetMove = false;

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::OpenHandCursor);

    m_toolMode[SELECTION_TOOL].type = SELECTION_TOOL;
    m_toolMode[SELECTION_TOOL].cursor = Qt::OpenHandCursor;

    m_toolMode[DRAW_TOOL].type = DRAW_TOOL;
    m_toolMode[DRAW_TOOL].cursor = QCursor(QPixmap(":/Medias/medias/draw.png"));

    m_currentTool = NULL;
}

QTBEngine::~QTBEngine()
{
    delete m_sceneParser;

    foreach(HistoryState* hs, m_history)
            delete hs;
}

void QTBEngine::initializeGL()
{
    using namespace tbe;
    using namespace tbe::scene;

    m_device = new Device;
    m_device->init();

    m_sceneManager = m_device->getSceneManager();
    m_eventManager = m_device->getEventManager();

    m_lightScene = new LightParallelScene;
    m_sceneManager->addParallelScene(m_lightScene);

    m_meshScene = new MeshParallelScene;
    m_sceneManager->addParallelScene(m_meshScene);

    m_particlesScene = new ParticlesParallelScene;
    m_sceneManager->addParallelScene(m_particlesScene);

    m_markScene = new MapMarkParallelScene;
    m_sceneManager->addParallelScene(m_markScene);

    m_sceneParser = new scene::SceneParser(m_sceneManager);
    m_sceneParser->setClassFactory(m_classFactory);
    m_sceneParser->setLightScene(m_lightScene);
    m_sceneParser->setMeshScene(m_meshScene);
    m_sceneParser->setParticlesScene(m_particlesScene);
    m_sceneParser->setMarkScene(m_markScene);

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

tbe::scene::Grid* QTBEngine::grid()
{
    return m_grid;
}

tbe::Vector3f QTBEngine::cameraPos() const
{
    return m_camera->getPos();
}

tbe::Vector3f QTBEngine::selectionPos() const
{
    return m_selectedNode->target()->getAbsoluteMatrix().getPos();
}

void QTBEngine::setupSelection()
{
    using namespace scene;

    m_penarea = new PenArea(m_meshScene);
    m_penarea->Node::setEnable(false);
    m_rootNode->addChild(m_penarea);

    m_selbox = new SelBox(m_meshScene);
    m_selbox->Node::setEnable(false);
    m_rootNode->addChild(m_selbox);

    m_grid = new Grid(m_meshScene, 8, 8);
    m_grid->setName("grid");
    m_grid->setEnable(false);

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

    m_centerTarget = m_selectedNode->target()->getAbsoluteMatrix().getPos();
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
    m_device->setViewportSize(Vector2i(w, h));
}

void QTBEngine::applyTranslationEvents()
{
    if(m_eventManager->notify != EventManager::EVENT_MOUSE_MOVE)
        return;

    if(m_eventManager->keyState[EventManager::KEY_LCTRL] && m_eventManager->mouseState[EventManager::MOUSE_BUTTON_MIDDLE])
    {
        float sensitivty = 0.2;
        m_centerTarget -= m_eventManager->mousePosRel.x * m_camera->getLeft().Y(0) * sensitivty;
        m_centerTarget -= m_eventManager->mousePosRel.y * m_camera->getTarget().Y(0) * sensitivty;
    }

    if(m_currentTool->type == SELECTION_TOOL)
    {
        if(m_selectedNode && m_eventManager->keyState[EventManager::KEY_LSHIFT])
        {
            tbe::scene::Node* selnode = m_selectedNode->target();

            Vector3f position = selnode->getMatrix().getPos();
            Quaternion rotation = selnode->getMatrix().getRotate();
            Vector3f scale = selnode->getMatrix().getScale();

            float moveSpeed = 0.01;

            Vector2f mousePosRel = m_eventManager->mousePosRel;

            Vector3f target = m_camera->getTarget();
            target.y = 0;
            target.normalize();

            Vector3f left = m_camera->getLeft();
            left.y = 0;
            left.normalize();

            Vector3f transform;

            if(m_eventManager->keyState[EventManager::KEY_LALT])
            {
                transform.y -= mousePosRel.y * moveSpeed;
            }
            else
            {
                Quaternion rota = selnode->getAbsoluteMatrix(false).getRotate();
                transform = rota * (-left * mousePosRel.x * moveSpeed);
                transform -= rota * (target * mousePosRel.y * moveSpeed);
                transform.y = 0;
            }

            position += transform;

            if(m_gridEnable)
            {
                m_grid->setPos(m_grid->getPos().Y(position.y));
            }

            Matrix4 apply;
            apply.setPos(position);
            apply.setRotate(rotation);
            apply.setScale(scale);

            selnode->setMatrix(apply);

            m_selectedNode->update();
        }
    }

    else if(m_currentTool->type == DRAW_TOOL)
    {
        if(m_eventManager->keyState[EventManager::KEY_LSHIFT])
        {
            float moveSpeed = 0.05;

            Vector2f mousePosRel = m_eventManager->mousePosRel;

            Vector3f transform;

            if(m_eventManager->keyState[EventManager::KEY_LALT])
            {
                transform.y -= mousePosRel.y * moveSpeed;
            }
            else
            {
                transform = -m_camera->getLeft() * mousePosRel.x * moveSpeed;
                transform -= m_camera->getTarget() * mousePosRel.y * moveSpeed;
                transform.y = 0;
            }

            m_penarea->setPos(m_penarea->getPos() + transform);
        }
    }

    m_eventManager->notify = EventManager::EVENT_NO_EVENT;
}

void QTBEngine::applyCameraEvents()
{
    Vector3f campos = m_camera->getCenter();

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
}

void QTBEngine::toggleSelBox(bool state)
{
    m_selbox->setVisible(state);
}

void QTBEngine::toggleGridDisplay(bool state)
{
    m_gridEnable = state;

    if(m_gridEnable)
    {
        m_grid->clear();

        Vector2i cuts;

        if(m_selectedNode)
        {
            AABB zone = m_selectedNode->target()->getAabb();
            cuts.x = std::max(zone.getLength() * 2.0f, 16.0f);
            cuts.y = std::max(zone.getLength() * 2.0f, 16.0f);
        }
        else
            cuts = 16;

        Vector2f size = cuts;

        m_grid->build(size, cuts);
        m_grid->getMaterial("main")->disable(scene::Material::FOGED);
        m_grid->getMaterial("main")->setColor(Vector4f(0.0, 0.0, 1.0, 1));

        if(m_selectedNode)
        {
            Vector3f pos = m_selectedNode->target()->getAbsoluteMatrix().getPos();
            pos = math::round(pos).Y(pos.y);

            m_grid->setPos(pos);
        }
    }

    m_grid->setEnable(m_gridEnable);
}

void QTBEngine::toggleMagnetMove(bool state)
{
    m_magnetMove = state;
}

void QTBEngine::deleteNode(QNodeInteractor* node)
{
    pushHistoryStat(new DeletionState(node));

    tbe::scene::Node* target = node->target();
    target->dettach();

    node->unsetup();
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
    if(!m_selectedNode)
        return;

    try
    {
        using namespace tbe::scene;

        QNodeInteractor* clone = cloneNode(m_selectedNode);

        emit selection(clone);
    }
    catch(std::exception& e)
    {
        QMessageBox::warning(parentWidget(), "Erreur", e.what());
    }
}

void QTBEngine::deleteSelected()
{
    using namespace tbe::scene;

    if(!m_selectedNode)
        return;

    deleteNode(m_selectedNode);

    emit deselection();
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
}

bool QTBEngine::isLockedNode(QNodeInteractor* node)
{
    return m_lockedNode.contains(node);
}

void QTBEngine::setLockedNode(QNodeInteractor* node, bool state)
{
    if(state)
    {
        m_lockedNode[node] = true;
    }

    else
    {
        if(m_lockedNode.contains(node))
            m_lockedNode.remove(node);
    }
}

void QTBEngine::baseOnFloor()
{
    if(!m_selectedNode)
        return;

    baseOnFloor(m_selectedNode);

    m_selectedNode->update();
}

void QTBEngine::centerOnFloor()
{
    if(!m_selectedNode)
        return;

    centerOnFloor(m_selectedNode);

    m_selectedNode->update();
}

void QTBEngine::baseOnFloor(QNodeInteractor* node)
{
    tbe::scene::Node* selnode = node->target();

    pushHistoryStat(new ModificationState(node));

    m_penarea->Node::setEnable(false);
    m_selbox->Node::setEnable(false);
    m_grid->setEnable(false);

    m_meshScene->setInFloor(selnode);

    m_selbox->Node::setEnable(true);
    m_penarea->Node::setEnable(m_currentTool->type == DRAW_TOOL);
    m_grid->setEnable(m_gridEnable);

    Vector3f adjust = selnode->getPos();
    adjust.y += -selnode->getAabb().min.y;
    selnode->setPos(adjust);
}

void QTBEngine::centerOnFloor(QNodeInteractor* node)
{
    tbe::scene::Node* selnode = node->target();

    pushHistoryStat(new ModificationState(node));

    m_selbox->Node::setEnable(false);
    m_penarea->Node::setEnable(false);
    m_grid->setEnable(false);

    m_meshScene->setInFloor(selnode);

    m_selbox->Node::setEnable(true);
    m_penarea->Node::setEnable(m_currentTool->type == DRAW_TOOL);
    m_grid->setEnable(m_gridEnable);
}

struct SelectionSort
{
    tbe::Vector3f cameraPos;
    int type;

    bool operator()(const tbe::scene::Node* node1, const tbe::scene::Node * node2)
    {
        if(!node1->getParent()->isRoot())
            return false;
        else if(type == 1)
            return node1->getAabb().getLength() < node2->getAabb().getLength();
        else if(type == 2)
            return(node1->getPos() - cameraPos) < (node2->getPos() - cameraPos);
        else
            return false;
    }
};

void QTBEngine::enterEvent(QEvent * event)
{
    setFocus();
}

struct Nearest
{
    Vector3f center;

    bool operator()(scene::Mesh* mesh1, scene::Mesh * mesh2)
    {
        return(mesh1->getAbsoluteMatrix().getPos() - center <
                mesh2->getAbsoluteMatrix().getPos() - center);
    }
};

void QTBEngine::mousePressEvent(QMouseEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_MOUSE_DOWN;

    if(ev->modifiers() & Qt::ShiftModifier)
        return;

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

        if(ev->modifiers() & Qt::ControlModifier)
            setCursor(Qt::BlankCursor);

        else if(m_meshScene->getSceneAabb().isInner(m_curCursor3D))
            m_centerTarget = m_curCursor3D;
    }

    else if(ev->button() == Qt::RightButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT] = 1;

        using namespace tbe;
        using namespace tbe::scene;

        if(m_currentTool->type == SELECTION_TOOL)
        {
            m_selbox->Node::setEnable(false);
            m_grid->setEnable(false);

            foreach(QNodeInteractor* node, m_lockedNode.keys())
            node->target()->setEnable(false);

            if(m_selectedNode)
                m_selectedNode->target()->setEnable(false);

            Vector3f campos = m_camera->getPos();
            Mesh::Array nodes = m_meshScene->findMeshs(campos, Vector3f::normalize(m_curCursor3D - campos));

            if(!nodes.empty())
            {
                Nearest pred = {campos};
                Mesh* nearest = *std::min_element(nodes.begin(), nodes.end(), pred);

                if(m_selectedNode)
                    m_selectedNode->target()->setEnable(true);

                if(m_nodeInterface.contains(nearest))
                    emit selection(m_nodeInterface[nearest]);
            }
            else
            {
                if(m_selectedNode)
                    m_selectedNode->target()->setEnable(true);
            }

            foreach(QNodeInteractor* node, m_lockedNode.keys())
            node->target()->setEnable(true);

            m_selbox->Node::setEnable(m_selectedNode);
            m_grid->setEnable(m_gridEnable);
        }
    }
}

void QTBEngine::mouseReleaseEvent(QMouseEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_MOUSE_UP;

    if(ev->modifiers() & Qt::ShiftModifier)
        return;

    if(ev->button() == Qt::LeftButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_LEFT] = 0;
        m_grabCamera = false;

        setCursor(m_currentTool->cursor);
    }

    else if(ev->button() == Qt::MiddleButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_MIDDLE] = 0;

        setCursor(m_currentTool->cursor);
    }

    else if(ev->button() == Qt::RightButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT] = 0;
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

void QTBEngine::mouseMoveEvent(QMouseEvent* ev)
{
    // Engine input injection --------------------------------------------------

    m_curCursorPos = qptovec(ev->pos());
    m_curCursorPos.y = size().height() - m_curCursorPos.y;

    Vector2f mousePosRel = m_curCursorPos - m_lastCursorPos;

    m_eventManager->notify = EventManager::EVENT_MOUSE_MOVE;
    m_eventManager->mousePos = m_curCursorPos;
    m_eventManager->mousePosRel = mousePosRel;

    m_lastCursorPos = m_curCursorPos;

    // Input process -----------------------------------------------------------

    if(m_grabCamera)
    {
        m_camera->onEvent(m_eventManager);
    }

    if(ev->buttons() & Qt::MidButton && ev->modifiers() & Qt::ControlModifier)
    {
        QCursor::setPos(m_cursorRelativeMove);
    }

    if(m_currentTool->type == SELECTION_TOOL)
    {
        if(ev->modifiers() & Qt::ShiftModifier && m_selectedNode)
        {
            QCursor::setPos(m_cursorRelativeMove);
        }
    }

    else if(m_currentTool->type == DRAW_TOOL)
    {
        if(ev->modifiers() & Qt::ShiftModifier)
        {
            if(ev->buttons() & Qt::RightButton)
            {
                if(ev->modifiers() & Qt::ControlModifier)
                {
                    float areaSize = std::max(m_penarea->getAreaSize(), 1.0);

                    foreach(QNodeInteractor* node, m_nodeInterface.values())
                    {
                        if(node->target()->getAbsoluteMatrix().getPos() - m_penarea->getPos() < areaSize)
                        {
                            deleteNode(node);
                        }
                    }
                }
                else if(m_selectedNode)
                {
                    if(m_penarea->drawPos - m_penarea->getPos() > m_penarea->getElemGap())
                    {
                        m_penarea->drawPos = m_penarea->getPos();

                        AABB area(m_penarea->getAreaSize());
                        int count(m_penarea->getElemCount());

                        Vector2f rotationRange = m_penarea->getRotationRange();
                        Vector2f scaleRange = m_penarea->getScaleRange();

                        for(int i = 0; i < count; i++)
                        {
                            QNodeInteractor* painting = m_selectedNode->clone();

                            m_selectedNode->target()->getParent()->addChild(painting->target());

                            painting->setup();

                            Matrix4 matset;

                            {// Scaling

                                Vector3f scale = math::rand(scaleRange.x, scaleRange.y);

                                if(!m_penarea->rotX)
                                    scale.x = 0;
                                if(m_penarea->rotY)
                                    scale.y = 0;
                                if(m_penarea->rotZ)
                                    scale.z = 0;

                                painting->setScale(scale);
                            }

                            {// Rotation

                                Vector3f rot = AABB(rotationRange.x, rotationRange.y).randPos();

                                if(!m_penarea->rotX)
                                    rot.x = 0;
                                if(!m_penarea->rotY)
                                    rot.y = 0;
                                if(!m_penarea->rotZ)
                                    rot.z = 0;

                                matset.setRotate(Quaternion(rot));
                            }

                            // Position
                            matset.setPos(m_penarea->drawPos + area.randPos());

                            painting->target()->setMatrix(matset);

                            if(m_penarea->onFloor == 1)
                                centerOnFloor(painting);

                            else if(m_penarea->onFloor == 2)
                                baseOnFloor(painting);

                        }
                    }
                }
            }

            QCursor::setPos(m_cursorRelativeMove);
        }

        else
            m_penarea->setPos(m_curCursor3D);
    }
}

void QTBEngine::keyPressEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_DOWN;

    if(ev->key() == Qt::Key_Shift)
    {
        if(m_selectedNode)
            pushHistoryStat(new ModificationState(m_selectedNode));

        setCursor(Qt::BlankCursor);

        m_cursorRelativeMove = QCursor::pos();
    }

    if(ev->key() == Qt::Key_Space)
    {
        m_mainwin->ui()->menuEditer->popup(QCursor::pos());
    }

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
        tbe::scene::Node* selnode = m_selectedNode->target();

        if(ev->modifiers() & Qt::ShiftModifier)
        {
            float factor = ev->modifiers() & Qt::ControlModifier ? 0.01 : 1;

            if(ev->key() == Qt::Key_Up)
            {
                Vector3f pos = selnode->getPos();

                if(ev->modifiers() & Qt::ALT)
                    pos += m_camera->getUp().X(0).Z(0).normalize() * factor;
                else
                    pos += m_camera->getTarget().Y(0).normalize().pinpoint() * factor;

                selnode->setPos(pos);
                m_selectedNode->update();
            }

            else if(ev->key() == Qt::Key_Down)
            {
                Vector3f pos = selnode->getPos();

                if(ev->modifiers() & Qt::ALT)
                    pos -= m_camera->getUp().X(0).Z(0).normalize() * factor;
                else
                    pos -= m_camera->getTarget().Y(0).normalize().pinpoint() * factor;

                selnode->setPos(pos);

                m_selectedNode->update();
            }

            else if(ev->key() == Qt::Key_Left)
            {
                Vector3f pos = selnode->getPos();
                pos -= m_camera->getLeft().Y(0).normalize().pinpoint() * factor;
                selnode->setPos(pos);

                m_selectedNode->update();
            }

            else if(ev->key() == Qt::Key_Right)
            {
                Vector3f pos = selnode->getPos();
                pos += m_camera->getLeft().Y(0).normalize().pinpoint() * factor;
                selnode->setPos(pos);

                m_selectedNode->update();
            }
        }

        if(ev->key() == Qt::Key_P && !selnode->getParent()->isRoot())
        {
            using namespace tbe::scene;

            Node* parent = selnode->getParent();

            emit selection(m_nodeInterface[parent]);
        }

        if(ev->key() == Qt::Key_Escape)
        {
            emit deselection();
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

    Vector3f gridSize(1);

    if(ev->key() == Qt::Key_Shift)
    {
        setCursor(m_currentTool->cursor);

        if(m_selectedNode && m_gridEnable)
        {
            Vector3f current_position = m_selectedNode->target()->getPos();

            current_position = math::round(current_position, gridSize).Y(current_position.y);

            m_selectedNode->target()->setPos(current_position);
            m_selectedNode->update();
        }

        if(m_selectedNode && m_magnetMove)
        {
            Node* current = m_selectedNode->target();
            Node* colset = NULL;

            Vector3f current_position = current->getAbsoluteMatrix().getPos();

            float mindist = m_meshScene->getSceneAabb().getLength();

            foreach(Node* node, m_nodeInterface.keys())
            {
                if(node != current)
                {
                    float dist = (current_position - node->getAbsoluteMatrix().getPos()).getMagnitude();
                    dist -= node->getAabb().getLength() / 2;

                    if(dist < mindist)
                    {
                        mindist = dist;
                        colset = node;
                    }
                }
            }

            if(colset)
            {
                m_selectedNode->target()->setPos(colset->getAbsoluteMatrix().getPos());
                m_selectedNode->update();

                #if 0
                QList<Vector3f> magnetpts;
                magnetpts.reserve(8 * 3);

                AABB colset_aabb = colset->getAabb();
                Vector3f colset_pos = colset->getAbsoluteMatrix().getPos();

                Vector3f::Array maincorner = colset_aabb.getPoints();

                foreach(Vector3f corner, maincorner)
                {
                }

                float mindist = colset_aabb.getLength() + 1;

                int colsetindex = -1;

                for(int i = 0; i < magnetpts.size(); i++)
                {
                    if(current_position - magnetpts[i] < mindist)
                    {
                        mindist = (current_position - magnetpts[i]).getMagnitude();
                        colsetindex = i;
                    }
                }

                if(colsetindex >= 0)
                {
                    current_position = magnetpts[colsetindex];

                    m_selectedNode->target()->setPos(current_position);
                    m_selectedNode->update();
                }
                #endif
            }
        }
    }

    if(ev->key() == Qt::Key_Alt)
    {
        if(m_selectedNode && m_gridEnable)
        {
            Vector3f current_position = m_selectedNode->target()->getPos();

            current_position = math::round(current_position, gridSize).X(current_position.x).Z(current_position.z);

            m_selectedNode->target()->setPos(current_position);
            m_selectedNode->update();
        }
    }

    int c = std::tolower(ev->key());
    translate(c);

    m_eventManager->keyState[c] = 0;
}

void QTBEngine::saveScene(const QString& filename)
{
    using namespace scene;

    m_sceneParser->exclude(m_selbox);
    m_sceneParser->exclude(m_grid);

    m_sceneParser->prepareScene();
    m_sceneParser->saveScene(filename.toStdString());
}

void QTBEngine::clearScene()
{
    m_rootNode->clearAllChild();

    m_sceneManager->clearParallelScenes(false);

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
    m_gridEnable = false;
    m_staticView = false;

    Texture::resetCache();

    m_lockedNode.clear();

    setupSelection();

    m_mainwin->zNear(m_sceneManager->getZNear());
    m_mainwin->zFar(m_sceneManager->getZFar());
    m_mainwin->fog(m_fog);
    m_mainwin->skybox(m_skybox);
    m_mainwin->ambiant(math::vec43(m_sceneManager->getAmbientLight()));
}

void QTBEngine::loadScene(const QString& filename)
{
    using namespace scene;

    clearScene();

    m_sceneParser->loadScene(filename.toStdString());
    m_sceneParser->buildScene();

    m_camera->setDistance(m_meshScene->getSceneAabb().getLength() / 8.0f);
    m_camera->setRotate(Vector2f(45, -45));

    m_selectedNode = NULL;
    m_centerTarget = 1;

    m_selbox->setEnable(false);

    m_mainwin->zNear(m_sceneManager->getZNear());
    m_mainwin->zFar(m_sceneManager->getZFar());
    m_mainwin->fog(m_fog);
    m_mainwin->skybox(m_skybox);
    m_mainwin->ambiant(math::vec43(m_sceneManager->getAmbientLight()));
}

struct RootSort
{
    scene::Node* root;

    bool operator ()(scene::Node* node1, scene::Node * node2)
    {
        // « node1 < node2 » Don't know but it fix it !
        // crash whene node's count >= 17

        if(node1->deepPosition() == node2->deepPosition())
            return node1 < node2;
        else
            return node1->deepPosition() < node2->deepPosition();
    }

};

tbe::scene::Node* QTBEngine::rootNode()
{
    return m_rootNode;
}

PenAreaInterface* QTBEngine::penArea()
{
    return m_penarea;
}

SelBoxInterface* QTBEngine::selBox()
{
    return m_selbox;
}

QMesh* QTBEngine::meshNew(const QString& filename)
{
    using namespace tbe;
    using namespace scene;

    QMesh* mesh = new QMesh(m_mainwin, OBJMesh(m_meshScene, filename.toStdString()));

    mesh->addToConstructionMap("filename", filename.toStdString());
    mesh->addToConstructionMap("class", "OBJMesh");

    /*
    QMesh* mesh = NULL;

    if(filename.endsWith("ball3d"))
        mesh = new QMesh(m_mainwin, Ball3DMesh(m_meshScene, filename.toStdString()));

    else if(filename.endsWith("obj"))
        mesh = new QMesh(m_mainwin, OBJMesh(m_meshScene, filename.toStdString()));

    else
        throw Exception("Format du fichier non reconnue \"%s\"", filename.toStdString().c_str());
     */

    m_rootNode->addChild(mesh);

    mesh->Node::setPos(m_selbox->getPos());
    mesh->setup();

    return mesh;
}

QLight* QTBEngine::lightNew()
{
    QLight* light = new QLight(m_mainwin);

    m_rootNode->addChild(light);

    light->Node::setPos(m_selbox->getPos());
    light->setup();

    return light;
}

QParticles* QTBEngine::particlesNew()
{
    QParticles* particles = new QParticles(m_mainwin);

    m_rootNode->addChild(particles);

    particles->Node::setPos(m_selbox->getPos());
    particles->setup();

    return particles;
}

QMapMark* QTBEngine::markNew()
{
    QMapMark* mark = new QMapMark(m_mainwin);

    m_rootNode->addChild(mark);

    mark->Node::setPos(m_selbox->getPos());
    mark->setup();

    return mark;
}

void QTBEngine::skyboxApply(const QStringList& texs)
{
    Texture skytex[6];

    for(unsigned i = 0; i < 6; i++)
        if(!texs[i].isEmpty())
            skytex[i] = texs[i].toStdString();

    m_skybox->setTextures(skytex);

    m_skybox->setEnable(true);
}

void QTBEngine::skyboxClear()
{
    m_skybox->clear();
}

void QTBEngine::fogApply(tbe::Vector4f color, float start, float end)
{
    m_fog->setColor(color);
    m_fog->setStart(start);
    m_fog->setEnd(end);

    m_fog->setEnable(true);
}

void QTBEngine::fogClear()
{
    m_fog->clear();
}

tbe::scene::SceneParser* QTBEngine::sceneParser() const
{
    return m_sceneParser;
}

void QTBEngine::setSceneAmbiant(const tbe::Vector3f& value)
{
    m_sceneManager->setAmbientLight(math::vec34(value));
}

void QTBEngine::selectNode(QNodeInteractor* qnode)
{
    m_lastSelectedNode = m_selectedNode;
    m_selectedNode = qnode;

    m_selbox->Node::setEnable(true);

    m_selectedNode->select();

    if(m_gridEnable)
        toggleGridDisplay(true);
}

void QTBEngine::deselectNode()
{
    if(m_selectedNode)
    {
        m_lastSelectedNode = m_selectedNode;
        m_selectedNode = NULL;
    }

    m_selbox->Node::setEnable(false);
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

void QTBEngine::unregisterInterface(QNodeInteractor* node)
{
    m_nodeInterface.remove(node->target());
}

void QTBEngine::registerInterface(QNodeInteractor* node)
{
    m_nodeInterface[node->target()] = node;
}

QNodeInteractor* QTBEngine::interface(tbe::scene::Node* node)
{
    if(m_nodeInterface.contains(node))
        return m_nodeInterface[node];
    else
        return NULL;
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

    m_penarea->Node::setEnable(false);
}

void QTBEngine::selectDrawTool()
{
    m_currentTool = &m_toolMode[DRAW_TOOL];

    setCursor(m_currentTool->cursor);

    m_penarea->Node::setEnable(true);
    m_penarea->Node::setPos(m_curCursor3D);
}

PenAreaInterface::PenAreaInterface()
{
    m_areaSize = 0;
    m_elemGap = 1;
    m_elemCount = 1;
}

void PenAreaInterface::setScaleRange(tbe::Vector2f scale)
{
    this->m_scaleRange = scale;
}

tbe::Vector2f PenAreaInterface::getScaleRange() const
{
    return m_scaleRange;
}

void PenAreaInterface::setRotationRange(tbe::Vector2f rotation)
{
    this->m_rotationRange = rotation;
}

tbe::Vector2f PenAreaInterface::getRotationRange() const
{
    return m_rotationRange;
}

void PenAreaInterface::setElemCount(int elemCount)
{
    this->m_elemCount = elemCount;
}

int PenAreaInterface::getElemCount() const
{
    return m_elemCount;
}

void PenAreaInterface::setElemGap(double elemGap)
{
    this->m_elemGap = elemGap;
}

double PenAreaInterface::getElemGap() const
{
    return m_elemGap;
}

void PenAreaInterface::setAreaSize(double areaSize)
{
    this->m_areaSize = areaSize;
}

double PenAreaInterface::getAreaSize() const
{
    return m_areaSize;
}

void PenAreaInterface::setRotationAxe(bool x, bool y, bool z)
{
    rotX = x;
    rotY = y;
    rotZ = z;
}

void PenAreaInterface::setScaleAxe(bool x, bool y, bool z)
{
    scaleX = x;
    scaleY = y;
    scaleZ = z;
}

void PenAreaInterface::setNotOnFloor()
{
    onFloor = 0;
}

void PenAreaInterface::setCenterOnFloor()
{
    onFloor = 1;
}

void PenAreaInterface::setBaseOnFloor()
{
    onFloor = 2;
}

PenArea::PenArea(tbe::scene::MeshParallelScene* parallelScene) : Sphere(parallelScene)
{
    setName("pencile");

    setAreaSize(0);
}

void PenArea::setAreaSize(double areaSize)
{
    using namespace tbe;
    using namespace scene;

    this->m_areaSize = areaSize;

    build(std::max(areaSize, 1.0), 40, 40);
    getMaterial("main")->enable(Material::COLORED | Material::BLEND_MOD | Material::VERTEX_SORT_CULL_TRICK);
    getMaterial("main")->disable(Material::LIGHTED | Material::FOGED);
    getMaterial("main")->setColor(Vector4f(0, 1, 0, 0.25));
}

SelBoxInterface::SelBoxInterface()
{
}

SelBox::SelBox(tbe::scene::MeshParallelScene* parallelScene) : Box(parallelScene, 1)
{
    using namespace tbe;
    using namespace scene;

    setName("selection");
    getMaterial("main")->enable(Material::COLORED | Material::BLEND_MOD | Material::VERTEX_SORT_CULL_TRICK);
    getMaterial("main")->disable(Material::LIGHTED | Material::FOGED);
    getMaterial("main")->setColor(Vector4f(0, 0, 1, 0.25));
}

void SelBox::setAround(tbe::scene::Node* node, tbe::Vector4f color)
{
    AABB selAabb = node->getAabb();

    setMatrix(node->getAbsoluteMatrix());
    setPos(node->getAbsoluteMatrix() * selAabb.getCenter());
    setSize(selAabb.getSize() / 2.0f + 0.1f);
    getMaterial("main")->setColor(color);
}
