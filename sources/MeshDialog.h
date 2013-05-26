/* 
 * File:   MeshDialog.h
 * Author: b4n92uid
 *
 * Created on 23 mai 2013, 17:09
 */

#ifndef MESHDIALOG_H
#define	MESHDIALOG_H

#include <Tbe.h>
#include <QDialog>
#include "ui_mesh.h"

class QMeshInteractor;
class MaterialDialog;
class MainWindow;

class MeshDialog : public QDialog, public Ui_MeshDialog
{
    Q_OBJECT

public:
    MeshDialog(MainWindow* parent);
    virtual ~MeshDialog();

    void bind(QMeshInteractor* mi);
    void unbind();

    void update(tbe::scene::Mesh* m);

public slots:
    void onOpenMaterialDialog();

    void onAttachMaterial();
    void onReleaseMaterial();

signals:
    void attachMaterialFile(QString);
    void releaseMaterialFile();

private:
    MainWindow* m_mainwin;
    MaterialDialog* m_materialDialog;
};

#endif	/* MESHDIALOG_H */

