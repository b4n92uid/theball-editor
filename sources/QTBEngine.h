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

#include "Metatype.h"

class QMesh;
class QLight;
class QParticles;
class QMapMark;

class QNodeInteractor;
class MainWindow;

class ClassFactory;
class HistoryState;

class PenArea;
class SelBox;

class QTBEngine : public QGLWidget
{
    Q_OBJECT

public:
    QTBEngine(QWidget* parent);
    ~QTBEngine();

    void clearScene();

    void loadScene(const QString& filename);
    void saveScene(const QString& filename);

    tbe::scene::SceneParser* sceneParser() const;

    tbe::scene::Node* rootNode();

    tbe::scene::Grid* grid();

    tbe::Vector3f cameraPos() const;
    tbe::Vector3f selectionPos() const;

    QStringList usedRessources();

    void registerInterface(QNodeInteractor* node);
    void unregisterInterface(QNodeInteractor* node);

    QNodeInteractor* interface(tbe::scene::Node* node);

    QMesh* meshNew(const QString& filename);

    QLight* lightNew();

    QParticles* particlesNew();

    QMapMark* markNew();

public slots:

    void selectNode(QNodeInteractor* qnode);
    void deselectNode();

    void pauseRendring();
    void resumeRendring();

    void placeCamera();

    void toggleGridDisplay(bool state);
    void toggleMagnetMove(bool state);
    void toggleSelBox(bool state);
    void toggleStaticView(bool state);

    void setLockedNode(QNodeInteractor* node, bool state);
    bool isLockedNode(QNodeInteractor* node);

    void cloneSelected();
    void deleteSelected();

    void baseOnFloor();
    void centerOnFloor();

    void pushHistoryStat(HistoryState* hs);
    void popHistoryStat();

    void setSceneAmbiant(const tbe::Vector3f& value);

    void setZNear(float value);
    void setZFar(float value);

    void skyboxApply(const QStringList& texs);
    void skyboxClear();

    void fogApply(tbe::Vector4f color, float start, float end);
    void fogClear();

    void selectionTool();
    void selectionToolSetAround(tbe::scene::Node* node, tbe::Vector4f color);

    void drawTool();
    void drawToolSetAreaSize(double areaSize);
    void drawToolSetElemGap(double elemGap);
    void drawToolSetElemCount(int elemCount);

    void eraserTool();

signals:
    void selection(QNodeInteractor*);
    void deselection();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void setupSelection();

    void enterEvent(QEvent * event);

    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void wheelEvent(QWheelEvent* ev);

    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);

    void applyTranslationEvents();
    void applyCameraEvents();

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
    bool m_magnetMove;
    bool m_staticView;

    struct ToolMode
    {
        ToolType type;
        QCursor cursor;
    };

    ToolMode m_toolMode[TOOL_COUNT];
    ToolMode* m_currentTool;

    tbe::Vector2i m_lastCursorPos;
    tbe::Vector2i m_curCursorPos;
    tbe::Vector3f m_curCursor3D;

    tbe::Vector3f m_centerTarget;

    tbe::scene::Node* m_rootNode;

    QNodeInteractor* m_selectedNode;
    QNodeInteractor* m_lastSelectedNode;

    PenArea* m_penarea;
    SelBox* m_selbox;

    tbe::scene::Grid* m_grid;

    QNodeInterfaceMap m_nodeInterface;

    QMap<QNodeInteractor*, bool> m_lockedNode;

    MainWindow* m_mainwin;

    ClassFactory* m_classFactory;

    QStack<HistoryState*> m_history;
};

class PenArea : public QObject, public tbe::scene::Sphere
{
    Q_OBJECT

public:
    PenArea(tbe::scene::MeshParallelScene* parallelScene);

    int GetElemCount() const;
    double GetElemGap() const;
    double GetAreaSize() const;

    tbe::Vector3f drawPos;

public slots:
    void SetElemCount(int elemCount);
    void SetElemGap(double elemGap);
    void SetAreaSize(double areaSize);

protected:
    double m_areaSize;
    double m_elemGap;
    int m_elemCount;
};

class SelBox : public QObject, public tbe::scene::Box
{
    Q_OBJECT

public:
    SelBox(tbe::scene::MeshParallelScene* parallelScene);
};


#endif	/* QTBENGINE_H */

