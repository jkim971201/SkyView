#ifndef SKYLINE_H
#define SKYLINE_H

#include <memory>
#include <unordered_map>

extern "C" {
struct Tcl_Interp;
}

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

class TclCmd;

class TclCmdList
{
  public:
    static void addTclCmd(Tcl_Interp* interp, std::unique_ptr<TclCmd> newCmd);
    static TclCmd* getCmdByName(const char* name);

  private:
    static std::unordered_map<std::string, std::unique_ptr<TclCmd>> name2Cmd_;
};

class SkyLine
{
  public:

    static SkyLine* getStaticPtr();

    void initSkyLine(Tcl_Interp* interp);

    void readLef          (const char* file_path);
    void readDef          (const char* file_path);
    void readVerilog      (const char* file_path);
    void readBookShelf    (const char* file_path);
    void setTopModuleName (const char*  top_name);

    void runGlobalPlace();
    void display();

  private:

    // For Singleton, we should make Constructor and Deconstructor as private
    SkyLine();
    ~SkyLine();

    std::shared_ptr<db::dbDatabase>     db_;
    std::unique_ptr<gui::SkyView>       gui_;
    std::unique_ptr<skyplace::SkyPlace> skyplace_;

    // Tcl Interface
    Tcl_Interp* interp_;

    void initTclCmds();

};

}

#endif
