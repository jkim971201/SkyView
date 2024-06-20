#include <cassert>
#include <iostream>

#include "db/dbTech.h"
#include "db/dbDesign.h"
#include "db/dbDie.h"
#include "db/dbInst.h"
#include "db/dbNet.h"
#include "db/dbITerm.h"
#include "db/dbBTerm.h"
#include "db/dbMTerm.h"
#include "db/dbRow.h"

#include "LayoutScene.h"
#include "gui_item/GuiDie.h"
#include "gui_item/GuiRow.h"
#include "gui_item/GuiInst.h"
#include "gui_item/GuiIO.h"
#include "gui_item/GuiPin.h"

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

void
LayoutScene::createGuiIO()
{
  double dbu = static_cast<double>(db_->getTech()->getDbu());

  int coreLx_dbu = db_->getDesign()->coreLx();
  int coreLy_dbu = db_->getDesign()->coreLy();
  int coreUx_dbu = db_->getDesign()->coreUx();
  int coreUy_dbu = db_->getDesign()->coreUy();

  for(auto bterm : db_->getDesign()->getBTerms())
  {
    GuiIO* io_gui = new GuiIO(bterm);
  
    int ioLx_dbu = bterm->lx();
    int ioLy_dbu = bterm->ly();
    int ioUx_dbu = bterm->ux();
    int ioUy_dbu = bterm->uy();
    int ioDx_dbu = bterm->dx();
    int ioDy_dbu = bterm->dy();

    if(ioLx_dbu <= coreLx_dbu)
      io_gui->setLeft();
    else if(ioLy_dbu <= coreLy_dbu)
      io_gui->setBottom();
    else if(ioUx_dbu >= coreUx_dbu)
      io_gui->setRight();
    else if(ioUy_dbu >= coreUy_dbu)
      io_gui->setTop();

    double ioLx_micron = ioLx_dbu / dbu;
    double ioLy_micron = ioLy_dbu / dbu;
    double ioDx_micron = ioDx_dbu / dbu;
    double ioDy_micron = ioDy_dbu / dbu;
  
    io_gui->setRect( QRectF(ioLx_micron, 
                            ioLy_micron, 
                            ioDx_micron, 
                            ioDy_micron) );
    this->addItem(io_gui);
  }
}

void
LayoutScene::createGuiPin()
{
  double dbu = static_cast<double>(db_->getTech()->getDbu());

  for(auto iterm : db_->getDesign()->getITerms())
  {
		if(!iterm->isSignal())
			continue;

    GuiPin* io_pin = new GuiPin(iterm);
  
    const auto itermRect = iterm->getRect();

    double pinLx_micron = static_cast<double>(itermRect.lx) / dbu;
    double pinLy_micron = static_cast<double>(itermRect.ly) / dbu;
    double pinUx_micron = static_cast<double>(itermRect.ux) / dbu;
    double pinUy_micron = static_cast<double>(itermRect.uy) / dbu;

    io_pin->setRect( QRectF(pinLx_micron, 
                            pinLy_micron,
                            pinUx_micron - pinLx_micron,
                            pinUy_micron - pinLy_micron) );
    this->addItem(io_pin);
  }
}

void
LayoutScene::expandScene()
{
  QRectF rect = sceneRect();
  double sceneW = rect.width();
  double sceneH = rect.height();

  // Make 10% blank margin along boundary
  rect.adjust(-0.1 * sceneW, -0.1 * sceneH, 
              +0.1 * sceneW, +0.1 * sceneH);

  this->setSceneRect(rect);
}

}
