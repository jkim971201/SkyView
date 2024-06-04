#ifndef GUI_IO_H
#define GUI_IO_H

#include "db/dbIO.h"
#include "GuiItem.h"

using namespace db;

namespace gui
{

class GuiIO : public GuiItem
{
  public:

    GuiIO(dbIO* io);

    void setRect(const QRectF& rect) { rect_ = rect; }

    QRectF boundingRect() const override;

    void paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option, 
               QWidget* widget) override;

    void setLeft()   { isLeft_   = true; }
    void setRight()  { isRight_  = true; }
    void setBottom() { isBottom_ = true; }
    void setTop()    { isTop_    = true; }

  private:

    dbIO* io_;
    QRectF rect_;

    bool isLeft_;
    bool isRight_;
    bool isBottom_;
    bool isTop_;
};

}

#endif
