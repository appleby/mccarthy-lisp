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

extern const ConsCell* kNil;
extern const ConsCell* kT;

void HackToFixNil();

const ConsCell* MakeSymbol(const std::string& name);
const ConsCell* MakeCons(const ConsCell* car, const ConsCell* cdr);

inline bool Symbolp(const ConsCell* c);
inline bool Consp(const ConsCell* c);

const std::string SymbolName(const ConsCell* symbol);

} // namespace mclisp

#endif // MCLISP_CONS_H_
