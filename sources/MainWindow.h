/*
 * File:   MainWindow.h
 * Author: b4n92uid
 *
 * Created on 3 décembre 2010, 17:20
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include <QtGui/QtGui>
#include <Tbe.h>

#include "ui_about.h"

#include "QNodeListView.h"
#include "QBrowsEdit.h"
#include "QVectorBox.h"
#include "QSignalBlocker.h"
#include "PackerDialog.h"
#include "ToolsDialogs.h"
#include "NodeListProxyModel.h"
#include "Define.h"

class Ui_mainWindow;

class QNodeInteractor;
class QTBEngine;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow();
    virtual ~MainWindow();

    void initWidgets();
    void initConnections();

    QTBEngine* tbeWidget() const;

    QString openFileName() const;

    Ui_mainWindow* ui();

    void registerNode(QNodeInteractor* node, QItemsList& items);
    void unregisterNode(QNodeInteractor* node);

protected:

    void pushFileHistory(const QString& filepath);
    void buildFileHistory();

    bool leaveSafely();

    void updateEnvGui();

    void outputScene(const QString& filename);

public slots:

    void newScene();

    void openSceneDialog();
    void openScene(const QString& filename);

    void saveSceneDialog();
    void saveScene(const QString& filename);
    void saveScene();

    void saveBackup();

    void openSceneFromHistory();

    void aboutDialog();

    void notifyChange(bool stat = true);

    void closeEvent(QCloseEvent *event);

    void skyboxWorkingDir(const QString& filename);

    void toggleFullWidget(bool full = true);

    void assignParent();
    void promoteChild();

    void pastMaterials();
    void pastFields();
    void pastPosition();
    void pastScale();
    void pastRotation();

    void takeScreenshot();

    void setCurrentTool(int type);

    void guiAddSceneField();
    void guiDelSceneField();
    void guiClearSceneField();

    void guiMeshNew();
    void guiLightNew();
    void guiParticlesNew();
    void guiMarkNew();

    void select(QNodeInteractor* qnode);
    void deselect(QNodeInteractor* qnode);
    void deselectAll();

    void guiSkyboxApply(bool enable = true);
    void guiFogApply(bool enable = true);
    void guiAmbiantApply(const tbe::Vector3f& value);
    void guiZNear(double value);
    void guiZFar(double value);

signals:

    void pauseRendring();
    void resumeRendring();

    friend class QNodeInteractor;
    friend class QLightInteractor;
    friend class QMeshInteractor;
    friend class QParticlesInteractor;
    friend class QMapMarkInteractor;

    friend class QLight;
    friend class QMesh;
    friend class QParticles;
    friend class QMapMark;

private:

    struct NodesGuiTab
    {

        struct LightGuiTab
        {
            QComboBox* type;
            QDoubleVector3Box* ambiant;
            QDoubleVector3Box* diffuse;
            QDoubleVector3Box* specular;
            QDoubleSpinBox* radius;

        } light;

        struct MeshGuiTab
        {
            QPushButton* editmatfile;
            QCheckBox* custommat;

            MaterialEditDialog* matedit;

            QCheckBox* billboardX;
            QCheckBox* billboardY;

        } mesh;

        struct ParticlesGuiTab
        {
            QDoubleVector3Box* gravity;
            QDoubleVector3Box* boxsize;
            QDoubleVector2Box* bulletsize;
            QDoubleSpinBox* freemove;
            QDoubleSpinBox* lifeinit;
            QDoubleSpinBox* lifedown;
            QSpinBox* number;
            QBrowsEdit* texture;
            QCheckBox* continiousmode;
            QCheckBox* pointsprite;

            QPushButton* build;

            QMap<QObject*, QString> sourcMap;

        } particles;

        QLineEdit* name;
        QDoubleVector3Box* position;
        QDoubleVector3Box* rotation;
        QDoubleVector3Box* scale;

        QCheckBox* enable;
        QCheckBox* lock;

        QTabWidget* attribTab;

        QTreeView* additionalView;
        QStandardItemModel* additionalModel;
        QPushButton *addField, *delField, *clearFields;

        QNodeListView* nodesListView;
        QStandardItemModel* nodesListModel;
        NodeListProxyModel* nodesListProxyModel;
        QCheckBox* displayMesh, *displayLights, *displayParticles, *displayMarks;

    } nodesGui;

    struct EnvGuiTab
    {

        struct
        {
            QGroupBox* enable;
            QBrowsEdit* vertex;
            QBrowsEdit* fragment;

        } shader;

        struct SkyboxGuiTab
        {
            QBrowsEdit * textures[6];
            QPushButton* apply;
            QGroupBox* enable;

        } skybox;

        struct FogGuiTab
        {
            QDoubleVector3Box* color;
            QDoubleSpinBox* start;
            QDoubleSpinBox* end;
            QGroupBox* enable;

        } fog;

        QDoubleSpinBox* znear, *zfar;

        QDoubleVector3Box* sceneAmbiant;

    } envGui;

    struct GeneralGuiTab
    {
        QLineEdit* title;
        QLineEdit* author;

        QLabel* information;

        QTreeView* additionalView;
        QStandardItemModel* additionalModel;

        QPushButton* addField;
        QPushButton* delField;
        QPushButton* clearFields;

    } genGui;

    QNodeInteractor* m_rootNode;

    QString m_filename;
    QTimer m_backupTimer;

    QTBEngine* m_tbeWidget;

    Ui_mainWindow* m_uinterface;

    PackerDialog* m_packerDialog;

    QSettings* m_config;

    bool m_somethingChange;

    QInterfaceItemsMap nodeItemBinder;

    struct
    {
        QString scene;
        QString mesh;
        QString meshTexture;

    } m_workingDir;
};

#endif	/* MAINWINDOW_H */

