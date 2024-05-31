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

	// Orientation Line
	
	qreal width  = rect_.width();
	qreal height = rect_.height();

	qreal width_l  = width  / 2.0;
	qreal height_l = height / 2.0;

	QPointF p1;
	QPointF p2;

	switch(inst_->orient())
	{
		case Orient::N  :
		case Orient::FW :
		{
			QPointF tl = rect_.topLeft();
			p1 = tl + QPointF(0.0, height_l);
			p2 = tl + QPointF(width_l, 0.0);
			break;
		}
		case Orient::S  :
		case Orient::FE :
		{
			QPointF br = rect_.bottomRight();
			p1 = br + QPointF(0.0, -height_l);
			p2 = br + QPointF(-width_l, 0.0);
			break;
		}
		case Orient::W  :
		case Orient::FN :
		{
			QPointF tr = rect_.topRight();
			p1 = tr + QPointF(0.0, height_l);
			p2 = tr + QPointF(-width_l, 0.0);
			break;
		}
		case Orient::E  :
		case Orient::FS :
		{
			QPointF bl = rect_.bottomLeft();
			p1 = bl + QPointF(0.0, -height_l);
			p2 = bl + QPointF(width_l, 0.0);
			break;
		}
		default:
			break;
	}

	QLineF line(p1, p2);
  painter->drawLine(line);
}

}
