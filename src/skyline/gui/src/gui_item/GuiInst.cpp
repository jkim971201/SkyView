#include <QStyleOptionGraphicsItem>

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
  //painter->setRenderHint(QPainter::Antialiasing);
  qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

  QBrush brush(Qt::gray, Qt::BrushStyle::Dense6Pattern);
  brush.setTransform(QTransform(painter->worldTransform().inverted()));

  QPen pen(Qt::gray, 1, Qt::PenStyle::SolidLine);
  pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
	pen.setWidthF(pen.widthF() / lod);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->drawRect(rect_);


  // Orientation Line
  qreal inst_width  = std::abs(rect_.width());
  qreal inst_height = std::abs(rect_.height());

  qreal delta_x = inst_width  / 4.0;
  qreal delta_y = inst_height / 4.0;

  QPointF p1;
  QPointF p2;

  switch(inst_->orient())
  {
    case Orient::N  :
    case Orient::FW :
    {
      QPointF tl = rect_.topLeft();
      p1 = tl + QPointF(0.0, delta_y);
      p2 = tl + QPointF(delta_x, 0.0);
      break;
    }
    case Orient::S  :
    case Orient::FE :
    {
      QPointF br = rect_.bottomRight();
      p1 = br + QPointF(0.0, -delta_y);
      p2 = br + QPointF(-delta_x, 0.0);
      break;
    }
    case Orient::W  :
    case Orient::FN :
    {
      QPointF tr = rect_.topRight();
      p1 = tr + QPointF(0.0, delta_y);
      p2 = tr + QPointF(-delta_x, 0.0);
      break;
    }
    case Orient::E  :
    case Orient::FS :
    {
      QPointF bl = rect_.bottomLeft();
      p1 = bl + QPointF(0.0, -delta_y);
      p2 = bl + QPointF(delta_x, 0.0);
      break;
    }
    default:
      break;
  }

  QLineF line(p1, p2);

	pen.setJoinStyle(Qt::PenJoinStyle::BevelJoin);
	painter->setPen(pen);
  painter->drawLine(line);
}

}
