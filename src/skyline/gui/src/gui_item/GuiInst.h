#ifndef GUI_INST_H
#define GUI_INST_H

#include "db/dbInst.h"
#include "GuiItem.h"

using namespace db;

namespace gui
{

class GuiInst : public GuiItem 
{
  public:

    GuiInst(dbInst* inst);

    void setRect(const QRectF& rect) { rect_ = rect; }

    QRectF boundingRect() const override;

    void paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option, 
               QWidget* widget) override;

  private:

    dbInst* inst_;
    QRectF rect_;
};

}

#endif
