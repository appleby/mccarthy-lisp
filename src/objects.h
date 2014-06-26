#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace mclisp
{

class Sexp
{
  public:
    virtual ~Sexp() {};
    explicit operator bool() const;
};

class Atom : public Sexp
{
  public:
    virtual ~Atom() {};
};

class Symbol : public Atom
{
  private:
    std::string name_;

  public:
    explicit Symbol(const std::string& name) : name_(name) {};
    explicit Symbol(const char* name) : name_(name) {};

    std::string name() const { return name_; }
};

extern const Symbol koNil;
extern const Symbol koT;

class Cons : public Sexp
{
  private:
    const Sexp* car_;
    const Sexp* cdr_;

  public:
    Cons(const Sexp* car, const Sexp* cdr) : car_(car), cdr_(cdr) {};
    Cons(const Sexp& car, const Sexp& cdr) : car_(&car), cdr_(&cdr) {};

    const Sexp& car() const { return *car_; }
    const Sexp& cdr() const { return *cdr_; }
};

#define MCLISP_DECLARE_BOOL_OPS(arg_type) \
bool operator==(arg_type lhs, arg_type rhs); \
bool operator!=(arg_type lhs, arg_type rhs); \
bool operator< (arg_type lhs, arg_type rhs); \
bool operator> (arg_type lhs, arg_type rhs); \
bool operator<=(arg_type lhs, arg_type rhs); \
bool operator>=(arg_type lhs, arg_type rhs)

MCLISP_DECLARE_BOOL_OPS(const Sexp&);
MCLISP_DECLARE_BOOL_OPS(const Symbol&);
MCLISP_DECLARE_BOOL_OPS(const Cons&);

std::ostream& operator<<(std::ostream& os, const Sexp& sexp);
std::ostream& operator<<(std::ostream& os, const Symbol& symbol);
std::ostream& operator<<(std::ostream& os, const Cons& cons);

} // namespace mclisp
#endif // OBJECTS_H_
