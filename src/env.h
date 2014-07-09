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
  private:
    static const std::string err_prefix_;

  public:
    explicit UnboundSymbolError(const mclisp::ConsCell* sym):
      Error(err_prefix_ + mclisp::SymbolName(sym)) {};
};

extern mclisp::ConsCell* g_init_env;
extern mclisp::ConsCell* g_user_env;

void Init();

mclisp::ConsCell* MakeEnv();
mclisp::ConsCell* Copy(const mclisp::ConsCell* env);
mclisp::ConsCell* Extend(mclisp::ConsCell* env, mclisp::ConsCell* sym,
                         mclisp::ConsCell* value);
mclisp::ConsCell* Lookup(mclisp::ConsCell* env, const mclisp::ConsCell* sym);
  
} // namespace env
} // namespace mclisp

#endif // ENV_H
