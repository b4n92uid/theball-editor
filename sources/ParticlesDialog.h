/* 
 * File:   ParticlesDialog.h
 * Author: b4n92uid
 *
 * Created on 23 mai 2013, 17:09
 */

#ifndef PARTICLESDIALOG_H
#define	PARTICLESDIALOG_H

#include <Tbe.h>
#include <QDialog>
#include "ui_particles.h"

class QParticlesInteractor;
class QBrowsEdit;
class QDoubleVector2Box;
class QDoubleVector3Box;
class MainWindow;

class ParticlesDialog : public QDialog, public Ui_ParticlesDialog
{
public:
    ParticlesDialog(MainWindow* parent);
    virtual ~ParticlesDialog();

    void bind(QParticlesInteractor* li);
    void unbind();

    void update(tbe::scene::ParticlesEmiter* l);
    
private:
    MainWindow* m_mainwin;
    QDoubleVector3Box* gravity;
    QDoubleVector3Box* boxsize;
    QDoubleVector2Box* bulletsize;
    QBrowsEdit* texture;
};

#endif	/* PARTICLESDIALOG_H */

