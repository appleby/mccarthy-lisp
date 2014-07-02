#include "init.h"
#include "alloc.h"
#include "cons.h"
#include "env.h"

namespace mclisp
{

void InitLisp()
{
  static bool initialized = false;
  
  if (initialized)
    return;

  Alloc::Init();

  if (kNil == nullptr)
  {
    kNil = MakeSymbol("NIL");
    HackToFixNil();
  }

  if (kT == nullptr)
    kT = MakeSymbol("T");

  if (kAtom == nullptr)
    kAtom = MakeSymbol("ATOM");

  if (kQuote == nullptr)
    kQuote = MakeSymbol("QUOTE");

  env::Init();

  initialized = true;
}

} // mclisp
