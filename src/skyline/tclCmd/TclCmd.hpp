#ifndef TCL_CMD_H
#define TCL_CMD_H

#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>

namespace skyline
{

class TclCmd
{
 public:

  TclCmd(const char* cmd_name)
  {
		name_ = std::string(cmd_name);
		arg_ = "";
    opts_.clear();
    optArgs_.clear();
  }

	~TclCmd()	{}

  const std::string& name() { return name_; }
  void giveArg(std::string& arg) { arg_ = arg; }
  void giveOptArg(std::string& opt_name, double number)
  {
    auto itr = optArgs_.find(opt_name);

    if(itr != optArgs_.end())
    {
      itr->second = number;
    }
    else 
    {
      printf("Wrong option %s for command %s ...\n", 
              opt_name.c_str(), name_.c_str() );
      exit(1);
    }
  }

  virtual void execute() = 0; // Pure Virtual Function

 protected:

  void addOption(std::string option_name)
  {
    opts_.push_back(option_name);
    optArgs_[option_name] = 0.0;
  }

  std::string name_;
  std::string arg_;                                 // Argument of this command 
  std::vector<std::string> opts_;                   // Options  of this command
  std::unordered_map<std::string, double> optArgs_; // Argument of Options
};


}

#endif
