#ifndef ADDRECORDDIALOG_H
#define ADDRECORDDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlDatabase>
#include <QMap>

class AddRecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRecordDialog(const QString &tableName, const QSqlDatabase &database, QWidget *parent = nullptr);

    QMap<QString, QVariant> getRecordData() const;

private:
    void setupUI();
    void populateFields(const QSqlRecord &record);

    QFormLayout *m_formLayout;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;

    QMap<QString, QWidget*> m_fieldWidgets;
    QMap<QString, QVariant> m_recordData;
};

#endif // ADDRECORDDIALOG_H
