/* 
 * File:   HistoryState.h
 * Author: b4n92uid
 *
 * Created on 10 octobre 2011, 19:46
 */

#ifndef HISTORYSTATE_H
#define	HISTORYSTATE_H

#include "QNodeInteractor.h"

class HistoryState
{
public:
    HistoryState();
    virtual ~HistoryState();

    virtual void restore() = 0;

    QNodeInteractor* node() const;

protected:
    QNodeInteractor* m_node;
};

class ModificationState : public HistoryState
{
public:
    ModificationState(QNodeInteractor* node);
    ~ModificationState();
    void restore();

protected:
    QNodeInteractor* m_source;
};

class DeletionState : public HistoryState
{
public:
    DeletionState(QNodeInteractor* node);
    ~DeletionState();
    void restore();

protected:
    tbe::scene::Node* m_parent;
};

#endif	/* HISTORYSTATE_H */

