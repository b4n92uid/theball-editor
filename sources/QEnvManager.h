/* 
 * File:   QEnvManager.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 00:04
 */

#ifndef QENVMANAGER_H
#define	QENVMANAGER_H

#include "QVectorBox.h"


#include "QBrowsEdit.h"


#include "ui_interface.h"

class QEnvManager : public QObject
{
    Q_OBJECT

public:
    QEnvManager(QObject* parent, Ui_mainWindow* uinterface);
    virtual ~QEnvManager();

private:

    struct
    {
        QBrowsEdit * textures[6];
        QPushButton apply;
        QPushButton clear;
        QCheckBox enable;

    } Skybox;

    struct
    {
        QVectorBox* color;
        QDoubleSpinBox* start;
        QDoubleSpinBox* end;
        QCheckBox enable;

    } Fog;
};

#endif	/* QENVMANAGER_H */

