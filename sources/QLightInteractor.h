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

    void setup();

    virtual QLightInteractor* clone();

    virtual QString typeName() const;

public slots:
    void bindWithGui();
    void unbindFromGui();
    void updateGui();

public slots:
    void setType(int type);
    void setAmbiant(const tbe::Vector3f& value);
    void setDiffuse(const tbe::Vector3f& value);
    void setSpecular(const tbe::Vector3f& value);
    void setRadius(double value);
    void setCastShadow(bool enable);

private:
    tbe::scene::Light* m_target;
};

#endif	/* QLIGHTINTERACTOR_H */

