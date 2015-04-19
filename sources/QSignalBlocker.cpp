/*
 * File:   QSignalBlocker.cpp
 * Author: b4n92uid
 *
 * Created on 21 juin 2011, 17:02
 */

#include "QSignalBlocker.h"

QSignalBlockerStream::QSignalBlockerStream()
{
}

QSignalBlockerStream::~QSignalBlockerStream()
{
}

void QSignalBlockerStream::block()
{

    foreach(QObject* obj, *this)
    {
        obj->blockSignals(true);
    }
}

void QSignalBlockerStream::unblock()
{

    foreach(QObject* obj, *this)
    {
        obj->blockSignals(false);
    }
    
    clear();
}
