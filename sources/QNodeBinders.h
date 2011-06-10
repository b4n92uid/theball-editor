/*
 * File:   QNodeBinders.h
 * Author: b4n92uid
 *
 * Created on 7 décembre 2010, 21:00
 */

#ifndef QNODEBINDERS_H
#define	QNODEBINDERS_H

#include <Tbe.h>
#include <QWidget>

class QNodeBinders : public QObject
{
    Q_OBJECT

public:
    QNodeBinders(QWidget* parent);

    void node(tbe::scene::Node* curNode);
    tbe::scene::Node* node() const;

    void mark(tbe::scene::MapMark* curNode);
    tbe::scene::MapMark* mark() const;

    void light(tbe::scene::Light* curlight);
    tbe::scene::Light* light() const;

    void mesh(tbe::scene::Mesh* curmesh);
    tbe::scene::Mesh* mesh() const;

    void particles(tbe::scene::ParticlesEmiter* curparticles);
    tbe::scene::ParticlesEmiter* particles() const;

    void water(tbe::scene::Water* curwater);
    tbe::scene::Water* water() const;

private:
    tbe::scene::Node* m_curNode;
    tbe::scene::Mesh* m_curmesh;
    tbe::scene::MapMark* m_curmark;
    tbe::scene::ParticlesEmiter* m_curparticles;
    tbe::scene::Light* m_curlight;
    tbe::scene::Water* m_curwater;

    QWidget* m_parentWidget;
};

#endif	/* QNODEBINDERS_H */

