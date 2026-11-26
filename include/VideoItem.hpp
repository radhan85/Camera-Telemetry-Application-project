#pragma once

#include <QImage>
#include <QQuickPaintedItem>

class QPainter;

class VideoItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit VideoItem(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

public slots:
    void setFrame(const QImage &frame);

private:
    QImage frame_;
};
