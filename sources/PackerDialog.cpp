/*
 * File:   PackerDialog.cpp
 * Author: b4n92uid
 *
 * Created on 17 juin 2011, 19:18
 */

#include "PackerDialog.h"

#include <quazip.h>
#include <quazipfile.h>
#include <quazipnewinfo.h>

#include "QTBEngine.h"
#include "MainWindow.h"

Q_DECLARE_METATYPE(QFileInfo)

using namespace tbe;
using namespace scene;

PackerDialog::PackerDialog(MainWindow* parent) : QDialog(parent)
{
    m_parent = parent;
    setupUi(this);

    m_fileListModel = new QStandardItemModel;
    m_fileListModel->setHorizontalHeaderLabels(QStringList() << "Fichiers" << "Taille");

    fileListView->setModel(m_fileListModel);
    fileListView->setRootIsDecorated(false);
    fileListView->setSelectionMode(QTreeView::ExtendedSelection);
    fileListView->header()->setResizeMode(QHeaderView::Stretch);

    connect(addFileButton, SIGNAL(clicked()), this, SLOT(openFilesDialog()));
    connect(delFileButton, SIGNAL(clicked()), this, SLOT(delFiles()));
    connect(exportButton, SIGNAL(clicked()), this, SLOT(exportPack()));
    connect(browseBaseDirectory, SIGNAL(clicked()), this, SLOT(changeBaseDir()));
}

PackerDialog::~PackerDialog() { }

void PackerDialog::recursivFilesFind(const tbe::scene::rtree& rel)
{

    BOOST_FOREACH(tbe::scene::rtree::value_type v, rel)
    {
        QString value = QString::fromStdString(v.second.get_value<std::string>());
        QFileInfo finfo = m_baseDir.filePath(value);

        if(finfo.isReadable())
            addRelativeFile(value);
    }

    BOOST_FOREACH(tbe::scene::rtree::value_type v, rel.get_child("node"))
    recursivFilesFind(v.second);
}

void PackerDialog::fillList()
{
    using namespace std;

    m_fileListModel->removeRows(0, m_fileListModel->rowCount());

    QTBEngine* tbewidget = m_parent->tbeWidget();
    SceneParser* parser = tbewidget->sceneParser();

    parser->prepare();

    const tbe::scene::rtree& descriptor = parser->scheme();

    addRelativeFile(QString::fromStdString(descriptor.get<string>("scene.skybox.front")));
    addRelativeFile(QString::fromStdString(descriptor.get<string>("scene.skybox.back")));
    addRelativeFile(QString::fromStdString(descriptor.get<string>("scene.skybox.top")));
    addRelativeFile(QString::fromStdString(descriptor.get<string>("scene.skybox.bottom")));
    addRelativeFile(QString::fromStdString(descriptor.get<string>("scene.skybox.left")));
    addRelativeFile(QString::fromStdString(descriptor.get<string>("scene.skybox.right")));

    foreach(QString path, m_parent->tbeWidget()->usedRessources())
    addAbsoluteFile(path);

    BOOST_FOREACH(tbe::scene::rtree::value_type v, descriptor.get_child("scene.content"))
    recursivFilesFind(v.second);

    m_fileListModel->sort(0);
}

void PackerDialog::changeBaseDir()
{
    QString basedir = QFileDialog::getExistingDirectory(this, "", m_baseDir.path());

    if(basedir.isEmpty())
        return;

    m_baseDir = basedir;

    baseDirectory->setText(m_baseDir.path());

    fillList();
}

void PackerDialog::exec()
{
    QString sceneFileName = m_parent->openFileName();

    if(sceneFileName.isEmpty())
    {
        QMessageBox::warning(m_parent, "Packer !", "Aucun fichier scene spécfier !\n"
                             "Enregistrer la scene en cour ou ouvrer un fichier scene");
        return;
    }

    m_sceneDir = m_baseDir = QFileInfo(sceneFileName).dir();

    baseDirectory->setText(m_baseDir.path());

    fillList();

    QDialog::exec();
}

inline qint64 unit(qint64 number, int it)
{
    for(; it > 0; it--)
        number /= 1024;

    return qint64(number + 0.5);
}

inline QString humainSize(qint64 size)
{
    if(size > pow(1024, 3)) // Go
        return QString("%1 Go").arg(unit(size, 3));

    else if(size > pow(1024, 2)) // Mo
        return QString("%1 Mo").arg(unit(size, 2));

    else if(size > pow(1024, 1)) // Ko
        return QString("%1 Ko").arg(unit(size, 1));

    else
        return QString("%1 Octet").arg(size);
}

void PackerDialog::addAbsoluteFile(QString absolutePath)
{
    QString relativePath = m_baseDir.relativeFilePath(absolutePath);

    if(absolutePath.isEmpty() || !m_fileListModel->findItems(relativePath).isEmpty())
        return;

    QFileInfo fileinfo = absolutePath;

    QVariant data;
    data.setValue(fileinfo);

    QStandardItem* item = new QStandardItem(relativePath);
    item->setData(data);
    item->setIcon(QFileIconProvider().icon(fileinfo.absoluteFilePath()));

    QStandardItem* size = new QStandardItem(humainSize(fileinfo.size()));

    m_fileListModel->appendRow(QList<QStandardItem*>() << item << size);
}

void PackerDialog::addRelativeFile(QString filename)
{
    QString absolutePath = m_sceneDir.absoluteFilePath(filename);
    QString relativePath = m_baseDir.relativeFilePath(absolutePath);

    if(filename.isEmpty() || !m_fileListModel->findItems(relativePath).isEmpty())
        return;

    QFileInfo fileinfo = absolutePath;

    QVariant data;
    data.setValue(fileinfo);

    QStandardItem* item = new QStandardItem(relativePath);
    item->setData(data);
    item->setIcon(QFileIconProvider().icon(absolutePath));

    QStandardItem* size = new QStandardItem(humainSize(fileinfo.size()));

    m_fileListModel->appendRow(QList<QStandardItem*>() << item << size);
}

void PackerDialog::openFilesDialog()
{
    QStringList filenames = QFileDialog::getOpenFileNames(m_parent);

    foreach(QString fn, filenames)
    addRelativeFile(fn);
}

void PackerDialog::delFiles()
{
    QItemSelectionModel* selection = fileListView->selectionModel();
    QModelIndexList list = selection->selectedRows();

    foreach(QModelIndex index, list)
    {
        QStandardItem* item = m_fileListModel->itemFromIndex(index);

        if(item->parent())
            item->parent()->removeRow(item->row());
        else
            m_fileListModel->removeRow(item->row());
    }
}

void PackerDialog::exportPack()
{
    QFileInfo baseFileInfo(m_parent->openFileName());

    QString defaultName =
            m_baseDir.filePath(QString("%1-%2.zip")
                               .arg(baseFileInfo.baseName())
                               .arg(QDate::currentDate().toString("yyyyMMdd")));

    QString filename = QFileDialog::getSaveFileName(m_parent, QString(), defaultName);

    if(!filename.isEmpty())
    {
        QuaZip ziparchive(filename);

        ziparchive.open(QuaZip::mdCreate);

        int cout = m_fileListModel->rowCount();

        for(int i = 0; i < cout; i++)
        {
            QStandardItem* item = m_fileListModel->item(i);

            QFileInfo fileinfo = item->data().value<QFileInfo > ();

            QuaZipFile zipfile(&ziparchive);

            QString zipfilename = m_baseDir.relativeFilePath(item->text());

            QuaZipNewInfo newinfo(zipfilename, fileinfo.filePath());
            zipfile.open(QIODevice::WriteOnly, newinfo);

            QFile reader(fileinfo.filePath());
            reader.open(QIODevice::ReadOnly);

            zipfile.write(reader.readAll());

            zipfile.close();
        }

        if(addScreenshot->isChecked())
        {
            QImage screenshot = m_parent->tbeWidget()->grabFrameBuffer();

            QuaZipFile zipfile(&ziparchive);

            QuaZipNewInfo newinfo(QString("%1-screenshot.png").arg(baseFileInfo.baseName()));
            zipfile.open(QIODevice::WriteOnly, newinfo);

            screenshot.save(&zipfile, "PNG");
        }

        QuaZipFile zipfile(&ziparchive);

        QuaZipNewInfo newinfo(m_baseDir.relativeFilePath(m_parent->openFileName()), m_parent->openFileName());
        zipfile.open(QIODevice::WriteOnly, newinfo);

        QFile reader(m_parent->openFileName());
        reader.open(QIODevice::ReadOnly);

        zipfile.write(reader.readAll());

        ziparchive.close();

        reject();
    }
}
