#include "GuiInst.h"

namespace gui
{

GuiInst::GuiInst(dbInst* inst)
  : inst_ (inst)
{

}

QRectF
GuiInst::boundingRect() const
{
  return rect_;
}

void
GuiInst::paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option,
               QWidget* widget)
{
  painter->setPen( QPen(Qt::blue, 0) );
  painter->drawRect(rect_);
}

}
