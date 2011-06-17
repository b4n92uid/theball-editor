/*
 * File:   QTBEngine.cpp
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 13:30
 */

#include "QTBEngine.h"
#include "Tools.h"

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

QTBEngine::QTBEngine(QWidget* parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_device = NULL;
    m_sceneManager = NULL;

    m_grabCamera = false;

    m_selectedNode = NULL;
    m_lastSelectedNode = NULL;

    m_gridEnable = false;

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::OpenHandCursor);
}

QTBEngine::~QTBEngine()
{
    delete m_sceneParser;
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

    m_markScene = new MapMarkParalleScene;
    m_sceneManager->addParallelScene(m_markScene);

    m_sceneParser = new scene::SceneParser(m_sceneManager);
    m_sceneParser->setLightScene(m_lightScene);
    m_sceneParser->setMeshScene(m_meshScene);
    m_sceneParser->setParticlesScene(m_particlesScene);
    m_sceneParser->setMarkScene(m_markScene);

    m_fog = m_sceneManager->getFog();
    m_skybox = m_sceneManager->getSkybox();

    m_camera = new OrbitalCamera;

    m_sceneManager->addCamera(m_camera);

    m_rootNode = m_sceneManager->getRootNode();

    m_axe = NULL;
    m_grid = NULL;

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_updateTimer->start(16);
}

void QTBEngine::placeSelection()
{
    using namespace tbe;
    using namespace scene;

    if(!m_axe || !m_selectedNode)
        return;

    AABB selAabb = m_selectedNode->getAabb();

    m_axe->setEnable(true);
    m_axe->setMatrix(m_selectedNode->getAbsoluteMatrix());
    m_axe->setPos(m_selectedNode->getAbsoluteMatrix() * selAabb.getCenter());

    if(tools::find(m_meshs, m_selectedNode))
    {
        m_axe->setSize(selAabb.getSize() / 2.0f + 0.01f);
        m_axe->setColor(Vector4f(0, 0, 1, 0.25));
    }

    else if(tools::find(m_lights, m_selectedNode))
    {
        m_axe->setSize(0.25);
        m_axe->setColor(Vector4f(1, 1, 1, 0.25));
    }

    else if(scene::ParticlesEmiter * emiter = tools::find(m_particles, m_selectedNode))
    {
        Vector3f size = emiter->getBoxSize();
        size.y = 1;

        m_axe->setSize(size / 2.0f + 0.1f);
        m_axe->setColor(Vector4f(0, 1, 1, 0.25));
        m_axe->setPos(m_selectedNode->getAbsoluteMatrix().getPos() + size / 2.0f);
    }

    else if(tools::find(m_marks, m_selectedNode))
    {
        m_axe->setSize(0.25);
        m_axe->setColor(Vector4f(1, 1, 0, 0.25));
    }

    else
    {
        m_axe->setSize(0.5);
        m_axe->setColor(Vector4f(1, 0, 0, 0.25));
    }

    m_grid->setPos(Vector3f::Y(m_axe->getPos().y));
}

void QTBEngine::setupSelection()
{
    using namespace scene;

    m_axe = new Box(m_meshScene, 1);
    m_axe->setName("selection");
    m_axe->getMaterial("main")->enable(Material::COLORED | Material::BLEND_MOD
                                       | Material::VERTEX_SORT_CULL_TRICK);
    m_axe->getMaterial("main")->disable(Material::LIGHTED | Material::FOGED);
    m_axe->setColor(Vector4f(0, 0, 1, 0.25));
    m_axe->setEnable(m_selectedNode);
    m_rootNode->addChild(m_axe);

    m_grid = new Grid(m_meshScene, 8, 8);
    m_grid->setName("grid");
    m_grid->getMaterial("main")->disable(Material::FOGED);
    m_grid->setColor(Vector4f(0.5, 0.5, 0.5, 1));
    m_grid->setEnable(false);

    m_rootNode->addChild(m_grid);
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

void QTBEngine::moveApply()
{
    using namespace scene;

    if(!m_selectedNode)
        return;

    if(!m_eventManager->keyState[EventManager::KEY_LSHIFT])
        return;

    Vector3f position = m_selectedNode->getMatrix().getPos();
    Quaternion rotation = m_selectedNode->getMatrix().getRotate();
    Vector3f scale = m_selectedNode->getMatrix().getScale();

    // Movement ----------------------------------------------------------------

    Vector3f gridSize(1);

    if(m_eventManager->notify == EventManager::EVENT_MOUSE_MOVE)
    {
        if(m_gridEnable)
        {
            if(m_eventManager->keyState[EventManager::KEY_LCTRL])
                gridSize /= 2.0f;

            if(m_eventManager->keyState[EventManager::KEY_LALT])
            {
                setCursor(Qt::BlankCursor);

                position.y += m_eventManager->mousePosRel.y / 2;
                tools::round(position, gridSize);
            }
            else
            {
                float backY = position.y;
                position.x = m_curCursor3D.x;
                position.z = m_curCursor3D.z;
                tools::round(position, gridSize);
                position.y = backY;
            }
        }
        else
        {
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
                transform = -left * mousePosRel.x * moveSpeed;
                transform -= target * mousePosRel.y * moveSpeed;
                transform.y = 0;
            }

            position += transform;
        }
    }

    Matrix4 apply;
    apply.setPos(position);
    apply.setRotate(rotation);
    apply.setScale(scale);

    m_selectedNode->setMatrix(apply);

    if(!m_gridEnable)
        m_centerTarget = m_selectedNode->getAbsoluteMatrix() * m_selectedNode->getAabb().getCenter();

    m_eventManager->notify = EventManager::EVENT_NO_EVENT;

    updateSelected();

    placeSelection();
}

void QTBEngine::paintGL()
{
    moveApply();

    Vector3f campos = m_camera->getCenter();

    if(m_centerTarget - campos > 0.01)
        m_camera->setCenter(campos + (m_centerTarget - campos) / 8.0f);

    m_device->beginScene();
    m_sceneManager->render();
    m_curCursor3D = m_sceneManager->screenToWorld(m_curCursorPos);
    m_device->endScene();
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
            return (node1->getPos() - cameraPos) < (node2->getPos() - cameraPos);
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
        return (mesh1->getAbsoluteMatrix().getPos() - center <
                mesh2->getAbsoluteMatrix().getPos() - center);
    }
};

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
        if(m_meshScene->getSceneAabb().isInner(m_curCursor3D))
            m_centerTarget = m_curCursor3D;
    }

    else if(ev->button() == Qt::RightButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT] = 1;

        using namespace tbe;
        using namespace tbe::scene;

        m_axe->setEnable(false);
        m_grid->setEnable(false);

        foreach(Node* node, m_lockedNode.keys())
        node->setEnable(false);

        if(m_selectedNode)
            m_selectedNode->setEnable(false);

        Vector3f campos = m_camera->getPos();
        Mesh::Array nodes = m_meshScene->findMeshs(campos, Vector3f::normalize(m_curCursor3D - campos));

        if(!nodes.empty())
        {
            Nearest pred = {campos};
            Mesh* nearest = *std::min_element(nodes.begin(), nodes.end(), pred);

            if(m_selectedNode)
                m_selectedNode->setEnable(true);

            meshSelect(nearest);
            emit notifyMeshSelect(nearest);
        }
        else
        {
            if(m_selectedNode)
                m_selectedNode->setEnable(true);
        }

        foreach(Node* node, m_lockedNode.keys())
        node->setEnable(true);

        m_axe->setEnable(m_selectedNode);
        m_grid->setEnable(m_gridEnable);
    }
}

void QTBEngine::mouseReleaseEvent(QMouseEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_MOUSE_UP;

    if(ev->button() == Qt::LeftButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_LEFT] = 0;
        m_grabCamera = false;

        setCursor(Qt::OpenHandCursor);
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
    m_curCursorPos = qptovec(ev->pos());
    m_curCursorPos.y = size().height() - m_curCursorPos.y;

    Vector2f mousePosRel = m_curCursorPos - m_lastCursorPos;

    m_eventManager->notify = EventManager::EVENT_MOUSE_MOVE;
    m_eventManager->mousePos = m_curCursorPos;
    m_eventManager->mousePosRel = mousePosRel;

    m_lastCursorPos = m_curCursorPos;

    if(m_grabCamera)
    {
        m_camera->onEvent(m_eventManager);
    }

    if(m_eventManager->keyState[EventManager::KEY_LSHIFT])
    {
        if(!m_gridEnable)
            QCursor::setPos(mapToGlobal(QPoint(size().width() / 2, size().height() / 2)));
    }
}

void QTBEngine::keyPressEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_DOWN;

    if(ev->key() == Qt::Key_Shift)
    {
        if(m_gridEnable)
            setCursor(Qt::ClosedHandCursor);
        else
            setCursor(Qt::BlankCursor);
    }

    else if(m_selectedNode)
    {
        if(ev->key() == Qt::Key_P && !m_selectedNode->getParent()->isRoot())
        {
            using namespace tbe::scene;

            if(Mesh * mesh = tools::find(m_meshs, m_selectedNode->getParent()))
            {
                meshSelect(mesh);
                emit notifyMeshSelect(mesh);
            }

            else if(Light * light = tools::find(m_lights, m_selectedNode->getParent()))
            {
                lightSelect(light);
                emit notifyLightSelect(light);
            }

            else if(ParticlesEmiter * particles = tools::find(m_particles, m_selectedNode->getParent()))
            {
                particlesSelect(particles);
                emit notifyParticlesSelect(particles);
            }

            else if(MapMark * mark = tools::find(m_marks, m_selectedNode->getParent()))
            {
                markSelect(mark);
                emit notifyMarkSelect(mark);
            }
        }

        if(ev->key() == Qt::Key_Delete)
        {
            using namespace tbe::scene;

            if(Mesh * mesh = tools::find(m_meshs, m_selectedNode))
            {
                meshDelete(mesh);
                emit notifyMeshDelete(mesh);

                deselect();
                emit notifyDeselect();
            }

            else if(Light * light = tools::find(m_lights, m_selectedNode))
            {
                lightDelete(light);
                emit notifyLightDelete(light);

                deselect();
                emit notifyDeselect();
            }

            else if(ParticlesEmiter * particles = tools::find(m_particles, m_selectedNode))
            {
                particlesDelete(particles);
                emit notifyParticlesDelete(particles);

                deselect();
                emit notifyDeselect();
            }

            else if(MapMark * mark = tools::find(m_marks, m_selectedNode))
            {
                markDelete(mark);
                emit notifyMarkDelete(mark);

                deselect();
                emit notifyDeselect();
            }
        }

        if(ev->key() == Qt::Key_C)
        {
            try
            {
                using namespace tbe::scene;

                if(Mesh * mesh = tools::find(m_meshs, m_selectedNode))
                {
                    Mesh* clone = meshClone(mesh);
                    meshSelect(clone);
                    emit notifyMeshSelect(clone);
                }

                else if(Light * light = tools::find(m_lights, m_selectedNode))
                {
                    Light* clone = lightClone(light);
                    lightSelect(clone);
                    emit notifyLightSelect(clone);
                }

                else if(ParticlesEmiter * particles = tools::find(m_particles, m_selectedNode))
                {
                    ParticlesEmiter* clone = particlesClone(particles);
                    particlesSelect(clone);
                    emit notifyParticlesSelect(clone);
                }

                else if(MapMark * mark = tools::find(m_marks, m_selectedNode))
                {
                    MapMark* clone = markClone(mark);
                    markSelect(clone);
                    emit notifyMarkSelect(clone);
                }
            }
            catch(std::exception& e)
            {
                QMessageBox::warning(parentWidget(), "Erreur", e.what());
            }
        }

        if(ev->key() == Qt::Key_R)
        {
            m_selectedNode->getMatrix().setRotate(0);

            updateSelected();
        }

        if(ev->key() == Qt::Key_S)
        {
            m_selectedNode->getMatrix().setScale(1);

            updateSelected();
        }

        if(ev->key() == Qt::Key_T)
        {
            m_selectedNode->getMatrix().setPos(0);

            updateSelected();
        }

        if(ev->key() == Qt::Key_L)
        {
            if(m_lockedNode.contains(m_selectedNode))
                m_lockedNode.remove(m_selectedNode);
            else
                m_lockedNode[m_selectedNode] = true;
        }

        if(ev->key() == Qt::Key_F)
        {
            m_grid->setEnable(false);
            m_axe->setEnable(false);
            m_meshScene->setInFloor(m_selectedNode);
            m_axe->setEnable(true);
            m_grid->setEnable(m_gridEnable);

            updateSelected();
        }

        if(ev->key() == Qt::Key_E)
        {
            m_grid->setEnable(false);
            m_axe->setEnable(false);
            m_meshScene->setInFloor(m_selectedNode);
            m_axe->setEnable(true);
            m_grid->setEnable(m_gridEnable);

            Vector3f adjust = m_selectedNode->getPos();
            adjust.y += -m_selectedNode->getAabb().min.y;

            m_selectedNode->setPos(adjust);

            updateSelected();
        }

        else if(ev->key() == Qt::Key_Up)
        {
            Vector3f pos = m_selectedNode->getPos();
            if(ev->modifiers() & Qt::ALT)
                pos.y += 1;
            else
                pos.z += 1;
            m_selectedNode->setPos(pos);

            updateSelected();
        }

        else if(ev->key() == Qt::Key_Down)
        {
            Vector3f pos = m_selectedNode->getPos();
            if(ev->modifiers() & Qt::ALT)
                pos.y -= 1;
            else
                pos.z -= 1;
            m_selectedNode->setPos(pos);

            updateSelected();
        }

        else if(ev->key() == Qt::Key_Left)
        {
            Vector3f pos = m_selectedNode->getPos();
            pos.x += 1;
            m_selectedNode->setPos(pos);

            updateSelected();
        }

        else if(ev->key() == Qt::Key_Right)
        {
            Vector3f pos = m_selectedNode->getPos();
            pos.x -= 1;
            m_selectedNode->setPos(pos);

            updateSelected();
        }

        else if(ev->key() == Qt::Key_Escape)
        {
            deselect();
        }
    }

    if(ev->key() == Qt::Key_G)
    {
        m_gridEnable = !m_gridEnable;

        if(m_gridEnable)
        {
            m_grid->clear();

            AABB sceneAabb = m_meshScene->getSceneAabb();

            Vector2i cuts;
            cuts.x = sceneAabb.max.x - sceneAabb.min.x;
            cuts.y = sceneAabb.max.z - sceneAabb.min.z;
            cuts = tools::nextPow2(cuts);

            Vector2f size = cuts;

            m_grid->setup(size, cuts);
            m_grid->getMaterial("main")->disable(scene::Material::FOGED);
            m_grid->setColor(Vector4f(0.5, 0.5, 0.5, 1));
        }

        m_grid->setEnable(m_gridEnable);
    }

    else if(ev->key() == Qt::Key_Q)
    {
        if(m_lastSelectedNode)
            select(m_lastSelectedNode);
    }

    else if(ev->key() == Qt::Key_Plus)
    {
        m_camera->setDistance(m_camera->getDistance() - 1);
    }

    else if(ev->key() == Qt::Key_Minus)
    {
        m_camera->setDistance(m_camera->getDistance() + 1);
    }

    else if(ev->key() == Qt::Key_1)
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

    else if(ev->key() == Qt::Key_2)
    {
        m_camera->rotate(Vector2f(0, 22.5));
        m_camera->setCenter(m_centerTarget);
    }

    else if(ev->key() == Qt::Key_3)
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

    else if(ev->key() == Qt::Key_4)
    {
        m_camera->rotate(Vector2f(22.5, 0));
        m_camera->setCenter(m_centerTarget);
    }

    else if(ev->key() == Qt::Key_5)
    {
        m_camera->setRotate(Vector2f(45, -45));
        m_centerTarget = 0;
        m_camera->setDistance(20);
    }

    else if(ev->key() == Qt::Key_6)
    {
        m_camera->rotate(Vector2f(-22.5, 0));
        m_camera->setCenter(m_centerTarget);
    }

    else if(ev->key() == Qt::Key_7)
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

    else if(ev->key() == Qt::Key_8)
    {
        m_camera->rotate(Vector2f(0, -22.5));
        m_camera->setCenter(m_centerTarget);
    }

    int c = std::tolower(ev->key());
    translate(c);

    m_eventManager->keyState[c] = 1;
}

void QTBEngine::keyReleaseEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_UP;

    if(ev->key() == Qt::Key_Shift)
        setCursor(Qt::OpenHandCursor);

    int c = std::tolower(ev->key());
    translate(c);

    m_eventManager->keyState[c] = 0;
}

void QTBEngine::saveScene(const QString& filename)
{
    using namespace scene;

    m_sceneParser->exclude(m_axe);
    m_sceneParser->exclude(m_grid);

    m_sceneParser->prepareScene();
    m_sceneParser->saveScene(filename.toStdString());
}

void QTBEngine::clearScene()
{
    m_rootNode->clearAllChild();

    m_sceneManager->clearParallelScenes(false);

    m_nodes.clear();
    m_meshs.clear();
    m_particles.clear();
    m_lights.clear();
    m_marks.clear();

    m_fog->clear();
    m_skybox->clear();

    m_axe = NULL;
    m_grid = NULL;
    m_selectedNode = NULL;

    m_centerTarget = 0;
    m_lastCursorPos = 0;
    m_curCursorPos = 0;
    m_curCursor3D = 0;

    m_grabCamera = false;
    m_gridEnable = false;

    Texture::resetCache();

    m_lockedNode.clear();

    setupSelection();

    emit notifyInitFog(m_fog);
    emit notifyInitSkybox(m_skybox);
    emit notifyInitAmbiant(vec43(m_sceneManager->getAmbientLight()));
}

void QTBEngine::loadScene(const QString& filename)
{
    using namespace scene;

    clearScene();

    m_sceneParser->loadScene(filename.toStdString());
    m_sceneParser->buildScene();

    rebuildList();

    m_selectedNode = NULL;
    m_centerTarget = 0;

    m_axe->setEnable(false);

    emit notifyInitFog(m_fog);
    emit notifyInitSkybox(m_skybox);
    emit notifyInitAmbiant(vec43(m_sceneManager->getAmbientLight()));
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

void QTBEngine::rebuildList()
{
    using namespace scene;

    m_meshs.clear();
    for(Iterator<Mesh*> it = m_meshScene->iterator(); it; it++)
    {
        if(*it == m_axe || *it == m_grid)
            continue;

        m_meshs.push_back(*it);
    }

    m_particles.clear();
    for(Iterator<ParticlesEmiter*> it = m_particlesScene->iterator(); it; it++)
        m_particles.push_back(*it);

    m_lights.clear();
    for(Iterator<Light*> it = m_lightScene->iterator(); it; it++)
        m_lights.push_back(*it);

    m_marks.clear();
    for(Iterator<MapMark*> it = m_markScene->iterator(); it; it++)
        m_marks.push_back(*it);

    m_nodes.clear();

    m_nodes.insert(m_nodes.end(), m_meshs.begin(), m_meshs.end());
    m_nodes.insert(m_nodes.end(), m_lights.begin(), m_lights.end());
    m_nodes.insert(m_nodes.end(), m_particles.begin(), m_particles.end());
    m_nodes.insert(m_nodes.end(), m_marks.begin(), m_marks.end());

    RootSort pred = {m_rootNode};
    std::sort(m_nodes.begin(), m_nodes.end(), pred);

    emit notifyListRebuild();

    foreach(Node* node, m_nodes)
    {
        if(Mesh * mesh = tools::find(m_meshs, node))
            emit notifyMeshAdd(mesh);

        else if(Light * light = tools::find(m_lights, node))
            emit notifyLightAdd(light);

        else if(ParticlesEmiter * particles = tools::find(m_particles, node))
            emit notifyParticlesAdd(particles);

        else if(MapMark * mark = tools::find(m_marks, node))
            emit notifyMarkAdd(mark);
    }
}

tbe::scene::Node* QTBEngine::rootNode()
{
    return m_rootNode;
}

void QTBEngine::fillTextInfo(QLabel* label)
{
    QString text;

    if(m_gridEnable)
        text += "<p><b>Déplacement en grille</b></p>";

    if(m_lockedNode.contains(m_selectedNode))
        text += "<p><b>Noeud verrouiller</b></p>";

    text += QString("<p>Curseur 3D: %1 %2 %3</p>").arg(m_curCursor3D.x).arg(m_curCursor3D.y).arg(m_curCursor3D.z);

    if(m_selectedNode)
    {
        QString name = QString::fromStdString(m_selectedNode->getName());
        tbe::Vector3f pos = m_selectedNode->getPos();
        tbe::AABB aabb = m_selectedNode->getAabb();

        text += QString("<p>");
        text += QString("Node: %1<br />").arg(name);
        text += QString("|_Pos: %1 %2 %3<br />").arg(pos.x).arg(pos.y).arg(pos.z);
        text += QString("|_Aabb: (%1,%2,%3) (%4,%5,%6)")
                .arg(aabb.min.x).arg(aabb.min.y).arg(aabb.min.z)
                .arg(aabb.max.x).arg(aabb.max.y).arg(aabb.max.z);

        text += QString("</p>");


        if(!m_selectedNode->getParent()->isRoot())
        {
            using namespace tbe::scene;

            Node* parent = m_selectedNode->getParent();

            QString name = QString::fromStdString(parent->getName());
            tbe::Vector3f pos = parent->getPos();

            text += QString("<p>");
            text += QString("Parent: %1<br />").arg(name);
            text += QString("|_Pos: %1 %2 %3").arg(pos.x).arg(pos.y).arg(pos.z);
            text += QString("</p>");
        }
    }

    else
    {

        text += QString("<p><i>Pas de séléction</i></p>");
    }

    label->setText(text);
}

tbe::scene::Mesh* QTBEngine::meshNew(const QString& filename)
{
    using namespace tbe;
    using namespace scene;

    Mesh* mesh = NULL;

    if(filename.endsWith("ball3d"))
        mesh = new Ball3DMesh(m_meshScene, filename.toStdString());

    else if(filename.endsWith("obj"))
        mesh = new OBJMesh(m_meshScene, filename.toStdString());

    else
        throw Exception("Format du fichier non reconnue \"%s\"", filename.toStdString().c_str());

    m_rootNode->addChild(mesh);

    return mesh;
}

void QTBEngine::meshSelect(tbe::scene::Mesh* mesh)
{
    if(m_selectedNode && m_selectedNode != mesh)
        m_lastSelectedNode = m_selectedNode;

    m_selectedNode = mesh;

    m_centerTarget = m_selectedNode->getAbsoluteMatrix().getPos();

    placeSelection();
}

void QTBEngine::meshRegister(tbe::scene::Mesh* mesh)
{
    m_nodes.push_back(mesh);
    m_meshs.push_back(mesh);
}

void QTBEngine::meshDelete(tbe::scene::Mesh* mesh)
{
    removeMesh(mesh);

    delete mesh;
}

tbe::scene::Mesh* QTBEngine::meshClone(tbe::scene::Mesh* mesh)
{
    using namespace tbe::scene;

    Mesh* newmesh = mesh->clone();

    mesh->getParent()->addChild(newmesh);

    rebuildList();

    return newmesh;
}

tbe::scene::Light* QTBEngine::lightNew()
{
    scene::Light* light = new scene::Light(m_lightScene);
    m_rootNode->addChild(light);

    return light;
}

void QTBEngine::lightRegister(tbe::scene::Light* light)
{
    m_nodes.push_back(light);
    m_lights.push_back(light);
}

void QTBEngine::lightDelete(tbe::scene::Light* light)
{
    removeLight(light);

    delete light;
}

void QTBEngine::lightSelect(tbe::scene::Light* light)
{
    if(m_selectedNode && m_selectedNode != light)
        m_lastSelectedNode = m_selectedNode;

    m_selectedNode = light;

    m_centerTarget = m_selectedNode->getAbsoluteMatrix().getPos();

    placeSelection();
}

tbe::scene::Light* QTBEngine::lightClone(tbe::scene::Light* light)
{
    using namespace tbe::scene;

    Light* newlight = light->clone();

    light->getParent()->addChild(newlight);

    rebuildList();

    return newlight;
}

tbe::scene::ParticlesEmiter* QTBEngine::particlesNew()
{
    scene::ParticlesEmiter* light = new scene::ParticlesEmiter(m_particlesScene);
    m_rootNode->addChild(light);

    return light;
}

void QTBEngine::particlesRegister(tbe::scene::ParticlesEmiter* particles)
{
    m_nodes.push_back(particles);
    m_particles.push_back(particles);
}

void QTBEngine::particlesDelete(tbe::scene::ParticlesEmiter* particles)
{
    removeParticules(particles);

    delete particles;
}

void QTBEngine::particlesSelect(tbe::scene::ParticlesEmiter* particles)
{
    if(m_selectedNode && m_selectedNode != particles)
        m_lastSelectedNode = m_selectedNode;

    m_selectedNode = particles;

    m_centerTarget = m_selectedNode->getAbsoluteMatrix().getPos();

    placeSelection();
}

tbe::scene::ParticlesEmiter* QTBEngine::particlesClone(tbe::scene::ParticlesEmiter* particles)
{
    using namespace tbe::scene;

    ParticlesEmiter* newparticles = particles->clone();

    particles->getParent()->addChild(newparticles);

    rebuildList();

    return newparticles;
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

tbe::scene::SceneParser* QTBEngine::getSceneParser() const
{
    return m_sceneParser;
}

void QTBEngine::setSceneAmbiant(const tbe::Vector3f& value)
{
    m_sceneManager->setAmbientLight(vec34(value));
}

void QTBEngine::select(tbe::scene::Node* node)
{
    using namespace scene;

    if(Mesh * mesh = tools::find(m_meshs, node))
    {
        meshSelect(mesh);
        emit notifyMeshSelect(mesh);
    }

    else if(Light * light = tools::find(m_lights, node))
    {
        lightSelect(light);
        emit notifyLightSelect(light);
    }

    else if(ParticlesEmiter * particles = tools::find(m_particles, node))
    {
        particlesSelect(particles);
        emit notifyParticlesSelect(particles);
    }

    else if(MapMark * mark = tools::find(m_marks, node))
    {
        markSelect(mark);
        emit notifyMarkSelect(mark);
    }
}

void QTBEngine::deselect()
{
    m_lastSelectedNode = m_selectedNode;
    m_selectedNode = NULL;

    m_axe->setEnable(false);
}

void QTBEngine::updateSelected()
{
    using namespace scene;

    if(Mesh * mesh = tools::find(m_meshs, m_selectedNode))
        emit notifyMeshUpdate(mesh);

    else if(Light * light = tools::find(m_lights, m_selectedNode))
        emit notifyLightUpdate(light);

    else if(ParticlesEmiter * particles = tools::find(m_particles, m_selectedNode))
        emit notifyParticlesUpdate(particles);

    else if(MapMark * mark = tools::find(m_marks, m_selectedNode))
        emit notifyMarkUpdate(mark);
}

tbe::scene::MapMark* QTBEngine::markNew()
{
    tbe::scene::MapMark* mark = new tbe::scene::MapMark(m_markScene);

    m_rootNode->addChild(mark);

    return mark;
}

void QTBEngine::markRegister(tbe::scene::MapMark* mark)
{
    m_nodes.push_back(mark);
    m_marks.push_back(mark);
}

void QTBEngine::markDelete(tbe::scene::MapMark* mark)
{
    removeMark(mark);

    delete mark;
}

void QTBEngine::markSelect(tbe::scene::MapMark* mark)
{
    if(m_selectedNode && m_selectedNode != mark)
        m_lastSelectedNode = m_selectedNode;

    m_selectedNode = mark;

    m_centerTarget = m_selectedNode->getAbsoluteMatrix().getPos();

    placeSelection();
}

tbe::scene::MapMark* QTBEngine::markClone(tbe::scene::MapMark* mark)
{
    using namespace tbe::scene;

    MapMark* newmark = mark->clone();

    mark->getParent()->addChild(newmark);

    rebuildList();

    return newmark;
}
