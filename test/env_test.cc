#include "env.h"
#include "mclisp_test.h"

#include <string>
#include <vector>

namespace
{
using namespace mclisp;

class EnvTest : public mclisp::testing::Test
{
  protected:
    EnvTest(): env_(env::MakeEnv()) {}

    ConsCell* env_;
};

TEST_F(EnvTest, EmptyEnv)
{
  EXPECT_THROW(env::Lookup(env_, kNil), env::UnboundSymbolError);
  EXPECT_THROW(env::Lookup(env_, kT), env::UnboundSymbolError);
  EXPECT_THROW(env::Lookup(env_, foo_), env::UnboundSymbolError);
}

TEST_F(EnvTest, GlobalInitEnv)
{
  EXPECT_EQ(*kT, *env::Lookup(env::g_init_env, kT));
  EXPECT_EQ(*kNil, *env::Lookup(env::g_init_env, kNil));
  EXPECT_THROW(env::Lookup(env::g_init_env, foo_), env::UnboundSymbolError);
}

TEST_F(EnvTest, GlobalUserEnv)
{
  EXPECT_EQ(*kT, *env::Lookup(env::g_user_env, kT));
  EXPECT_EQ(*kNil, *env::Lookup(env::g_user_env, kNil));
  EXPECT_THROW(env::Lookup(env::g_user_env, foo_), env::UnboundSymbolError);
}

TEST_F(EnvTest, Copy)
{
  env_ = env::Copy(env::g_user_env);
  env_ = env::Extend(env_, foo_, kT);

  EXPECT_EQ(*kNil, *env::Lookup(env_, kNil));
  EXPECT_EQ(*kT, *env::Lookup(env_, kT));
  EXPECT_EQ(*kT, *env::Lookup(env_, foo_));

  EXPECT_EQ(*kNil, *env::Lookup(env::g_user_env, kNil));
  EXPECT_EQ(*kT, *env::Lookup(env::g_user_env, kT));
  EXPECT_THROW(env::Lookup(env::g_user_env, foo_), env::UnboundSymbolError);
}

TEST_F(EnvTest, Extend)
{
  env_ = env::Extend(env_, foo_, kT);
  EXPECT_EQ(*kT, *env::Lookup(env_, foo_));
  EXPECT_THROW(env::Lookup(env_, bar_), env::UnboundSymbolError);

  env_ = env::Extend(env_, bar_, kNil);
  EXPECT_EQ(*kT, *env::Lookup(env_, foo_));
  EXPECT_EQ(*kNil, *env::Lookup(env_, bar_));

  env_ = env::Extend(env_, bar_, foobar_);
  EXPECT_EQ(*kT, *env::Lookup(env_, foo_));
  EXPECT_EQ(*foobar_, *env::Lookup(env_, bar_));
}

TEST_F(EnvTest, ManyBindings)
{
  std::vector<ConsCell *> symbols;
  for (unsigned i = 0; i < 100; ++i)
  {
    ConsCell* sym = MakeSymbol("SYM_" + std::to_string(i));
    env_ = env::Extend(env_, sym, sym);
    symbols.push_back(sym);
  }

  for (auto si : symbols)
    EXPECT_EQ(*si, *env::Lookup(env_, si));
}

TEST_F(EnvTest, ExtendAll)
{
  ConsCell *newsym = MakeSymbol("NEWSYM");
  env_ = env::ExtendAll(env_, List(foo_, bar_, newsym), List(kT, kNil, foo_));
  EXPECT_EQ(*kT, *env::Lookup(env_, foo_));
  EXPECT_EQ(*kNil, *env::Lookup(env_, bar_));
  EXPECT_EQ(*foo_, *env::Lookup(env_, newsym));
}
} // namespace
