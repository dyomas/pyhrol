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

// $Date: 2014-04-04 16:35:38 +0400 (Fri, 04 Apr 2014) $
// $Revision: 906 $

#ifndef __pyhrol_type_special_hpp__
#define __pyhrol_type_special_hpp__

#include <stddef.h>
#include <tr1/memory>

#include "pyhrol_type_special.h"
#include "pyhrol_type_base.h"
#include "pyhrol_trace.h"

namespace pyhrol
{

template <typename T> TypeSpecial<T>::TypeSpecial()
{
}

template <typename T> template <void (*F)(Tuples &)> void TypeSpecial<T>::m_add_static_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_static_method2<F>, help, METH_STATIC);
}

template <typename T> template <void (*F)(Tuples &)> void TypeSpecial<T>::m_add_static_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_static_method3<F>, help, METH_STATIC);
}

template <typename T> template <void (*F)(Tuples &)> void TypeSpecial<T>::m_add_static_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_static_method<F>, help, METH_STATIC);
}

template <typename T> template <void (*F)(const PyTypeObject &, Tuples &)> void TypeSpecial<T>::m_add_class_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_class_method2<F>, help, METH_CLASS);
}

template <typename T> template <void (*F)(const PyTypeObject &, Tuples &)> void TypeSpecial<T>::m_add_class_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_class_method3<F>, help, METH_CLASS);
}

template <typename T> template <void (*F)(const PyTypeObject &, Tuples &)> void TypeSpecial<T>::m_add_class_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_class_method<F>, help, METH_CLASS);
}

template <typename T> template <void (*F)(Tuples &)> void TypeSpecial<T>::call_static<F>::call()
{
  if (signature)
  {
    PYHROL_TRACE(tpMediator, NULL, signature)
  }
  (*F)(args);
}

template <typename T> template <void (*F)(Tuples &)> TypeSpecial<T>::call_static<F>::call_static(Tuples &_args, const char *_signature)
  : args(_args)
  , signature(_signature)
{
}

template <typename T> template <void (*F)(const PyTypeObject &, Tuples &)> void TypeSpecial<T>::call_class<F>::call()
{
  if (signature)
  {
    PYHROL_TRACE(tpMediator, NULL, signature)
  }
  if (ptype)
  {
    (*F)(*reinterpret_cast<const PyTypeObject *>(ptype), args);
  }
  else
  {
    (*F)(*Py_None->ob_type, args);
  }
}

template <typename T> template <void (*F)(const PyTypeObject &, Tuples &)> TypeSpecial<T>::call_class<F>::call_class(Tuples &_args, const char *_signature, const PyObject *_ptype)
  : args(_args)
  , signature(_signature)
  , ptype(_ptype)
{
}

template <typename T> template <void (*F)(Tuples &)> PyObject *TypeSpecial<T>::m_static_method(PyObject *self)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_static_method_no_arg, reinterpret_cast<size_t>(m_static_method<F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_static<F> c(*tuples, self ? NULL : __PRETTY_FUNCTION__);
  return tuples->ubiquitous_caller(c, NULL, NULL, self);
}

template <typename T> template <void (*F)(Tuples &)> PyObject *TypeSpecial<T>::m_static_method2(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_static_method, reinterpret_cast<size_t>(m_static_method2<F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_static<F> c(*tuples, self ? NULL : __PRETTY_FUNCTION__);
  return tuples->ubiquitous_caller(c, args, NULL, self);
}

template <typename T> template <void (*F)(Tuples &)> PyObject *TypeSpecial<T>::m_static_method3(PyObject *self, PyObject *args, PyObject *kwds)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_static_method_kw, reinterpret_cast<size_t>(m_static_method3<F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_static<F> c(*tuples, self ? NULL : __PRETTY_FUNCTION__);
  return tuples->ubiquitous_caller(c, args, kwds, self);
}

template <typename T> template <void (*F)(const PyTypeObject &, Tuples &)> PyObject *TypeSpecial<T>::m_class_method(PyObject *self)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_method_no_arg, reinterpret_cast<size_t>(m_class_method<F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_class<F> c(*tuples, self ? __PRETTY_FUNCTION__ : NULL, self);
  return tuples->ubiquitous_caller(c, NULL, NULL, !self);
}

template <typename T> template <void (*F)(const PyTypeObject &, Tuples &)> PyObject *TypeSpecial<T>::m_class_method2(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_method, reinterpret_cast<size_t>(m_class_method2<F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_class<F> c(*tuples, self ? __PRETTY_FUNCTION__ : NULL, self);
  return tuples->ubiquitous_caller(c, args, NULL, !self);
}

template <typename T> template <void (*F)(const PyTypeObject &, Tuples &)> PyObject *TypeSpecial<T>::m_class_method3(PyObject *self, PyObject *args, PyObject *kwds)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_method_kw, reinterpret_cast<size_t>(m_class_method3<F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_class<F> c(*tuples, self ? __PRETTY_FUNCTION__ : NULL, self);
  return tuples->ubiquitous_caller(c, args, kwds, !self);
}

} //namespace pyhrol

#endif //__pyhrol_type_special_hpp__
