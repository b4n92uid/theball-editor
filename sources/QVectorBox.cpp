/*
 * File:   QVectorBox.cpp
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 00:14
 */

#include "QVectorBox.h"

QDoubleVector3Box::QDoubleVector3Box(QObject* parent, QDoubleSpinBox* x, QDoubleSpinBox* y, QDoubleSpinBox* z) : QObject(parent)
{
    m_x = x;
    m_y = y;
    m_z = z;

    connect(m_x, SIGNAL(valueChanged(double)), this, SLOT(componentValueChanged(double)));
    connect(m_y, SIGNAL(valueChanged(double)), this, SLOT(componentValueChanged(double)));
    connect(m_z, SIGNAL(valueChanged(double)), this, SLOT(componentValueChanged(double)));
}

QDoubleVector3Box::~QDoubleVector3Box()
{
}

tbe::Vector3f QDoubleVector3Box::value()
{
    return tbe::Vector3f(m_x->value(), m_y->value(), m_z->value());
}

void QDoubleVector3Box::setValue(const tbe::Vector3f& value)
{
    m_x->setValue(value.x);
    m_y->setValue(value.y);
    m_z->setValue(value.z);
}

void QDoubleVector3Box::componentValueChanged(double d)
{
    emit valueChanged(value());
}

void QDoubleVector3Box::clear()
{
    m_x->clear();
    m_y->clear();
    m_z->clear();
}

void QDoubleVector3Box::setReadOnly(bool state)
{
    m_x->setReadOnly(state);
    m_y->setReadOnly(state);
    m_z->setReadOnly(state);
}

void QDoubleVector3Box::setEnabled(bool state)
{
    m_x->setEnabled(state);
    m_y->setEnabled(state);
    m_z->setEnabled(state);
}

QDoubleVector2Box::QDoubleVector2Box(QObject* parent, QDoubleSpinBox* x, QDoubleSpinBox* y)
{
    m_x = x;
    m_y = y;

    connect(m_x, SIGNAL(valueChanged(double)), this, SLOT(componentValueChanged(double)));
    connect(m_y, SIGNAL(valueChanged(double)), this, SLOT(componentValueChanged(double)));
}

QDoubleVector2Box::~QDoubleVector2Box()
{
}

tbe::Vector2f QDoubleVector2Box::value()
{
    return tbe::Vector2f(m_x->value(), m_y->value());
}

void QDoubleVector2Box::setValue(const tbe::Vector2f& value)
{
    m_x->setValue(value.x);
    m_y->setValue(value.y);
}

void QDoubleVector2Box::componentValueChanged(double d)
{
    emit valueChanged(value());
}

void QDoubleVector2Box::clear()
{
    m_x->clear();
    m_y->clear();
}

void QDoubleVector2Box::setReadOnly(bool state)
{
    m_x->setReadOnly(state);
    m_y->setReadOnly(state);
}

QIntVector2Box::QIntVector2Box(QObject* parent, QSpinBox* x, QSpinBox* y)
{
    m_x = x;
    m_y = y;

    connect(m_x, SIGNAL(valueChanged(int)), this, SLOT(componentValueChanged(int)));
    connect(m_y, SIGNAL(valueChanged(int)), this, SLOT(componentValueChanged(int)));
}

QIntVector2Box::~QIntVector2Box()
{
}

tbe::Vector2i QIntVector2Box::value()
{
    return tbe::Vector2i(m_x->value(), m_y->value());
}

void QIntVector2Box::setValue(const tbe::Vector2i& value)
{
    m_x->setValue(value.x);
    m_y->setValue(value.y);
}

void QIntVector2Box::componentValueChanged(int d)
{
    emit valueChanged(value());
}

void QIntVector2Box::clear()
{
    m_x->clear();
    m_y->clear();
}

void QIntVector2Box::setReadOnly(bool state)
{
    m_x->setReadOnly(state);
    m_y->setReadOnly(state);
}
