#ifndef GUI_DIE_H
#define GUI_DIE_H

#include <QColor>
#include <QPainter>
#include <QGraphicsItem>
#include "db/dbDie.h"

using namespace db;

namespace gui
{

class GuiDie : public QGraphicsItem
{
  public:

    GuiDie(dbDie* die);

    void setRect(const QRectF& rect) { rect_ = rect; }

    QRectF boundingRect() const override;

    void paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option, 
               QWidget* widget) override;

  private:

    dbDie* die_;
    QRectF rect_;
};

}

#endif
