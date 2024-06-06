#include "dbITerm.h"

#include <iostream>

namespace db
{

dbITerm::dbITerm()
  : name_  (""),
    net_   (nullptr),
    inst_  (nullptr),
    mterm_ (nullptr)
{
}

dbITerm::dbITerm(const std::string& name, dbInst* inst, dbMTerm* mterm)
  : name_  (name),
    net_   (nullptr),
    inst_  (inst),
    mterm_ (mterm)
{
}

void
dbITerm::print() const
{
  std::cout << "ITerm Name : " << name_ << std::endl;
  if(net_ != nullptr)
    std::cout << "Connected to net: " << net_->name() << std::endl;
  std::cout << std::endl;
}

}
