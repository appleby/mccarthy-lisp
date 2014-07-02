#ifndef MCLISP_CONS_H_
#define MCLISP_CONS_H_

#include <string>

namespace mclisp
{

struct ConsCell
{
  ConsCell* car;
  ConsCell* cdr;
};

typedef struct ConsCell ConsCell;

bool operator==(const ConsCell& lhs, const ConsCell& rhs);
bool operator!=(const ConsCell& lhs, const ConsCell& rhs);
bool operator< (const ConsCell& lhs, const ConsCell& rhs);
bool operator> (const ConsCell& lhs, const ConsCell& rhs);
bool operator<=(const ConsCell& lhs, const ConsCell& rhs);
bool operator>=(const ConsCell& lhs, const ConsCell& rhs);

std::ostream& operator<<(std::ostream& os, const ConsCell& cons);

extern ConsCell* kNil;
extern ConsCell* kT;

void HackToFixNil();

ConsCell* MakeSymbol(const std::string& name);
ConsCell* MakeCons(ConsCell* car, ConsCell* cdr);

inline bool Symbolp(const ConsCell* c);
inline bool Consp(const ConsCell* c);

const std::string SymbolName(const ConsCell* symbol);

} // namespace mclisp

#endif // MCLISP_CONS_H_
