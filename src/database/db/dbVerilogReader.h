#ifndef DB_VERILOG_READER_H
#define DB_VERILOG_READER_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "verilog_driver.hpp"

#include "dbTech.h"
#include "dbDesign.h"

namespace db
{

class dbVerilogReader : public verilog::ParserVerilogInterface
{
  public:

    virtual ~dbVerilogReader() {}
  
    dbVerilogReader(std::shared_ptr<dbTech>   tech,
                    std::shared_ptr<dbDesign> design);

    void add_module(std::string&& name)
    {
      std::cout << "Module name = " << name << '\n';
    }
  
    void add_port(verilog::Port&& port) 
    {
      std::cout << "Port: " << port << '\n';
      ports_.push_back(std::move(port));
    }  
  
    void add_net(verilog::Net&& net) 
    {
      std::cout << "Net: " << net << '\n';
      nets_.push_back(std::move(net));
    }  
  
    void add_assignment(verilog::Assignment&& ast) 
    {
      std::cout << "Assignment: " << ast << '\n';
      assignments_.push_back(std::move(ast));
    }  
  
    void add_instance(verilog::Instance&& inst) 
    {
      std::cout << "Instance: " << inst << '\n';
      insts_.push_back(std::move(inst));
    }
    
  private:

    std::shared_ptr<dbTech>   tech_;
    std::shared_ptr<dbDesign> design_;

    std::vector<verilog::Port>       ports_;
    std::vector<verilog::Net>        nets_;
    std::vector<verilog::Assignment> assignments_;
    std::vector<verilog::Instance>   insts_;
};

}

#endif
