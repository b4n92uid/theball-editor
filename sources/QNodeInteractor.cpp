/* 
 * File:   QNodeInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:22
 */

#include "QNodeInteractor.h"
#include "MainWindow.h"

QNodeInteractor::QNodeInteractor() : m_mainwin(NULL), m_target(NULL)
{
    m_protectNode = false;
}

QNodeInteractor::QNodeInteractor(MainWindow* mainwin, tbe::scene::Node* target)
: m_mainwin(mainwin), m_target(target)
{
    m_protectNode = false;
}

QNodeInteractor::~QNodeInteractor()
{
    if(!m_protectNode)
    {
        QStandardItem* item = m_mainwin->nodesGui.nodeItemBinder[this];
        QStandardItem* parent = item->parent();

        m_mainwin->nodesGui.nodeItemBinder.remove(this);

        if(parent)
            parent->removeRow(item->row());
        else
            m_mainwin->nodesGui.nodesListModel->removeRow(item->row());

        m_mainwin->notifyChanges(true);
    }
}

QNodeInteractor::operator tbe::scene::Node*()
{
    return m_target;
}

tbe::scene::Node* QNodeInteractor::operator->()
{
    return m_target;
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
        m_mainwin->notifyChanges(true);

        m_mainwin->getTbeWidget()->placeCamera();
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
    }
}

void QNodeInteractor::setScale(const tbe::Vector3f& v)
{
    if(m_target)
    {
        /*
        if(m_selectedNode->mesh())
            m_selectedNode->mesh()->setVertexScale(v);
         */

        m_target->getMatrix().setScale(v);
        m_mainwin->notifyChanges(true);
    }
}

void QNodeInteractor::setMatrix(const tbe::Matrix4& m)
{
    if(m_target)
    {
        m_target->setMatrix(m);

        m_mainwin->notifyChanges(true);
    }
}

void QNodeInteractor::setEnalbe(bool stat)
{
    m_target->setEnable(stat);

    m_mainwin->notifyChanges(true);
}

void QNodeInteractor::setProtectNode(bool protectNode)
{
    this->m_protectNode = protectNode;
}

bool QNodeInteractor::isProtectNode() const
{
    return m_protectNode;
}

tbe::scene::Node* QNodeInteractor::getTarget() const
{
    return m_target;
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

void QNodeInteractor::select()
{
    connect(m_mainwin->nodesGui.name, SIGNAL(textChanged(const QString&)), this, SLOT(setName(const QString&)));
    connect(m_mainwin->nodesGui.position, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setPos(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.rotation, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setRotation(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.scale, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setScale(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.enable, SIGNAL(clicked(bool)), this, SLOT(setEnalbe(bool)));

    connect(m_mainwin->nodesGui.additionalModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(changeNodeField(QStandardItem*)));

    connect(m_mainwin->nodesGui.addField, SIGNAL(clicked()), this, SLOT(addNodeField()));
    connect(m_mainwin->nodesGui.delField, SIGNAL(clicked()), this, SLOT(delNodeField()));
    connect(m_mainwin->nodesGui.clearFields, SIGNAL(clicked()), this, SLOT(clearNodeField()));
}

void QNodeInteractor::deselect()
{
    disconnect(m_mainwin->nodesGui.name, SIGNAL(textChanged(const QString&)), 0, 0);
    disconnect(m_mainwin->nodesGui.position, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.rotation, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.scale, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(m_mainwin->nodesGui.enable, SIGNAL(clicked(bool)), 0, 0);


    m_mainwin->nodesGui.name->clear();
    m_mainwin->nodesGui.position->clear();
    m_mainwin->nodesGui.rotation->clear();
    m_mainwin->nodesGui.scale->clear();
    m_mainwin->nodesGui.enable->setChecked(false);
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

    QStandardItem* item = m_mainwin->nodesGui.nodeItemBinder[this];

    if(item->parent())
        item->parent()->child(item->row(), 1)->setText(QString::fromStdString(m_target->getName()));
    else
        m_mainwin->nodesGui.nodesListModel->item(item->row(), 1)->setText(QString::fromStdString(m_target->getName()));

    m_mainwin->nodesGui.name->setText(QString::fromStdString(m_target->getName()));

    m_mainwin->nodesGui.position->setValue(m_target->getPos());

    /*
    if(m_selectedNode->mesh())
        nodesGui.scale->setValue(m_selectedNode->mesh()->getVertexScale());
     */

    m_mainwin->nodesGui.scale->setValue(m_target->getMatrix().getScale());
    m_mainwin->nodesGui.rotation->setValue(m_target->getMatrix().getRotate().getEuler() * 180 / M_PI);
    m_mainwin->nodesGui.enable->setChecked(m_target->isEnable());

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

    m_mainwin->getTbeWidget()->placeCamera();
}

QNodeInteractor* QNodeInteractor::clone()
{
    return NULL;
}
