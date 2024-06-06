#ifndef DB_BTERM
#define DB_BTERM

#include <string>
#include <cstdlib>
#include <vector>

#include "dbTypes.h"
#include "dbLayer.h"
#include "dbRect.h"
#include "dbNet.h"

namespace db
{

class dbBTerm
{
  public:

    dbBTerm();

    void print() const;

    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setOrient    (Orient     orient) { orient_ = orient; } 
    void setDirection (PinDirection  dir) { direction_ = dir; }
    void setStatus    (Status     status) { status_ = status; } 

    void setOrigX(int val) { origX_ = val; }
    void setOrigY(int val) { origY_ = val; }

    void setNet(dbNet* net) { net_   = net; }
    void addRect(dbRect rect) { rects_.push_back(rect); }

    void setLocation();

    // Getters
    int ux() const { return ux_; }
    int uy() const { return uy_; }
    int lx() const { return lx_; }
    int ly() const { return ly_; }
    int dx() const { return ux_ - lx_; }
    int dy() const { return uy_ - ly_; }
    int cx() const { return (lx_ + ux_) / 2; }
    int cy() const { return (ly_ + uy_) / 2; }

    int64_t area() const { return static_cast<int64_t>( dx() ) 
                                * static_cast<int64_t>( dy() ); }

    int origX() const { return origX_; }
    int origY() const { return origY_; }

    bool isFixed()  const { return status_ == Status::FIXED;  }
    bool isCover()  const { return status_ == Status::COVER;  }
    bool isPlaced() const { return status_ == Status::PLACED; }

    const std::string& name() const { return name_; }

    Orient          orient() const { return orient_;    }
    PinDirection direction() const { return direction_; }
    Status          status() const { return status_;    }

    const dbNet* net() const { return net_; }
    const std::vector<dbRect>& rects() const { return rects_; }

  private:

    std::string name_;

    int lx_;
    int ly_;

    int ux_;
    int uy_;

    int origX_;
    int origY_;

    Orient       orient_;
    PinDirection direction_;
    Status       status_;

    dbNet* net_;
    std::vector<dbRect> rects_;
};

}

#endif
