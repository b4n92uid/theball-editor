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

    m_sceneParser = new scene::SceneParser(m_sceneManager);
    m_sceneParser->setLightScene(m_lightScene);
    m_sceneParser->setMeshScene(m_meshScene);
    m_sceneParser->setParticlesScene(m_particlesScene);

    m_fog = m_sceneManager->getFog();
    m_skybox = m_sceneManager->getSkybox();

    m_ffcamera = new FreeFlyCamera;
    m_orbcamera = new OrbitalCamera;

    m_sceneManager->addCamera(m_ffcamera);
    m_sceneManager->addCamera(m_orbcamera);

    m_rootNode = m_sceneManager->getRootNode();

    m_camera = m_orbcamera;

    setupSelection();

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_updateTimer->start(16);
}

void QTBEngine::placeSelection()
{
    if(!m_axe)
        return;

    AABB selAabb = m_selectedNode->getAabb();

    if(selAabb.max < 0.0001 && selAabb.min < 0.0001)
    {
        selAabb = 0.5;
        m_axe->applyColor("main", Vector4f(1, 0, 0, 0.25));
    }
    else
        m_axe->applyColor("main", Vector4f(0, 0, 1, 0.25));

    m_axe->setMatrix(m_selectedNode->getAbsoluteMatrix());
    m_axe->setPos(m_selectedNode->getAbsoluteMatrix() * selAabb.getCenter());
    m_axe->setSize(selAabb.getSize() / 2.0f + 0.01f);

    m_axe->setEnable(m_selectedNode);
}

void QTBEngine::setupSelection()
{
    using namespace scene;

    m_axe = new Box(m_meshScene, 1);
    m_axe->getMaterial("main")->enable(Material::COLOR | Material::BLEND_MOD);
    m_axe->applyColor("main", Vector4f(0, 0, 1, 0.25));
    m_axe->setEnable(m_selectedNode);
    m_rootNode->addChild(m_axe);
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

    // Rotation ----------------------------------------------------------------

    if(m_eventManager->notify == EventManager::EVENT_KEY_DOWN)
    {
        Quaternion apply;

        float angle = M_PI / 8;

        if(m_eventManager->keyState[EventManager::KEY_LCTRL])
            angle = M_PI / 2;

        if(m_eventManager->keyState['a'])
            apply.setAxisAngle(angle, Vector3f::X());

        if(m_eventManager->keyState['z'])
            apply.setAxisAngle(-angle, Vector3f::X());

        if(m_eventManager->keyState['q'])
            apply.setAxisAngle(angle, Vector3f::Y());

        if(m_eventManager->keyState['s'])
            apply.setAxisAngle(-angle, Vector3f::Y());

        if(m_eventManager->keyState['w'])
            apply.setAxisAngle(angle, Vector3f::Z());

        if(m_eventManager->keyState['x'])
            apply.setAxisAngle(-angle, Vector3f::Z());

        rotation = apply * rotation;
    }

    // Movement ----------------------------------------------------------------

    if(m_eventManager->notify == EventManager::EVENT_MOUSE_MOVE)
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

    Matrix4 apply;
    apply.setPos(position);
    apply.setScale(scale);
    apply.setRotate(rotation);

    m_selectedNode->setMatrix(apply);

    m_centerTarget = m_selectedNode->getAbsoluteMatrix() * m_selectedNode->getAabb().getCenter();

    if(Mesh * mesh = tools::find(m_meshs, m_selectedNode))
        emit notifyMeshUpdate(mesh);

    else if(Light * light = tools::find(m_lights, m_selectedNode))
        emit notifyLightUpdate(light);

    else if(ParticlesEmiter * particles = tools::find(m_particles, m_selectedNode))
        emit notifyParticlesUpdate(particles);

    m_eventManager->notify = EventManager::EVENT_NO_EVENT;

    placeSelection();
}

void QTBEngine::paintGL()
{
    moveApply();

    Vector3f campos = m_orbcamera->getCenter();

    if(m_centerTarget - campos > 0.01)
        m_orbcamera->setCenter(campos + (m_centerTarget - campos) / 8.0f);

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
            return (node1->getPos() - cameraPos) < (node2->getPos() - cameraPos);
        else if(type == 2)
            return node1->getAabb().getSize() < node2->getAabb().getSize();
        else
            return false;
    }
};

void QTBEngine::enterEvent(QEvent * event)
{
    setFocus();
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
        m_centerTarget = m_curCursor3D;
    }

    else if(ev->button() == Qt::RightButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT] = 1;

        using namespace tbe;
        using namespace tbe::scene;

        m_selectedNode = NULL;

        SelectionSort sortfunc;
        sortfunc.cameraPos = m_camera->getPos();

        sortfunc.type = 1;
        std::sort(m_meshs.begin(), m_meshs.end(), sortfunc);

        sortfunc.type = 2;
        std::sort(m_meshs.begin(), m_meshs.end(), sortfunc);

        foreach(Mesh * node, m_meshs)
        {
            if(node->Mesh::getAbsolutAabb().add(Vector3f(0.5f)).isInner(m_curCursor3D))
            {
                meshSelect(node);
                emit notifyMeshSelect(node);
                break;
            }
        }

        m_axe->setEnable(m_selectedNode);
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
        QCursor::setPos(mapToGlobal(QPoint(size().width() / 2, size().height() / 2)));
    }
}

void QTBEngine::keyPressEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_DOWN;

    if(ev->key() == Qt::Key_Shift)
    {
        setCursor(Qt::BlankCursor);
    }

    else
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
        }

        if(ev->key() == Qt::Key_Delete)
        {
            using namespace tbe::scene;

            if(Mesh * mesh = tools::find(m_meshs, m_selectedNode))
                meshDelete(mesh);

            else if(Light * light = tools::find(m_lights, m_selectedNode))
                lightDelete(light);
        }

        if(ev->key() == Qt::Key_C)
        {
            using namespace tbe::scene;

            if(Mesh * mesh = tools::find(m_meshs, m_selectedNode))
                meshClone(mesh);

            else if(Light * light = tools::find(m_lights, m_selectedNode))
                lightClone(light);
        }

        if(ev->key() == Qt::Key_R)
        {
            m_selectedNode->getMatrix().setRotate(0);
        }

        if(ev->key() == Qt::Key_S)
        {
            m_selectedNode->getMatrix().setScale(1);
        }

        if(ev->key() == Qt::Key_G)
        {
            Vector3f pos = m_selectedNode->getPos();
            tools::round(pos, Vector3f(1));

            m_selectedNode->setPos(pos);
        }

        if(ev->key() == Qt::Key_F)
        {
            m_meshScene->setInFloor(m_selectedNode);
        }

        if(ev->key() == Qt::Key_E)
        {
            m_meshScene->setInFloor(m_selectedNode);

            Vector3f adjust = m_selectedNode->getPos();
            adjust.y += -m_selectedNode->getAabb().min.y;

            m_selectedNode->setPos(adjust);
        }

        else if(ev->key() == Qt::Key_V)
        {
            swapcontainer(m_camera, m_ffcamera, m_orbcamera);
            m_sceneManager->setCurCamera(m_camera);
        }
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

    m_sceneParser->archive(m_rootNode);
    m_sceneParser->exclude(m_axe);

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

    m_fog->clear();
    m_skybox->clear();

    m_axe = NULL;

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

    rebuildList();

    m_selectedNode = NULL;
    m_centerTarget = 0;

    m_axe->setEnable(false);

    emit notifyInitFog(m_fog);
    emit notifyInitSkybox(m_skybox);
    emit notifyInitAmbiant(vec43(m_sceneManager->getAmbientLight()));
}

void QTBEngine::rebuildList()
{
    using namespace scene;

    emit notifyListRebuild();

    for(Iterator<Mesh*> it = m_meshScene->iterator(); it; it++)
    {
        if(m_axe == *it)
            continue;

        m_meshs.push_back(*it);
        m_nodes.push_back(*it);

        emit notifyMeshAdd(*it);
    }

    for(Iterator<Light*> it = m_lightScene->iterator(); it; it++)
    {
        m_lights.push_back(*it);
        m_nodes.push_back(*it);

        emit notifyLightAdd(*it);
    }

    for(Iterator<ParticlesEmiter*> it = m_particlesScene->iterator(); it; it++)
    {
        m_particles.push_back(*it);
        m_nodes.push_back(*it);

        emit notifyParticlesAdd(*it);
    }
}

tbe::scene::Node* QTBEngine::rootNode()
{
    return m_rootNode;
}

void QTBEngine::fillTextInfo(QLabel* label)
{
    QString text;

    text += QString("Curseur 3D: %1 %2 %3\n\n").arg(m_curCursor3D.x).arg(m_curCursor3D.y).arg(m_curCursor3D.z);

    if(m_selectedNode)
    {
        QString name = m_selectedNode->getName().c_str();
        tbe::Vector3f pos = m_selectedNode->getPos();
        tbe::AABB aabb = m_selectedNode->getAabb();

        text += QString("Node: %1\n").arg(name);
        text += QString("|_Pos: %1 %2 %3\n").arg(pos.x).arg(pos.y).arg(pos.z);
        text += QString("|_Aabb: (%1,%2,%3) (%4,%5,%6)")
                .arg(aabb.min.x).arg(aabb.min.y).arg(aabb.min.z)
                .arg(aabb.max.x).arg(aabb.max.y).arg(aabb.max.z);


        if(!m_selectedNode->getParent()->isRoot())
        {
            using namespace tbe::scene;

            Node* parent = m_selectedNode->getParent();

            QString name = parent->getName().c_str();
            tbe::Vector3f pos = parent->getPos();

            text += "\n\n";
            text += QString("Parent: %1\n").arg(name);
            text += QString("|_Pos: %1 %2 %3").arg(pos.x).arg(pos.y).arg(pos.z);
        }
    }

    else
    {
        text += QString("Pas de séléction");
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
    m_selectedNode = mesh;

    if(m_selectedNode)
    {
        m_centerTarget = m_selectedNode->getAbsoluteMatrix().getPos();

        placeSelection();
    }
}

void QTBEngine::meshRegister(tbe::scene::Mesh* mesh)
{
    m_meshs.push_back(mesh);
    m_nodes.push_back(mesh);

    meshSelect(mesh);

    emit notifyMeshSelect(mesh);
}

void QTBEngine::meshDelete(tbe::scene::Mesh* mesh)
{
    tools::erase(m_nodes, mesh);
    tools::erase(m_meshs, mesh);

    delete mesh;

    meshSelect(NULL);

    emit notifyMeshSelect(NULL);
}

void QTBEngine::meshClone(tbe::scene::Mesh* mesh)
{
    using namespace tbe::scene;

    Mesh* newmesh = mesh->clone();

    mesh->getParent()->addChild(newmesh);

    rebuildList();

    meshSelect(newmesh);
}

tbe::scene::Light* QTBEngine::lightNew()
{
    scene::Light* light = new scene::Light(m_lightScene);
    m_rootNode->addChild(light);

    return light;
}

void QTBEngine::lightRegister(tbe::scene::Light* light)
{
    m_lights.push_back(light);
    m_nodes.push_back(light);

    lightSelect(light);

    emit notifyLightSelect(light);
}

void QTBEngine::lightDelete(tbe::scene::Light* light)
{
    tools::erase(m_nodes, light);
    tools::erase(m_lights, light);

    delete light;

    lightSelect(NULL);

    emit notifyLightSelect(NULL);
}

void QTBEngine::lightSelect(tbe::scene::Light* light)
{
    m_selectedNode = light;

    if(m_selectedNode)
    {
        m_centerTarget = m_selectedNode->getAbsoluteMatrix().getPos();

        placeSelection();
    }
}

void QTBEngine::lightClone(tbe::scene::Light* light)
{
    using namespace tbe::scene;

    Light* newlight = light->clone();

    light->getParent()->addChild(newlight);

    rebuildList();

    lightSelect(newlight);
}

tbe::scene::ParticlesEmiter* QTBEngine::particlesNew()
{
    scene::ParticlesEmiter* light = new scene::ParticlesEmiter(m_particlesScene);
    m_rootNode->addChild(light);

    return light;
}

void QTBEngine::particlesRegister(tbe::scene::ParticlesEmiter* particles)
{
    m_particles.push_back(particles);
    m_nodes.push_back(particles);

    particlesSelect(particles);

    emit notifyParticlesSelect(particles);
}

void QTBEngine::particlesDelete(tbe::scene::ParticlesEmiter* particles)
{
    tools::erase(m_nodes, particles);
    tools::erase(m_particles, particles);

    delete particles;

    particlesSelect(NULL);

    emit notifyParticlesSelect(NULL);
}

void QTBEngine::particlesSelect(tbe::scene::ParticlesEmiter* particles)
{
    m_selectedNode = particles;

    if(m_selectedNode)
    {
        m_centerTarget = m_selectedNode->getAbsoluteMatrix().getPos();

        placeSelection();
    }
}

void QTBEngine::particlesClone(tbe::scene::ParticlesEmiter* particles)
{
    using namespace tbe::scene;

    ParticlesEmiter* newparticles = particles->clone();

    particles->getParent()->addChild(newparticles);

    rebuildList();

    particlesSelect(newparticles);
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

void QTBEngine::sceneAmbiant(const tbe::Vector3f& value)
{
    m_sceneManager->setAmbientLight(vec34(value));
}
