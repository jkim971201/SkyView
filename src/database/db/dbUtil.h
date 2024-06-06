#ifndef DB_UTIL_H
#define DB_UTIL_H

#include <string>
#include <unordered_map>

namespace db
{

template <typename M>
inline bool duplicateCheck(M& map, const std::string& name)
{
  if(map.find(name) != map.end())
    return true;
  else return false;
}

inline std::string removeBackSlashBracket(const std::string& str)
{
  std::string newStr = str;
  //printf("before : %s\n", newStr.c_str());
  if(newStr.find("\\[") != std::string::npos && newStr.find("\\]") != std::string::npos)
  {
    size_t bracket1 = newStr.find("\\[");
    while(bracket1 != std::string::npos)
    {
      newStr.erase(newStr.begin() + bracket1);
      bracket1 = newStr.find("\\[");
    }

    size_t bracket2 = newStr.find("\\]");
    while(bracket2 != std::string::npos)
    {
      newStr.erase(newStr.begin() + bracket2);
      bracket2 = newStr.find("\\]");
    }
  }
  //printf("after : %s\n", newStr.c_str());

  return newStr;
}

}

#endif
