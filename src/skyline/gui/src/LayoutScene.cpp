#include "LayoutScene.h"
#include "gui_item/GuiInst.h"
#include "gui_item/GuiDie.h"

#include <cassert>
#include <iostream>

namespace gui
{

LayoutScene::LayoutScene(QObject* parent)
{

}

void
LayoutScene::createGuiDie()
{
  dbDie* die = db_->getDesign()->getDie();
  GuiDie* die_gui = new GuiDie(die);

  double dbu = static_cast<double>(db_->getTech()->getDbu());

  double dieLx = die->lx() / dbu;
  double dieLy = die->ly() / dbu;
  double dieDx = die->dx() / dbu;
  double dieDy = die->dy() / dbu;

  std::cout << "dieLx : " << dieLx << std::endl;
  std::cout << "dieLy : " << dieLy << std::endl;
  std::cout << "dieDx : " << dieDx << std::endl;
  std::cout << "dieDy : " << dieDy << std::endl;

  die_gui->setRect( QRectF(dieLx, dieLy, dieDx, dieDy) );

  this->addItem(die_gui);
}

void
LayoutScene::createGuiInst()
{
  double dbu = static_cast<double>(db_->getTech()->getDbu());

  for(auto inst : db_->getDesign()->getInsts())
  {
    GuiInst* inst_gui = new GuiInst(inst);
  
    double cellLx = inst->lx() / dbu;
    double cellLy = inst->ly() / dbu;
    double cellDx = inst->dx() / dbu;
    double cellDy = inst->dy() / dbu;
  
    inst_gui->setRect( QRectF(cellLx, cellLy, cellDx, cellDy) );
    this->addItem(inst_gui);
  }
}

}
