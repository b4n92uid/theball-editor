/* 
 * File:   QBrowsEdit.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 00:15
 */

#ifndef QBROWSEDIT_H
#define	QBROWSEDIT_H

#include <QtGui/QtGui>

class QBrowsEdit : public QObject
{
    Q_OBJECT

public:
    QBrowsEdit(QObject* parent, QLineEdit* lineEdit, QPushButton* browse);
    virtual ~QBrowsEdit();

    void SetOpenFileName(const QString& str);
    QString GetOpenFileName();

public slots:
    void OpenFileName();

signals:
    void textChanged(const QString& filename);

private:
    QLineEdit* m_lineEdit;
    QPushButton* m_browse;
};

#endif	/* QBROWSEDIT_H */

