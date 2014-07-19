#include "alloc.h"

#include <list>
#include <memory>
#include <vector>

#include "gtest/gtest.h"

namespace
{
class AllocatorTest: public ::testing::Test
{
protected:
  mclisp::ConsAllocator allocator_;
};

class AllocNTimesOkTest:
  public AllocatorTest,
  public ::testing::WithParamInterface<mclisp::ConsAllocator::size_type>
{};

TEST_F(AllocatorTest, Init)
{
  EXPECT_EQ(allocator_.max_heap_size(), allocator_.max_size());
}

INSTANTIATE_TEST_CASE_P(
 AllocNTimesOk,
 AllocNTimesOkTest,
 ::testing::Values(1, 2, 10, 100, mclisp::ConsAllocator::max_heap_size()));

TEST_P(AllocNTimesOkTest, OneAtATime)
{
  std::vector<mclisp::ConsCell*> cells(GetParam(), nullptr);

  // Allocate
  for (size_t i = 0; i < GetParam(); ++i)
  {
    EXPECT_NO_THROW(cells[i] = allocator_.Allocate());
    EXPECT_NE(nullptr, cells[i]);
    EXPECT_EQ(allocator_.max_heap_size() - i - 1, allocator_.max_size());
  }

  // Deallocate
  const size_t heapSizeBaseOffset = allocator_.max_heap_size() - GetParam();
  for (size_t i = 0; i < GetParam(); ++i)
  {
    EXPECT_NO_THROW(allocator_.Deallocate(cells[i]));
    EXPECT_EQ(heapSizeBaseOffset + i + 1, allocator_.max_size());
  }
}

TEST_P(AllocNTimesOkTest, NAtATime)
{
  std::list<mclisp::ConsCell*> cells(GetParam(), nullptr);
  EXPECT_NO_THROW(cells = allocator_.Allocate(GetParam()));
  EXPECT_EQ(allocator_.max_heap_size() - GetParam(), allocator_.max_size());

  for (auto it : cells)
  {
    EXPECT_NE(nullptr, it);
    EXPECT_NO_THROW(allocator_.Deallocate(it));
  }

  EXPECT_EQ(allocator_.max_heap_size(), allocator_.max_size());
}

TEST_F(AllocatorTest, AllocTooManyThrowsException)
{
  EXPECT_THROW(allocator_.Allocate(mclisp::ConsAllocator::max_heap_size() + 1),
               std::bad_alloc);
}
} // namespace 
