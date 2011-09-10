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

class QMesh;
class QLight;
class QParticles;
class QMapMark;

class QNodeInteractor;
class MainWindow;

class ClassFactory;

class QTBEngine : public QGLWidget
{
    Q_OBJECT

public:
    QTBEngine(QWidget* parent);
    ~QTBEngine();

    void clearScene();

    void loadScene(const QString& filename);
    void saveScene(const QString& filename);

    void fillTextInfo(QLabel* label);

    tbe::scene::Node* rootNode();

    tbe::scene::SceneParser* getSceneParser() const;

    QStringList usedRessources();

    QNodeInteractor* fetchInterface(tbe::scene::Node* node);

public slots:

    void select(QNodeInteractor* qnode);
    void selectNode(QNodeInteractor* qnode);

    void deselect();
    void deselectNode();

    void pauseRendring();
    void resumeRendring();

    void placeCamera();

    // Setting scene ambiant light
    void setSceneAmbiant(const tbe::Vector3f& value);

    void skyboxApply(const QStringList& texs);
    void skyboxClear();

    void fogApply(tbe::Vector4f color, float start, float end);
    void fogClear();

    tbe::scene::Box* getAxe();
    tbe::scene::Grid* getGrid();

    QMesh* meshNew(const QString& filename);
    QMesh* meshClone(tbe::scene::Mesh* mesh);
    void meshDelete(tbe::scene::Mesh* mesh);
    void meshRegister(QMesh* mesh);

    QLight* lightNew();
    QLight* lightClone(tbe::scene::Light* light);
    void lightDelete(tbe::scene::Light* light);
    void lightRegister(QLight* light);

    QParticles* particlesNew();
    QParticles* particlesClone(tbe::scene::ParticlesEmiter* particles);
    void particlesDelete(tbe::scene::ParticlesEmiter* particles);
    void particlesRegister(QParticles* particles);

    QMapMark* markNew();
    QMapMark* markClone(tbe::scene::MapMark* mark);
    void markDelete(tbe::scene::MapMark* mark);
    void markRegister(QMapMark* mark);

signals:
    void notifyInitAmbiant(const tbe::Vector3f&);
    void notifyInitFog(tbe::scene::Fog*);
    void notifyInitSkybox(tbe::scene::SkyBox*);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void enterEvent(QEvent * event);

    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void wheelEvent(QWheelEvent* ev);

    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);

    void moveApply();

    void setupSelection();

private:
    tbe::Device* m_device;
    tbe::EventManager* m_eventManager;

    tbe::scene::SceneManager* m_sceneManager;
    tbe::scene::SkyBox* m_skybox;
    tbe::scene::Fog* m_fog;

    tbe::scene::OrbitalCamera* m_camera;

    tbe::scene::MeshParallelScene* m_meshScene;
    tbe::scene::LightParallelScene* m_lightScene;
    tbe::scene::ParticlesParallelScene* m_particlesScene;
    tbe::scene::WaterParallelScene* m_waterScene;
    tbe::scene::MapMarkParallelScene* m_markScene;

    tbe::scene::SceneParser* m_sceneParser;

    QTimer* m_updateTimer;

    bool m_grabCamera;
    bool m_gridEnable;
    bool m_propertyCopyMode;

    tbe::Vector2i m_lastCursorPos;
    tbe::Vector2i m_curCursorPos;
    tbe::Vector3f m_curCursor3D;

    tbe::Vector3f m_centerTarget;

    tbe::scene::Node* m_rootNode;

    QNodeInteractor* m_selectedNode;
    QNodeInteractor* m_lastSelectedNode;

    tbe::scene::Box* m_axe;
    tbe::scene::Grid* m_grid;

    QList<QNodeInteractor*> m_nodes;

    QList<QMesh*> m_meshs;
    QList<QLight*> m_lights;
    QList<QParticles*> m_particles;
    QList<QMapMark*> m_marks;

    QMap<tbe::scene::Node*, QNodeInteractor*> m_nodeInterface;

    QMap<QNodeInteractor*, bool> m_lockedNode;

    MainWindow* m_mainwin;

    ClassFactory* m_classFactory;
};



#endif	/* QTBENGINE_H */

