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

// $Date: 2015-04-07 01:20:06 +0300 (Вт., 07 апр. 2015) $
// $Revision: 1029 $

#ifndef __pyhrol_type_pointer_h__
#define __pyhrol_type_pointer_h__

#include "pyhrol_type_base.h"
#include "pyhrol_ptr.h"
#include "pyhrol_exception_handler.h"

namespace pyhrol
{

template <typename T> struct pointer
{
private:
  T *m_data;

public:
  typedef T data_t;

  pointer();
  pointer(T *data);
  pointer(const pointer &);
  pointer &operator =(T *data);
  void reset (T *data);
  operator T *() const;
  T *operator->() const;
  operator T *();
  T *operator->();
};

template <class T, class I = T> class TypePointer: public virtual TypeBase<pointer<T> >
{
protected:
  virtual const void *address(const pointer<T> &) const;
  virtual void print(std::ostream &, const pointer<T> &) const;
  virtual const std::string repr(const pointer<T> &) const;
  virtual void str(std::ostream &, const pointer<T> &) const;

public:
  virtual PyObject *convert(T *obj);

protected:
  template <void (I::*F)(const Ptr<T> &, Tuples &) const> void m_add_method(const char */*name*/, const char */*help*/);
  template <void (I::*F)(const Ptr<T> &, Tuples &) const> void m_add_method_with_keywords(const char */*name*/, const char */*help*/);
  template <void (I::*F)(const Ptr<T> &, Tuples &) const> void m_add_method_no_args(const char */*name*/, const char */*help*/);

  template <void (I::*F)(const Ptr<const T> &, Tuples &) const> void m_add_method_const(const char */*name*/, const char */*help*/);
  template <void (I::*F)(const Ptr<const T> &, Tuples &) const> void m_add_method_with_keywords_const(const char */*name*/, const char */*help*/);
  template <void (I::*F)(const Ptr<const T> &, Tuples &) const> void m_add_method_no_args_const(const char */*name*/, const char */*help*/);

  template <void (I::*G)(const Ptr<const T> &, Tuples &) const> void m_add_getter(const char */*name*/, const char */*doc*/);
  template <void (I::*S)(const Ptr<T> &, Tuples &) const> void m_add_setter(const char */*name*/, const char */*doc*/);
  template <void (I::*G)(const Ptr<const T> &, Tuples &) const, void (I::*S)(const Ptr<T> &, Tuples &) const> void m_add_getseter(const char */*name*/, const char */*doc*/);


  template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> void m_add_method(const char */*name*/, const char */*help*/);
  template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> void m_add_method_with_keywords(const char */*name*/, const char */*help*/);
  template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> void m_add_method_no_args(const char */*name*/, const char */*help*/);

  template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> void m_add_method_const(const char */*name*/, const char */*help*/);
  template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> void m_add_method_with_keywords_const(const char */*name*/, const char */*help*/);
  template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> void m_add_method_no_args_const(const char */*name*/, const char */*help*/);

  template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const> void m_add_getter(const char */*name*/, const char */*doc*/);
  template <typename O, void (O::*S)(const Ptr<T> &, Tuples &) const> void m_add_setter(const char */*name*/, const char */*doc*/);
  template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const, void (O::*S)(const Ptr<T> &, Tuples &) const> void m_add_getseter(const char */*name*/, const char */*doc*/);

private:
  template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> struct call_ptr: exceptionHandler
  {
    Tuples &args;
    PyObject *self;
    const char * const signature;
    virtual void call();
    call_ptr(Tuples &, PyObject *, const char *);
  };

  template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> struct call_ptr_const: exceptionHandler
  {
    Tuples &args;
    PyObject *self;
    const char * const signature;
    virtual void call();
    call_ptr_const(Tuples &, PyObject *, const char *);
  };


  template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> static PyObject *m_method(PyObject */*self*/);
  template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> static PyObject *m_method2(PyObject */*self*/, PyObject */*args*/);
  template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> static PyObject *m_method3(PyObject */*self*/, PyObject */*args*/, PyObject */*kwds*/);

  template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> static PyObject *m_method_const(PyObject */*self*/);
  template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> static PyObject *m_method2_const(PyObject */*self*/, PyObject */*args*/);
  template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> static PyObject *m_method3_const(PyObject */*self*/, PyObject */*args*/, PyObject */*kwds*/);

  template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const> static PyObject *m_getter(PyObject */*self*/, void */*closure*/);
  template <typename O, void (O::*S)(const Ptr<T> &, Tuples &) const> static int m_setter(PyObject */*self*/, PyObject */*value*/, void */*closure*/);
  template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const, void (O::*S)(const Ptr<T> &, Tuples &) const> static PyObject *m_getter_paired(PyObject */*self*/, void */*closure*/);
  template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const, void (O::*S)(const Ptr<T> &, Tuples &) const> static int m_setter_paired(PyObject */*self*/, PyObject */*value*/, void */*closure*/);
};

} //namespace pyhrol

#endif //__pyhrol_type_pointer_h__
