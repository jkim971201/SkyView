#include "LayoutScene.h"
#include "gui_item/GuiInst.h"
#include "gui_item/GuiDie.h"

namespace gui
{

LayoutScene::LayoutScene(QObject* parent)
{

}

void
LayoutScene::createGuiDie()
{
  const dbDie* die = db_->getDesign()->getDie();
}

void
LayoutScene::createGuiInst()
{
}

}
