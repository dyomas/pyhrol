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

#ifndef __pyhrol_type_sequence_h__
#define __pyhrol_type_sequence_h__

#include "pyhrol_type_base.h"
#include "pyhrol_ptr.h"
#include "pyhrol_exception_handler.h"

namespace pyhrol
{

void check_range(size_t limit, Py_ssize_t begin, Py_ssize_t end);

template <class T> class TypeSequence: virtual public TypeBase<T>
{
public:
  TypeSequence();

protected:
  virtual Py_ssize_t length(const Ptr<const T> &) const;
  virtual void concat(const Ptr<T> &, const Ptr<const T> &) const;
  virtual void concat(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const;
  virtual void repeat(const Ptr<T> &, Py_ssize_t) const;
  virtual void repeat(const Ptr<T> &, const Ptr<const T> &, Py_ssize_t) const;
  virtual void get(const Ptr<T> &, const Ptr<const T> &, Py_ssize_t, Py_ssize_t) const;
  virtual void assign(const Ptr<T> &, const Ptr<const T> &, Py_ssize_t, Py_ssize_t) const;
  virtual void del(const Ptr<T> &, Py_ssize_t, Py_ssize_t) const;
  virtual bool contains(const Ptr<const T> &, const Ptr<const T> &) const;

  virtual void concat(const Ptr<T> &, Tuples &) const;
  virtual void concat(const Ptr<T> &, const Ptr<const T> &, Tuples &) const;
  virtual void assign(const Ptr<T> &, Tuples &, Py_ssize_t, Py_ssize_t) const;
  virtual void contains(bool &, const Ptr<const T> &, Tuples &) const;

  PySequenceMethods m_methods;

private:
  struct call_del: exceptionHandler
  {
    PyObject *self;
    Py_ssize_t begin, end;
    const char * const signature;
    virtual void call();
    call_del(PyObject *, Py_ssize_t _begin, Py_ssize_t _end, const char *);
  };

  struct call_length: exceptionHandler
  {
    PyObject *self;
    const char * const signature;
    Py_ssize_t retval;
    virtual void call();
    call_length(PyObject *, const char *);
  };

  struct call_repeat: exceptionHandler
  {
    PyObject *self;
    const Py_ssize_t cnt;
    const char * const signature;
    PyObject *retval;
    virtual void call();
    call_repeat(PyObject *, Py_ssize_t _cnt, const char *);
  };

  struct call_repeat_inplace: exceptionHandler
  {
    PyObject *self;
    const Py_ssize_t cnt;
    const char * const signature;
    virtual void call();
    call_repeat_inplace(PyObject *, Py_ssize_t _cnt, const char *);
  };

  struct call_get: exceptionHandler
  {
    PyObject *self;
    const Py_ssize_t begin, end;
    const char * const signature;
    PyObject *retval;
    virtual void call();
    call_get(PyObject *, Py_ssize_t _begin, Py_ssize_t _end, const char *);
  };

  struct call_concat: exceptionHandler
  {
    Tuples &args;
    PyObject *self;
    const char * const signature;
    PyObject *retval;
    virtual void call();
    call_concat(Tuples &, PyObject *, const char *);
  };

  struct call_concat_inplace: exceptionHandler
  {
    Tuples &args;
    PyObject *self;
    const char * const signature;
    virtual void call();
    call_concat_inplace(Tuples &, PyObject *, const char *);
  };

  struct call_assign: exceptionHandler
  {
    Tuples &args;
    PyObject *self;
    const Py_ssize_t begin, end;
    const char * const signature;
    virtual void call();
    call_assign(Tuples &args, PyObject *, const Py_ssize_t begin, const Py_ssize_t end, const char *);
  };

  struct call_contains: exceptionHandler
  {
    Tuples &args;
    PyObject *self;
    bool retval;
    const char * const signature;
    virtual void call();
    call_contains(Tuples &args, PyObject *, const char *);
  };

  void m_init();

  // x.__len__() <==> len(x)
  static Py_ssize_t mediator_sq_length(PyObject *);
  // x.__add__(y) <==> x+y
  static PyObject *mediator_sq_concat(PyObject *, PyObject *);
  // x.__mul__(n) <==> x*n
  // x.__rmul__(n) <==> n*x
  static PyObject *mediator_sq_repeat(PyObject *, Py_ssize_t);
  // x.__getitem__(y) <==> x[y]
  static PyObject *mediator_sq_item(PyObject *, Py_ssize_t);
  // x.__getslice__(i, j) <==> x[i:j]
  static PyObject *mediator_sq_slice(PyObject *, Py_ssize_t, Py_ssize_t);
  // x.__setitem__(i, y) <==> x[i]=y
  // x.__delitem__(y) <==> del x[y]
  static int mediator_sq_ass_item(PyObject *, Py_ssize_t, PyObject *);
  // x.__delslice__(i, j) <==> del x[i:j]
  // x.__setslice__(i, j, y) <==> x[i:j]=y
  static int mediator_sq_ass_slice(PyObject *, Py_ssize_t, Py_ssize_t, PyObject *);
  // x.__contains__(y) <==> y in x
  static int mediator_sq_contains(PyObject *, PyObject *);
  // x.__iadd__(y) <==> x+=y
  static PyObject *mediator_sq_inplace_concat(PyObject *, PyObject *);
  // x.__imul__(y) <==> x*=y
  static PyObject *mediator_sq_inplace_repeat(PyObject *, Py_ssize_t);
};

} //namespace pyhrol

#endif //__pyhrol_type_sequence_h__
