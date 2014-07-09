#ifndef ERROR_H_
#define ERROR_H_

#include <stdexcept>

namespace mclisp
{

class Error: public std::logic_error
{
  private:
    static const std::string err_prefix_;

  public:
    explicit Error(const std::string& what_arg):
      logic_error(err_prefix_ + what_arg) {};

    explicit Error(const char* what_arg):
      logic_error(err_prefix_ + what_arg) {};
};
} // mclisp

#endif // ERROR_H_
