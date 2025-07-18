#include "SimpleImageViewer.h"
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <qpoint.h>

SimpleImageViewer::SimpleImageViewer(QWidget* parent)
    : QWidget(parent)
    , m_isDragging(false)
{
    setMouseTracking(true);
}

void SimpleImageViewer::setImage(const QImage& image, bool reset_view)
{
    bool need_reset_view = reset_view || m_image.isNull();
    m_image = image;
    if (need_reset_view) {
        resetView();
    }
    update();
}

void SimpleImageViewer::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    if (!m_image.isNull()) {
        painter.setTransform(m_transform);
        painter.drawImage(0, 0, m_image);
    }
}

void SimpleImageViewer::wheelEvent(QWheelEvent* event)
{
    if (m_image.isNull()) return;

    QPointF mousePos = event->position();
    QPointF imagePos = mapToImage(mousePos);

    double factor = event->angleDelta().y() > 0 ? 1.1 : 0.9;
    double newScale = getScale() * factor;

    if (newScale < 0.1 || newScale > 50.0) return;

    m_transform.scale(factor, factor);

#if 0
    // QPointF newImagePos = mapToImage(mousePos);
    // QPointF delta = (newImagePos - imagePos) * getScale();
    // m_transform.translate(delta.x() / getScale(), delta.y() / getScale());
#endif
    auto delta = (mousePos - mapFromImage(imagePos));
    m_transform.translate(delta.x() / getScale(), delta.y() / getScale());
    update();
}

void SimpleImageViewer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void SimpleImageViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging) {
        QPointF delta = event->pos() - m_lastMousePos;
        m_transform.translate(delta.x() / getScale(), delta.y() / getScale());
        m_lastMousePos = event->pos();
        update();
    }
}

void SimpleImageViewer::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit sigMouseDoubleClicked();
    } else if (event->button() == Qt::RightButton) {
    }
    QWidget::mouseDoubleClickEvent(event);
}

void SimpleImageViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
}

QPointF SimpleImageViewer::mapToImage(const QPointF& pos) const
{
    return m_transform.inverted().map(pos);
}

QPointF SimpleImageViewer::mapFromImage(const QPointF& pos) const
{
    return m_transform.map(pos);
}

void SimpleImageViewer::resetView()
{
    if (!m_image.isNull()) {
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
        auto delta = (rect().center() - mapFromImage(m_image.rect().center()));
        m_transform.translate(delta.x() / getScale(), delta.y() / getScale());
    }
}

void SimpleImageViewer::resizeEvent(QResizeEvent* event)
{
    if (m_image.isNull()) return;

    resetView();
    update();
    emit sigResized();
}
