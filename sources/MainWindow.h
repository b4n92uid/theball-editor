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

#include "QEnvManager.h"
#include "QLightsManager.h"
#include "QNodesManager.h"
#include "QTBEngine.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();

public slots:
    void OpenFileDialog();
    void OpenFile(const QString& filename);

    void SaveFile();
    void SaveFileDialog();
    void SaveFile(const QString& filename);

    void UpdateGui();

private:
    QNodesManager* m_nodsManager;
    QLightsManager* m_lightsManager;
    QEnvManager* m_envManager;

    QTBEngine* m_tbeWidget;

    QString m_filename;

    QLabel* m_infoText;
};

#endif	/* MAINWINDOW_H */

