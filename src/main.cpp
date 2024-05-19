#include <cstdio>
#include <iostream>
#include <filesystem>

#include "db/dbDatabase.h"

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    std::cout << "Please give input file" << std::endl;
    exit(0);
  }

  char* file_name = argv[1];

  db::dbDatabase _db;
  _db.readLef(std::filesystem::path(std::string(file_name)));

  return 0;
}
