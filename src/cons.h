#ifndef MCLISP_CONS_H_
#define MCLISP_CONS_H_

namespace mclisp
{

class ConsCell
{
public:
    ConsCell(): car_(nullptr), cdr_(nullptr) {}
    ConsCell(ConsCell* car, ConsCell* cdr): car_(car), cdr_(cdr) {}

    ConsCell* car_;
    ConsCell* cdr_;
};

} // namespace mclisp

#endif // MCLISP_CONS_H_
