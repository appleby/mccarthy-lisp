#include <sstream>

#include "cons.h"
#include "utils.h"

namespace mclisp
{

template<typename T>
std::string container_to_string(T items)
{
  std::ostringstream oss;
  for(auto item : items)
    oss << item;
  return oss.str();
}

template std::string container_to_string<>(std::set<Token> items);

// TODO Figure out where to put ShouldQuit
bool ShouldQuit(ConsCell *exp)
{
  std::ostringstream oss;
  oss << *exp;
  return EQ(exp, EOF) || oss.str() == "QUIT" || oss.str() == "(QUIT)";
}

} // namespace mclisp
