/* 
 * File:   QTBEngine.cpp
 * Author: b4n92uid
 * 
 * Created on 4 décembre 2010, 13:30
 */

#include "QTBEngine.h"
#include "Tools.h"

using namespace tbe;

QTBEngine::QTBEngine(QWidget* parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_device = NULL;
    m_sceneManager = NULL;

    m_grabCamera = false;

    m_selectedNode = NULL;

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
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
    if(!m_selectedNode)
        return;

    if(!m_eventManager->keyState[EventManager::KEY_SPACE])
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

    // In the floor ------------------------------------------------------------

    if(m_eventManager->keyState['e'])
    {
        pos.y = -m_selectedNode->GetAabb().min.y;
        pos = m_meshScene->FindFloor(pos);

        pos = m_selectedNode->MapFromGlobal(pos);
    }

    else if(m_eventManager->keyState['f'])
    {
        pos.y = 0;
        pos = m_meshScene->FindFloor(pos);

        pos = m_selectedNode->MapFromGlobal(pos);
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

        if(m_eventManager->keyState['c'])
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

    emit notifyMeshSelect(*std::find(m_meshs.begin(), m_meshs.end(), m_selectedNode));

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
        if(type == 1)
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
    }

    else if(ev->button() == Qt::MiddleButton)
    {
        m_axe->SetPos(m_curCursor3D);
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

    if(m_eventManager->keyState[EventManager::KEY_SPACE])
    {
        QCursor::setPos(mapToGlobal(QPoint(size().width() / 2, size().height() / 2)));
    }
}

void QTBEngine::keyPressEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_DOWN;
    m_eventManager->keyState[std::tolower(ev->key())] = 1;

    if(ev->key() == Qt::Key_Space)
        setCursor(Qt::BlankCursor);

    if(ev->key() == Qt::Key_V)
    {
        swapcontainer(m_camera, m_ffcamera, m_orbcamera);
        m_sceneManager->SetCurCamera(m_camera);
    }
}

void QTBEngine::keyReleaseEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_UP;
    m_eventManager->keyState[std::tolower(ev->key())] = 0;

    if(ev->key() == Qt::Key_Space)
        unsetCursor();
}

void QTBEngine::saveScene(const QString& filename)
{
}

void QTBEngine::loadScene(const QString& filename)
{
    using namespace scene;

    m_sceneManager->ClearLights();
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
}

void QTBEngine::fillTextInfo(QLabel* label)
{
    QString text;

    text += QString("Curseur 3D: %1 %2 %3\n").arg(m_curCursor3D.x).arg(m_curCursor3D.y).arg(m_curCursor3D.z);
    text += QString("Curseur: %1 %2\n\n").arg(m_curCursorPos.x).arg(m_curCursorPos.y);
    text += QString("Move: %1 %2\n\n").arg(m_eventManager->mousePosRel.x).arg(m_eventManager->mousePosRel.y);

    if(m_selectedNode)
    {
        QString name = m_selectedNode->GetName().c_str();
        tbe::Vector3f pos = m_selectedNode->GetPos();

        text += QString("Node: %1\n").arg(name);
        text += QString("Pos: %1 %2 %3").arg(pos.x).arg(pos.y).arg(pos.z);
    }
    else
        text += QString("Pas de séléction");

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
    m_selectedNode = mesh;

    m_meshs.push_back(mesh);
    m_nodes.push_back(mesh);
}
