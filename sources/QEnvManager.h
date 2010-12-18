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
    QEnvManager(QWidget* parent, Ui_mainWindow* uinterface);
    virtual ~QEnvManager();

public slots:
    void skyboxChanged(bool stat = true);
    void fogChanged(bool stat = true);

    void fogInit(tbe::scene::Fog* tbefog);
    void skyboxInit(tbe::scene::SkyBox* tbesky);
    void ambiantInit(const tbe::Vector3f& value);

signals:
    void skyboxApply(const QStringList&);
    void skyboxClear();

    void fogApply(tbe::Vector4f, float, float);
    void fogClear();

    void sceneAmbiantUpdate(const tbe::Vector3f&);

    void pauseRendring();
    void resumeRendring();

private:

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

    QVectorBox* m_sceneAmbiant;

    QWidget* m_parent;
};

#endif	/* QENVMANAGER_H */

