/*
 * File:   MainWindow.h
 * Author: b4n92uid
 *
 * Created on 3 décembre 2010, 17:20
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include <Tbe.h>
#include <QtGui/QtGui>

#include "ui_about.h"

#include "MeshDialog.h"
#include "LightDialog.h"
#include "ParticlesDialog.h"
#include "EnvironmentDialog.h"
#include "QNodeListView.h"
#include "QBrowsEdit.h"
#include "QVectorBox.h"
#include "QSignalBlocker.h"
#include "PackerDialog.h"
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

    void registerInteractor(QNodeInteractor* node, QItemsList& items);
    void unregisterInteractor(QNodeInteractor* node);

protected:

    void pushFileHistory(const QString& filepath);
    void buildFileHistory();

    bool leaveSafely();

    void updateInterface();

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

    void toggleFullWidget(bool full = true);

    void assignParent();
    void promoteChild();

    void pastMaterials();
    void pastFields();
    void pastPosition();
    void pastScale();
    void pastRotation();

    void takeScreenshot();

    void openEnvironmentDialog();
    void openPropertyDialog();

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

    EnvironmentDialog* getEnvironmentDialog() const;
    ParticlesDialog* getParticlesDialog() const;
    LightDialog* getLightDialog() const;
    MeshDialog* getMeshDialog() const;

signals:

    void pauseRendring();
    void resumeRendring();

    friend class QNodeInteractor;

private:

    MeshDialog* m_meshDialog;
    LightDialog* m_lightDialog;
    ParticlesDialog* m_particlesDialog;
    EnvironmentDialog* m_environmentDialog;

    struct NodesGuiTab
    {
        QLineEdit* name;
        QDoubleVector3Box* position;
        QDoubleVector3Box* rotation;
        QDoubleVector3Box* scale;

        QCheckBox* enable;
        QCheckBox* lock;

        QTreeView* additionalView;
        QStandardItemModel* additionalModel;
        QPushButton *addField, *delField, *clearFields;

        QNodeListView* nodesListView;
        QStandardItemModel* nodesListModel;
        NodeListProxyModel* nodesListProxyModel;
        QCheckBox* displayMesh, *displayLights, *displayParticles, *displayMarks;

    } nodesGui;

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

    QTBEngine* m_tbeWidget;

    Ui_mainWindow* m_uinterface;

    PackerDialog* m_packerDialog;

    QSettings* m_config;

    bool m_somethingChange;

    QInterfaceItemsMap nodeItemBinder;
};

#endif	/* MAINWINDOW_H */

