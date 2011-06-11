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

#include "ui_interface.h"
#include "ui_about.h"
#include "ui_matedit.h"

#include "QBrowsEdit.h"
#include "QVectorBox.h"
#include "QVector2Box.h"
#include "QTBEngine.h"
#include "QNodeBinders.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow();
    virtual ~MainWindow();

    void initWidgets();
    void initConnections();


protected:

    void pushFileHistory(const QString& filepath);
    void buildFileHistory();

    bool leaveSafely();

    tbe::scene::Node* itemNode(QStandardItem* item);

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

public slots: // Node manager

    // Update node info box
    void updateNodeInfo(tbe::scene::Node*);

    // On nodes list item select
    void guiSelect(const QModelIndex& index);

    void scopeNode(int move);

    // -------- GUI -> NODE

    void guiNodeSetName(const QString& s);
    void guiNodeSetPos(const tbe::Vector3f& v);
    void guiNodeSetRotation(const tbe::Vector3f& v);
    void guiNodeSetScale(const tbe::Vector3f& v);
    void guiNodeSetMatrix(const tbe::Matrix4& m);
    void guiNodeSetEnalbe(bool stat);

    // Mesh GUI Buttons
    void guiMeshNew();
    void guiMeshClone();
    void guiMeshDelete();

    void guiAddSceneField();
    void guiDelSceneField();
    void guiClearSceneField();

    void guiAddNodeField();
    void guiDelNodeField();
    void guiClearNodeField();
    void guiChangeNodeField(QStandardItem*);

    void guiMeshMaterialSelected(const QModelIndex& index);

    void guiMeshTextureSelected(const QModelIndex& index);

    void guiMeshAddTexture();
    void guiMeshDelTexture();
    void guiMeshTextureUp();
    void guiMeshTextureDown();

    void guiMeshTextureSetBlendMode();
    void guiMeshMaterialSetBlendMode();

    void guiMeshSetOpacity(double value);
    void guiMeshSetAlphaThreshold(double value);

    void guiMeshSetTextured(bool stat);
    void guiMeshSetLighted(bool stat);
    void guiMeshSetCullTrick(bool stat);

    void guiMeshSetBlend(bool stat);
    void guiMeshSetAlpha(bool stat);

    void guiMeshSetSaveMaterial(bool stat);

    void setSelectedTexture(tbe::Texture& tex);
    tbe::Texture getSelectedTexture();

    tbe::scene::Material* getSelectedMaterial();

    // Light GUI Buttons
    void guiLightNew();
    void guiLightClone();
    void guiLightDelete();

    void guiLightSetType(int type);
    void guiLightSetAmbiant(const tbe::Vector3f& value);
    void guiLightSetDiffuse(const tbe::Vector3f& value);
    void guiLightSetSpecular(const tbe::Vector3f& value);
    void guiLightSetRadius(double value);

    // Particles GUI Buttons
    void guiParticlesNew();
    void guiParticlesClone();
    void guiParticlesDelete();

    void guiParticleSetGravity(const tbe::Vector3f& v);
    void guiParticleSetBoxsize(const tbe::Vector3f& v);
    void guiParticleSetFreemove(double v);
    void guiParticleSetLifeinit(double v);
    void guiParticleSetLifedown(double v);
    void guiParticleSetNumber(int v);
    void guiParticleSetTexture(const QString& v);
    void guiParticleSetContinousMode(int v);
    void guiParticleBuild();

    // MapMark GUI Buttons
    void guiMarkNew();
    void guiMarkClone();
    void guiMarkDelete();

    void guiMarkSetType(int index);
    void guiMarkSetSize(double value);
    void guiMarkSetColor(int index);

    // Water GUI Buttons
    void guiWaterSetDeform(double v);
    void guiWaterSetSize(const tbe::Vector2f& v);
    void guiWaterSetUvRepeat(const tbe::Vector2f& v);
    void guiWaterSetSpeed(double v);
    void guiWaterSetBlend(double v);


    // -------- NODE -> GUI

    void nodeUpdate(tbe::scene::Node* node);

    // Registre & Select mesh infos on GUI
    void meshRegister(tbe::scene::Mesh* mesh);
    void meshSelect(tbe::scene::Mesh* mesh, bool upList = true);
    void meshUpdate(tbe::scene::Mesh* mesh);
    void meshDelete(tbe::scene::Mesh* mesh);

    // Registre & Select light infos on GUI
    void lightRegister(tbe::scene::Light* light);
    void lightSelect(tbe::scene::Light* light, bool upList = true);
    void lightUpdate(tbe::scene::Light* light);
    void lightDelete(tbe::scene::Light* light);

    // Registre & Select particles infos on GUI
    void particlesRegister(tbe::scene::ParticlesEmiter* particles);
    void particlesSelect(tbe::scene::ParticlesEmiter* particles, bool upList = true);
    void particlesUpdate(tbe::scene::ParticlesEmiter* particles);
    void particlesDelete(tbe::scene::ParticlesEmiter* particles);

    // Registre & Select mark infos on GUI
    void markRegister(tbe::scene::MapMark* mark);
    void markSelect(tbe::scene::MapMark* mark, bool upList = true);
    void markUpdate(tbe::scene::MapMark* mark);
    void markDelete(tbe::scene::MapMark* mark);

    void unselect();

public slots: // Env manager

    // -------- GUI -> MANAGER

    void guiSkyboxApply(bool enable = true);
    void guiFogApply(bool enable = true);

    // -------- MANAGER -> GUI

    void skyboxRegister(tbe::scene::SkyBox* tbesky);
    void fogRegister(tbe::scene::Fog* tbefog);

    void sceneAmbiantUpdate(const tbe::Vector3f& value);

signals:

    void pauseRendring();
    void resumeRendring();

private:

    class MaterialEditDialog : public QDialog, public Ui::MaterialEdit
    {
    public:

        MaterialEditDialog(QWidget* parent) : QDialog(parent)
        {
            setupUi(this);
        }
    };

    struct
    {

        struct
        {
            QComboBox* type;
            QVectorBox* ambiant;
            QVectorBox* diffuse;
            QVectorBox* specular;
            QDoubleSpinBox* radius;

            QPushButton* add;
            QPushButton* clone;
            QPushButton* del;

        } lighTab;

        struct
        {
            QPushButton* add;
            QPushButton* clone;
            QPushButton* del;

            QDoubleSpinBox* opacity;

            QPushButton* openmatedit;

            QListView* materialsView;
            QStandardItemModel* materialsModel;

            MaterialEditDialog* matedit;
            QStandardItemModel* textureModel;

            QCheckBox* saveMaterials;

        } meshTab;

        struct
        {
            QDoubleSpinBox* deform;
            QVector2Box* size;
            QVector2Box* uvrepeat;
            QDoubleSpinBox* speed;
            QDoubleSpinBox* blend;

            QPushButton* add;
            QPushButton* clone;
            QPushButton* del;

            QMap<QObject*, QString> sourcMap;

        } waterTab;

        struct
        {
            QVectorBox* gravity;
            QVectorBox* boxsize;
            QDoubleSpinBox* freemove;
            QDoubleSpinBox* lifeinit;
            QDoubleSpinBox* lifedown;
            QSpinBox* number;
            QBrowsEdit* texture;
            QCheckBox* continiousmode;

            QPushButton* build;

            QPushButton* add;
            QPushButton* clone;
            QPushButton* del;

            QMap<QObject*, QString> sourcMap;

        } particlesTab;

        struct
        {
            QDoubleSpinBox* size;
            QComboBox* type;
            QComboBox* color;

            QPushButton* add;
            QPushButton* clone;
            QPushButton* del;

        } markTab;

        QLineEdit* name;
        QVectorBox* position;
        QVectorBox* rotation;
        QVectorBox* scale;

        QCheckBox* enable;

        QTabWidget* attribTab;

        QPushButton *nodeUp, *nodeDown, *nodeLeft, *nodeRight;

        QTreeView* additionalView;
        QStandardItemModel* additionalModel;
        QPushButton *addField, *delField, *clearFields;

        QTreeView* nodesListView;
        QStandardItemModel* nodesListModel;

        QMap<tbe::scene::Node*, QStandardItem*> nodeItemBinder;

    } nodesGui;

    struct
    {

        struct
        {
            QBrowsEdit * textures[6];
            QPushButton* apply;
            QGroupBox* enable;

        } skybox;

        struct
        {
            QVectorBox* color;
            QDoubleSpinBox* start;
            QDoubleSpinBox* end;
            QGroupBox* enable;

        } fog;

        QVectorBox* sceneAmbiant;

    } envGui;

    struct
    {
        QLineEdit* title;
        QLineEdit* author;

        QTreeView* additionalView;
        QStandardItemModel* additionalModel;

        QPushButton* addField;
        QPushButton* delField;
        QPushButton* clearFields;

    } genGui;

    QNodeBinders* m_selectedNode;

    QString m_filename;

    QTBEngine* m_tbeWidget;

    QLabel* m_infoText;

    QTimer* m_timer;

    Ui_mainWindow m_uinterface;

    QSettings* m_config;

    QSignalMapper* m_historyMapping;

    bool m_somethingChange;

    struct
    {
        QString scene;
        QString mesh;

    } m_workingDir;
};

#endif	/* MAINWINDOW_H */

