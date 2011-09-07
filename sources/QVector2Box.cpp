/*
 * File:   QVector2Box.cpp
 * Author: b4n92uid
 *
 * Created on 5 décembre 2010, 23:01
 */

#include "QVector2Box.h"

QVector2Box::QVector2Box(QObject* parent, QDoubleSpinBox* x, QDoubleSpinBox* y)
{
    m_x = x;
    m_y = y;

    connect(m_x, SIGNAL(valueChanged(double)), this, SLOT(componentValueChanged(double)));
    connect(m_y, SIGNAL(valueChanged(double)), this, SLOT(componentValueChanged(double)));
}

QVector2Box::~QVector2Box()
{
}

tbe::Vector2f QVector2Box::value()
{
    return tbe::Vector2f(m_x->value(), m_y->value());
}

void QVector2Box::setValue(const tbe::Vector2f& value)
{
    m_x->setValue(value.x);
    m_y->setValue(value.y);
}

void QVector2Box::componentValueChanged(double d)
{
    emit valueChanged(value());
}

void QVector2Box::clear()
{
    m_x->clear();
    m_y->clear();
}
