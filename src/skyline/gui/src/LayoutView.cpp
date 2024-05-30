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
	setDragMode(ScrollHandDrag);
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void
LayoutView::wheelEvent(QWheelEvent* event)
{
	double numDegrees = -event->delta() / 8.0;
	double numSteps   = numDegrees / 15.0;
	double factor     = std::pow(1.125, numSteps);
	scale(factor, factor);

  centerOn(this->width() / 2.0, this->height() / 2.0);

  std::cout << "sceneRect W : " << this->sceneRect().width()  << std::endl;
  std::cout << "sceneRect H : " << this->sceneRect().height() << std::endl;

  std::cout << "ViewRect W : " << this->width()  << std::endl;
  std::cout << "ViewRect H : " << this->height() << std::endl;

//  if(event->modifiers() & Qt::ControlModifier) 
//	{
//    QApplication::setOverrideCursor(Qt::WaitCursor);
//    if(event->angleDelta().y() > 0) 
//      zoomUp();
//		else 
//      zoomDown();
//    event->accept();
//  } 
//	else 
//	{
//    event->accept();
//    QGraphicsView::wheelEvent(e);
//  }
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
  scale_ *= 2;
  std::cout << "zoomUp scale = " << scale_ << std::endl;
}

void 
LayoutView::zoomDown() 
{
  scale_ *= 0.5;
  std::cout << "zoomDown scale = " << scale_ << std::endl;
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

  std::cout << "sceneRect W : " << this->sceneRect().width()  << std::endl;
  std::cout << "sceneRect H : " << this->sceneRect().height() << std::endl;

  std::cout << "ViewRect W : " << this->width()  << std::endl;
  std::cout << "ViewRect H : " << this->height() << std::endl;

  std::cout << "scaleW : " << scaleW << std::endl;
  std::cout << "scaleH : " << scaleH << std::endl;

  // qreal scaleW = this->sceneRect().width()  / this->width()   * 0.8;
  // qreal scaleH = this->sceneRect().height() / this->height()  * 0.8;

  scaleFit_ = std::min(scaleW, scaleH);
  zoom(scaleFit_);

  centerOn(this->width() / 2.0, this->height() / 2.0);
}

}
