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

    void fillTextInfo(QLabel* label);

    tbe::scene::Node* rootNode();

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

    void setupSelection();

public slots:

    // Create new mesh on engine
    tbe::scene::Mesh* meshNew(const QString& filename);

    void meshRegister(tbe::scene::Mesh* mesh); // Register mesh on engine
    void meshDelete(tbe::scene::Mesh* mesh); // Delete mesh from engine
    void meshSelect(tbe::scene::Mesh* mesh); // Select mesh on engine
    void meshClone(tbe::scene::Mesh* mesh); // Clone mesh on engine

    // Create new light on engine
    tbe::scene::Light* lightNew();

    void lightRegister(tbe::scene::Light* light); // Register light on engin
    void lightDelete(tbe::scene::Light* light); // Delete light from engin
    void lightSelect(tbe::scene::Light* light); // Select light on engine
    void lightClone(tbe::scene::Light* light); // Clone light on engine

    // Create new particles on engine
    tbe::scene::ParticlesEmiter* particlesNew();

    void particlesRegister(tbe::scene::ParticlesEmiter* particles); // Register particles on engin
    void particlesDelete(tbe::scene::ParticlesEmiter* particles); // Delete particles from engin
    void particlesSelect(tbe::scene::ParticlesEmiter* particles); // Select particles on engine
    void particlesClone(tbe::scene::ParticlesEmiter* particles); // Clone particles on engine

    // Setting scene ambiant light
    void sceneAmbiant(const tbe::Vector3f& value);

    void pauseRendring();
    void resumeRendring();

    void skyboxApply(const QStringList& texs);
    void skyboxClear();

    void fogApply(tbe::Vector4f color, float start, float end);
    void fogClear();

signals:
    void notifyMeshAdd(tbe::scene::Mesh*);
    void notifyMeshSelect(tbe::scene::Mesh*);
    void notifyMeshUpdate(tbe::scene::Mesh*);

    void notifyLightAdd(tbe::scene::Light*);
    void notifyLightSelect(tbe::scene::Light*);
    void notifyLightUpdate(tbe::scene::Light*);

    void notifyParticlesAdd(tbe::scene::ParticlesEmiter*);
    void notifyParticlesSelect(tbe::scene::ParticlesEmiter*);
    void notifyParticlesUpdate(tbe::scene::ParticlesEmiter*);

    void notifyInitAmbiant(const tbe::Vector3f&);
    void notifyInitFog(tbe::scene::Fog*);
    void notifyInitSkybox(tbe::scene::SkyBox*);

private:
    tbe::Device* m_device;
    tbe::EventManager* m_eventManager;

    tbe::scene::Node* m_rootNode;
    tbe::scene::SceneManager* m_sceneManager;
    tbe::scene::SkyBox* m_skybox;
    tbe::scene::Fog* m_fog;

    tbe::scene::UserCamera* m_camera;
    tbe::scene::FreeFlyCamera* m_ffcamera;
    tbe::scene::OrbitalCamera* m_orbcamera;

    tbe::scene::MeshParallelScene* m_meshScene;
    tbe::scene::LightParallelScene* m_lightScene;
    tbe::scene::ParticlesParallelScene* m_particlesScene;
    tbe::scene::WaterParallelScene* m_waterScene;

    tbe::scene::SceneParser* m_sceneParser;

    QTimer* m_updateTimer;

    bool m_grabCamera;

    tbe::Vector2i m_lastCursorPos;
    tbe::Vector2i m_curCursorPos;
    tbe::Vector3f m_curCursor3D;

    tbe::Vector3f m_centerTarget;

    tbe::scene::Node* m_selectedNode;
    tbe::scene::Box* m_axe;

    tbe::scene::Node::Array m_nodes;
    tbe::scene::Mesh::Array m_meshs;
    tbe::scene::Light::Array m_lights;
    tbe::scene::ParticlesEmiter::Array m_particles;
};

#endif	/* QTBENGINE_H */

