#ifndef DB_DIE
#define DB_DIE

#include <cstdint>

namespace db
{

// only supports rectangular for the time being
class dbDie
{
  public:

    dbDie();
    
    void print() const;

    // Setters
    void setLx(int lx) { lx_ = lx; }
    void setLy(int ly) { ly_ = ly; }
    void setUx(int ux) { ux_ = ux; }
    void setUy(int uy) { uy_ = uy; }

    // Getters
    int lx() const { return lx_; }
    int ly() const { return ly_; }
    int ux() const { return ux_; }
    int uy() const { return uy_; }

    int64_t area() const { return static_cast<int64_t>(ux_ - lx_)
                                * static_cast<int64_t>(uy_ - ly_); }
    
  private:

    int lx_;
    int ly_;
    int ux_;
    int uy_;
};

}

#endif