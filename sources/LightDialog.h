/* 
 * File:   LightDialog.h
 * Author: b4n92uid
 *
 * Created on 23 mai 2013, 17:08
 */

#ifndef LIGHTDIALOG_H
#define	LIGHTDIALOG_H

#include <Tbe.h>
#include <QDialog>
#include "ui_light.h"

class QLightInteractor;
class QDoubleVector3Box;
class QBrowsEdit;
class MainWindow;

class LightDialog : public QDialog, public Ui_LightDialog
{
public:
    LightDialog(MainWindow* parent);
    virtual ~LightDialog();

    void bind(QLightInteractor* li);
    void unbind();

    void update(tbe::scene::Light* l);

private:
    MainWindow* m_mainwin;
    QDoubleVector3Box* ambiant;
    QDoubleVector3Box* diffuse;
    QDoubleVector3Box* specular;
    QDoubleVector3Box* rays_offset;
    QBrowsEdit* rays_noise;
};

#endif	/* LIGHTDIALOG_H */

