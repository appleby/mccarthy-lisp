#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <set>

#include "cons.h"
#include "lexer.h"

namespace mclisp {

template<typename T>
std::string ContainerToString(T items);

extern template std::string ContainerToString<>(std::set<Token> items);

struct ConsCell;
bool ShouldQuit(ConsCell *exp);

std::string ToUpper(std::string s);
std::string ToLower(std::string s);

} // namespace mclisp

#endif // UTILS_H_
