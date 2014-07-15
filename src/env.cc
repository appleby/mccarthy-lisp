#include <cassert>

#include "env.h"
#include "cons.h"

namespace mclisp
{
namespace env
{

using ::mclisp::Acons;
using ::mclisp::Assoc;
using ::mclisp::ConsCell;
using ::mclisp::CopyAlist;
using ::mclisp::Eq;
using ::mclisp::kNil;
using ::mclisp::Symbolp;

ConsCell* g_init_env = nullptr;
ConsCell* g_user_env = nullptr;

void Init()
{
  bool initialized = false;

  if (initialized)
    return;

  g_init_env = MakeEnv();
  g_init_env = Extend(g_init_env, kNil, kNil);
  g_init_env = Extend(g_init_env, kT, kT);
  g_user_env = Copy(g_init_env);
  initialized = true;
}

ConsCell* MakeEnv()
{
  return kNil;
}

ConsCell* Copy(const ConsCell* env)
{
  TYPECHECK(env, Listp);
  return CopyAlist(env);
}

ConsCell* Extend(ConsCell* env, ConsCell* sym, ConsCell* value)
{
  TYPECHECK(env, Listp);
  TYPECHECK(sym, Symbolp);
  return Acons(sym, value, env);
}

ConsCell* ExtendAll(ConsCell* env, ConsCell* symbols, ConsCell* values)
{
  // TODO unit test for ExtendAll
  TYPECHECK(env, Listp);
  TYPECHECK(symbols, Listp);
  TYPECHECK(values, Listp);
  return Append(Pair(symbols, values), env);
}

ConsCell* Lookup(ConsCell* env, const ConsCell* sym)
{
  TYPECHECK(env, Listp);
  TYPECHECK(sym, Symbolp);
  ConsCell* value = Assoc(sym, env);

  if (value == nullptr)
    UNBOUND_SYMBOL_ERROR(sym);

  return value;
}

} // namespace env
} // namespace mclisp
