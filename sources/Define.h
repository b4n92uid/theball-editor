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

typedef QList<QStandardItem*> QItemsList;

class QNodeInteractor;
class QMeshInteractor;

typedef QMap<QNodeInteractor*, QStandardItem*> QInterfaceItemsMap;
typedef QMap<tbe::scene::Node*, QNodeInteractor*> QNodeInterfaceMap;

Q_DECLARE_METATYPE(tbe::scene::Node*)
Q_DECLARE_METATYPE(tbe::scene::Mesh*)
Q_DECLARE_METATYPE(tbe::scene::Water*)
Q_DECLARE_METATYPE(tbe::scene::ParticlesEmiter*)
Q_DECLARE_METATYPE(tbe::scene::Light*)
Q_DECLARE_METATYPE(tbe::scene::Material*)
Q_DECLARE_METATYPE(tbe::scene::MapMark*)
Q_DECLARE_METATYPE(tbe::Texture)
Q_DECLARE_METATYPE(tbe::Vector3f)
Q_DECLARE_METATYPE(QNodeInteractor*)
Q_DECLARE_METATYPE(QMeshInteractor*)

enum ItemRole
{
    ITEM_ROLE_NAME = Qt::UserRole + 1,
    ITEM_ROLE_TYPE = Qt::UserRole + 2,
    ITEM_ROLE_NODE = Qt::UserRole + 3
};

enum ToolType
{
    SELECTION_TOOL, ROTATE_TOOL, SCALE_TOOL, DRAW_TOOL, TOOL_COUNT
};

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

inline QDebug operator<<(QDebug d, const tbe::Vector3f& vec)
{
    return d << vec.x << "," << vec.y << "," << vec.z;
}

inline QDebug operator<<(QDebug d, const tbe::AABB& aabb)
{
    return d << aabb.min << aabb.max;
}

#endif	/* TOOLS_H */

