/* 
 * File:   QNodeInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:22
 */

#include "QNodeInteractor.h"
#include "MainWindow.h"
#include "ui_interface.h"

QNodeInteractor::QNodeInteractor(MainWindow* mainwin, tbe::scene::Node* target) :
m_mainwin(mainwin), m_target(target)
{
    m_locked = false;
}

QNodeInteractor::~QNodeInteractor()
{
    unsetup();
}

void QNodeInteractor::setName(const QString& s)
{
    if(m_target)
    {
        m_target->setName(s.toStdString());

        m_mainwin->notifyChanges(true);
    }
}

void QNodeInteractor::setPos(const tbe::Vector3f& v)
{
    if(m_target)
    {
        m_target->setPos(v);

        m_mainwin->tbeWidget()->placeCamera();

        m_mainwin->notifyChanges(true);

        update();
    }
}

void QNodeInteractor::setRotation(const tbe::Vector3f& v)
{
    if(m_target)
    {
        using namespace tbe;

        Matrix4 newmat = m_target->getMatrix();
        newmat.setRotate(tbe::Quaternion(v * M_PI / 180));

        m_target->setMatrix(newmat);

        m_mainwin->notifyChanges(true);

        update();
    }
}

void QNodeInteractor::setScale(const tbe::Vector3f& v)
{
    if(m_target)
    {
        m_target->getMatrix().setScale(v);

        m_mainwin->notifyChanges(true);

        update();
    }
}

void QNodeInteractor::setMatrix(const tbe::Matrix4& m)
{
    if(m_target)
    {
        m_target->setMatrix(m);

        m_mainwin->tbeWidget()->placeCamera();

        m_mainwin->notifyChanges(true);

        update();
    }
}

bool QNodeInteractor::isEnable()
{
    return m_target->isEnable();
}

void QNodeInteractor::setEnable(bool state)
{
    m_target->setEnable(state);

    QList<QStandardItem*> list = itemRows();

    foreach(QStandardItem* item, list)
    item->setForeground(state ? Qt::black : Qt::gray);
}

bool QNodeInteractor::isLocked()
{
    return m_locked;
}

void QNodeInteractor::setLocked(bool state)
{
    m_locked = state;

    QList<QStandardItem*> list = itemRows();

    foreach(QStandardItem* item, list)
    {
        QFont font = item->font();
        font.setItalic(state);

        item->setFont(font);
    }
}

QItemsList QNodeInteractor::itemRows()
{
    QStandardItem* itemc0 = m_mainwin->nodeItemBinder[this];
    QStandardItem* itemc1 = m_mainwin->nodesGui.nodesListModel->item(itemc0->row(), 1);

    return QItemsList() << itemc0 << itemc1;
}

QStandardItem* QNodeInteractor::item()
{
    return m_mainwin->nodeItemBinder[this];
}

tbe::scene::Node* QNodeInteractor::target() const
{
    return m_target;
}

MainWindow* QNodeInteractor::mainwin() const
{
    return m_mainwin;
}

QString QNodeInteractor::typeName() const
{
    return "Node";
}

void QNodeInteractor::addNodeField()
{
    if(!m_target)
        return;

    QList<QStandardItem*> newfield;

    newfield
            << new QStandardItem("[nouvelle clé]")
            << new QStandardItem("[nouvelle valeur]");

    m_mainwin->nodesGui.additionalModel->appendRow(newfield);

    m_mainwin->notifyChanges(true);
}

void QNodeInteractor::delNodeField()
{
    if(!m_target)
        return;

    QModelIndex i = m_mainwin->nodesGui.additionalView->currentIndex();

    if(i.isValid())
    {
        QString key = m_mainwin->nodesGui.additionalModel->item(i.row(), 0)->text();

        m_target->delUserData(key.toStdString());

        m_mainwin->nodesGui.additionalModel->removeRow(i.row());

        m_mainwin->notifyChanges(true);
    }
}

void QNodeInteractor::clearNodeField()
{
    if(!m_target)
        return;

    if(m_mainwin->nodesGui.additionalModel->rowCount() <= 0)
        return;

    int re = QMessageBox::warning(m_mainwin, "Question ?", "Effacer tout les champs ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if(re == QMessageBox::Yes)
    {
        m_target->clearUserData();
        m_mainwin->nodesGui.additionalModel->removeRows(0, m_mainwin->nodesGui.additionalModel->rowCount());
        m_mainwin->notifyChanges(true);
    }
}

void QNodeInteractor::changeNodeField(QStandardItem* item)
{
    if(!m_target)
        return;

    if(item->column() == 0)
    {
        QStandardItem* value = m_mainwin->nodesGui.additionalModel->item(item->row(), 1);

        m_target->delUserData(item->text().toStdString());
        m_target->setUserData(item->text().toStdString(), value->text().toStdString());
    }
    else if(item->column() == 1)
    {
        QStandardItem* key = m_mainwin->nodesGui.additionalModel->item(item->row(), 0);
        m_target->setUserData(key->text().toStdString(), item->text().toStdString());
    }

    m_mainwin->notifyChanges(true);
}

void QNodeInteractor::unsetup()
{
    m_mainwin->unreg(this);
}

void QNodeInteractor::setup()
{
}

void QNodeInteractor::select()
{
    connect(m_mainwin->nodesGui.name, SIGNAL(textChanged(const QString&)), this, SLOT(setName(const QString&)));
    connect(m_mainwin->nodesGui.position, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setPos(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.rotation, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setRotation(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.scale, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setScale(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.enable, SIGNAL(clicked(bool)), this, SLOT(setEnable(bool)));
    connect(m_mainwin->nodesGui.lock, SIGNAL(clicked(bool)), this, SLOT(setLocked(bool)));

    connect(m_mainwin->nodesGui.additionalModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(changeNodeField(QStandardItem*)));

    connect(m_mainwin->nodesGui.addField, SIGNAL(clicked()), this, SLOT(addNodeField()));
    connect(m_mainwin->nodesGui.delField, SIGNAL(clicked()), this, SLOT(delNodeField()));
    connect(m_mainwin->nodesGui.clearFields, SIGNAL(clicked()), this, SLOT(clearNodeField()));


    QStandardItem* item = m_mainwin->nodeItemBinder[this];

    m_mainwin->nodesGui.nodesListView->blockSignals(true);

    QModelIndex index = m_mainwin->nodesGui.nodesListProxyModel->mapFromSource(item->index());

    if(index.isValid())
        m_mainwin->nodesGui.nodesListView->setCurrentIndex(index);

    m_mainwin->nodesGui.nodesListView->blockSignals(false);
}

void QNodeInteractor::deselect()
{
    disconnect(m_mainwin->nodesGui.name, SIGNAL(textChanged(const QString&)), 0, 0);
    disconnect(m_mainwin->nodesGui.position, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.rotation, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.scale, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.enable, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.lock, SIGNAL(clicked(bool)), 0, 0);

    disconnect(m_mainwin->nodesGui.additionalModel, SIGNAL(itemChanged(QStandardItem*)), 0, 0);

    disconnect(m_mainwin->nodesGui.addField, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.delField, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.clearFields, SIGNAL(clicked()), 0, 0);

    m_mainwin->nodesGui.name->clear();
    m_mainwin->nodesGui.position->clear();
    m_mainwin->nodesGui.rotation->clear();
    m_mainwin->nodesGui.scale->clear();
    m_mainwin->nodesGui.enable->setChecked(false);

    m_mainwin->nodesGui.additionalModel
            ->removeRows(0, m_mainwin->nodesGui.additionalModel->rowCount());
}

void QNodeInteractor::update()
{
    using namespace tbe;

    QSignalBlocker blocker;
    blocker
            << m_mainwin->nodesGui.name
            << m_mainwin->nodesGui.position
            << m_mainwin->nodesGui.scale
            << m_mainwin->nodesGui.rotation
            << m_mainwin->nodesGui.enable;

    blocker.block();

    QStandardItem* item = m_mainwin->nodeItemBinder[this];

    if(item->parent())
        item->parent()->child(item->row(), 1)->setText(QString::fromStdString(m_target->getName()));
    else
        m_mainwin->nodesGui.nodesListModel->item(item->row(), 1)->setText(QString::fromStdString(m_target->getName()));

    m_mainwin->nodesGui.name->setText(QString::fromStdString(m_target->getName()));

    m_mainwin->nodesGui.position->setValue(m_target->getPos());
    m_mainwin->nodesGui.scale->setValue(m_target->getMatrix().getScale());
    m_mainwin->nodesGui.rotation->setValue(m_target->getMatrix().getRotate().getEuler() * 180 / M_PI);

    m_mainwin->nodesGui.enable->setChecked(m_target->isEnable());
    m_mainwin->nodesGui.lock->setChecked(m_locked);

    blocker.unblock();

    // Additional field

    const Any::Map userData = m_target->getUserDatas();

    m_mainwin->nodesGui.additionalModel->removeRows(0, m_mainwin->nodesGui.additionalModel->rowCount());

    for(Any::Map::const_iterator it = userData.begin(); it != userData.end(); it++)
    {
        QList<QStandardItem*> newfield;

        newfield
                << new QStandardItem(QString::fromStdString(it->first))
                << new QStandardItem(QString::fromStdString(it->second.getValue<std::string > ()));

        m_mainwin->nodesGui.additionalModel->appendRow(newfield);
    }

    m_mainwin->tbeWidget()->placeCamera();
}

QNodeInteractor* QNodeInteractor::clone()
{
    return NULL;
}
