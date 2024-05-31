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

    void fitView();

  protected:

    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

  private:

    bool firstShow_;
};

}

#endif
