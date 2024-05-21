#include <memory>
#include "db/dbDatabase.h"

using namespace db;

namespace skyline
{

class SkyLine
{
	public:

    static SkyLine* getStaticPtr();

    void readLef(const char* file_path);

	private:

		// For Singleton,
		// we should make Constructor and Deconstructor as private
    SkyLine();
		~SkyLine();

		std::shared_ptr<dbDatabase> db_;
};

}
