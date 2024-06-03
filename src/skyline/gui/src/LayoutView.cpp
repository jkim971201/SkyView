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
LayoutView::zoomFit()
{
  QRectF rectToFit = sceneRect();
  double sceneW = rectToFit.width();
  double sceneH = rectToFit.height();

	// Make 10% blank margin along boundary
  rectToFit.adjust(-0.1 * sceneW, -0.1 * sceneH, 
                   +0.1 * sceneW, +0.1 * sceneH);

  fitInView(rectToFit, Qt::KeepAspectRatio);
}

void
LayoutView::zoomIn()
{
  scale(1.25, 1.25);
}

void
LayoutView::zoomOut()
{
  scale(0.75, 0.75);
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
LayoutView::paintEvent(QPaintEvent* event)
{
  if(firstShow_ == false)
  {
    zoomFit();
    firstShow_ = true;
  }

  QGraphicsView::paintEvent(event);
}

void
LayoutView::keyPressEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_F)
	{
		// Zoom Fit
		zoomFit();
	}
	else if(event->key() == Qt::Key_Z && event->modifiers() != Qt::ShiftModifier)
	{
		// Zoom In
		zoomIn();
	}
	else if(event->key() == Qt::Key_Z && event->modifiers() == Qt::ShiftModifier)
	{
		// Zoom Out
		zoomOut();
	}
	else if(event->key() == Qt::Key_Q)
	{
    close();
		exit(0);
	}
	else
	  QGraphicsView::keyPressEvent(event);
}

// Slots
void
LayoutView::zoomIn_slot()
{
	zoomIn();
}

void
LayoutView::zoomOut_slot()
{
	zoomOut();
}

void
LayoutView::zoomFit_slot()
{
	zoomFit();
}

}
