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

class PenAreaInterface;
class PenArea;

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

    tbe::scene::Node* rootNode();

    tbe::scene::Grid* grid();

    SelBoxInterface* selBox();

    PenAreaInterface* penArea();

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

    void cloneSelected();
    void deleteSelected();

    void deleteNode(QNodeInteractor* node);
    QNodeInteractor* cloneNode(QNodeInteractor* node);

    void baseOnFloor();
    void centerOnFloor();

    void baseOnFloor(QNodeInteractor* node);
    void centerOnFloor(QNodeInteractor* node);

    void pushHistoryStat(HistoryState* hs);
    void popHistoryStat();

    void setSceneAmbiant(const tbe::Vector3f& value);

    void setZNear(float value);
    void setZFar(float value);

    void skyboxApply(const QStringList& texs);
    void skyboxClear();

    void fogApply(tbe::Vector4f color, float start, float end);
    void fogClear();

    void selectSelectionTool();
    void selectDrawTool();

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
    bool m_magnetMove;
    bool m_staticView;

    struct GridSet
    {
        bool enable;
        tbe::Vector3f size;

    } m_gridset;

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

    MainWindow* m_mainwin;

    ClassFactory* m_classFactory;

    QStack<HistoryState*> m_history;

    QPoint m_cursorRelativeMove;
};

class PenAreaInterface : public QObject
{
    Q_OBJECT

public:
    PenAreaInterface();

    tbe::Vector3f drawPos;

    double getAreaSize() const;

    double getElemGap() const;
    int getElemCount() const;

    tbe::Vector2f getRotationRange() const;
    tbe::Vector2f getScaleRange() const;

    bool rotX, rotY, rotZ;
    bool scaleX, scaleY, scaleZ;
    int onFloor;

public slots:
    virtual void setAreaSize(double areaSize) = 0;

    void setElemGap(double elemGap);
    void setElemCount(int elemCount);

    void setRotationRange(tbe::Vector2f rotation);
    void setScaleRange(tbe::Vector2f scale);

    void setRotationAxe(bool x, bool y, bool z);
    void setScaleAxe(bool x, bool y, bool z);

    void setNotOnFloor();
    void setCenterOnFloor();
    void setBaseOnFloor();

protected:
    double m_areaSize;
    double m_elemGap;
    int m_elemCount;

    tbe::Vector2f m_rotationRange;
    tbe::Vector2f m_scaleRange;
};

class PenArea : public PenAreaInterface, public tbe::scene::Sphere
{
    Q_OBJECT

public:
    PenArea(tbe::scene::MeshParallelScene* parallelScene);

public slots:
    void setAreaSize(double areaSize);
};

class SelBoxInterface : public QObject
{
    Q_OBJECT

public:
    SelBoxInterface();

    virtual void setAround(tbe::scene::Node* node, tbe::Vector4f color) = 0;
};

class SelBox : public SelBoxInterface, public tbe::scene::Box
{
    Q_OBJECT

public:
    SelBox(tbe::scene::MeshParallelScene* parallelScene);

    void setAround(tbe::scene::Node* node, tbe::Vector4f color);
};


#endif	/* QTBENGINE_H */

