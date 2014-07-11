#ifndef ERROR_H_
#define ERROR_H_

#include <stdexcept>
#include <string>

#include "cons.h"

namespace mclisp
{

class Error: public std::logic_error
{
  public:
    explicit Error(const char* file, const char* func, int line,
                   const std::string& what_arg):
      logic_error(what_arg), file_(file), func_(func), line_(line)
    {};

    explicit Error(const char* file, const char* func, int line,
                   const char* what_arg):
      Error(file, func, line, std::string(what_arg))
    {};

    virtual const char* what() const noexcept;

  protected:
    virtual const char* prefix() const noexcept
    { return "Error:"; }

    const char* file_;
    const char* func_;
    int line_;
};

class TypeError: public Error
{
  public:
    explicit TypeError(const char* file, const char* func, int line,
                       const ConsCell* object, const std::string& should_satisfy):
      Error(file, func, line, ""), object_(object), object_str_(ToString(object)),
      should_satisfy_(should_satisfy)
    {};

    virtual const char* what() const noexcept;

  protected:
    virtual const char* prefix() const noexcept
    { return "TypeError:"; }

  private:
    const ConsCell* object_;
    const std::string object_str_;
    const std::string should_satisfy_;
};

#define THROW(etype, args...) \
  throw etype(__FILE__, __FUNCTION__, __LINE__, args)

#define ERROR(msg) THROW(Error, (msg))
#define TYPE_ERROR(obj, pred) THROW(TypeError, obj, pred)

// Assumes obj does not expand to an expression with side-effects.
#define TYPECHECK(obj, pred) \
  if (!pred(obj)) TYPE_ERROR(obj, #pred)

} // mclisp

#endif // ERROR_H_
