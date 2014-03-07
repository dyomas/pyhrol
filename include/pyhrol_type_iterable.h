/*
 *   Copyright (c) 2013 Pyhrol, pyhrol@rambler.ru
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

// $Date: 2013-12-10 16:05:29 +0400 (Tue, 10 Dec 2013) $
// $Revision: 862 $

#ifndef __pyhrol_type_iterable_h__
#define __pyhrol_type_iterable_h__


#include "pyhrol_type_base.h"
#include "pyhrol_ptr.h"
#include "pyhrol_exception_handler.h"

namespace pyhrol
{

template <class T, class I> class TypeIterable: virtual public TypeBase<T>
{
public:
  TypeIterable();

protected:
  virtual void iter(const Ptr<I> &, const Ptr<const T> &) const;
  virtual bool next(const Ptr<I> &, const Ptr<T> &) const;

private:
  void m_init();

  struct call_iter: exceptionHandler
  {
    PyObject *self;
    const char * const signature;
    PyObject *retval;
    virtual void call();
    call_iter(PyObject *, const char *_signature);
  };

  struct call_next: exceptionHandler
  {
    PyObject *self;
    const char * const signature;
    PyObject *retval;
    virtual void call();
    call_next(PyObject *, const char *_signature);
  };

  static PyObject *mediator_tp_iter(PyObject *);
  static PyObject *mediator_tp_iternext(PyObject *);
};

}; //namespace pyhrol

#endif //__pyhrol_type_iterable_h__
