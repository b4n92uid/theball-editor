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

#include "Define.h"
#include "QMeshInteractor.h"
#include "QLightInteractor.h"
#include "QParticlesInteractor.h"
#include "QMapMarkInteractor.h"

class QNodeInteractor;
class MainWindow;

class ClassFactory;
class HistoryState;

class SelBoxInterface;
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
    tbe::scene::Node* rootNode() const;
    tbe::scene::Grid* grid() const;
    tbe::scene::Camera* camera() const;

    QStringList usedRessources();

    void highlight(QNodeInteractor* node);
    void unhighlight(QNodeInteractor* node);

    QMeshInteractor* newMesh(const QString& filename);
    QLightInteractor* newLight();
    QParticlesInteractor* newParticles();
    QMapMarkInteractor* newMark();

    void registerInteractor(QNodeInteractor* node);
    void unregisterInteractor(QNodeInteractor* node);
    QNodeInteractor* getInteractor(tbe::scene::Node* node);

    const QList<QNodeInteractor*>& selection();

public slots:

    void selectNode(QNodeInteractor* qnode);
    void deselectNode(QNodeInteractor* qnode);

    void deselectAllNode();

    void pauseRendring();
    void resumeRendring();

    void placeCamera();

    void toggleGrid(bool state);
    void toggleSelBox(bool state);
    void toggleStaticView(bool state);

    void cloneSelected();
    void deleteSelected();

    QNodeInteractor* cloneNode(QNodeInteractor* node);
    void deleteNode(QNodeInteractor* node);

    void baseOnFloor();
    void centerOnFloor();

    void baseOnFloor(QNodeInteractor* node);
    void centerOnFloor(QNodeInteractor* node);

    void pushHistoryStat(HistoryState* hs);
    void popHistoryStat();

    void setSceneAmbiant(const tbe::Vector3f& value);

    void setZNear(float value);
    void setZFar(float value);

    void setSkybox(const QStringList& texs);
    void setSkybox(bool enable);

    void setFog(tbe::Vector4f color, float start, float end);
    void setFog(bool enable);

    void selectSelectionTool();
    void selectRotateTool();
    void selectScaleTool();
    
    void setGridSize(double value);
    void setGridCuts(int value);
    void setSelectionSensitiv(double value);
    void setLockAxisX(bool check);
    void setLockAxisY(bool check);
    void setLockAxisZ(bool check);
    void setRestorePos();
    void setRestoreRotation();
    void setRestoreScale();

signals:

    void selection(QNodeInteractor*);
    void deselection(QNodeInteractor*);
    void deselectionAll();
    void notifyChange();

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void setupSelection();

    void enterEvent(QEvent * event);

    void selectFromPick(QMouseEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void wheelEvent(QWheelEvent* ev);

    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);

    void applyTranslationEvents();
    void applyCameraEvents();

    void setupNewNode(tbe::scene::Node* thenew);

    void updateInformationGui();

private:

    tbe::Device* m_device;
    tbe::EventManager* m_eventManager;

    tbe::scene::SceneManager* m_sceneManager;
    tbe::scene::SkyBox* m_skybox;
    tbe::scene::Fog* m_fog;

    tbe::scene::OrbitalCamera* m_camera;

    tbe::scene::MeshParallelScene* m_meshScene;
    tbe::scene::ParticlesParallelScene* m_particlesScene;
    tbe::scene::WaterParallelScene* m_waterScene;
    tbe::scene::MapMarkParallelScene* m_markScene;

    tbe::scene::SceneParser* m_sceneParser;

    QTimer* m_updateTimer;

    bool m_grabCamera;
    bool m_moveCamera;
    bool m_moveObject;
    bool m_staticView;

    struct ToolMode
    {
        ToolType type;
        QCursor cursor;
        tbe::Vector3f axis;
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

    QList<QNodeInteractor*> m_selection;

    SelBox* m_selbox;
    QMap<QNodeInteractor*, SelBox*> m_selboxArray;

    tbe::scene::Grid* m_grid;

    QNodeInterfaceMap m_nodeInterface;

    MainWindow* m_mainwin;

    ClassFactory* m_classFactory;

    QStack<HistoryState*> m_history;

    QPoint m_cursorRelativeMove;

    struct GridSet
    {
        bool enable;
        tbe::Vector2f size;
        tbe::Vector2i cuts;

    } m_gridset;

    struct SensivitySet
    {
        float selection;
        float camera;

    } m_sensivitySet;
};

class SelBoxInterface : public QObject
{
    Q_OBJECT

public:
    SelBoxInterface();

    virtual void setAround(tbe::scene::Node* node) = 0;
};

class SelBox : public SelBoxInterface, public tbe::scene::QuadBox
{
    Q_OBJECT

public:
    SelBox(tbe::scene::MeshParallelScene* parallelScene);

    void setAround(tbe::scene::Node* node);
};


#endif	/* QTBENGINE_H */

