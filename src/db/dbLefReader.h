#pragma once

#include <memory>

#include "lef/lefrReader.hpp"
#include "lef/lefwWriter.hpp"
#include "lef/lefiDebug.hpp"
#include "lef/lefiEncryptInt.hpp"
#include "lef/lefiUtil.hpp"

#include "dbTech.h"

namespace db
{

class dbLefReader
{
	public:

    dbLefReader(std::shared_ptr<dbTypes> types, std::shared_ptr<dbTech> tech)
			: types_          (types),
			  tech_           ( tech),
				parse65nm_      (    0),
				parseLef58Type_ (    0)
	  {}

	private:

    std::shared_ptr<dbTypes> types_;
    std::shared_ptr<dbTech>  tech_;

    int parse65nm_;
		int parseLef58Type_;
};

}
