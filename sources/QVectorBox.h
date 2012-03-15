/* 
 * File:   QVectorBox.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 00:14
 */

#ifndef QVECTORBOX_H
#define	QVECTORBOX_H

#include <Tbe.h>
#include <QtGui/QtGui>

class QDoubleVector3Box : public QObject
{
    Q_OBJECT

public:
    QDoubleVector3Box(QObject* parent, QDoubleSpinBox* x, QDoubleSpinBox* y, QDoubleSpinBox* z);
    virtual ~QDoubleVector3Box();

public slots:
    tbe::Vector3f value();
    void setValue(const tbe::Vector3f& value);

    void clear();

    void componentValueChanged(double d);
    
    void setReadOnly(bool state);

signals:
    void valueChanged(const tbe::Vector3f& vec);

private:
    QDoubleSpinBox* m_x;
    QDoubleSpinBox* m_y;
    QDoubleSpinBox* m_z;
};

class QDoubleVector2Box : public QObject
{
    Q_OBJECT

public:
    QDoubleVector2Box(QObject* parent, QDoubleSpinBox* x, QDoubleSpinBox* y);
    virtual ~QDoubleVector2Box();

public slots:
    tbe::Vector2f value();
    void setValue(const tbe::Vector2f& value);

    void clear();

    void componentValueChanged(double d);
    
    void setReadOnly(bool state);

signals:
    void valueChanged(const tbe::Vector2f&);

private:
    QDoubleSpinBox* m_x;
    QDoubleSpinBox* m_y;
};

class QIntVector2Box : public QObject
{
    Q_OBJECT

public:
    QIntVector2Box(QObject* parent, QSpinBox* x, QSpinBox* y);
    virtual ~QIntVector2Box();

public slots:
    tbe::Vector2i value();
    void setValue(const tbe::Vector2i& value);

    void clear();

    void componentValueChanged(int d);
    
    void setReadOnly(bool state);

signals:
    void valueChanged(const tbe::Vector2i&);

private:
    QSpinBox* m_x;
    QSpinBox* m_y;
};

#endif	/* QVECTORBOX_H */

