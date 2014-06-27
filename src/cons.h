#ifndef MCLISP_CONS_H_
#define MCLISP_CONS_H_

#include <string>

namespace mclisp
{

class ConsCell
{
public:
    ConsCell(): car_(nullptr), cdr_(nullptr) {}
    ConsCell(ConsCell* car, ConsCell* cdr): car_(car), cdr_(cdr) {}

    ConsCell* car_;
    ConsCell* cdr_;
};

extern const ConsCell* kNil;
extern const ConsCell* kT;

const ConsCell* MakeSymbol(const std::string& name);
const ConsCell* MakeCons(const ConsCell* car, const ConsCell* cdr);

inline bool Symbolp(const ConsCell* c);
const std::string SymbolName(const ConsCell* symbol);

} // namespace mclisp

#endif // MCLISP_CONS_H_
