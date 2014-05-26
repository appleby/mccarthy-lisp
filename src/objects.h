#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <iostream>
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

extern const Symbol kNil;
extern const Symbol kT;

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

#define MCLISP_DEF_DERIVED_OPS(arg_type) \
inline bool operator!=(arg_type lhs, arg_type rhs){return !operator==(lhs,rhs);} \
inline bool operator> (arg_type lhs, arg_type rhs){return  operator< (rhs,lhs);} \
inline bool operator<=(arg_type lhs, arg_type rhs){return !operator> (lhs,rhs);} \
inline bool operator>=(arg_type lhs, arg_type rhs){return !operator< (lhs,rhs);}

inline bool operator==(const Symbol& lhs, const Symbol& rhs) { return lhs.name() == rhs.name(); }
inline bool operator< (const Symbol& lhs, const Symbol& rhs) { return lhs.name() < rhs.name(); }
MCLISP_DEF_DERIVED_OPS(const Symbol&);

// These need to be forward declared so they can be used by the corresponding
// operators for the Cons type (since car and cdr are Sexps).
inline bool operator==(const Sexp& lhs, const Sexp& rhs);
inline bool operator<(const Sexp& lhs, const Sexp& rhs);

inline bool operator==(const Cons& lhs, const Cons& rhs) {return lhs.car() == rhs.car() && lhs.cdr() == rhs.cdr();}
inline bool operator< (const Cons& lhs, const Cons& rhs) {return lhs.car() < rhs.car() || (lhs.car() == rhs.car() && lhs.cdr() < rhs.cdr());}
MCLISP_DEF_DERIVED_OPS(const Cons&);

// Sexp operators attempt to cast both args to the same concrete subtype and
// defer to the operator overloaded on that type.
#define MCLISP_DEF_SEXP_LOGICAL_OP(op) \
inline bool op(const Sexp& lhs, const Sexp& rhs) { \
  try { \
    const Symbol& sym_lhs = dynamic_cast<const Symbol&>(lhs); \
    const Symbol& sym_rhs = dynamic_cast<const Symbol&>(rhs); \
    return op(sym_lhs, sym_rhs); \
  } catch (std::bad_cast) {} \
  try { \
    const Cons& cons_lhs = dynamic_cast<const Cons&>(lhs); \
    const Cons& cons_rhs = dynamic_cast<const Cons&>(rhs); \
    return op(cons_lhs, cons_rhs); \
  } catch (std::bad_cast) {} \
  return false; \
}

MCLISP_DEF_SEXP_LOGICAL_OP(operator==);
MCLISP_DEF_SEXP_LOGICAL_OP(operator<);
MCLISP_DEF_DERIVED_OPS(const Sexp&);

} // namespace mclisp
#endif // OBJECTS_H_
