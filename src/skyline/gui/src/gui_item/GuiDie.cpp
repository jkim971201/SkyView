#include "GuiDie.h"

namespace gui
{

GuiDie::GuiDie(dbDie* die)
  : die_ (die)
{

}

QRectF
GuiDie::boundingRect() const
{
  return rect_;
}

void
GuiDie::paint(QPainter* painter, 
              const QStyleOptionGraphicsItem* option,
              QWidget* widget)
{
  painter->setPen( QPen(Qt::red, 1) );
  //painter->setBrush(Qt::red);
  painter->drawRect(rect_);
  painter->fillRect(rect_, painter->brush() );
}

}
