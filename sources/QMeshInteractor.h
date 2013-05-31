/* 
 * File:   QMeshInteractor.h
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 07:23
 */

#ifndef QMESHINTERACTOR_H
#define	QMESHINTERACTOR_H

#include "QNodeInteractor.h"

class QMeshInteractor : public QNodeInteractor
{
    Q_OBJECT

public:
    QMeshInteractor(MainWindow* mainwin, tbe::scene::Mesh* target);
    virtual ~QMeshInteractor();

    void setup();

    QMeshInteractor* clone();

    QString typeName() const;

public slots:
    void bindInterface();
    void unbindFromGui();
    void updateGui();
    void triggerDialog();

    void setBillBoardX(bool x);
    void setBillBoardY(bool y);
    void setReceiveShadow(bool s);
    void setCastShadow(bool s);
    void setComputeNormal();
    void setComputeTangent();

private:
    tbe::scene::Mesh* m_target;
};

#endif	/* QMESHINTERACTOR_H */

