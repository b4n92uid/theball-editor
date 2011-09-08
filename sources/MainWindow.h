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
#include "ui_matedit.h"

#include "QBrowsEdit.h"
#include "QVectorBox.h"
#include "QVector2Box.h"
#include "NodeFactory.h"
#include "QTBEngine.h"
#include "QNodeBinders.h"
#include "QSignalBlocker.h"
#include "PackerDialog.h"

#include "Metatype.h"

class Ui_mainWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow();
    virtual ~MainWindow();

    void initWidgets();
    void initConnections();

    QTBEngine* getTbeWidget() const;

    QString getOpenFileName() const;

protected:

    void pushFileHistory(const QString& filepath);
    void buildFileHistory();

    bool leaveSafely();

public slots:

    void toggleFullWidget(bool full = true);

    void newScene();

    void openSceneDialog();
    void openScene(const QString& filename);

    void saveSceneDialog();
    void saveScene(const QString& filename);
    void saveScene();

    void updateInfoBox();

    void about();

    void notifyChanges(bool stat = true);

    void clearNodeList();

    void closeEvent(QCloseEvent *event);

    void skyboxWorkingDir(const QString& filename);

    void openFileHistory();

public slots:

    // On nodes list item select
    void guiSelect(const QModelIndex& index);

    void scopeNode(int move);

    void screenshot();

    void guiAddSceneField();
    void guiDelSceneField();
    void guiClearSceneField();

    void guiMeshNew();
    void guiLightNew();
    void guiParticlesNew();
    void guiMarkNew();

    void guiClone();
    void guiDelete();

    void select(QNodeInteractor* qnode);
    void deselect();

public slots: // Env manager

    // -------------------------------------------------------------------------
    // GUI -> MANAGER
    // -------------------------------------------------------------------------

    void guiSkyboxApply(bool enable = true);
    void guiFogApply(bool enable = true);
    void guiSceneAmbiantApply(const tbe::Vector3f& value);

    // -------------------------------------------------------------------------
    // MANAGER -> GUI
    // -------------------------------------------------------------------------

    void skyboxRegister(tbe::scene::SkyBox* tbesky);
    void fogRegister(tbe::scene::Fog* tbefog);
    void sceneAmbiantRegister(const tbe::Vector3f& value);

signals:
    void pauseRendring();
    void resumeRendring();

    friend class QNodeInteractor;
    friend class QLightInteractor;
    friend class QMeshInteractor;
    friend class QParticlesInteractor;
    friend class QMapMarkInteractor;

private:

    class MaterialEditDialog : public QDialog, public Ui::MaterialEdit
    {
    public:

        MaterialEditDialog(QWidget* parent) : QDialog(parent)
        {
            setupUi(this);
        }
    };

    struct NodesGuiTab
    {

        struct LightGuiTab
        {
            QComboBox* type;
            QVectorBox* ambiant;
            QVectorBox* diffuse;
            QVectorBox* specular;
            QDoubleSpinBox* radius;

            QPushButton* add;

        } lighTab;

        struct MeshGuiTab
        {
            QPushButton* add;

            QDoubleSpinBox* opacity;

            QPushButton* openmatedit;

            QListView* materialsView;
            QStandardItemModel* materialsModel;

            MaterialEditDialog* matedit;
            QStandardItemModel* textureModel;

            QCheckBox* saveMaterials;

            QCheckBox* billboardX;
            QCheckBox* billboardY;

        } meshTab;

        struct WaterGuiTab
        {
            QDoubleSpinBox* deform;
            QVector2Box* size;
            QVector2Box* uvrepeat;
            QDoubleSpinBox* speed;
            QDoubleSpinBox* blend;

            QPushButton* add;

            QMap<QObject*, QString> sourcMap;

        } waterTab;

        struct ParticlesGuiTab
        {
            QVectorBox* gravity;
            QVectorBox* boxsize;
            QVector2Box* bulletsize;
            QDoubleSpinBox* freemove;
            QDoubleSpinBox* lifeinit;
            QDoubleSpinBox* lifedown;
            QSpinBox* number;
            QBrowsEdit* texture;
            QCheckBox* continiousmode;
            QCheckBox* pointsprite;

            QPushButton* build;

            QPushButton* add;

            QMap<QObject*, QString> sourcMap;

        } particlesTab;

        struct MapMarkGuiTab
        {
            QPushButton* add;

        } markTab;

        QLineEdit* name;
        QVectorBox* position;
        QVectorBox* rotation;
        QVectorBox* scale;

        QPushButton* clone;
        QPushButton* del;

        QCheckBox* enable;

        QTabWidget* attribTab;

        QPushButton *nodeUp, *nodeDown, *nodeLeft, *nodeRight;

        QTreeView* additionalView;
        QStandardItemModel* additionalModel;
        QPushButton *addField, *delField, *clearFields;

        QTreeView* nodesListView;
        QStandardItemModel* nodesListModel;

        QMap<QNodeInteractor*, QStandardItem*> nodeItemBinder;

    } nodesGui;

    struct EnvGuiTab
    {

        struct SkyboxGuiTab
        {
            QBrowsEdit * textures[6];
            QPushButton* apply;
            QGroupBox* enable;

        } skybox;

        struct FogGuiTab
        {
            QVectorBox* color;
            QDoubleSpinBox* start;
            QDoubleSpinBox* end;
            QGroupBox* enable;

        } fog;

        QVectorBox* sceneAmbiant;

    } envGui;

    struct GeneralGuiTab
    {
        QLineEdit* title;
        QLineEdit* author;

        QTreeView* additionalView;
        QStandardItemModel* additionalModel;

        QPushButton* addField;
        QPushButton* delField;
        QPushButton* clearFields;

    } genGui;

    QNodeInteractor* m_rootNode;
    QNodeInteractor* m_selectedNode;
    QNodeInteractor* m_lastSelectedNode;

    QString m_filename;

    QTBEngine* m_tbeWidget;

    QLabel* m_infoText;

    QTimer* m_timer;

    Ui_mainWindow* m_uinterface;

    PackerDialog* m_packerDialog;

    QSettings* m_config;

    bool m_somethingChange;

    struct
    {
        QString scene;
        QString mesh;
        QString meshTexture;

    } m_workingDir;
};

#endif	/* MAINWINDOW_H */

