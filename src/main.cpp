#include <cstdio>
#include <iostream>
#include <filesystem>

#include "lef/lefrReader.hpp"
#include "lef/lefwWriter.hpp"
#include "lef/lefiDebug.hpp"
#include "lef/lefiEncryptInt.hpp"
#include "lef/lefiUtil.hpp"

#include "db/dbDatabase.h"

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    std::cout << "Please give input file" << std::endl;
    exit(0);
  }

  printf("Read lef %s \n", argv[1]);

//	// For each .lef, we should call this
//  lefrReset();
//  (void)lefrEnableReadEncrypted();
//
    char* file_name = argv[1];
//	char* userData;
//  FILE* file = fopen(file_name, "r");
//  int status = lefwInit(stdout); // initialize the lef writer,
//  int res    = lefrRead(file, file_name, (void*)userData);
//  fclose(file);
//
//  lefrClear();    

  db::dbDatabase _db;

	_db.readLef(std::filesystem::path(std::string(file_name)));

	return 0;
}
