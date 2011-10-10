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
    *m_node->getTarget() = *m_source->getTarget();

    m_node->update();
}

DeletionState::DeletionState(QNodeInteractor* node)
{
    m_node = node->clone();
    m_parent = node->getTarget()->getParent();
}

DeletionState::~DeletionState()
{
}

void DeletionState::restore()
{
    m_node->setup();
    m_parent->addChild(m_node->getTarget());

    m_node->update();
}
