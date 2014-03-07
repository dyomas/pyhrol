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

// $Date: 2013-12-04 02:49:13 +0400 (Wed, 04 Dec 2013) $
// $Revision: 854 $

#ifndef __pyhrol_type_special_h__
#define __pyhrol_type_special_h__

#include "pyhrol_type_base.h"
#include "pyhrol_exception_handler.h"


namespace pyhrol
{

template <class T> class TypeSpecial: public virtual TypeBase<T>
{
public:
  TypeSpecial();

protected:
  template <void (*F)(Tuples &)> void m_add_static_method(const char */*name*/, const char */*help*/);
  template <void (*F)(Tuples &)> void m_add_static_method_with_keywords(const char */*name*/, const char */*help*/);
  template <void (*F)(Tuples &)> void m_add_static_method_no_args(const char */*name*/, const char */*help*/);

  template <void (*F)(const PyTypeObject &, Tuples &)> void m_add_class_method(const char */*name*/, const char */*help*/);
  template <void (*F)(const PyTypeObject &, Tuples &)> void m_add_class_method_with_keywords(const char */*name*/, const char */*help*/);
  template <void (*F)(const PyTypeObject &, Tuples &)> void m_add_class_method_no_args(const char */*name*/, const char */*help*/);

private:
  template <void (*F)(Tuples &)> struct call_static: exceptionHandler
  {
    Tuples &args;
    const char * const signature;
    virtual void call();
    call_static(Tuples &, const char *);
  };

  template <void (*F)(const PyTypeObject &, Tuples &)> struct call_class: exceptionHandler
  {
    Tuples &args;
    const char * const signature;
    const PyObject *ptype;
    virtual void call();
    call_class(Tuples &, const char *, const PyObject *);
  };

  template <void (*F)(Tuples &)> static PyObject *m_static_method(PyObject */*self*/);
  template <void (*F)(Tuples &)> static PyObject *m_static_method2(PyObject */*self*/, PyObject */*args*/);
  template <void (*F)(Tuples &)> static PyObject *m_static_method3(PyObject */*self*/, PyObject */*args*/, PyObject */*kwds*/);

  template <void (*F)(const PyTypeObject &, Tuples &)> static PyObject *m_class_method(PyObject */*self*/);
  template <void (*F)(const PyTypeObject &, Tuples &)> static PyObject *m_class_method2(PyObject */*self*/, PyObject */*args*/);
  template <void (*F)(const PyTypeObject &, Tuples &)> static PyObject *m_class_method3(PyObject */*self*/, PyObject */*args*/, PyObject */*kwds*/);
};

}; //namespace pyhrol

#endif //__pyhrol_type_special_h__
