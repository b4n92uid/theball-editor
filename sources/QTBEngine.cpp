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
    m_mainwin = dynamic_cast<MainWindow*>(parent->parentWidget());

    m_device = NULL;
    m_sceneManager = NULL;

    m_grabCamera = false;

    m_selectedNode = NULL;
    m_lastSelectedNode = NULL;

    m_classFactory = new ClassFactory(m_mainwin);

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

    m_axe = NULL;
    m_grid = NULL;

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_updateTimer->start(16);
}

tbe::scene::Box* QTBEngine::getAxe()
{
    return m_axe;
}

tbe::scene::Grid* QTBEngine::getGrid()
{
    return m_grid;
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

void QTBEngine::placeCamera()
{
    if(!m_gridEnable && m_selectedNode)
        m_centerTarget = m_selectedNode->getTarget()->getAbsoluteMatrix().getPos();
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

    tbe::scene::Node* selnode = m_selectedNode->getTarget();

    Vector3f position = selnode->getMatrix().getPos();
    Quaternion rotation = selnode->getMatrix().getRotate();
    Vector3f scale = selnode->getMatrix().getScale();

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
                math::round(position, gridSize);
            }
            else
            {
                float backY = position.y;
                position.x = m_curCursor3D.x;
                position.z = m_curCursor3D.z;
                math::round(position, gridSize);
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
                Quaternion rota = selnode->getAbsoluteMatrix(false).getRotate();
                transform = rota * (-left * mousePosRel.x * moveSpeed);
                transform -= rota * (target * mousePosRel.y * moveSpeed);
                transform.y = 0;
            }

            position += transform;
        }
    }

    Matrix4 apply;
    apply.setPos(position);
    apply.setRotate(rotation);
    apply.setScale(scale);

    selnode->setMatrix(apply);

    m_selectedNode->update();

    m_eventManager->notify = EventManager::EVENT_NO_EVENT;
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

        foreach(QNodeInteractor* node, m_lockedNode.keys())
        node->getTarget()->setEnable(false);

        if(m_selectedNode)
            m_selectedNode->getTarget()->setEnable(false);

        Vector3f campos = m_camera->getPos();
        Mesh::Array nodes = m_meshScene->findMeshs(campos, Vector3f::normalize(m_curCursor3D - campos));

        if(!nodes.empty())
        {
            Nearest pred = {campos};
            Mesh* nearest = *std::min_element(nodes.begin(), nodes.end(), pred);

            if(m_selectedNode)
                m_selectedNode->getTarget()->setEnable(true);

            if(m_nodeInterface.contains(nearest))
                select(m_nodeInterface[nearest]);
        }
        else
        {
            if(m_selectedNode)
                m_selectedNode->getTarget()->setEnable(true);
        }

        foreach(QNodeInteractor* node, m_lockedNode.keys())
        node->getTarget()->setEnable(true);

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

void QTBEngine::enableGrid(bool stat)
{
    m_gridEnable = stat;

    if(m_gridEnable)
    {
        m_grid->clear();

        Vector2i cuts;

        if(m_selectedNode)
        {
            AABB zone = m_selectedNode->getTarget()->getAabb();
            cuts.x = std::max(zone.getLength() * 2.0f, 16.0f);
            cuts.y = std::max(zone.getLength() * 2.0f, 16.0f);
        }
        else
            cuts = 16;

        Vector2f size = cuts;

        m_grid->build(size, cuts);
        m_grid->getMaterial("main")->disable(scene::Material::FOGED);
        m_grid->setColor(Vector4f(0.5, 0.5, 0.5, 1));

        if(m_selectedNode)
        {
            m_grid->setPos(m_selectedNode->getTarget()->getAbsoluteMatrix().getPos());
        }
    }

    m_grid->setEnable(m_gridEnable);
}

void QTBEngine::pushHistoryStat()
{
    if(!m_selectedNode)
        return;

    if(m_history.size() > 32)
        m_history.pop_front();

    HistoryStat hs = {m_selectedNode->clone(), m_selectedNode};

    m_history.push(hs);

    m_mainwin->ui()->actionUndo->setEnabled(true);
}

void QTBEngine::popHistoryStat()
{
    if(m_history.empty())
        return;

    HistoryStat hs = m_history.pop();

    *hs.destination->getTarget() = *hs.source->getTarget();

    hs.destination->update();

    select(hs.destination);

    if(m_history.empty())
        m_mainwin->ui()->actionUndo->setEnabled(false);
}

void QTBEngine::keyPressEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_DOWN;

    if(ev->key() == Qt::Key_Shift)
    {
        pushHistoryStat();

        if(m_gridEnable)
            setCursor(Qt::ClosedHandCursor);
        else
            setCursor(Qt::BlankCursor);
    }

    if(ev->key() == Qt::Key_Space)
    {
        m_mainwin->ui()->menuEditer->popup(QCursor::pos());
    }

    if(m_selectedNode)
    {
        tbe::scene::Node* selnode = m_selectedNode->getTarget();

        if(ev->key() == Qt::Key_P && !selnode->getParent()->isRoot())
        {
            using namespace tbe::scene;

            Node* parent = selnode->getParent();

            select(m_nodeInterface[parent]);
        }

        if(ev->key() == Qt::Key_Delete)
        {
            using namespace tbe::scene;

            // pushHistoryStat();

            QNodeInteractor* todelete = m_selectedNode;

            deselect();

            delete todelete;
        }

        if(ev->key() == Qt::Key_C)
        {
            try
            {
                using namespace tbe::scene;

                QNodeInteractor* clone = m_selectedNode->clone();

                m_selectedNode->getTarget()->getParent()->addChild(clone->getTarget());

                clone->setup();

                select(clone);
            }
            catch(std::exception& e)
            {
                QMessageBox::warning(parentWidget(), "Erreur", e.what());
            }
        }

        if(ev->key() == Qt::Key_R)
        {
            selnode->getMatrix().setRotate(0);
            m_selectedNode->update();
        }

        if(ev->key() == Qt::Key_S)
        {
            selnode->getMatrix().setScale(1);
            m_selectedNode->update();
        }

        if(ev->key() == Qt::Key_T)
        {
            selnode->getMatrix().setPos(0);
            m_selectedNode->update();
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
            pushHistoryStat();

            m_grid->setEnable(false);
            m_axe->setEnable(false);
            m_meshScene->setInFloor(selnode);
            m_axe->setEnable(true);
            m_grid->setEnable(m_gridEnable);

            m_selectedNode->update();
        }

        if(ev->key() == Qt::Key_E)
        {
            pushHistoryStat();

            m_grid->setEnable(false);
            m_axe->setEnable(false);
            m_meshScene->setInFloor(selnode);
            m_axe->setEnable(true);
            m_grid->setEnable(m_gridEnable);

            Vector3f adjust = selnode->getPos();
            adjust.y += -selnode->getAabb().min.y;

            selnode->setPos(adjust);
            m_selectedNode->update();
        }

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

        if(ev->key() == Qt::Key_Escape)
        {
            deselect();
        }
    }

    if(ev->key() == Qt::Key_G)
    {
        enableGrid(!m_gridEnable);
    }

    else if(ev->key() == Qt::Key_Q)
    {
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

    m_mainwin->zNear(m_sceneManager->getZNear());
    m_mainwin->zFar(m_sceneManager->getZFar());

    emit notifyInitFog(m_fog);
    emit notifyInitSkybox(m_skybox);
    emit notifyInitAmbiant(math::vec43(m_sceneManager->getAmbientLight()));
}

void QTBEngine::loadScene(const QString& filename)
{
    using namespace scene;

    clearScene();

    m_sceneParser->loadScene(filename.toStdString());
    m_sceneParser->buildScene();

    m_selectedNode = NULL;
    m_centerTarget = 0;

    m_axe->setEnable(false);

    m_mainwin->zNear(m_sceneManager->getZNear());
    m_mainwin->zFar(m_sceneManager->getZFar());

    emit notifyInitFog(m_fog);
    emit notifyInitSkybox(m_skybox);
    emit notifyInitAmbiant(math::vec43(m_sceneManager->getAmbientLight()));
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
        tbe::scene::Node* selnode = m_selectedNode->getTarget();

        QString name = QString::fromStdString(selnode->getName());
        tbe::Vector3f pos = selnode->getPos();
        tbe::AABB aabb = selnode->getAabb();

        text += QString("<p>");
        text += QString("Node: %1<br />").arg(name);
        text += QString("|_Pos: %1 %2 %3<br />").arg(pos.x).arg(pos.y).arg(pos.z);
        text += QString("|_Aabb: (%1,%2,%3) (%4,%5,%6)")
                .arg(aabb.min.x).arg(aabb.min.y).arg(aabb.min.z)
                .arg(aabb.max.x).arg(aabb.max.y).arg(aabb.max.z);

        text += QString("</p>");


        if(!selnode->getParent()->isRoot())
        {
            using namespace tbe::scene;

            Node* parent = selnode->getParent();

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

void QTBEngine::meshRegister(QMesh* mesh)
{
    m_nodes.push_back(mesh);
    m_meshs.push_back(mesh);

    m_nodeInterface[mesh] = mesh;
}

QMesh* QTBEngine::meshNew(const QString& filename)
{
    using namespace tbe;
    using namespace scene;

    QMesh* mesh = new QMesh(m_mainwin, OBJMesh(m_meshScene, filename.toStdString()));

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

    meshRegister(mesh);

    mesh->setup();

    return mesh;
}

void QTBEngine::lightRegister(QLight* light)
{
    m_nodes.push_back(light);
    m_lights.push_back(light);

    m_nodeInterface[light] = light;
}

QLight* QTBEngine::lightNew()
{
    QLight* light = new QLight(m_mainwin);

    m_rootNode->addChild(light);

    lightRegister(light);

    light->setup();

    return light;
}

void QTBEngine::particlesRegister(QParticles* particles)
{
    m_nodes.push_back(particles);
    m_particles.push_back(particles);

    m_nodeInterface[particles] = particles;
}

QParticles* QTBEngine::particlesNew()
{
    QParticles* particles = new QParticles(m_mainwin);

    m_rootNode->addChild(particles);

    particlesRegister(particles);

    particles->setup();

    return particles;
}

void QTBEngine::markRegister(QMapMark* mark)
{
    m_nodes.push_back(mark);
    m_marks.push_back(mark);

    m_nodeInterface[mark] = mark;
}

QMapMark* QTBEngine::markNew()
{
    QMapMark* mark = new QMapMark(m_mainwin);

    m_rootNode->addChild(mark);

    markRegister(mark);

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
}

void QTBEngine::select(QNodeInteractor* qnode)
{
    m_mainwin->select(qnode);
}

void QTBEngine::deselectNode()
{
    if(m_selectedNode)
    {
        m_lastSelectedNode = m_selectedNode;
        m_selectedNode = NULL;
    }

    m_axe->setEnable(false);
}

void QTBEngine::deselect()
{
    m_mainwin->deselect();
}

QStringList QTBEngine::usedRessources()
{
    using namespace std;
    using namespace tbe;
    using namespace scene;

    QStringList ressPath;

    foreach(Mesh* mesh, m_meshs)
    {
        vector<string> meshRessPath = mesh->getUsedRessources();

        foreach(string path, meshRessPath)
        ressPath << QString::fromStdString(path);
    }

    foreach(ParticlesEmiter* particles, m_particles)
    {
        ressPath << QString::fromStdString(particles->getTexture().getFilename());
    }

    return ressPath;
}

QNodeInteractor* QTBEngine::fetchInterface(tbe::scene::Node* node)
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
