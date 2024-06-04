#ifndef GUI_ITEM_H
#define GUI_ITEM_H

#include <QPainter>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QGraphicsItem>

namespace gui
{

class GuiItem : public QGraphicsItem
{
  public:

    GuiItem();

    bool isVisible() const { return isVisible_; }

    void setInvisible() { isVisible_ = false; }

  private:

    bool isVisible_;
};

}

#endif
