#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QScrollArea>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QThreadPool>
#include <QLabel>
#include <QMouseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onThumbnailClicked();

private:
    void setupUI();
    void generateTestImages();
    void addThumbnail(const QPixmap &pixmap, int index);
    void showDetail(int index);

    QScrollArea *scrollArea;
    QWidget *containerWidget;
    QGridLayout *gridLayout;
    int currentRow;
    int currentCol;
    const int MAX_COLUMNS = 5;
    QVector<QPixmap> m_images;
};

class SimpleImageViewer : public QLabel
{
    Q_OBJECT

public:
    explicit SimpleImageViewer(QWidget *parent = nullptr) : QLabel(parent) {
        setFixedSize(256, 256);
        setScaledContents(true);
    }

    void loadImage(const QString &path) {
        setPixmap(QPixmap(path).scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    void setPixmap(const QPixmap &pixmap) {
        QLabel::setPixmap(pixmap);
    }

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
        QLabel::mousePressEvent(event);
    }
};

#endif // MAINWINDOW_H
