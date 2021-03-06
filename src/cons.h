#ifndef MCLISP_CONS_H_
#define MCLISP_CONS_H_

#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace mclisp
{

struct ConsCell
{
  explicit operator bool() const;

  ConsCell* car;
  ConsCell* cdr;
};

typedef struct ConsCell ConsCell;

namespace cons
{
void Init();
}

bool operator==(const ConsCell& lhs, const ConsCell& rhs);
bool operator!=(const ConsCell& lhs, const ConsCell& rhs);
bool operator< (const ConsCell& lhs, const ConsCell& rhs);
bool operator> (const ConsCell& lhs, const ConsCell& rhs);
bool operator<=(const ConsCell& lhs, const ConsCell& rhs);
bool operator>=(const ConsCell& lhs, const ConsCell& rhs);

std::ostream& operator<<(std::ostream& os, const ConsCell& cons);
std::string ToString(const ConsCell* cons);

extern ConsCell* kNil;
extern ConsCell* kT;
extern std::map<std::string, ConsCell *> g_builtin_symbols;

ConsCell* FromBool(bool pred);
ConsCell* MakeSymbol(const std::string& name);
ConsCell* Cons(ConsCell* car, ConsCell* cdr);
ConsCell* Acons(ConsCell* key, ConsCell* value, ConsCell* alist);

bool Atom(const ConsCell* c);
bool Symbolp(const ConsCell* c);
bool Consp(const ConsCell* c);
bool Listp(const ConsCell* c);
bool Eq(const ConsCell* a, const ConsCell* b);
bool Null(const ConsCell* c);

// Get the global builtin symbol corresponding to sym.
#define BUILTIN(sym) mclisp::g_builtin_symbols[#sym]

// Test if exp is Eq to a builtin symbol. This should probably be named
// EQ_BUILTIN or similar, but the extra characters hurt readability. Note that
// we don't use the BUILTIN macro for the second arg, since we don't want that
// arg macro expanded. We need to prevent macro expansion of that arg because
// EOF is both a mclisp symbol name and a standard C macro. 
#define EQ(exp, sym) mclisp::Eq(exp, mclisp::g_builtin_symbols[#sym])

ConsCell* Car(const ConsCell* c);
ConsCell* Cdr(const ConsCell* c);
ConsCell* Caar(const ConsCell* c);
ConsCell* Cadr(const ConsCell* c);
ConsCell* Cadar(const ConsCell* c);
ConsCell* Caddr(const ConsCell* c);
ConsCell* Cdddr(const ConsCell* c);
ConsCell* Caddar(const ConsCell* c);

ConsCell* Assoc(const ConsCell* k, const ConsCell* alist);
ConsCell* CopyAlist(const ConsCell* alist);
ConsCell* CopyList(const ConsCell* list);

ConsCell* List_(std::vector<ConsCell *>);
#define List(values...) List_({values})

size_t Length(const ConsCell *list);

ConsCell* Append(const ConsCell* L1, ConsCell* L2);
ConsCell* Pair(const ConsCell *L1, const ConsCell *L2);

const std::string SymbolName(const ConsCell* symbol);

} // namespace mclisp

#endif // MCLISP_CONS_H_
