#ifndef DB_NET
#define DB_NET

#include <string>

namespace db
{

class dbNet
{
  public:

    dbNet();

    // Setters
    void setName(const std::string& name) { name_ = name; }

    // Getters
    const std::string& name() const { return name_; }

  private:

    std::string name_;
    
};

}

#endif
