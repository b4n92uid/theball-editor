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
    QNodeInteractor(MainWindow* mainwin, tbe::scene::Node* target);
    virtual ~QNodeInteractor();

    tbe::scene::Node* target() const;
    MainWindow* mainwin() const;

    virtual QString typeName() const;

    QItemsList itemRows();
    QStandardItem* item();
    
    bool isLocked();
    bool isEnable();
    

public slots:
    virtual void setup();
    virtual void unsetup();
    virtual void bindWithGui();
    virtual void unbindFromGui();
    virtual void updateGui();

    virtual QNodeInteractor* clone();

public slots:
    void addNodeField();
    void delNodeField();
    void clearNodeField();
    void changeNodeField(QStandardItem*);

    void setName(const QString& s);
    void setPos(const tbe::Vector3f& v);
    void setRotation(const tbe::Vector3f& v);
    void setScale(const tbe::Vector3f& v);
    void setMatrix(const tbe::Matrix4& m);
    void setEnable(bool state);
    void setLocked(bool state);

protected:
    MainWindow* m_mainwin;
    tbe::scene::Node* m_target;

    bool m_locked;
};

#endif	/* QNODEINTERACTOR_H */

