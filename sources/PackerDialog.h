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
    void exec();

    void addFile(std::string filename);
    void addFile(QString filename);
    void addFile();
    void delFile();

    void exportPack();

protected:
    void recursivFilesFind(const tbe::scene::SceneParser::Relation& rel);

private:
    MainWindow* m_parent;
    QStandardItemModel* m_fileListModel;
    QDir m_baseDir;
};

#endif	/* PACKERDIALOG_H */

