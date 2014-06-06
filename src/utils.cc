#include <sstream>

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

} // namespace mclisp
