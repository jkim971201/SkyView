#ifndef LAYOUT_SCENE_H
#define LAYOUT_SCENE_H

#include <memory>
#include <QColor>
#include <QPainter>
#include <QGraphicsScene>

#include "db/dbDatabase.h"

using namespace db;

namespace gui
{

class LayoutScene : public QGraphicsScene
{
  Q_OBJECT

  public:
    
    LayoutScene(QObject* parent = nullptr);

    void setDatabase(std::shared_ptr<dbDatabase> db) { db_ = db; }
		void createGuiDie();
		void createGuiRow();
		void createGuiInst();

  private:

    std::shared_ptr<dbDatabase> db_;
};

}

#endif
