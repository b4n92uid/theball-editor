/* 
 * File:   QTBEngine.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 13:30
 */

#ifndef QTBENGINE_H
#define	QTBENGINE_H

#include <Tbe.h>
#include <QtGui/QtGui>
#include <QtOpenGL/QtOpenGL>

class QTBEngine : public QGLWidget
{
    Q_OBJECT

public:
    QTBEngine(QWidget* parent);
    ~QTBEngine();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);

    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);

signals:
    void MeshUpdated(tbe::scene::Mesh*);
    void ParticlesUpdated(tbe::scene::ParticlesEmiter*);
    void WaterUpdated(tbe::scene::Water*);

private:
    tbe::Device* m_device;
    tbe::EventManager* m_eventManager;
    tbe::scene::SceneManager* m_sceneManager;
    tbe::scene::UserCamera* m_camera;

    bool m_grabCamera;
    tbe::Vector2i m_lastCursorPos;

    tbe::scene::Node* m_selectedNode;
};

#endif	/* QTBENGINE_H */

