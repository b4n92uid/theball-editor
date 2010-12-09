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
    void NodeNameChanged(const QString& str);
    void NodePosChanged(const tbe::Vector3f& vec);
    void NodeRotChanged(const tbe::Vector3f& vec);

    void MeshNew();
    void MeshAdd(tbe::scene::Mesh* mesh);
    void MeshSelect(tbe::scene::Mesh* mesh);

signals:

private:
    QLineEdit* m_nodeName;
    QVectorBox* m_nodePos;
    QVectorBox* m_nodeRot;

    struct
    {
        QPushButton* add;
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

    QTreeView* m_nodeList;
    QStandardItemModel* m_nodeModel;

    tbe::scene::Node* m_selectedNode;

    QWater* m_qwater;
    QMesh* m_qmesh;
    QParticles* m_qparticles;
};

#endif	/* QNODESMANAGER_H */

