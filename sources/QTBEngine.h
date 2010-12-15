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

    void loadScene(const QString& filename);
    void saveScene(const QString& filename);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void wheelEvent(QWheelEvent* ev);

    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);

    void moveApply();

public slots:
    void meshAdd(tbe::scene::Mesh* mesh);
    void meshSelect(tbe::scene::Mesh* mesh);
    void meshClone(tbe::scene::Mesh* mesh);

    void fillTextInfo(QLabel* label);

    void pauseRendring();
    void resumeRendring();

    void skyboxApply(const QStringList& texs);
    void skyboxClear();

    void fogApply(tbe::Vector4f color, float start, float end);
    void fogClear();

signals:
    void notifyMeshAdd(tbe::scene::Mesh*);
    void notifyMeshSelect(tbe::scene::Mesh*);

    void notifyInitFog(tbe::scene::Fog*);
    void notifyInitSkybox(tbe::scene::SkyBox*);

private:
    tbe::Device* m_device;
    tbe::EventManager* m_eventManager;

    tbe::scene::SceneManager* m_sceneManager;
    tbe::scene::SkyBox* m_skybox;
    tbe::scene::Fog* m_fog;

    tbe::scene::UserCamera* m_camera;
    tbe::scene::FreeFlyCamera* m_ffcamera;
    tbe::scene::OrbitalCamera* m_orbcamera;

    tbe::scene::MeshParallelScene* m_meshScene;
    tbe::scene::ParticlesParallelScene* m_particlesScene;
    tbe::scene::WaterParallelScene* m_waterScene;

    QTimer* m_updateTimer;

    bool m_grabCamera;

    tbe::Vector2i m_lastCursorPos;
    tbe::Vector2i m_curCursorPos;
    tbe::Vector3f m_curCursor3D;

    tbe::scene::Node* m_selectedNode;
    tbe::scene::Mesh* m_axe;

    tbe::scene::Node::Array m_nodes;
    tbe::scene::Mesh::Array m_meshs;
};

#endif	/* QTBENGINE_H */

