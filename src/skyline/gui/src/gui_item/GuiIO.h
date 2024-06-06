#ifndef GUI_IO_H
#define GUI_IO_H

#include "db/dbBTerm.h"
#include "GuiItem.h"

using namespace db;

namespace gui
{

class GuiIO : public GuiItem
{
  public:

    GuiIO(dbBTerm* io);

    void setRect(const QRectF& rect);

    QRectF boundingRect() const override;

    void paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option, 
               QWidget* widget) override;

    void setLeft()   { isLeft_   = true; }
    void setRight()  { isRight_  = true; }
    void setBottom() { isBottom_ = true; }
    void setTop()    { isTop_    = true; }

  private:

    dbBTerm* io_;
    QRectF rect_;

    bool isLeft_;
    bool isRight_;
    bool isBottom_;
    bool isTop_;

		double polyLx_;
		double polyLy_;
		double polyUx_;
		double polyUy_;
};

}

#endif
