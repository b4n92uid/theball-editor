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
    void guiMeshNew();
    void guiMeshSelect(const QModelIndex& index);
    void guiMeshClone();

    void meshAdd(tbe::scene::Mesh* mesh);
    void meshSelect(tbe::scene::Mesh* mesh);

    void updateList();

signals:
    void notifyMeshAdd(tbe::scene::Mesh* mesh);
    void notifyMeshClone(tbe::scene::Mesh* mesh);
    void notifyMeshSelect(tbe::scene::Mesh* mesh);

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

    QNode* m_qnode;
    QWater* m_qwater;
    QMesh* m_qmesh;
    QParticles* m_qparticles;
};

#endif	/* QNODESMANAGER_H */

