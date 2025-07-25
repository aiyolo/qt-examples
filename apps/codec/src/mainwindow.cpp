#include "mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
  : QWidget(parent)
{
    auto layout = new QVBoxLayout(this);
    label_ = new QLabel;
    layout->addWidget(label_);
    label_->setText("你好");
}
