#include "init.h"
#include "alloc.h"
#include "cons.h"
#include "env.h"
#include "reader.h"

namespace mclisp
{

void InitLisp()
{
  static bool initialized = false;
  
  if (initialized)
    return;

  alloc::Init();
  cons::Init();
  env::Init();
  reader::Init();

  initialized = true;
}

} // mclisp
