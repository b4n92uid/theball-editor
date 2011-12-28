/*
 * File:   QBrowsEdit.cpp
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 00:15
 */

#include "QBrowsEdit.h"

QBrowsEdit::QBrowsEdit(QObject* parent, QLineEdit* lineEdit, QPushButton* browse) : QObject(parent)
{
    m_lineEdit = lineEdit;
    m_browse = browse;

    connect(m_browse, SIGNAL(clicked()), this, SLOT(openFileName()));
    connect(m_lineEdit, SIGNAL(textChanged(const QString&)), this, SIGNAL(textChanged(const QString&)));

    connect(this, SIGNAL(beginChoose()), parent, SIGNAL(pauseRendring()));
    connect(this, SIGNAL(endChoose()), parent, SIGNAL(resumeRendring()));
}

QBrowsEdit::~QBrowsEdit()
{
}

void QBrowsEdit::openFileName()
{
    emit beginChoose();

    QString filename = QFileDialog::getOpenFileName(m_browse, QString(), m_workDir);

    if(!filename.isEmpty())
    {
        m_lineEdit->setText(filename);
        m_workDir = QFileInfo(filename).path();
    }

    emit endChoose();
}

void QBrowsEdit::clear()
{
    m_lineEdit->blockSignals(true);
    m_lineEdit->clear();
    m_lineEdit->blockSignals(false);
}

void QBrowsEdit::setWorkDir(QString workDir)
{
    this->m_workDir = workDir;
}

QString QBrowsEdit::getWorkDir() const
{
    return m_workDir;
}

void QBrowsEdit::setOpenFileName(const QString& str)
{
    m_lineEdit->setText(str);
}

QString QBrowsEdit::getOpenFileName()
{
    return m_lineEdit->text();
}
