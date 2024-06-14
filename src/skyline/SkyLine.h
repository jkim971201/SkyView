#include <memory>
#include "db/dbDatabase.h"
#include "gui/SkyView.h"
#include "SkyPlaceGP/SkyPlace.h"

using namespace db;
using namespace gui;
using namespace skyplace;

namespace skyline
{

class SkyLine
{
  public:

    static SkyLine* getStaticPtr();

    void readLef          (const char* file_path);
    void readDef          (const char* file_path);
    void readVerilog      (const char* file_path);
    void readBookShelf    (const char* file_path);
		void setTopModuleName (const char*  top_name);

    void display();

  private:

    // For Singleton,
    // we should make Constructor and Deconstructor as private
    SkyLine();
    ~SkyLine();

    std::shared_ptr<dbDatabase> db_;
    std::shared_ptr<SkyPlace>   skyPlace_;
    std::shared_ptr<SkyView>    gui_;
};

}
