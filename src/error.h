#ifndef ERROR_H_
#define ERROR_H_

#include <stdexcept>
#include <string>

namespace mclisp
{

class Error: public std::logic_error
{
  public:
    explicit Error(const char* file, const char* func, int line,
                   const std::string& what_arg):
      logic_error(what_arg), file_(file), func_(func), line_(line) {};

    explicit Error(const char* file, const char* func, int line,
                   const char* what_arg):
      Error(file, func, line, std::string(what_arg)) {};

    virtual const char* what() const noexcept;

  protected:
    virtual const char* prefix() const noexcept
    { return "Error:"; }

    const char* file_;
    const char* func_;
    int line_;
};

#define THROW(etype, args...) \
  throw etype(__FILE__, __FUNCTION__, __LINE__, args)

#define ERROR(msg) THROW(Error, (msg))

} // mclisp

#endif // ERROR_H_
