/*
 * File:   PackerDialog.h
 * Author: b4n92uid
 *
 * Created on 17 juin 2011, 19:18
 */

#ifndef PACKERDIALOG_H
#define	PACKERDIALOG_H

#include <QStandardItemModel>
#include <QDir>
#include <Tbe.h>

#include "ui_packer.h"

class MainWindow;

class PackerDialog : public QDialog, public Ui::Packer
{
    Q_OBJECT

public:
    PackerDialog(MainWindow* parent);
    virtual ~PackerDialog();

public slots:
    int exec();

    void openFilesDialog();

    void addRelativeFile(QString filename);
    void addAbsoluteFile(QString filename);

    void delFiles();

    void fillList();

    void changeBaseDir();

    void exportPack();

protected:
    void recursivFilesFind(const tbe::rtree& rel);

private:
    MainWindow* m_parent;
    QStandardItemModel* m_fileListModel;
    QDir m_baseDir;
    QDir m_sceneDir;
};

#endif	/* PACKERDIALOG_H */

