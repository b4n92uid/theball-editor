/*
 * File:   QNodeBinders.cpp
 * Author: b4n92uid
 *
 * Created on 7 décembre 2010, 21:00
 */

#include "QNodeBinders.h"
#include <QMessageBox>

QNodeBinders::QNodeBinders(QWidget* parent) : QObject(parent)
{
    m_parentWidget = parent;

    m_curmesh = NULL;
    m_curmark = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
    m_curNode = NULL;
}

void QNodeBinders::node(tbe::scene::Node* curNode)
{
    m_curNode = curNode;
    m_curmesh = NULL;
    m_curmark = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
}

tbe::scene::Node* QNodeBinders::node() const
{
    return m_curNode;
}

void QNodeBinders::mesh(tbe::scene::Mesh* curmesh)
{
    m_curNode = m_curmesh = curmesh;
    m_curwater = NULL;
    m_curparticles = NULL;
    m_curlight = NULL;
    m_curmark = NULL;
}

tbe::scene::Mesh* QNodeBinders::mesh() const
{
    return m_curmesh;
}

void QNodeBinders::light(tbe::scene::Light* curlight)
{
    m_curNode = m_curlight = curlight;
    m_curmesh = NULL;
    m_curparticles = NULL;
    m_curwater = NULL;
    m_curmark = NULL;
}

tbe::scene::Light* QNodeBinders::light() const
{
    return m_curlight;
}

void QNodeBinders::water(tbe::scene::Water* curwater)
{
    m_curNode = m_curwater = curwater;
    m_curmesh = NULL;
    m_curparticles = NULL;
    m_curlight = NULL;
    m_curmark = NULL;
}

tbe::scene::Water* QNodeBinders::water() const
{
    return m_curwater;
}

void QNodeBinders::particles(tbe::scene::ParticlesEmiter* curparticles)
{
    m_curNode = m_curparticles = curparticles;
    m_curmesh = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
    m_curmark = NULL;
}

void QNodeBinders::mark(tbe::scene::MapMark* curNode)
{
    m_curNode = m_curmark = curNode;
    m_curparticles = NULL;
    m_curmesh = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
}

tbe::scene::MapMark* QNodeBinders::mark() const
{
    return m_curmark;
}

tbe::scene::ParticlesEmiter* QNodeBinders::particles() const
{
    return m_curparticles;
}

void QNodeBinders::deselect()
{
    m_curNode = NULL;
    m_curmark = NULL;
    m_curparticles = NULL;
    m_curmesh = NULL;
    m_curwater = NULL;
    m_curlight = NULL;
}