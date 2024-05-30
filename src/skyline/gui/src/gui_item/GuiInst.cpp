#include "GuiInst.h"

namespace gui
{

GuiInst::GuiInst(dbInst* inst)
  : inst_ (inst)
{

}

void
GuiInst::paint(QPainter* painter, 
               const QStyleOptionGraphicsItem* option,
               QWidget* widget)
{
}

}
