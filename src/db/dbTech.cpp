#include <iostream>
#include <algorithm>

#include "dbTech.h"

namespace db
{

dbLayer*
dbTech::getLayerByName(std::string& name)
{
  auto itr = str2dbLayer_.find(name);

	if(itr == str2dbLayer_.end())
	{
    std::cout << "Error - Layer " << name;
    std::cout << " does not exist in the database..." << std::endl;
    exit(0);
	}
	else
		return itr->second;
}

}
