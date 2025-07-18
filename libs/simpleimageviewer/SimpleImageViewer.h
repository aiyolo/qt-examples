#pragma once

#include <QImage>
#include <QTransform>
#include <QWidget>
#include <qpoint.h>

class SimpleImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleImageViewer(QWidget* parent = nullptr);

    void setImage(const QImage& image, bool reset_view = true);
    QImage getImage() const { return m_image; }
    QPointF mapToImage(const QPointF& pos) const;
    QPointF mapFromImage(const QPointF& pos) const;
    double getScale() const { return m_transform.m11(); }
    void clearData() { setImage(QImage()); }
    void resetView();

signals:
    void sigMouseDoubleClicked();
    void sigResized();

protected:
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QImage m_image;
    QTransform m_transform;
    QPointF m_lastMousePos;
    bool m_isDragging;
};
