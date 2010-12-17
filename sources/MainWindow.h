/* 
 * File:   MainWindow.h
 * Author: b4n92uid
 *
 * Created on 3 décembre 2010, 17:20
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include <QMainWindow>

#include <Tbe.h>

#include "ui_interface.h"

#include "QEnvManager.h"
#include "QNodesManager.h"
#include "QTBEngine.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();

public slots:
    void openSceneDialog();
    void openScene(const QString& filename);

    void saveScene();
    void saveSceneDialog();
    void saveScene(const QString& filename);

    void updateGui();

private:
    QNodesManager* m_nodsManager;
    QEnvManager* m_envManager;

    QTBEngine* m_tbeWidget;

    QString m_filename;

    QLabel* m_infoText;

    QTimer* m_timer;

    Ui_mainWindow m_uinterface;
};

#endif	/* MAINWINDOW_H */

