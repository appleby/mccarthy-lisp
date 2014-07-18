#include <algorithm>
#include <cctype>
#include <sstream>

#include "cons.h"
#include "utils.h"

namespace mclisp
{

template<typename T>
std::string ContainerToString(T items)
{
  std::ostringstream oss;
  for(auto item : items)
    oss << item;
  return oss.str();
}

template std::string ContainerToString<>(std::set<Token> items);

// TODO Figure out where to put ShouldQuit
bool ShouldQuit(ConsCell *exp)
{
  std::ostringstream oss;
  oss << *exp;
  return EQ(exp, EOF) || oss.str() == "QUIT" || oss.str() == "(QUIT)";
}

std::string ToUpper(std::string s)
{
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);
  return s;
}

std::string ToLower(std::string s)
{
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s;
}

} // namespace mclisp
