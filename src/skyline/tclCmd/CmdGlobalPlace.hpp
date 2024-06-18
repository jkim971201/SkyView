#include <cassert>

#include "TclCmd.hpp"

namespace skyline
{
  class CmdGlobalPlace : public TclCmd
  {
    public:

      CmdGlobalPlace(const char* cmd_name)
        : TclCmd(cmd_name)
      {
        addOption("-target_density");
      }

      void execute() override
      {
        printf("success!\n");

        double den = optArgs_["target_density"];
        printf("target_density : %f\n", den);
      }
  };
}
