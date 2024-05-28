#include <memory>

#include "db/dbDatabase.h"

namespace gui
{

using namespace db;

class Viewer
{

  public:

		Viewer(std::shared_ptr<dbDatabase> db);

		void display();

  private:

		std::unique_ptr<MainWindow> window_;
		std::shared_ptr<dbDatabase> db_;
};

}
