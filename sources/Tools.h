/* 
 * File:   Tools.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 22:00
 */

#ifndef TOOLS_H
#define	TOOLS_H

#include <Tbe.h>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QDebug>

inline tbe::Vector2i qstovec(QSize size)
{
    return tbe::Vector2i(size.width(), size.height());
}

inline tbe::Vector2i qptovec(QPoint size)
{
    return tbe::Vector2i(size.x(), size.y());
}

inline tbe::Vector2f qpftovec(QPointF point)
{
    return tbe::Vector2f(point.x(), point.y());
}

QDebug operator<<(QDebug d, const tbe::Vector3f& vec)
{
    return d << vec.x << "," << vec.y << "," << vec.z;
}

QDebug operator<<(QDebug d, const tbe::AABB& aabb)
{
    return d << aabb.min << aabb.max;
}

#endif	/* TOOLS_H */

