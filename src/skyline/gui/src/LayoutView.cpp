#include "LayoutView.h"

#include <QtWidgets>
#include <cmath>
#include <iostream>

namespace gui
{

LayoutView::LayoutView(QWidget* parent)
  : firstShow_ (false)
{
	scale(1.0, -1.0);
  setDragMode(QGraphicsView::ScrollHandDrag);
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void
LayoutView::wheelEvent(QWheelEvent* event)
{
  double numDegrees = -event->delta() / 8.0;
  double numSteps   = numDegrees / 15.0;
  double factor     = std::pow(1.125, numSteps);
  scale(factor, factor);
}

void 
LayoutView::fitView()
{
  QRectF rectToFit = sceneRect();
  double sceneW = rectToFit.width();
  double sceneH = rectToFit.height();

  rectToFit.adjust(-0.1 * sceneW, -0.1 * sceneH, 
                   +0.1 * sceneW, +0.1 * sceneH);

  fitInView(rectToFit, Qt::KeepAspectRatio);
}

void
LayoutView::paintEvent(QPaintEvent* event)
{
  if(firstShow_ == false)
  {
    fitView();
    firstShow_ = true;
  }

  QGraphicsView::paintEvent(event);
}

}
