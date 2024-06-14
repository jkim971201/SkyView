#ifndef SKY_VIEW_H
#define SKY_VIEW_H

#include <memory>

#include "db/dbDatabase.h"

using namespace db;

namespace gui
{

class SkyView
{

  public:

    SkyView(std::shared_ptr<dbDatabase> db);
    ~SkyView();

    void linkDatabase(std::shared_ptr<dbDatabase> db);
    void display();

  private:

    std::shared_ptr<dbDatabase> db_;
};

}

#endif
