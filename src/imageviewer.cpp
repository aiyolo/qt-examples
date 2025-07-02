#include "imageviewer.h"
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QDebug>
#include <qpoint.h>

ImageViewer::ImageViewer(QWidget* parent) : QWidget(parent), m_isDragging(false)
{
    setMouseTracking(true);
}

void ImageViewer::setImage(const QImage& image)
{
    m_image = image;
    fitToView();
    update();
}

void ImageViewer::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    if (!m_image.isNull())
    {
        painter.setTransform(m_transform);
        painter.drawImage(0, 0, m_image);
    }
}

void ImageViewer::wheelEvent(QWheelEvent* event)
{
    if (m_image.isNull())
        return;

    QPointF mousePos = event->position();
    QPointF imagePos = mapToImage(mousePos);

    // 计算缩放因子
    double factor = event->angleDelta().y() > 0 ? 1.1 : 0.9;
    double newScale = getScale() * factor;

    // 限制缩放范围
    if (newScale < 0.1 || newScale > 10.0)
        return;

    // 应用新的缩放
    m_transform.scale(factor, factor);

// 计算新的图片位置，保持鼠标指向的图片位置不变
#if 0
    // QPointF newImagePos = mapToImage(mousePos);
    // QPointF delta = (newImagePos - imagePos) * getScale();
    // m_transform.translate(delta.x() / getScale(), delta.y() / getScale());
#endif
    auto delta = (mousePos - mapFromImage(imagePos));
    m_transform.translate(delta.x() / getScale(), delta.y() / getScale());
    update();
}

void ImageViewer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void ImageViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging)
    {
        QPointF delta = event->pos() - m_lastMousePos;
        m_transform.translate(delta.x() / getScale(), delta.y() / getScale());
        m_lastMousePos = event->pos();
        update();
    }
    
    if (!m_image.isNull())
    {
        emit mouseMoved(mapToImage(event->pos()));
    }
}

void ImageViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
}

void ImageViewer::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (!m_image.isNull())
    {
        emit doubleClicked(mapToImage(event->pos()));
    }
}

void ImageViewer::contextMenuEvent(QContextMenuEvent* event)
{
    if (!m_image.isNull())
    {
        emit rightClicked(mapToImage(event->pos()));
        fitToView();
    }
}

QPointF ImageViewer::mapToImage(const QPointF& pos) const
{
    return m_transform.inverted().map(pos);
}

QPointF ImageViewer::mapFromImage(const QPointF& pos) const
{
    return m_transform.map(pos);
}

void ImageViewer::fitToView()
{
    if (!m_image.isNull())
    {
        // 计算缩放比例，使图片适应窗口大小
        double scaleX = (double)width() / m_image.width();
        double scaleY = (double)height() / m_image.height();
        double scale = qMin(scaleX, scaleY);

        // 重置变换矩阵并应用缩放
        m_transform.reset();
        m_transform.scale(scale, scale);

#if 0
        // 计算居中位置
        double imageWidth = m_image.width() * scale;
        double imageHeight = m_image.height() * scale;
        double offsetX = (width() - imageWidth) / 2.0;
        double offsetY = (height() - imageHeight) / 2.0;
        m_transform.translate(offsetX / scale, offsetY / scale);
#endif
        auto delta = (rect().center() - mapFromImage(m_image.rect().center())) / 2;
        m_transform.translate(delta.x() / getScale(), delta.y() / getScale());
    }
}

void ImageViewer::resizeEvent(QResizeEvent* event)
{
    if (m_image.isNull())
        return;

    fitToView();
    update();
}
