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

class MainWindow : public QMainWindow
{
public:
    MainWindow();
    virtual ~MainWindow();

private:
    QNodesManager* m_nodsManager;
    QLightsManager* m_lightsManager;
    QEnvManager* m_envManager;
};

#endif	/* MAINWINDOW_H */

