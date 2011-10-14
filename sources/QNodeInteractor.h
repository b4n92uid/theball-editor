/* 
 * File:   QNodeInteractor.h
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 07:22
 */

#ifndef QNODEINTERACTOR_H
#define	QNODEINTERACTOR_H

#include <Tbe.h>

#include <QObject>
#include <QtGui/QtGui>

#include "Metatype.h"

class MainWindow;

class QNodeInteractor : public QObject
{
    Q_OBJECT

public:
    QNodeInteractor();
    QNodeInteractor(MainWindow* mainwin, tbe::scene::Node* target);
    virtual ~QNodeInteractor();

    tbe::scene::Node* target() const;
    MainWindow* mainwin() const;

public slots:
    virtual void setup();
    virtual void unsetup();
    virtual void select();
    virtual void deselect();
    virtual void update();

    virtual QNodeInteractor* clone();

public slots:
    void addNodeField();
    void delNodeField();
    void clearNodeField();
    void changeNodeField(QStandardItem*);

    void setName(const QString& s);
    void setPos(const tbe::Vector3f& v);
    void setRotation(const tbe::Vector3f& v);
    virtual // Because mesh type are vertex scalled
    void setScale(const tbe::Vector3f& v);
    void setMatrix(const tbe::Matrix4& m);
    void setEnalbe(bool state);
    void setLocked(bool state);

protected:
    QItemsList itemRows();

protected:
    MainWindow* m_mainwin;
    tbe::scene::Node* m_target;
};

#endif	/* QNODEINTERACTOR_H */

