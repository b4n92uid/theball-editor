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

public slots:

    void openSceneDialog();
    void openScene(const QString& filename);

    void saveSceneDialog();
    void saveScene(const QString& filename);
    void saveScene();

    void updateGui();

public slots: // ------------------------------------------------- Node manager

    // On nodes list item select
    void guiSelect(const QModelIndex& index);

    // Update node info box
    void updateNodeInfo(tbe::scene::Node*);

    void scopeNode(int move);

    // Mesh GUI Buttons
    void guiMeshNew();
    void guiMeshClone();
    void guiMeshDelete();

    // Light GUI Buttons
    void guiLightNew();
    void guiLightClone();
    void guiLightDelete();

    // Particles GUI Buttons
    void guiParticlesNew();
    void guiParticlesClone();
    void guiParticlesDelete();

    // Registre & Select mesh infos on GUI
    void meshRegister(tbe::scene::Mesh* mesh);
    void meshSelect(tbe::scene::Mesh* mesh, bool upList = true);

    // Registre & Select light infos on GUI
    void lightRegister(tbe::scene::Light* light);
    void lightSelect(tbe::scene::Light* light, bool upList = true);

    // Registre & Select particles infos on GUI
    void particlesRegister(tbe::scene::ParticlesEmiter* particles);
    void particlesSelect(tbe::scene::ParticlesEmiter* particles, bool upList = true);

    static tbe::scene::Node* itemNode(QStandardItem* item);

public slots: // -------------------------------------------------- Env manager

    void skyboxInit(tbe::scene::SkyBox* tbesky);
    void skyboxApply(bool enable = true);

    void fogInit(tbe::scene::Fog* tbefog);
    void fogApply(bool enable = true);

    void sceneAmbiant(const tbe::Vector3f& value);

signals:

    void pauseRendring();
    void resumeRendring();

private:

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

        QLineEdit* name;
        QVectorBox* pos;

        QTabWidget* attribTab;

        QPushButton *nodeUp, *nodeDown, *nodeLeft, *nodeRight;

        QTreeView* nodesListView;
        QStandardItemModel* nodesListModel;

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

    QNodeBinders* m_qnodebind;

    QString m_filename;

    QTBEngine* m_tbeWidget;

    QLabel* m_infoText;

    QTimer* m_timer;

    Ui_mainWindow m_uinterface;
};

#endif	/* MAINWINDOW_H */

