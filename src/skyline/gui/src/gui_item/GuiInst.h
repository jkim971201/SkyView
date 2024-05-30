#ifndef GUI_INST_H
#define GUI_INST_H

#include <QGraphicsItem>
#include "db/dbInst.h"

using namespace db;

namespace gui
{

class GuiInst : public QGraphicsItem
{
  public:

    GuiInst(dbInst* inst);

    void paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option, 
               QWidget* widget);

  private:

    dbInst* inst_;
};

}

#endif
