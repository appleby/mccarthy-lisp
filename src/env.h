#ifndef ENV_H
#define ENV_H

#include <string>

#include "cons.h"
#include "error.h"

namespace mclisp
{
namespace env
{

class UnboundSymbolError: public Error
{
  public:
    explicit UnboundSymbolError(const char* file, const char* func, int line,
                                const mclisp::ConsCell* sym):
      Error(file, func, line, mclisp::SymbolName(sym)) {};

  protected:
    virtual const char* prefix() const noexcept
    { return "Unbound Symbol Error:"; }
};

#define UNBOUND_SYMBOL_ERROR(sym) THROW(UnboundSymbolError, sym)

extern mclisp::ConsCell* g_init_env;
extern mclisp::ConsCell* g_user_env;

void Init();

mclisp::ConsCell* MakeEnv();
mclisp::ConsCell* Copy(const mclisp::ConsCell* env);
mclisp::ConsCell* Extend(mclisp::ConsCell* env, mclisp::ConsCell* sym,
                         mclisp::ConsCell* value);
mclisp::ConsCell* ExtendAll(mclisp::ConsCell* env, mclisp::ConsCell* symbols,
                            mclisp::ConsCell* values);
mclisp::ConsCell* Lookup(mclisp::ConsCell* env, const mclisp::ConsCell* sym);
  
} // namespace env
} // namespace mclisp

#endif // ENV_H
