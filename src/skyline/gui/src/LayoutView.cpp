#include "LayoutView.h"

#include <QtWidgets>
#include <cmath>
#include <iostream>

namespace gui
{

LayoutView::LayoutView(QWidget* parent)
  : scale_    (1.0),
    scaleFit_ (1.0),
    width_    (0.0),
    height_   (0.0)
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
LayoutView::zoom(qreal scale) 
{
  scale_ = scale;
  QTransform matrix;
  matrix.scale(scale_, scale_);
  setTransform(matrix);

  std::cout << "scale = " << scale_ << std::endl;
}

void 
LayoutView::zoomUp() 
{
  scale_ *= 1.2;
  std::cout << "scale = " << scale_ << std::endl;
}

void 
LayoutView::zoomDown() 
{
  scale_ /= 1.2;
  std::cout << "scale = " << scale_ << std::endl;
}

void 
LayoutView::setScaleFitToDesign(qreal width, qreal height) 
{
  width_  = width * 1.1;
  height_ = height * 1.1;
  zoomFit();
}

void 
LayoutView::zoomFit() 
{
  qreal scaleW = this->width()  / this->sceneRect().width()   * 0.8;
  qreal scaleH = this->height() / this->sceneRect().height()  * 0.8;

  scaleFit_ = std::min(scaleW, scaleH);
  zoom(scaleFit_);

  centerOn(this->width() / 2.0, this->height() / 2.0);
}

}
