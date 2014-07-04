#ifndef MCLISP_ALLOC_H_
#define MCLISP_ALLOC_H_

#include <array>
#include <cstddef>
#include <list>

#include "cons.h"

namespace mclisp
{

class ConsAllocator
{
public:
  typedef std::size_t size_type;

  ConsAllocator();
  ConsCell* Allocate();
  std::list<ConsCell*> Allocate(size_type n);
  void Deallocate(ConsCell* p);

  inline size_type max_size() const { return free_list_.size(); }
  static constexpr size_type max_heap_size() { return kMaxHeapSize; }

private:
  static constexpr size_type kMaxHeapSize = 15000;

  std::list<ConsCell*> free_list_;
  std::array<ConsCell, kMaxHeapSize> heap_;
};

namespace alloc
{

void Init();
void Shutdown();
mclisp::ConsCell* Allocate();

} // namespace alloc
} // namespace mclisp

#endif // MCLISP_ALLOC_H_
