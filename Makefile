CXX=clang++
CPPFLAGS+=-I $(srcdir)
CXXFLAGS+=-g -O3 -Wall -Wextra -std=c++11

ifdef DEBUG
    CXXFLAGS+=-g3 -O0
endif

srcdir=./src
srcs=$(wildcard $(srcdir)/*.cc)
main_obj=$(srcdir)/main.o
objects=$(filter-out $(main_obj), $(patsubst %.cc, %.o, $(srcs)))

testdir=./test
test_srcs=$(wildcard $(testdir)/*.cc)
test_objs=$(patsubst %.cc, %.o, $(test_srcs))

gtestdir=$(testdir)/gtest-1.7.0
gtestlib=$(gtestdir)/make/gtest.a
gtest_CPPFLAGS= -isystem $(gtestdir)/include $(CPPFLAGS)
gtest_CXXFLAGS= -pthread $(CXXFLAGS)
gtest_headers = $(gtestdir)/include/gtest/*.h \
                $(gtestdir)/include/gtest/internal/*.h

all: mclisp

mclisp: $(main_obj) $(objects)
	$(CXX) $^ -o $@

$(main_obj): %.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@

$(objects): %.o: %.cc %.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@

$(test_objs): %.o: %.cc $(gtest_headers)
	$(CXX) $(gtest_CPPFLAGS) $(gtest_CXXFLAGS) $< -c -o $@

testrunner: $(test_objs) $(objects) $(gtestlib)
	$(CXX) $(gtest_CPPFLAGS) $(gtest_CXXFLAGS) -lpthread $^ -o $@

test: testrunner
	./testrunner

$(gtestlib):
	$(MAKE) -C $(gtestdir)/make $(notdir $@)

.PHONY : gtestclean
gtestclean:
	$(MAKE) -C $(gtestdir)/make clean

.PHONY : clean
clean::
	rm -f $(main_obj) $(test_objs) $(objects) mclisp testrunner
