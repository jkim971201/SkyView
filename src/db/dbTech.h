#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "lef/lefrReader.hpp"

#include "dbLayer.h"

namespace db
{

class dbTech
{
	public:

    dbTech(std::shared_ptr<dbTypes> types)
			: types_ (types) 
		{}

    std::string name() const { return name_; }

          dbLayer* getLayerByName(std::string& name);
    const dbLayer* getLayerByName(std::string& name) const
		{
			return getLayerByName(name);
		};

	private:

    std::string name_;

    std::shared_ptr<dbTypes> types_;

    std::unordered_map<std::string, dbLayer*> str2dbLayer_;

		std::vector<dbLayer*> layers_;
};

}
