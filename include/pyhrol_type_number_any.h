/*
 *   Copyright (c) 2013, 2014, Pyhrol, pyhrol@rambler.ru
 *   GEO: N55.703431,E37.623324 .. N48.742359,E44.536997
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   4. Neither the name of the Pyhrol nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 *   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 *   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *   ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *   SUCH DAMAGE.
 */

// $Date: 2014-04-04 16:35:38 +0400 (Пт., 04 апр. 2014) $
// $Revision: 906 $

#ifndef __pyhrol_type_number_any_h__
#define __pyhrol_type_number_any_h__

#include "pyhrol_type_number_common.h"
#include "pyhrol_ptr.h"
#include "pyhrol_exception_handler.h"

namespace pyhrol
{

template <class T> class TypeNumberAny: public TypeNumberCommon<T>
{
public:
  TypeNumberAny();

protected:
  // x.__add__(y) <==> x+y
  // x.__radd__(y) <==> y+x
  virtual void add(const Ptr<T> &, Tuples &) const;
  // x.__sub__(y) <==> x-y
  // x.__rsub__(y) <==> y-x
  virtual void subtract(const Ptr<T> &, Tuples &) const;
  // x.__mul__(y) <==> x*y
  // x.__rmul__(y) <==> y*x
  virtual void multiply(const Ptr<T> &, Tuples &) const;
  // x.__div__(y) <==> x/y
  // x.__rdiv__(y) <==> y/x
  virtual void divide(const Ptr<T> &, Tuples &) const;
  // x.__mod__(y) <==> x%y
  // x.__rmod__(y) <==> y%x
  virtual void remainder(const Ptr<T> &, Tuples &) const;
  // x.__divmod__(y) <==> divmod(x, y)
  // x.__rdivmod__(y) <==> divmod(y, x)
  virtual void divmod(const Ptr<T> &, const Ptr<T> &, Tuples &) const;
  // x.__pow__(y[, z]) <==> pow(x, y[, z])
  // y.__rpow__(x[, z]) <==> pow(x, y[, z])
  virtual void power(const Ptr<T> &, Tuples &) const;
  // x.__lshift__(y) <==> x<<y
  // x.__rlshift__(y) <==> y<<x
  virtual void lshift(const Ptr<T> &, Tuples &) const;
  // x.__rshift__(y) <==> x>>y
  // x.__rrshift__(y) <==> y>>x
  virtual void rshift(const Ptr<T> &, Tuples &) const;
  // x.__and__(y) <==> x&y
  // x.__rand__(y) <==> y&x
  virtual void nb_and(const Ptr<T> &, Tuples &) const;
  // x.__xor__(y) <==> x^y
  // x.__rxor__(y) <==> y^x
  virtual void nb_xor(const Ptr<T> &, Tuples &) const;
  // x.__or__(y) <==> x|y
  // x.__ror__(y) <==> y|x
  virtual void nb_or(const Ptr<T> &, Tuples &) const;
  // x.__floordiv__(y) <==> x//y
  // x.__rfloordiv__(y) <==> y//x
  virtual void floor_divide(const Ptr<T> &, Tuples &) const;
  // x.__rtruediv__(y) <==> y/x
  // x.__truediv__(y) <==> x/y
  virtual void true_divide(const Ptr<T> &, Tuples &) const;

  // x.__iadd__(y) <==> x+=y
  virtual void inplace_add(const Ptr<T> &, Tuples &) const;
  // x.__isub__(y) <==> x-=y
  virtual void inplace_subtract(const Ptr<T> &, Tuples &) const;
  // x.__imul__(y) <==> x*=y
  virtual void inplace_multiply(const Ptr<T> &, Tuples &) const;
  // x.__idiv__(y) <==> x/=y
  virtual void inplace_divide(const Ptr<T> &, Tuples &) const;
  // x.__imod__(y) <==> x%=y
  virtual void inplace_remainder(const Ptr<T> &, Tuples &) const;
  // x.__ipow__(y) <==> x**=y
  virtual void inplace_power(const Ptr<T> &, Tuples &) const;
  // x.__ilshift__(y) <==> x<<=y
  virtual void inplace_lshift(const Ptr<T> &, Tuples &) const;
  // x.__irshift__(y) <==> x>>=y
  virtual void inplace_rshift(const Ptr<T> &, Tuples &) const;
  // x.__iand__(y) <==> x&=y
  virtual void inplace_and(const Ptr<T> &, Tuples &) const;
  // x.__ixor__(y) <==> x^=y
  virtual void inplace_xor(const Ptr<T> &, Tuples &) const;
  // x.__ior__(y) <==> x|=y
  virtual void inplace_or(const Ptr<T> &, Tuples &) const;
  // x.__ifloordiv__(y) <==> x//y
  virtual void inplace_floor_divide(const Ptr<T> &, Tuples &) const;
  // x.__itruediv__(y) <==> x/y
  virtual void inplace_true_divide(const Ptr<T> &, Tuples &) const;

private:
  template <void (TypeNumberAny<T>::*M)(const Ptr<T> &, Tuples &) const> struct call_std: exceptionHandler
  {
    const bool is_probe;
    Tuples &args;
    PyObject *retval;
    const char * const signature;
    virtual void call();
    call_std(Tuples &, const char *, const bool);
  };

  template <void (TypeNumberAny<T>::*M)(const Ptr<T> &, Tuples &) const> struct call_inplace: exceptionHandler
  {
    Tuples &args;
    PyObject *self;
    const char * const signature;
    virtual void call();
    call_inplace(Tuples &, PyObject *, const char *);
  };

  struct call_divmod: exceptionHandler
  {
    const bool is_probe;
    Tuples &args;
    PyObject *retval;
    const char * const signature;
    virtual void call();
    call_divmod(Tuples &, const char *, const bool);
  };

  void m_init();

  static PyObject *mediator_nb_add(PyObject *, PyObject *);
  static PyObject *mediator_nb_subtract(PyObject *, PyObject *);
  static PyObject *mediator_nb_multiply(PyObject *, PyObject *);
  static PyObject *mediator_nb_divide(PyObject *, PyObject *);
  static PyObject *mediator_nb_remainder(PyObject *, PyObject *);
  static PyObject *mediator_nb_divmod(PyObject *, PyObject *);
  static PyObject *mediator_nb_power(PyObject *, PyObject *, PyObject *);
  static PyObject *mediator_nb_lshift(PyObject *, PyObject *);
  static PyObject *mediator_nb_rshift(PyObject *, PyObject *);
  static PyObject *mediator_nb_and(PyObject *, PyObject *);
  static PyObject *mediator_nb_xor(PyObject *, PyObject *);
  static PyObject *mediator_nb_or(PyObject *, PyObject *);
  static PyObject *mediator_nb_floor_divide(PyObject *, PyObject *);
  static PyObject *mediator_nb_true_divide(PyObject *, PyObject *);

  static PyObject *mediator_nb_inplace_add(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_subtract(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_multiply(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_divide(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_remainder(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_power(PyObject *, PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_lshift(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_rshift(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_and(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_xor(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_or(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_floor_divide(PyObject *, PyObject *);
  static PyObject *mediator_nb_inplace_true_divide(PyObject *, PyObject *);
};

} //namespace pyhrol

#endif //__pyhrol_type_number_any_h__
