#ifndef SKYLINE_H
#define SKYLINE_H

#include <memory>

namespace db { 
  class dbDatabase; 
}

namespace gui { 
  class SkyView;    
}

namespace skyplace { 
  class SkyPlace;   
}

namespace skyline
{

class SkyLine
{
  public:

    static SkyLine* getStaticPtr();

    void readLef          (const char* file_path);
    void readDef          (const char* file_path);
    void readVerilog      (const char* file_path);
    void readBookShelf    (const char* file_path);
    void setTopModuleName (const char*  top_name);

    void runGlobalPlace(double target_ovf = 0.07, double target_den = 1.0);
    void display();

  private:

    // For Singleton, we should make Constructor and Deconstructor as private
    SkyLine();
    ~SkyLine();

    std::shared_ptr<db::dbDatabase>     db_;
    std::unique_ptr<gui::SkyView>       gui_;
    std::unique_ptr<skyplace::SkyPlace> skyplace_;

};

}

#endif
