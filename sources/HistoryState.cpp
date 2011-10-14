/* 
 * File:   HistoryState.cpp
 * Author: b4n92uid
 * 
 * Created on 10 octobre 2011, 19:46
 */

#include "HistoryState.h"

HistoryState::HistoryState()
{
}

HistoryState::~HistoryState()
{
}

QNodeInteractor* HistoryState::node() const
{
    return m_node;
}

ModificationState::ModificationState(QNodeInteractor* node)
{
    m_node = node;
    m_source = node->clone();
}

ModificationState::~ModificationState()
{
    delete m_source;
}

void ModificationState::restore()
{
    *m_node->target() = *m_source->target();

    m_node->update();
}

DeletionState::DeletionState(QNodeInteractor* node)
{
    m_node = node;
    m_parent = node->target()->getParent();
}

DeletionState::~DeletionState()
{
    if(m_node)
        delete m_node;
}

void DeletionState::restore()
{
    m_parent->addChild(m_node->target());

    m_node->setup();

    m_node->update();

    m_node = NULL;
}
