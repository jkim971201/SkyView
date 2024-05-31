#include "LayoutView.h"

#include <QtWidgets>
#include <cmath>
#include <iostream>

namespace gui
{

LayoutView::LayoutView(QWidget* parent)
{
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
LayoutView::zoomFit() 
{
}

}
