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
    m_fileListModel->setHorizontalHeaderLabels(QStringList() << "Fichiers");

    fileListView->setModel(m_fileListModel);

    connect(exportButton, SIGNAL(clicked()), this, SLOT(exportPack()));
    connect(addFileButton, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(delFileButton, SIGNAL(clicked()), this, SLOT(delFile()));
}

PackerDialog::~PackerDialog()
{
}

void PackerDialog::recursivFilesFind(const SceneParser::Relation& rel)
{
    for(SceneParser::AttribMap::const_iterator it = rel.attr.begin(); it != rel.attr.end(); it++)
    {
        QString value = QString::fromStdString(it->second);
        QFileInfo finfo = m_baseDir.filePath(value);

        if(finfo.isReadable())
            addFile(value);
    }

    for(unsigned i = 0; i < rel.child.size(); i++)
        recursivFilesFind(rel.child[i]);
}

void PackerDialog::exec()
{
    m_baseDir = QFileInfo(m_parent->getOpenFileName()).dir();

    m_fileListModel->removeRows(0, m_fileListModel->rowCount());

    QTBEngine* tbewidget = m_parent->getTbeWidget();
    SceneParser* parser = tbewidget->getSceneParser();

    parser->prepareScene();

    SceneParser::MapDescriptor& descriptor = parser->getMapDescriptor();

    addFile(descriptor.skybox.front);
    addFile(descriptor.skybox.back);
    addFile(descriptor.skybox.top);
    addFile(descriptor.skybox.bottom);
    addFile(descriptor.skybox.left);
    addFile(descriptor.skybox.right);

    for(unsigned i = 0; i < descriptor.nodes.size(); i++)
        recursivFilesFind(descriptor.nodes[i]);

    m_fileListModel->sort(0);

    QDialog::exec();
}

void PackerDialog::addFile(std::string filename)
{
    addFile(QString::fromStdString(filename));
}

void PackerDialog::addFile(QString filename)
{
    if(!m_fileListModel->findItems(filename).isEmpty())
        return;

    QFileInfo fileinfo = m_baseDir.filePath(filename);

    QVariant data;
    data.setValue(fileinfo);

    QStandardItem* item = new QStandardItem(filename);
    item->setData(data);
    item->setIcon(QFileIconProvider().icon(fileinfo.absoluteFilePath()));

    QModelIndex index = fileListView->currentIndex();

    if(index.isValid())
        m_fileListModel->itemFromIndex(index)->appendRow(item);
    else
        m_fileListModel->appendRow(item);
}

void PackerDialog::addFile()
{
    QString filename = QFileDialog::getOpenFileName(m_parent);

    if(!filename.isEmpty())
        addFile(filename);
}

void PackerDialog::delFile()
{
    QModelIndex index = fileListView->currentIndex();
    QStandardItem* item = m_fileListModel->itemFromIndex(index);

    if(item->parent())
        item->parent()->removeRow(item->row());
    else
        m_fileListModel->removeRow(item->row());
}

void PackerDialog::exportPack()
{
    QFileInfo baseFileInfo(m_parent->getOpenFileName());

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

            QuaZipNewInfo newinfo(m_baseDir.relativeFilePath(fileinfo.filePath()), fileinfo.filePath());
            zipfile.open(QIODevice::WriteOnly, newinfo);

            QFile writer(fileinfo.filePath());
            writer.open(QIODevice::ReadOnly);

            zipfile.write(writer.readAll());

            zipfile.close();
        }

        if(addScreenshot->isChecked())
        {
            QImage screenshot = m_parent->getTbeWidget()->grabFrameBuffer();

            QuaZipFile zipfile(&ziparchive);

            QuaZipNewInfo newinfo(QString("%1-screenshot.png").arg(baseFileInfo.baseName()));
            zipfile.open(QIODevice::WriteOnly, newinfo);

            screenshot.save(&zipfile, "PNG");
        }

        QuaZipFile zipfile(&ziparchive);

        QuaZipNewInfo newinfo(m_baseDir.relativeFilePath(m_parent->getOpenFileName()), m_parent->getOpenFileName());
        zipfile.open(QIODevice::WriteOnly, newinfo);

        QFile writer(m_parent->getOpenFileName());
        writer.open(QIODevice::ReadOnly);

        zipfile.write(writer.readAll());

        ziparchive.close();
    }
}
