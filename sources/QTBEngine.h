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

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);

    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);

    void moveApply();

signals:
    void notifyMeshAlloc(tbe::scene::Mesh*);
    void notifyMeshUpdate(tbe::scene::Mesh*);

public slots:
    void fillTextInfo(QLabel* label);

private:
    tbe::Device* m_device;
    tbe::EventManager* m_eventManager;
    tbe::scene::SceneManager* m_sceneManager;

    tbe::scene::UserCamera* m_camera;
    tbe::scene::FreeFlyCamera* m_ffcamera;
    tbe::scene::OrbitalCamera* m_orbcamera;

    tbe::scene::MeshParallelScene* m_meshScene;
    tbe::scene::ParticlesParallelScene* m_particlesScene;
    tbe::scene::WaterParallelScene* m_waterScene;

    bool m_grabCamera;

    tbe::Vector2i m_lastCursorPos;
    tbe::Vector2i m_curCursorPos;
    tbe::Vector3f m_curCursor3D;

    tbe::scene::Node* m_selectedNode;
    tbe::scene::Mesh* m_axe;

    tbe::scene::Node::Array m_nodes;

    template< typename TC, typename T1, typename T2>
    void swapcontainer(TC& c, T1& v1, T2& v2)
    {
        c = (TC) v1 == c ? (TC) v2 : (TC) v1;
    }
};

#endif	/* QTBENGINE_H */

