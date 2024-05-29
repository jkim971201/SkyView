#include <memory>
#include <QApplication>

#include "db/dbDatabase.h"

using namespace db;

namespace gui
{

class MainWindow;

class SkyView
{

  public:

    SkyView();
		~SkyView();

    void linkDatabase(std::shared_ptr<dbDatabase> db);
    void display();

  private:

    std::shared_ptr<dbDatabase>   db_;
    std::unique_ptr<QApplication> app_;
    std::unique_ptr<MainWindow>   window_;
};

}
