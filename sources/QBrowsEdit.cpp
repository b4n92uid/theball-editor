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

    QString filename = QFileDialog::getOpenFileName(m_browse);

    if(!filename.isEmpty())
        m_lineEdit->setText(filename);

    emit endChoose();
}

void QBrowsEdit::setOpenFileName(const QString& str)
{
    m_lineEdit->setText(str);
}

QString QBrowsEdit::getOpenFileName()
{
    return m_lineEdit->text();
}
