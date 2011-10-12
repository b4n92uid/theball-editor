/* 
 * File:   Metatype.h
 * Author: b4n92uid
 *
 * Created on 3 septembre 2011, 15:31
 */

#ifndef METATYPE_H
#define	METATYPE_H

#include <QStandardItem>
#include <QList>

#include <Tbe.h>

#include "QNodeInteractor.h"

typedef QList<QStandardItem*> QItemsList;

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

enum ItemRole
{
    ITEM_ROLE_NAME = Qt::UserRole + 1,
    ITEM_ROLE_TYPE = Qt::UserRole + 2,
    ITEM_ROLE_NODE = Qt::UserRole + 3
};

#endif	/* METATYPE_H */

