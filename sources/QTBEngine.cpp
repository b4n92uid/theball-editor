/* 
 * File:   QTBEngine.cpp
 * Author: b4n92uid
 * 
 * Created on 4 décembre 2010, 13:30
 */

#include "QTBEngine.h"
#include "Tools.h"

using namespace tbe;

QTBEngine::QTBEngine(QWidget* parent) : QGLWidget(parent)
{
    m_device = NULL;
    m_sceneManager = NULL;

    m_grabCamera = false;

    m_selectedNode = NULL;

    setMouseTracking(true);
}

QTBEngine::~QTBEngine()
{
}

void QTBEngine::initializeGL()
{
    m_device = new Device;
    m_device->Init();

    m_eventManager = m_device->GetEventManager();

    m_sceneManager = m_device->GetSceneManager();

    scene::OrbitalCamera* orbCamera = new scene::OrbitalCamera;
    m_sceneManager->AddCamera(orbCamera);

    m_camera = orbCamera;

    scene::SceneParser scenefile(m_sceneManager);
    scenefile.LoadScene("D:\\projets\\cpp\\tbengine\\demo\\bin\\medias\\scene.ballscene");

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);
}

void QTBEngine::resizeGL(int w, int h)
{
    m_device->SetViewportSize(Vector2i(w, h));
}

void QTBEngine::paintGL()
{
    m_device->BeginScene();
    m_sceneManager->Render();
    m_device->EndScene();
}

void QTBEngine::mousePressEvent(QMouseEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_MOUSE_DOWN;


    if(ev->button() == Qt::LeftButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_LEFT] = true;
        m_grabCamera = true;
        m_lastCursorPos = qptovec(ev->pos());
    }

    else if(ev->button() == Qt::RightButton)
    {
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT] = true;
    }

    else if(ev->button() == Qt::MiddleButton)
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_MIDDLE] = true;

    m_camera->OnEvent(m_eventManager);
}

void QTBEngine::mouseReleaseEvent(QMouseEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_MOUSE_UP;

    m_grabCamera = false;

    if(ev->button() == Qt::LeftButton)
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_LEFT] = false;

    else if(ev->button() == Qt::RightButton)
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_RIGHT] = false;

    else if(ev->button() == Qt::MiddleButton)
        m_eventManager->mouseState[EventManager::MOUSE_BUTTON_MIDDLE] = false;

    m_camera->OnEvent(m_eventManager);
}

void QTBEngine::mouseMoveEvent(QMouseEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_MOUSE_MOVE;

    if(m_grabCamera)
    {
        Vector2i curPos = qptovec(ev->pos());

        m_eventManager->mousePosRel = curPos - m_lastCursorPos;
        m_eventManager->mousePos = curPos;

        m_lastCursorPos = curPos;

        m_camera->OnEvent(m_eventManager);
    }
}

void QTBEngine::keyPressEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_DOWN;
    m_eventManager->keyState[ev->key()] = true;

    m_camera->OnEvent(m_eventManager);
}

void QTBEngine::keyReleaseEvent(QKeyEvent* ev)
{
    m_eventManager->notify = EventManager::EVENT_KEY_UP;
    m_eventManager->keyState[ev->key()] = false;

    m_camera->OnEvent(m_eventManager);
}