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
    void UpdatMeshGui(tbe::scene::Mesh*);
    void UpdatParticlesGui(tbe::scene::Mesh*);
    void UpdatWaterGui(tbe::scene::Mesh*);

public slots:
    void RegisterMesh(tbe::scene::Mesh* newmesh);
    void RegisterParticles(tbe::scene::ParticlesEmiter* newparticles);
    void RegisterWater(tbe::scene::Water* newwater);

private:
    tbe::Device* m_device;
    tbe::EventManager* m_eventManager;
    tbe::scene::SceneManager* m_sceneManager;
    tbe::scene::UserCamera* m_camera;

    tbe::scene::MeshParallelScene* m_meshScene;
    tbe::scene::ParticlesParallelScene* m_particlesScene;
    tbe::scene::WaterParallelScene* m_waterScene;

    bool m_grabCamera;
    tbe::Vector2i m_lastCursorPos;

    tbe::scene::Node* m_selectedNode;
};

#endif	/* QTBENGINE_H */

