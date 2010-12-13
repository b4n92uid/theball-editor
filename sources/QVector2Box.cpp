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

    connect(m_x, SIGNAL(valueChanged(double)), this, SLOT(ComponentValueChanged(double)));
    connect(m_y, SIGNAL(valueChanged(double)), this, SLOT(ComponentValueChanged(double)));

    m_x->setSingleStep(0.1);
    m_y->setSingleStep(0.1);
}

QVector2Box::~QVector2Box()
{
}

tbe::Vector2f QVector2Box::GetVectorValue()
{
    return tbe::Vector2f(m_x->value(), m_y->value());
}

void QVector2Box::SetVectorValue(const tbe::Vector2f& value)
{
    m_x->setValue(value.x);
    m_y->setValue(value.y);
}

void QVector2Box::ComponentValueChanged(double d)
{
    emit valueChanged(GetVectorValue());
}
