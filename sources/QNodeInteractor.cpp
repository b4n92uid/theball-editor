/*
 * File:   QNodeInteractor.cpp
 * Author: b4n92uid
 *
 * Created on 29 août 2011, 07:22
 */

#include "QNodeInteractor.h"
#include "MainWindow.h"
#include "QTBEngine.h"
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

QNodeInteractor* QNodeInteractor::getParent()
{
    return m_mainwin->tbeWidget()->getInteractor(m_target->getParent());
}

void QNodeInteractor::setName(const QString& s)
{
    if(m_target)
    {
        m_target->setName(s.toStdString());
        itemRows()[1]->setText(s);

        m_mainwin->notifyChange(true);
    }
}

void QNodeInteractor::setPos(const tbe::Vector3f& v)
{
    if(m_target)
    {
        m_target->setPos(v);

        m_mainwin->tbeWidget()->placeCamera();

        m_mainwin->notifyChange(true);

        updateGui();
    }
}

void QNodeInteractor::setRotation(const tbe::Vector3f& v)
{
    if(m_target)
    {
        using namespace tbe;

        m_target->setRotation(Quaternion(v * M_PI / 180));

        m_mainwin->notifyChange(true);

        updateGui();
    }
}

void QNodeInteractor::setScale(const tbe::Vector3f& v)
{
    if(m_target)
    {
        m_target->setScale(v);

        m_mainwin->notifyChange(true);

        updateGui();
    }
}

void QNodeInteractor::setMatrix(const tbe::Matrix4& m)
{
    if(m_target)
    {
        m_target->setMatrix(m);

        m_mainwin->tbeWidget()->placeCamera();

        m_mainwin->notifyChange(true);

        updateGui();
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

    QStandardItem* itemc1 = (itemc0->parent()
            ? itemc0->parent()
            : m_mainwin->nodesGui.nodesListModel->invisibleRootItem())
            ->child(itemc0->row(), 1);

    return QItemsList() << itemc0 << itemc1;
}

QItemSelection QNodeInteractor::itemSelection()
{
    QStandardItem* itemc0 = m_mainwin->nodeItemBinder[this];

    QStandardItem* itemc1 = (itemc0->parent()
            ? itemc0->parent()
            : m_mainwin->nodesGui.nodesListModel->invisibleRootItem())
            ->child(itemc0->row(), 1);

    return QItemSelection(itemc0->index(), itemc1->index());
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

    m_mainwin->notifyChange(true);
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

        m_mainwin->notifyChange(true);
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
        m_mainwin->notifyChange(true);
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

    m_mainwin->notifyChange(true);
}

void QNodeInteractor::unsetup()
{
    m_mainwin->unregisterInteractor(this);
}

void QNodeInteractor::bindInterface()
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

    m_mainwin->nodesGui.nodesListView->blockSignals(true);
    m_mainwin->nodesGui.nodesListView->highlightItem(this);
    m_mainwin->nodesGui.nodesListView->blockSignals(false);

    QList<QStandardItem*> list = itemRows();

    foreach(QStandardItem* it, list)
    {
        QFont font = it->font();
        font.setBold(true);

        it->setFont(font);
    }
}

void QNodeInteractor::unbindFromGui()
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

    QList<QStandardItem*> list = itemRows();

    foreach(QStandardItem* it, list)
    {
        QFont font = it->font();
        font.setBold(false);

        it->setFont(font);
    }
}

void QNodeInteractor::updateGui()
{
    using namespace tbe;

    QSignalBlockerStream blocker;
    blocker
            << m_mainwin->nodesGui.name
            << m_mainwin->nodesGui.position
            << m_mainwin->nodesGui.scale
            << m_mainwin->nodesGui.rotation
            << m_mainwin->nodesGui.enable
            << m_mainwin->nodesGui.lock;

    blocker.block();

    QStandardItem* item = m_mainwin->nodeItemBinder[this];

    if(item->parent())
        item->parent()->child(item->row(), 1)->setText(QString::fromStdString(m_target->getName()));
    else
        m_mainwin->nodesGui.nodesListModel->item(item->row(), 1)->setText(QString::fromStdString(m_target->getName()));

    m_mainwin->nodesGui.name->setText(QString::fromStdString(m_target->getName()));

    tbe::Vector3f position, scale;
    tbe::Quaternion rotation;

    position = m_target->getPos();
    rotation = m_target->getRotation();
    scale = m_target->getScale();

    m_mainwin->nodesGui.position->setValue(position);
    m_mainwin->nodesGui.scale->setValue(scale);
    m_mainwin->nodesGui.rotation->setValue(rotation.getEuler() * 180 / M_PI);

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
