#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <set>

#include "lexer.h"

namespace mclisp {

template<typename T>
std::string container_to_string(T items);

extern template std::string container_to_string<>(std::set<Token> items);

} // namespace mclisp

#endif // UTILS_H_
