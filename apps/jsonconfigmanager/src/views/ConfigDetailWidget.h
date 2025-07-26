#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtCore/QString>

class ConfigDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigDetailWidget(QWidget* parent = nullptr);
    ~ConfigDetailWidget();

    void loadConfig(const QString& configName, const QString& filePath);
    void saveConfig();

private slots:
    void onField1Changed(const QString& text);
    void onField2Changed(const QString& text);
    void onSaveClicked();

private:
    void setupUI();
    void clearContent();

    QString m_currentConfigName;
    QString m_currentFilePath;

    QLabel* m_titleLabel;
    QLineEdit* m_field1Edit;
    QLineEdit* m_field2Edit;
    QPushButton* m_saveButton;

    bool m_isModified;
};
