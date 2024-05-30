#include "LayoutScene.h"
#include "gui_item/GuiInst.h"
#include "gui_item/GuiDie.h"

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
  double dieUx = die->ux() / dbu;
  double dieUy = die->uy() / dbu;

  std::cout << "dieLx : " << dieLx << std::endl;
  std::cout << "dieLy : " << dieLy << std::endl;
  std::cout << "dieUx : " << dieUx << std::endl;
  std::cout << "dieUy : " << dieUy << std::endl;

  die_gui->setRect( QRectF(dieLx, dieLy, dieUx, dieUy) );

	this->addItem(die_gui);
}

void
LayoutScene::createGuiInst()
{
}

}
