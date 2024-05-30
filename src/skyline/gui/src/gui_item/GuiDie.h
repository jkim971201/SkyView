#ifndef GUI_DIE_H
#define GUI_DIE_H

#include <QGraphicsItem>
#include "db/dbDie.h"

using namespace db;

namespace gui
{

class GuiDie : public QGraphicsItem
{
  public:

    GuiDie(dbDie* die);

    void paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option, 
               QWidget* widget);

  private:

    dbDie* die_;
};

}

#endif
