#include <QApplication>
#include <QStyleOptionGraphicsItem>

#include "GuiPin.h"

namespace gui
{

GuiPin::GuiPin(dbITerm* iterm)
  : iterm_ (iterm)
{
}

QRectF
GuiPin::boundingRect() const
{
  return rect_;
}

void
GuiPin::paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option,
               QWidget* widget)
{
  qreal lod 
		= option->levelOfDetailFromTransform(painter->worldTransform());
	if(lod > 25.0)
	{
    QBrush brush(Qt::blue, Qt::BrushStyle::DiagCrossPattern);
    brush.setTransform(QTransform(painter->worldTransform().inverted()));

    painter->setPen( QPen(Qt::blue, 0, Qt::PenStyle::SolidLine) );
    painter->setBrush( brush );
    painter->drawRect(rect_);
	}
}

}
