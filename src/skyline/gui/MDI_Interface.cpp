#include "MDI_Interface.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>

namespace gui
{

MDI_Interface::MDI_Interface(QWidget *parent)
  : QMainWindow(parent)
{
  setWindowTitle(QString::fromUtf8("My MDI"));

  QMdiArea* area = new QMdiArea();
  area->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

  // MdiSubWindow
  QMdiSubWindow* subWindow1 = new QMdiSubWindow();
  subWindow1->resize(300, 200);

  QPushButton *btn = new QPushButton(QString("Button"));
  subWindow1->setWidget(btn);

  QMdiSubWindow* subWindow2 = new QMdiSubWindow();
  subWindow2->resize(300, 200);

  // MDIMainWindows
  area->addSubWindow(subWindow1);
  area->addSubWindow(subWindow2);

  setCentralWidget(area);
}

}
