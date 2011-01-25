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

public slots:
    void openSceneDialog();
    void openScene(const QString& filename);

    void saveSceneDialog();
    void saveScene(const QString& filename);
    void saveScene();

    void updateGui();

public slots: // ------------------------------------------------- Node manager

    void guiSelect(const QModelIndex& index);
    void updateList();

    void guiMeshNew();
    void guiMeshClone();
    void guiMeshDelete();

    void guiLightNew();
    void guiLightClone();
    void guiLightDelete();

    void meshAdd(tbe::scene::Mesh* mesh);
    void meshSelect(tbe::scene::Mesh* mesh, bool upList = true);

    void lightAdd(tbe::scene::Light* light);
    void lightSelect(tbe::scene::Light* light, bool upList = true);


public slots: // -------------------------------------------------- Env manager

    void skyboxInit(tbe::scene::SkyBox* tbesky);
    void skyboxApply(bool enable = true);

    void fogInit(tbe::scene::Fog* tbefog);
    void fogApply(bool enable = true);

    void ambiantScene(const tbe::Vector3f& value);


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

            QPushButton* add;
            QPushButton* clone;
            QPushButton* del;

            QMap<QObject*, QString> sourcMap;

        } particlesTab;

        QLineEdit* name;
        QVectorBox* pos;

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

