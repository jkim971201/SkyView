#include <cassert>
#include <iostream>

#include <QStyleOptionGraphicsItem>
#include "GuiIO.h"

namespace gui
{

GuiIO::GuiIO(dbBTerm* io)
  : io_       (io),
    isLeft_   (false),
    isRight_  (false),
    isBottom_ (false),
    isTop_    (false),
    polyLx_   (std::numeric_limits<double>::max()),
    polyLy_   (std::numeric_limits<double>::max()),
    polyUx_   (std::numeric_limits<double>::min()),
    polyUy_   (std::numeric_limits<double>::min())
{
}

void 
GuiIO::setRect(const QRectF& rect)
{
  rect_ = rect;
  polyLx_ = std::min(polyLx_, rect.left());
  polyLy_ = std::min(polyLy_, rect.bottom());
  polyUx_ = std::max(polyUx_, rect.right());
  polyUy_ = std::max(polyUy_, rect.top());
}

QRectF
GuiIO::boundingRect() const
{
  // QGrahicsView decide to re-paint a QGraphicsItem when
  // its boudingRect() is inside the scene.
  // If boudingRect() is computed properly,
  // an item can disappear while repainting.
  return QRectF(polyLx_, polyLy_, polyUx_ - polyLx_, polyUy_ - polyLy_);
}

void
GuiIO::paint(QPainter* painter, 
             const QStyleOptionGraphicsItem* option,
             QWidget* widget)
{
  QColor color;
  if(io_->direction() == PinDirection::INPUT)
    color = Qt::red;
  else if(io_->direction() == PinDirection::OUTPUT)
    color = Qt::cyan;
  else
    color = Qt::yellow;

  QBrush br(color, Qt::BrushStyle::DiagCrossPattern);
  br.setTransform(QTransform(painter->worldTransform().inverted()));
  // To avoid weird bug of Qt (rouding error issue?)
  // https://www.qtcentre.org/threads/2907-QBrush-pattern-problem

  painter->setPen(QPen(color, 0, Qt::PenStyle::SolidLine));
  painter->setBrush(br);

  painter->drawRect(rect_);

  qreal ioCx = rect_.center().x();
  qreal ioCy = rect_.center().y();
  qreal ioLx = rect_.left();
  qreal ioLy = rect_.bottom();
  qreal ioUx = rect_.right();
  qreal ioUy = rect_.top();
  qreal polyLen = std::min(
                  std::abs(rect_.width()), 
                  std::abs(rect_.height())) * 4.0;

  qreal p1X = 0.0;
  qreal p1Y = 0.0;
  qreal p2X = 0.0;
  qreal p2Y = 0.0;
  qreal p3X = 0.0;
  qreal p3Y = 0.0;

  // 0.8660 ~= sqrt(3)/2
  if(isLeft_)
  {
    p1X = ioLx;
    p1Y = (ioUy + ioLy) / 2.0;
    p2X = p1X - 0.866 * polyLen;
    p2Y = p1Y - polyLen / 2.0;
    p3X = p1X - 0.866 * polyLen;
    p3Y = p1Y + polyLen / 2.0;

    polyLx_ = std::min(polyLx_, p2X);

    polyLy_ = std::min(polyLy_, p2Y);
    polyUy_ = std::max(polyUy_, p3Y);
  }
  else if(isRight_)
  {
    p1X = ioUx;
    p1Y = (ioUy + ioLy) / 2.0;
    p2X = p1X + 0.866 * polyLen;
    p2Y = p1Y - polyLen / 2.0;
    p3X = p1X + 0.866 * polyLen;
    p3Y = p1Y + polyLen / 2.0;

    polyUx_ = std::max(polyUx_, p2X);

    polyLy_ = std::min(polyLy_, p2Y);
    polyUy_ = std::max(polyUy_, p3Y);
  }
  else if(isBottom_)
  {
    p1X = (ioLx + ioUx) / 2.0;
    p1Y = ioLy;
    p2X = p1X - polyLen / 2.0;
    p2Y = p1Y - 0.866 * polyLen;
    p3X = p1X + polyLen / 2.0;
    p3Y = p1Y - 0.866 * polyLen;

    polyLx_ = std::min(polyLx_, p2X);
    polyUx_ = std::max(polyUx_, p3X);

    polyLy_ = std::min(polyLy_, p2Y);
  }
  else if(isTop_)
  {
    p1X = (ioLx + ioUx) / 2.0;
    p1Y = ioUy;
    p2X = p1X - polyLen / 2.0;
    p2Y = p1Y + 0.866 * polyLen;
    p3X = p1X + polyLen / 2.0;
    p3Y = p1Y + 0.866 * polyLen;

    polyLx_ = std::min(polyLx_, p2X);
    polyUx_ = std::max(polyUx_, p3X);

    polyUy_ = std::max(polyUy_, p3Y);
  }
  else
  {
    // 0.5774 ~= sqrt(3) / 3
    p1X = ioCx;
    p1Y = ioCy + 0.5774 * polyLen;
    p2X = ioCx + 0.5774 * polyLen;
    p2Y = ioCy - 0.5774 * polyLen;
    p3X = ioCx - 0.5774 * polyLen;
    p3Y = ioCy - 0.5774 * polyLen;
  }

  QPainterPath path;
  path.moveTo(p1X, p1Y);
  path.lineTo(p2X, p2Y);
  path.lineTo(p3X, p3Y);
  path.lineTo(p1X, p1Y);

  painter->drawPath(path);
}

}
