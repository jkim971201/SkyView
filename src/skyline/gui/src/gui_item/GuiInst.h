#ifndef GUI_INST_H
#define GUI_INST_H

#include <QColor>
#include <QPainter>
#include <QGraphicsItem>
#include "db/dbInst.h"

using namespace db;

namespace gui
{

class GuiInst : public QGraphicsItem
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
