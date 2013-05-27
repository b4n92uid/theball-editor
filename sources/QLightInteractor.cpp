/* 
 * File:   QLightInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:46
 */

#include "QLightInteractor.h"
#include "MainWindow.h"
#include "QTBEngine.h"

QLightInteractor::QLightInteractor(MainWindow* mainwin, tbe::scene::Light* target)
: QNodeInteractor(mainwin, target), m_target(target)
{
    using namespace tbe;
    using namespace scene;

    //    m_visualIndicator = new tbe::scene::Plane(target->getParallelScene(), 2, 2);
    //
    //    m_visualIndicator->getMaterial("main")->setTexture("light_node.png");
    //    m_visualIndicator->getMaterial("main")->setRenderFlags(Material::TEXTURED | Material::ALPHA | Material::COLORED);
    //    m_visualIndicator->getMaterial("main")->setAlphaThershold(0.4f);
    //    m_visualIndicator->getMaterial("main")->setColor(Vector4f(1, 0, 0, 1));
    //
    //    m_visualIndicator->setBillBoard(true);
    //    m_visualIndicator->setSerialized(false);
    //    m_visualIndicator->setCastShadow(false);
    //    m_visualIndicator->setReceiveShadow(false);
    //
    //    m_target->addChild(m_visualIndicator);
}

QLightInteractor::~QLightInteractor() { }

void QLightInteractor::triggerDialog()
{
    m_mainwin->getLightDialog()->show();
}

void QLightInteractor::setup()
{
    QVariant interface;
    interface.setValue((QNodeInteractor*) this);

    QStandardItem* itemType = new QStandardItem("Light");
    itemType->setIcon(QIcon(":/Medias/medias/light.png"));
    itemType->setData(interface, ITEM_ROLE_NODE);
    itemType->setData(QString::fromStdString(m_target->getName()), ITEM_ROLE_NAME);
    itemType->setData("Light", ITEM_ROLE_TYPE);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(m_target->getName()));
    itemName->setData(interface, ITEM_ROLE_NODE);
    itemName->setData(QString::fromStdString(m_target->getName()), ITEM_ROLE_NAME);
    itemName->setData("Light", ITEM_ROLE_TYPE);

    QItemsList items;
    items << itemType << itemName;

    m_mainwin->registerInteractor(this, items);
}

QLightInteractor* QLightInteractor::clone()
{
    QLightInteractor* inter = new QLightInteractor(m_mainwin, m_target->clone());
    return inter;
}

QString QLightInteractor::typeName() const
{
    return "Light";
}

void QLightInteractor::setType(int type)
{
    m_target->setType((tbe::scene::Light::Type)type);
}

void QLightInteractor::setAmbiant(const tbe::Vector3f& value)
{
    m_target->setAmbient(tbe::math::vec34(value));
}

void QLightInteractor::setDiffuse(const tbe::Vector3f& value)
{
    m_target->setDiffuse(tbe::math::vec34(value));
}

void QLightInteractor::setSpecular(const tbe::Vector3f& value)
{
    m_target->setSpecular(tbe::math::vec34(value));
}

void QLightInteractor::setRadius(double value)
{
    m_target->setRadius((float) value);
}

void QLightInteractor::setCastShadow(bool enable)
{
    m_target->setCastShadow(enable);
}

void QLightInteractor::setShadowSize(int size)
{
    if(!tbe::math::isPow2(size))
        return;

    m_target->getShadowMap()->setFrameSize(size);
}

void QLightInteractor::setShadowBlur(int pass)
{
    m_target->getShadowMap()->setBlurPass(pass);
}

void QLightInteractor::setShadowIntensity(double value)
{
    m_target->getShadowMap()->setIntensity((float) value);
}

void QLightInteractor::setShadowShader(bool enable)
{
    m_target->getShadowMap()->setShaderHandled(enable);
}

void QLightInteractor::setCastRays(bool enable)
{
    m_target->setCastRays(enable);
}

void QLightInteractor::setRaysOffset(const tbe::Vector3f& value)
{
    m_target->getVolumeLight()->setOffset(value);
}

void QLightInteractor::setRaysFrameSize(int size)
{
    if(!tbe::math::isPow2(size))
        return;

    m_target->getVolumeLight()->setFrameSize(size);
}

void QLightInteractor::setRaysLightSize(int size)
{
    if(!tbe::math::isPow2(size))
        return;

    m_target->getVolumeLight()->setLightSize(size);
}

void QLightInteractor::setRaysNoiseLayer(QString path)
{
    m_target->getVolumeLight()->setNoiseLayer(path.toStdString());
}

void QLightInteractor::bindInterface()
{
    QNodeInteractor::bindInterface();

    m_mainwin->getLightDialog()->bind(this);

    updateGui();
}

void QLightInteractor::unbindFromGui()
{
    QNodeInteractor::unbindFromGui();

    m_mainwin->getLightDialog()->unbind();
}

void QLightInteractor::updateGui()
{
    QNodeInteractor::updateGui();

    m_mainwin->getLightDialog()->update(m_target);

    m_mainwin->tbeWidget()->highlight(this);
}
