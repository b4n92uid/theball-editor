/* 
 * File:   QLightsManager.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 00:04
 */

#ifndef QLIGHTSMANAGER_H
#define	QLIGHTSMANAGER_H

#include "ui_interface.h"
#include "QVectorBox.h"

class QLightsManager : public QObject
{
    Q_OBJECT

public:
    QLightsManager(QObject* parent, Ui_mainWindow* uinterface);
    virtual ~QLightsManager();

private:
    QComboBox* m_lightType;
    QVectorBox* m_lightPos;
    QVectorBox* m_lightAmbiant;
    QVectorBox* m_lightDiffuse;
    QVectorBox* m_lightSpecular;
    QDoubleSpinBox* m_lightShininess;

    QVectorBox* m_lightSceneAmbiant;

    QTreeView* m_lightList;

    QPushButton* m_lightAdd;
        QPushButton* m_lightDel;
};

#endif	/* QLIGHTSMANAGER_H */

