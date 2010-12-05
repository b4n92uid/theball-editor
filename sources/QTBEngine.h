/* 
 * File:   QTBEngine.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 13:30
 */

#ifndef QTBENGINE_H
#define	QTBENGINE_H

#include <Tbe.h>
#include <QtGui/QtGui>
#include <QtOpenGL/QtOpenGL>

class QTBEngine : public QGLWidget
{
public:
    QTBEngine(QWidget* parent);
    virtual ~QTBEngine();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mouseMoveEvent(QMouseEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouse(QMouseEvent* ev);
    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);

private:
    tbe::Device* m_device;
    tbe::EventManager* m_eventManager;
    tbe::scene::SceneManager* m_sceneManager;
    tbe::scene::UserCamera* m_camera;

    bool m_grabCamera;
    tbe::Vector2i m_lastCursorPos;
};

#endif	/* QTBENGINE_H */

