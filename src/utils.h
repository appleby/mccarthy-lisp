#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <set>

#include "lexer.h"

namespace mclisp {

template<typename T>
std::string container_to_string(T items);

extern template std::string container_to_string<>(std::set<Token> items);

struct ConsCell;
bool ShouldQuit(ConsCell *exp);

std::string ToUpper(std::string s);
std::string ToLower(std::string s);

} // namespace mclisp

#endif // UTILS_H_
