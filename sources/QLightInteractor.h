/* 
 * File:   QLightInteractor.h
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 07:46
 */

#ifndef QLIGHTINTERACTOR_H
#define	QLIGHTINTERACTOR_H

#include "QNodeInteractor.h"

class QLightInteractor : public QNodeInteractor
{
    Q_OBJECT

public:
    QLightInteractor(MainWindow* mainwin, tbe::scene::Light* target);
    virtual ~QLightInteractor();

public slots:
    void select();
    void deselect();
    void update();

public slots:
    void setType(int type);
    void setAmbiant(const tbe::Vector3f& value);
    void setDiffuse(const tbe::Vector3f& value);
    void setSpecular(const tbe::Vector3f& value);
    void setRadius(double value);

private:
    tbe::scene::Light* m_target;
};

#endif	/* QLIGHTINTERACTOR_H */

