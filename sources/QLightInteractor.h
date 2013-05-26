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
    void triggerDialog();

public slots:
    void setType(int type);
    void setAmbiant(const tbe::Vector3f& value);
    void setDiffuse(const tbe::Vector3f& value);
    void setSpecular(const tbe::Vector3f& value);
    void setRadius(double value);
    void setCastShadow(bool enable);
    void setShadowSize(int size);
    void setShadowBlur(int pass);
    void setShadowIntensity(double value);
    void setShadowShader(bool enable);
    void setCastRays(bool enable);
    void setRaysOffset(const tbe::Vector3f& value);
    void setRaysFrameSize(int size);
    void setRaysLightSize(int size);
    void setRaysNoiseLayer(QString path);

private:
    tbe::scene::Light* m_target;
};

#endif	/* QLIGHTINTERACTOR_H */

