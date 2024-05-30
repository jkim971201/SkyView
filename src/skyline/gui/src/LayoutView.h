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

  private:
};

}

#endif
