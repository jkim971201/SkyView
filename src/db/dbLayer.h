#pragma once

#include <string>

#include "dbTypes.h"

namespace db
{

class dbLayer
{

  public:

    dbLayer() {}

    std::string name() const { return name_; }

    int pitch()   const { return pitch_;   }
    int offset()  const { return offset_;  }
    int width()   const { return width_;   }
    int spacing() const { return spacing_; }

    double area() const { return area_; }

  private:

    // LEF Syntax
    std::string name_;

    int pitch_;
    int offset_;
    int width_;
    int spacing_;

    double area_;
};

}
