#pragma once
#include <qobject.h>
#include <QLabel>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    QLabel* label_;
};
