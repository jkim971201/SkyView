#pragma once

#include <vector>

#include "dbLayer.h"
#include "dbRect.h"

namespace db
{

class dbObs
{
  public: 

    dbObs();

    void print() const;

    // Setters
    void addRect(dbRect& rect) { rects_.push_back(rect); }

    // Getters
    const std::vector<dbRect>& rects() const { return rects_; }

  private:

    std::vector<dbRect> rects_;
};

}
