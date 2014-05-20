CC=clang++
CPPFLAGS=-std=c++11

srcdir=src
srcs=$(wildcard $(srcdir)/*.cc)
objects=$(patsubst %.cc, %.o, $(srcs))

all: mclisp

mclisp: $(objects)
	$(CC) $(LDFLAGS) $< -o $@

$(objects): %.o: %.cc
	$(CC) $(CPPFLAGS) $< -c -o $@

clean::
	-rm $(objects)
	-rm mclisp
