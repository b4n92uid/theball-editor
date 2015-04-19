/*
 * File:   QBrowsEdit.h
 * Author: b4n92uid
 *
 * Created on 4 décembre 2010, 00:15
 */

#ifndef QBROWSEDIT_H
#define	QBROWSEDIT_H

#include <QObject>
#include <QString>
#include <QPushButton>
#include <QLineEdit>

class QBrowsEdit : public QObject
{
    Q_OBJECT

public:
    QBrowsEdit(QObject* parent, QLineEdit* lineEdit, QPushButton* browse);
    virtual ~QBrowsEdit();

    void setOpenFileName(const QString& str);
    QString getOpenFileName();

    void setWorkDir(QString workDir);
    QString getWorkDir() const;

public slots:
    void openFileName();
    void clear();

signals:
    void textChanged(const QString& filename);

    void beginChoose();
    void endChoose();

private:
    QLineEdit* m_lineEdit;
    QPushButton* m_browse;
    QString m_workDir;
};

#endif	/* QBROWSEDIT_H */

