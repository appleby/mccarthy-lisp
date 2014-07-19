#include "env.h"
#include "cons.h"
#include "init.h"

#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace
{
using namespace mclisp;

class EnvTest : public ::testing::Test
{
  protected:
    EnvTest()
    {
      env_ = env::MakeEnv();
      foosym_ = MakeSymbol("FOO");
      barsym_ = MakeSymbol("BAR");
      foobar_ = Cons(foosym_, barsym_);
    }
    ConsCell* env_;
    ConsCell* foosym_;
    ConsCell* barsym_;
    ConsCell* foobar_;
};

TEST_F(EnvTest, EmptyEnv)
{
  EXPECT_THROW(env::Lookup(env_, kNil), env::UnboundSymbolError);
  EXPECT_THROW(env::Lookup(env_, kT), env::UnboundSymbolError);
  EXPECT_THROW(env::Lookup(env_, foosym_), env::UnboundSymbolError);
}

TEST_F(EnvTest, GlobalInitEnv)
{
  EXPECT_EQ(*kT, *env::Lookup(env::g_init_env, kT));
  EXPECT_EQ(*kNil, *env::Lookup(env::g_init_env, kNil));
  EXPECT_THROW(env::Lookup(env::g_init_env, foosym_), env::UnboundSymbolError);
}

TEST_F(EnvTest, GlobalUserEnv)
{
  EXPECT_EQ(*kT, *env::Lookup(env::g_user_env, kT));
  EXPECT_EQ(*kNil, *env::Lookup(env::g_user_env, kNil));
  EXPECT_THROW(env::Lookup(env::g_user_env, foosym_), env::UnboundSymbolError);
}

TEST_F(EnvTest, Copy)
{
  env_ = env::Copy(env::g_user_env);
  env_ = env::Extend(env_, foosym_, kT);

  EXPECT_EQ(*kNil, *env::Lookup(env_, kNil));
  EXPECT_EQ(*kT, *env::Lookup(env_, kT));
  EXPECT_EQ(*kT, *env::Lookup(env_, foosym_));

  EXPECT_EQ(*kNil, *env::Lookup(env::g_user_env, kNil));
  EXPECT_EQ(*kT, *env::Lookup(env::g_user_env, kT));
  EXPECT_THROW(env::Lookup(env::g_user_env, foosym_), env::UnboundSymbolError);
}

TEST_F(EnvTest, Extend)
{
  env_ = env::Extend(env_, foosym_, kT);
  EXPECT_EQ(*kT, *env::Lookup(env_, foosym_));
  EXPECT_THROW(env::Lookup(env_, barsym_), env::UnboundSymbolError);

  env_ = env::Extend(env_, barsym_, kNil);
  EXPECT_EQ(*kT, *env::Lookup(env_, foosym_));
  EXPECT_EQ(*kNil, *env::Lookup(env_, barsym_));

  env_ = env::Extend(env_, barsym_, foobar_);
  EXPECT_EQ(*kT, *env::Lookup(env_, foosym_));
  EXPECT_EQ(*foobar_, *env::Lookup(env_, barsym_));
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
  env_ = env::ExtendAll(env_, List(foosym_, barsym_, newsym), List(kT, kNil, foosym_));
  EXPECT_EQ(*kT, *env::Lookup(env_, foosym_));
  EXPECT_EQ(*kNil, *env::Lookup(env_, barsym_));
  EXPECT_EQ(*foosym_, *env::Lookup(env_, newsym));
}
} // namespace 
