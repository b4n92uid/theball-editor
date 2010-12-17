/* 
 * File:   QNodesManager.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 00:04
 */

#ifndef QNODESMANAGER_H
#define	QNODESMANAGER_H

#include "ui_interface.h"
#include "QVectorBox.h"
#include "QBrowsEdit.h"
#include "QVector2Box.h"
#include "QNodeBinders.h"

class QNodesManager : public QObject
{
    Q_OBJECT

public:
    QNodesManager(QObject* parent, Ui_mainWindow* uinterface);
    virtual ~QNodesManager();

public slots:
    void guiSelect(const QModelIndex& index);

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

    void updateList();

signals:
    void notifyMeshAdd(tbe::scene::Mesh* mesh);
    void notifyMeshClone(tbe::scene::Mesh* mesh);
    void notifyMeshSelect(tbe::scene::Mesh* mesh);

    void notifyLightNew(tbe::scene::Light* light);
    void notifyLightDelete(tbe::scene::Light* light);
    void notifyLightSelect(tbe::scene::Light* light);

    void pauseRendring();
    void resumeRendring();

private:

    struct
    {
        QLineEdit* name;
        QVectorBox* pos;

    } NodeTab;

    struct
    {
        QComboBox* type;
        QVectorBox* ambiant;
        QVectorBox* diffuse;
        QVectorBox* specular;
        QDoubleSpinBox* radius;

        QPushButton* add;
        QPushButton* del;

    } LighTab;

    struct
    {
        QPushButton* add;
        QPushButton* clone;
        QPushButton* del;

    } MeshTab;

    struct
    {
        QDoubleSpinBox* deform;
        QVector2Box* size;
        QVector2Box* uvrepeat;
        QDoubleSpinBox* speed;
        QDoubleSpinBox* blend;

        QPushButton* add;
        QPushButton* del;

        QMap<QObject*, QString> sourcMap;

    } WaterTab;

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
        QPushButton* del;

        QMap<QObject*, QString> sourcMap;

    } ParticlesTab;

    QTreeView* m_nodesListView;
    QStandardItemModel* m_nodesListModel;

    tbe::scene::Node* m_selectedNode;

    QNodeBinders* m_qnodebind;
};

#endif	/* QNODESMANAGER_H */

