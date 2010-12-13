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

class QVectorBox : public QObject
{
    Q_OBJECT

public:
    QVectorBox(QObject* parent, QDoubleSpinBox* x, QDoubleSpinBox* y, QDoubleSpinBox* z);
    virtual ~QVectorBox();

public slots:
    tbe::Vector3f getValue();
    void setValue(const tbe::Vector3f& value);

    void componentValueChanged(double d);

signals:
    void valueChanged(const tbe::Vector3f& vec);

private:
    QDoubleSpinBox* m_x;
    QDoubleSpinBox* m_y;
    QDoubleSpinBox* m_z;
};

#endif	/* QVECTORBOX_H */

