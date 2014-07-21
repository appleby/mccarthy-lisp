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

class ArgumentError: public Error
{
  public:
    explicit ArgumentError(const char* file, const char* func, int line,
                           const std::string& what_arg):
      Error(file, func, line, std::string(func) + ": " + what_arg)
    {};

    explicit ArgumentError(const char* file, const char* func, int line,
                           const char* what_arg):
      ArgumentError(file, func, line, std::string(what_arg))
    {};

  protected:
    virtual const char* prefix() const noexcept
    { return "Invalid Argument:"; }
};

class ParityError: public Error
{
  public:
    explicit ParityError(const char* file, const char* func, int line,
                         ConsCell *formals, ConsCell *actuals):
      Error(file, func, line, ConstructWhat(formals, actuals))
    {};

  protected:
    const std::string ConstructWhat(ConsCell *formals, ConsCell *actuals);

    virtual const char* prefix() const noexcept
    { return "ParityError:"; }
};

class TypeError: public Error
{
  public:
    explicit TypeError(const char* file, const char* func, int line,
                       const ConsCell* object, const std::string& should_satisfy):
      Error(file, func, line, ""), object_str_(ToString(object)),
      should_satisfy_(should_satisfy)
    {};

    virtual const char* what() const noexcept;

  protected:
    virtual const char* prefix() const noexcept
    { return "TypeError:"; }

  private:
    const std::string object_str_;
    const std::string should_satisfy_;
};

#define THROW(etype, args...) \
  throw etype(__FILE__, __FUNCTION__, __LINE__, args)

#define ERROR(msg) THROW(Error, (msg))
#define ARGUMENT_ERROR(msg) THROW(ArgumentError, (msg))
#define PARITY_ERROR(formals, actuals) THROW(ParityError, formals, actuals)
#define TYPE_ERROR(obj, pred) THROW(TypeError, obj, pred)

// Assumes arguments do not expand to expressions with side-effects.
#define PARITY_CHECK(formals, actuals) \
  if (Length(formals) != Length(actuals)) \
      PARITY_ERROR(formals, actuals)

// Assumes obj does not expand to an expression with side-effects.
#define TYPECHECK(obj, pred) \
  if (!pred(obj)) TYPE_ERROR(obj, #pred)

} // mclisp

#endif // ERROR_H_
