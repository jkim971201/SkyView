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

void
dbVerilogReader::readFile(const std::string& filename)
{
  size_t dot = filename.find_last_of('.');
  std::string filetype = filename.substr(dot + 1);

  if(filetype != "v")
  {
    printf("Please give .v file!\n");
    exit(1);
  }

  // read(...) of ParserVerilogInterface will do
  // file existence check
  bool is_success = this->read(filename);

  if(!is_success)
  {
    printf("Fail to open %s\n", filename.c_str());
    exit(1);
  }
}

}
