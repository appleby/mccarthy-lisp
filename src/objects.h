#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <string>
#include <iostream>

namespace mclisp
{

class Sexp
{
  public:
    virtual bool operator==(const Sexp* other) const = 0;
    virtual operator bool() const;
};

class Atom : public Sexp {};
class Symbol : public Atom
{
  private:
    std::string name_;

  public:
    explicit Symbol(const std::string& name) : name_(name) {};
    explicit Symbol(const char* name) : name_(name) {};

    bool operator==(const Symbol& other) const
    { 
      std::cout << "========" << std::endl;
      return name_ == other.name_; 
    }
    virtual bool operator==(const Sexp* other) const
    { 
      std::cout << "=======*" << std::endl;
      const Symbol* so = dynamic_cast<const Symbol*>(other);
      return so && *this == *so;
    }

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

    const Sexp* car() const { return car_; }
    const Sexp* cdr() const { return cdr_; }

    bool operator==(const Cons& other) const
    {
      return car_ == other.car_ && cdr_ == other.cdr_;
    }
    virtual bool operator==(const Sexp* other) const
    { 
      const Cons* co = dynamic_cast<const Cons*>(other);
      return co && *this == *co;
    }
};

} // namespace mclisp
#endif // OBJECTS_H_
