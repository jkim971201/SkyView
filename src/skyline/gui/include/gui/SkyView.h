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

    SkyView(const std::shared_ptr<dbDatabase> db);
    ~SkyView();

    void linkDatabase(std::shared_ptr<dbDatabase> db);
    void display();

  private:

    std::shared_ptr<dbDatabase> db_;
};

}
