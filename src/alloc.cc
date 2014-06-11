#include <cassert>
#include <new>
#include <string>

#include "alloc.h"

namespace mclisp
{

ConsAllocator::ConsAllocator()
{
  for (auto i = heap_.begin(); i != heap_.end(); ++i)
    free_list_.push_back(&*i);
}

ConsCell* ConsAllocator::Allocate()
{
  return Allocate(1).front();
}

std::list<ConsCell*> ConsAllocator::Allocate(size_type n)
{
  if (n > max_size())
    throw std::bad_alloc();

  std::list<ConsCell*> cells;
  for(size_type i = 0; i < n; ++i)
  {
    cells.push_back(free_list_.front());
    free_list_.pop_front();
  }

  return cells;
}

void ConsAllocator::Deallocate(ConsCell* p)
{
  assert(max_size() < kMaxHeapSize);
  free_list_.push_front(p);
}

} // namespace mclisp
