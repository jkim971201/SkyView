#include <QStyleOptionGraphicsItem>

#include "GuiIO.h"

namespace gui
{

GuiIO::GuiIO(dbIO* io)
  : io_       (io),
    isLeft_   (false),
    isRight_  (false),
    isBottom_ (false),
    isTop_    (false)
{
}

QRectF
GuiIO::boundingRect() const
{
  return rect_;
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
}

}
