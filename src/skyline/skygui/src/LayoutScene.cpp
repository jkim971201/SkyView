#include <cassert>
#include <iostream>

#include "db/dbTech.h"
#include "db/dbDesign.h"
#include "db/dbDie.h"
#include "db/dbInst.h"
#include "db/dbNet.h"
#include "db/dbITerm.h"
#include "db/dbBTerm.h"
#include "db/dbRow.h"

#include "LayoutScene.h"
#include "gui_item/GuiDie.h"
#include "gui_item/GuiRow.h"
#include "gui_item/GuiInst.h"
#include "gui_item/GuiIO.h"

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

  int coreLx_micron = db_->getDesign()->coreLx();
  int coreLy_micron = db_->getDesign()->coreLy();
  int coreUx_micron = db_->getDesign()->coreUx();
  int coreUy_micron = db_->getDesign()->coreUy();

  for(auto bterm : db_->getDesign()->getBTerms())
  {
    GuiIO* io_gui = new GuiIO(bterm);
  
    int ioLx_micron = bterm->lx();
    int ioLy_micron = bterm->ly();
		int ioUx_micron = bterm->ux();
		int ioUy_micron = bterm->uy();
    int ioDx_micron = bterm->dx();
    int ioDy_micron = bterm->dy();

		if(ioLx_micron <= coreLx_micron)
			io_gui->setLeft();
		else if(ioLy_micron <= coreLy_micron)
			io_gui->setBottom();
		else if(ioUx_micron >= coreUx_micron)
			io_gui->setRight();
		else if(ioUy_micron >= coreUy_micron)
			io_gui->setTop();

    double ioLx_dbu = ioLx_micron / dbu;
    double ioLy_dbu = ioLy_micron / dbu;
    double ioDx_dbu = ioDx_micron / dbu;
    double ioDy_dbu = ioDy_micron / dbu;
  
    io_gui->setRect( QRectF(ioLx_dbu, ioLy_dbu, ioDx_dbu, ioDy_dbu) );
    this->addItem(io_gui);
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
