/* 
 * File:   QTBEngine.cpp
 * Author: b4n92uid
 * 
 * Created on 4 décembre 2010, 13:30
 */

#include "QTBEngine.h"
#include "Tools.h"

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
}

void QTBEngine::initializeGL()
{
    m_device = new Device;
    m_device->Init();

    m_sceneManager = m_device->GetSceneManager();
    m_eventManager = m_device->GetEventManager();

    m_fog = m_sceneManager->GetFog();
    m_skybox = m_sceneManager->GetSkybox();

    using namespace scene;

    m_ffcamera = new FreeFlyCamera;
    m_orbcamera = new OrbitalCamera;

    m_sceneManager->AddCamera(m_ffcamera);
    m_sceneManager->AddCamera(m_orbcamera);

    m_camera = m_orbcamera;

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_updateTimer->start(16);
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
    m_device->SetViewportSize(Vector2i(w, h));
}

void QTBEngine::moveApply()
{
    using namespace scene;

    if(!m_selectedNode)
        return;

    if(!m_eventManager->keyState[EventManager::KEY_LSHIFT])
        return;

    Vector3f pos = m_selectedNode->GetPos();
    Matrix4f matrix = m_selectedNode->GetMatrix();

    // Rotation ----------------------------------------------------------------

    float rotateSpeed = 5 * M_PI / 180;

    if(m_eventManager->notify == EventManager::EVENT_KEY_DOWN)
    {
        if(m_eventManager->keyState['a'])
            matrix.SetRotateX(-rotateSpeed);

        if(m_eventManager->keyState['z'])
            matrix.SetRotateX(rotateSpeed);

        if(m_eventManager->keyState['q'])
            matrix.SetRotateY(-rotateSpeed);

        if(m_eventManager->keyState['s'])
            matrix.SetRotateY(rotateSpeed);

        if(m_eventManager->keyState['w'])
            matrix.SetRotateZ(-rotateSpeed);

        if(m_eventManager->keyState['x'])
            matrix.SetRotateZ(rotateSpeed);
    }

    // In the gride ------------------------------------------------------------

    if(m_eventManager->keyState['g'])
        tools::round(pos, Vector3f(1));

    matrix.SetPos(pos);

    m_selectedNode->SetMatrix(matrix);

    m_axe->SetPos(m_selectedNode->GetAbsoluteMatrix().GetPos());

    // Movement ----------------------------------------------------------------

    if(m_eventManager->notify == EventManager::EVENT_MOUSE_MOVE)
    {
        float moveSpeed = 0.01;

        Vector2f mousePosRel = m_eventManager->mousePosRel;

        Vector3f target = m_camera->GetTarget();
        target.y = 0;
        target.Normalize();

        Vector3f left = m_camera->GetLeft();
        left.y = 0;
        left.Normalize();

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

        transform += m_selectedNode->GetPos();

        m_selectedNode->SetPos(transform);

        Vector3f abs = m_selectedNode->GetAbsoluteMatrix().GetPos();

        m_axe->SetPos(abs);
        m_orbcamera->SetCenter(abs);

    }

    if(Mesh * mesh = tools::find(m_meshs, m_selectedNode))
        emit notifyMeshSelect(mesh);

    else if(Light * light = tools::find(m_lights, m_selectedNode))
        emit notifyLightSelect(light);

    m_eventManager->notify = EventManager::EVENT_NO_EVENT;
}

void QTBEngine::paintGL()
{
    moveApply();

    m_device->BeginScene();
    m_sceneManager->Render();
    m_curCursor3D = m_sceneManager->ScreenToWorld(m_curCursorPos);
    m_device->EndScene();
}

struct SelectionSort
{
    tbe::Vector3f cameraPos;
    int type;

    bool operator()(const tbe::scene::Node* node1, const tbe::scene::Node * node2)
    {
        if(node1->HasParent())
            return false;
        else if(type == 1)
            return (node1->GetPos() - cameraPos) < (node2->GetPos() - cameraPos);
        else if(type == 2)
            return node1->GetAabb().GetSize() < node2->GetAabb().GetSize();
        else
            return false;
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
        m_axe->SetPos(m_curCursor3D);
        m_orbcamera->SetCenter(m_curCursor3D);
    }

    else if(ev->button() == Qt::RightButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT] = 1;

        using namespace tbe;
        using namespace tbe::scene;

        m_selectedNode = NULL;

        SelectionSort sortfunc;
        sortfunc.cameraPos = m_camera->GetPos();

        sortfunc.type = 1;
        std::sort(m_meshs.begin(), m_meshs.end(), sortfunc);

        sortfunc.type = 2;
        std::sort(m_meshs.begin(), m_meshs.end(), sortfunc);

        foreach(Mesh* node, m_meshs)
        {
            if(node->GetAbsolutAabb().Add(0.5).IsInner(m_curCursor3D))
            {
                meshSelect(node);
                emit notifyMeshSelect(node);
                break;
            }
        }
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

    m_camera->OnEvent(m_eventManager);

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
        m_camera->OnEvent(m_eventManager);
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
        if(ev->key() == Qt::Key_P && m_selectedNode->HasParent())
        {
            using namespace tbe::scene;

            if(Mesh * mesh = tools::find(m_meshs, m_selectedNode->GetParent()))
            {
                meshSelect(mesh);
                emit notifyMeshSelect(mesh);
            }

            else if(Light * light = tools::find(m_lights, m_selectedNode->GetParent()))
            {
                lightSelect(light);
                emit notifyLightSelect(light);
            }
        }

        if(ev->key() == Qt::Key_C)
        {
            using namespace tbe::scene;

            if(Mesh * mesh = tools::find(m_meshs, m_selectedNode))
                meshClone(mesh);

            else if(Light * light = tools::find(m_lights, m_selectedNode))
                lightClone(light);
        }

        if(ev->key() == Qt::Key_F)
        {
            m_meshScene->SetInFloor(m_selectedNode);
        }

        if(ev->key() == Qt::Key_E)
        {
            m_meshScene->SetInFloor(m_selectedNode);

            Vector3f adjust = m_selectedNode->GetPos();
            adjust.y += -m_selectedNode->GetAabb().min.y;

            m_selectedNode->SetPos(adjust);
        }

        else if(ev->key() == Qt::Key_V)
        {
            swapcontainer(m_camera, m_ffcamera, m_orbcamera);
            m_sceneManager->SetCurCamera(m_camera);
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
}

void QTBEngine::loadScene(const QString& filename)
{
    using namespace scene;

    m_sceneManager->ClearParallelScenes();

    m_fog->Clear();
    m_skybox->Clear();

    SceneParser scenefile(m_sceneManager);
    scenefile.LoadScene(filename.toStdString());

    m_meshScene = scenefile.GetMeshScene();

    for(Iterator<Mesh*> it = m_meshScene->GetIterator(); it; it++)
    {
        m_meshs.push_back(*it);
        m_nodes.push_back(*it);

        emit notifyMeshAdd(*it);
    }

    m_axe = new Axes(4, 4);
    m_meshScene->RegisterMesh(m_axe);

    m_lightScene = scenefile.GetLightScene();

    for(Iterator<Light*> it = m_lightScene->GetIterator(); it; it++)
    {
        m_lights.push_back(*it);
        m_nodes.push_back(*it);

        emit notifyLightAdd(*it);
    }

    emit notifyInitFog(m_fog);
    emit notifyInitSkybox(m_skybox);
    emit notifyInitAmbiant(vec43(m_sceneManager->GetAmbientLight()));
}

void QTBEngine::fillTextInfo(QLabel* label)
{
    QString text;

    text += QString("Curseur 3D: %1 %2 %3\n\n").arg(m_curCursor3D.x).arg(m_curCursor3D.y).arg(m_curCursor3D.z);

    if(m_selectedNode)
    {
        QString name = m_selectedNode->GetName().c_str();
        tbe::Vector3f pos = m_selectedNode->GetPos();

        text += QString("Node: %1\n").arg(name);
        text += QString("|_Pos: %1 %2 %3").arg(pos.x).arg(pos.y).arg(pos.z);


        if(m_selectedNode->HasParent())
        {
            using namespace tbe::scene;

            Node* parent = m_selectedNode->GetParent();

            QString name = parent->GetName().c_str();
            tbe::Vector3f pos = parent->GetPos();

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

void QTBEngine::meshSelect(tbe::scene::Mesh* mesh)
{
    m_selectedNode = mesh;

    m_orbcamera->SetCenter(m_selectedNode->GetAbsoluteMatrix().GetPos());
    m_axe->SetPos(m_selectedNode->GetAbsoluteMatrix().GetPos());
}

void QTBEngine::meshAdd(tbe::scene::Mesh* mesh)
{
    m_meshScene->RegisterMesh(mesh);

    m_meshs.push_back(mesh);
    m_nodes.push_back(mesh);

    meshSelect(mesh);
}

void QTBEngine::meshClone(tbe::scene::Mesh* mesh)
{
    using namespace tbe::scene;

    Mesh* newmesh = new Mesh(*mesh);

    meshAdd(newmesh);

    emit notifyMeshAdd(newmesh);
}

void QTBEngine::lightAdd(tbe::scene::Light* light)
{
    m_lightScene->Register(light);

    m_lights.push_back(light);
    m_nodes.push_back(light);

    lightSelect(light);
}

void QTBEngine::lightSelect(tbe::scene::Light* light)
{
    m_selectedNode = light;

    m_orbcamera->SetCenter(m_selectedNode->GetAbsoluteMatrix().GetPos());
    m_axe->SetPos(m_selectedNode->GetAbsoluteMatrix().GetPos());
}

void QTBEngine::lightClone(tbe::scene::Light* light)
{
    using namespace tbe::scene;

    Light* newlight = new Light(*light);

    lightAdd(newlight);

    emit notifyLightAdd(newlight);
}

void QTBEngine::skyboxApply(const QStringList& texs)
{
    Texture skytex[6];

    for(unsigned i = 0; i < 6; i++)
        skytex[i] = texs[i].toStdString();

    m_skybox->SetTextures(skytex);

    m_skybox->SetEnable(true);
}

void QTBEngine::skyboxClear()
{
    m_skybox->Clear();
}

void QTBEngine::fogApply(tbe::Vector4f color, float start, float end)
{
    m_fog->SetColor(color);
    m_fog->SetStart(start);
    m_fog->SetEnd(end);

    m_fog->SetEnable(true);
}

void QTBEngine::fogClear()
{
    m_fog->Clear();
}

void QTBEngine::sceneAmbiant(const tbe::Vector3f& value)
{
    m_sceneManager->SetAmbientLight(vec34(value));
}
