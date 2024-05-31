#include "LayoutScene.h"
#include "gui_item/GuiDie.h"
#include "gui_item/GuiRow.h"
#include "gui_item/GuiInst.h"

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

  die_gui->setRect( QRectF(dieLx, dieLy, dieDx, dieDy) );

  this->addItem(die_gui);
}

void
LayoutScene::createGuiRow()
{
  double dbu = static_cast<double>(db_->getTech()->getDbu());

  for(auto row : db_->getDesign()->getRows())
  {
    GuiRow* row_gui = new GuiRow(row);

    double rowLx = row->lx() / dbu;
    double rowLy = row->ly() / dbu;
    double rowDx = row->dx() / dbu;
    double rowDy = row->dy() / dbu;
  
    row_gui->setRect( QRectF(rowLx, rowLy, rowDx, rowDy) );
  
    this->addItem(row_gui);
  }
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
