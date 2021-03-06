/*
 * File:   QSignalBlocker.h
 * Author: b4n92uid
 *
 * Created on 21 juin 2011, 17:02
 */

#ifndef QSIGNALBLOCKER_H
#define	QSIGNALBLOCKER_H

#include <QObject>
#include <QList>

class QSignalBlockerStream : public QList<QObject*>
{
public:
    QSignalBlockerStream();
    virtual ~QSignalBlockerStream();

    void block();
    void unblock();
};

#endif	/* QSIGNALBLOCKER_H */

