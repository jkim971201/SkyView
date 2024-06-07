#include "dbVerilogReader.h"

namespace db
{

dbVerilogReader::dbVerilogReader(std::shared_ptr<dbTech>   tech,
                                 std::shared_ptr<dbDesign> design)
  : tech_    (  tech),
    design_  (design)
{
  ports_.clear();
  nets_.clear();
  assignments_.clear();
  insts_.clear();
}

}
