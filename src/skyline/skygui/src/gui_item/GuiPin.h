#ifndef GUI_PIN_H
#define GUI_PIN_H

#include "GuiItem.h"
#include "db/dbITerm.h"

using namespace db;

namespace gui
{

class GuiPin : public GuiItem
{
  public:

    GuiPin(dbITerm* iterm);

    void setRect(const QRectF& rect) { rect_ = rect; }

    QRectF boundingRect() const override;

    void paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option, 
               QWidget* widget) override;

  private:

    dbITerm* iterm_;
    QRectF rect_;
};

}

#endif
