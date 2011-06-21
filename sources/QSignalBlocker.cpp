/*
 * File:   QSignalBlocker.cpp
 * Author: b4n92uid
 *
 * Created on 21 juin 2011, 17:02
 */

#include "QSignalBlocker.h"

QSignalBlocker::QSignalBlocker()
{
}

QSignalBlocker::~QSignalBlocker()
{
}

void QSignalBlocker::block()
{

    foreach(QObject* obj, *this)
    {
        obj->blockSignals(true);
    }
}

void QSignalBlocker::unblock()
{

    foreach(QObject* obj, *this)
    {
        obj->blockSignals(false);
    }
}
