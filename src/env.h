#ifndef ENV_H
#define ENV_H

#include <stdexcept>
#include <string>

#include "cons.h"

namespace mclisp
{
namespace env
{

class UnboundSymbolError: public std::logic_error
{
  private:
    static const std::string err_prefix_;

  public:
    explicit UnboundSymbolError(const mclisp::ConsCell* sym):
      logic_error(err_prefix_ + mclisp::SymbolName(sym)) {};
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
