/*
 * File:   QVector2Box.h
 * Author: b4n92uid
 *
 * Created on 5 décembre 2010, 23:01
 */

#ifndef QVECTOR2BOX_H
#define	QVECTOR2BOX_H

#include <Tbe.h>
#include <QtGui/QtGui>

class QVector2Box : public QObject
{
    Q_OBJECT

public:
    QVector2Box(QObject* parent, QDoubleSpinBox* x, QDoubleSpinBox* y);
    virtual ~QVector2Box();

public slots:
    tbe::Vector2f value();
    void setValue(const tbe::Vector2f& value);

    void clear();

    void componentValueChanged(double d);

signals:
    void valueChanged(const tbe::Vector2f&);

private:
    QDoubleSpinBox* m_x;
    QDoubleSpinBox* m_y;
};

#endif	/* QVECTOR2BOX_H */

