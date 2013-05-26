/* 
 * File:   EnvironmentDialog.h
 * Author: b4n92uid
 *
 * Created on 23 mai 2013, 17:14
 */

#ifndef ENVIRONMENTDIALOG_H
#define	ENVIRONMENTDIALOG_H

#include <Tbe.h>
#include <QDialog>
#include "ui_environment.h"
#include "QTBEngine.h"

class QDoubleVector3Box;
class MainWindow;

class EnvironmentDialog : public QDialog, public Ui_EnvironmentDialog
{
    Q_OBJECT

public:
    EnvironmentDialog(MainWindow* parent);
    virtual ~EnvironmentDialog();

    void initConnection();
    void updateInterface();

public slots:
    void guiSkyboxEnable(bool enable);
    void guiSkyboxBrowse();
    void guiSkyboxChange();
    void guiSkyboxShift();

    void guiFogEnable(bool enable);
    void guiFogChange();

    void guiAmbient(const tbe::Vector3f& value);
    void guiZNear(double value);
    void guiZFar(double value);

private:
    MainWindow* m_mainwin;
    QTBEngine* m_tbeWidget;
    QDoubleVector3Box* fog_color;
    QDoubleVector3Box* sceneAmbiant;
};

#endif	/* ENVIRONMENTDIALOG_H */

