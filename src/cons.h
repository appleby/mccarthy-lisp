#ifndef MCLISP_CONS_H_
#define MCLISP_CONS_H_

#include <string>

namespace mclisp
{

struct ConsCell
{
  explicit operator bool() const;

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
std::string ToString(const ConsCell* cons);

// TODO Initialize these in a saner way.
extern ConsCell* kNil;
extern ConsCell* kT;
extern ConsCell* kQuote;
extern ConsCell* kAtom;

void HackToFixNil();

ConsCell* FromBool(bool pred);
ConsCell* MakeSymbol(const std::string& name);
ConsCell* MakeCons(ConsCell* car, ConsCell* cdr);
ConsCell* Cons(ConsCell* car, ConsCell* cdr);
ConsCell* Acons(ConsCell* key, ConsCell* value, ConsCell* alist);

bool Atom(const ConsCell* c);
bool Symbolp(const ConsCell* c);
bool Consp(const ConsCell* c);
bool Eq(const ConsCell* a, const ConsCell* b);
bool Null(const ConsCell* c);

ConsCell* Car(const ConsCell* c);
ConsCell* Cdr(const ConsCell* c);
ConsCell* Caar(const ConsCell* c);
ConsCell* Cadr(const ConsCell* c);
ConsCell* Cadar(const ConsCell* c);

ConsCell* Assoc(const ConsCell* k, const ConsCell* alist);
ConsCell* CopyAlist(const ConsCell* alist);
ConsCell* CopyList(const ConsCell* list);

const std::string SymbolName(const ConsCell* symbol);

} // namespace mclisp

#endif // MCLISP_CONS_H_
