#ifndef LAYOUT_VIEW_H
#define LAYOUT_VIEW_H

#include <QGraphicsView>

namespace gui
{

class LayoutView : public QGraphicsView
{
  Q_OBJECT

  public:
    
    LayoutView(QWidget* parent = nullptr);

    qreal getScale() { return scale_; }

    void setScaleFitToDesign(qreal width, qreal height);

    void zoom(qreal scale = 1);
    void zoomUp();
    void zoomDown();
    void zoomFit();

	protected:

		void wheelEvent(QWheelEvent* event);

  private:

    qreal scale_;
		qreal scaleFit_;
    qreal width_;
    qreal height_;
};

}

#endif
