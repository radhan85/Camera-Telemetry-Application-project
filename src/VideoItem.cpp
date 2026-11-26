#include "VideoItem.hpp"

#include <QPainter>

VideoItem::VideoItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
}

void VideoItem::setFrame(const QImage &frame)
{
    frame_ = frame;
    update();
}

void VideoItem::paint(QPainter *painter)
{
    if (frame_.isNull()) {
        return;
    }

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    QSizeF fittedSize = frame_.size();
    fittedSize.scale(boundingRect().size(), Qt::KeepAspectRatio);
    QRectF target(QPointF(0, 0), fittedSize);
    target.moveCenter(boundingRect().center());
    painter->drawImage(target, frame_);
}
