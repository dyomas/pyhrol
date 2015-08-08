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

// $Date: 2015-06-06 00:53:03 +0300 (Сб., 06 июня 2015) $
// $Revision: 1036 $

#ifndef __pyhrol_type_smart_hpp__
#define __pyhrol_type_smart_hpp__

#include "pyhrol_type_smart.h"
#include "pyhrol_common.h"
#include "pyhrol_type.hpp"

namespace pyhrol
{

namespace __smart_internal
{

template <typename E, typename O, void (O::*G)(const Ptr<const E> &, Tuples &) const, void (O::*S)(const Ptr<E> &, Tuples &) const> TuplesData &data(const size_t address)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_getsetter, address));
  return *data;
}

} //namespace __smart_internal


template <typename T, class E, E* (*C)(T *), class I> template <void (I::*F)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method2<I, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <void (I::*F)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method3<I, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <void (I::*F)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method<I, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <void (I::*F)(const Ptr<const E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method2<I, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <void (I::*F)(const Ptr<const E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method3<I, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <void (I::*F)(const Ptr<const E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method<I, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <void (I::*G)(const Ptr<const E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_getter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, m_getter<I, G>, NULL, doc);
}

template <typename T, class E, E* (*C)(T *), class I> template <void (I::*S)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_setter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, NULL, m_setter<I, S>, doc);
}

template <typename T, class E, E* (*C)(T *), class I> template <void (I::*G)(const Ptr<const E> &, Tuples &) const, void (I::*S)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_getseter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, m_getter_paired<I, G, S>, m_setter_paired<I, G, S>, doc);
}


template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method2<O, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method3<O, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method<O, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<const E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method2<O, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<const E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method3<O, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<const E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method<O, F>, help);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*G)(const Ptr<const E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_getter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, m_getter<O, G>, NULL, doc);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*S)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_setter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, NULL, m_setter<O, S>, doc);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*G)(const Ptr<const E> &, Tuples &) const, void (O::*S)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::m_add_getseter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, m_getter_paired<O, G, S>, m_setter_paired<O, G, S>, doc);
}


template <class T, typename E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<E> &, Tuples &) const> void TypeSmart<T, E, C, I>::call_ptr<O, F>::call()
{
  E *endosome = NULL;
  if (self && !(endosome = (*C)(&(reinterpret_cast<typename TypeBase<T>::T_struct *>(self))->endosome)))
  {
    throw NullPointer();
  }
  if (endosome)
  {
    PYHROL_TRACE(tpMediator, endosome, signature);
  }
  (dynamic_cast<O &>(TypeBase<T>::m_get()).*F)(Ptr<E>(endosome, self), args);
}

template <class T, typename E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<E> &, Tuples &) const> TypeSmart<T, E, C, I>::call_ptr<O, F>::call_ptr(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , signature(_signature)
{
}


template <class T, typename E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<const E> &, Tuples &) const> void TypeSmart<T, E, C, I>::call_ptr_const<O, F>::call()
{
  E *endosome = NULL;
  if (self && !(endosome = (*C)(&(reinterpret_cast<typename TypeBase<T>::T_struct *>(self))->endosome)))
  {
    throw NullPointer();
  }
  if (endosome)
  {
    PYHROL_TRACE(tpMediator, endosome, signature);
  }
  (dynamic_cast<const O &>(TypeBase<T>::m_get()).*F)(Ptr<const E>(endosome, self), args);
}

template <class T, typename E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<const E> &, Tuples &) const> TypeSmart<T, E, C, I>::call_ptr_const<O, F>::call_ptr_const(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , signature(_signature)
{
}


template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<E> &, Tuples &) const> PyObject *TypeSmart<T, E, C, I>::m_method(PyObject *self)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_method_no_arg, reinterpret_cast<size_t>(m_method<O, F>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  call_ptr<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, NULL, NULL, !self);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<E> &, Tuples &) const> PyObject *TypeSmart<T, E, C, I>::m_method2(PyObject *self, PyObject *args)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_method, reinterpret_cast<size_t>(m_method2<O, F>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  call_ptr<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, args, NULL, !self);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<E> &, Tuples &) const> PyObject *TypeSmart<T, E, C, I>::m_method3(PyObject *self, PyObject *args, PyObject *kwds)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_method_kw, reinterpret_cast<size_t>(m_method3<O, F>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  call_ptr<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, args, kwds, !self);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<const E> &, Tuples &) const> PyObject *TypeSmart<T, E, C, I>::m_method(PyObject *self)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_method_no_arg_const, reinterpret_cast<size_t>(m_method<O, F>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  call_ptr_const<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, NULL, NULL, !self);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<const E> &, Tuples &) const> PyObject *TypeSmart<T, E, C, I>::m_method2(PyObject *self, PyObject *args)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_method_const, reinterpret_cast<size_t>(m_method2<O, F>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  call_ptr_const<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, args, NULL, !self);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*F)(const Ptr<const E> &, Tuples &) const> PyObject *TypeSmart<T, E, C, I>::m_method3(PyObject *self, PyObject *args, PyObject *kwds)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_method_kw_const, reinterpret_cast<size_t>(m_method3<O, F>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  call_ptr_const<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, args, kwds, !self);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*G)(const Ptr<const E> &, Tuples &) const> PyObject *TypeSmart<T, E, C, I>::m_getter(PyObject *self, void */*closure*/)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_getter, reinterpret_cast<size_t>(m_getter<O, G>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  call_ptr_const<O, G> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, NULL, NULL, !self);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*S)(const Ptr<E> &, Tuples &) const> int TypeSmart<T, E, C, I>::m_setter(PyObject *self, PyObject *value, void */*closure*/)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_setter, reinterpret_cast<size_t>(m_setter<O, S>)));
  PyObject *value_packed = pack_tuple(value);
  if (value && !value_packed)
  {
    return -1;
  }
  tuples_auto_release_t tuples(Tuples::factory(*data));
  call_ptr<O, S> c(*tuples, self, __PRETTY_FUNCTION__ );
  tuples->ubiquitous_caller(c, value_packed, NULL, !self);
  Py_DecRef(value_packed);
  return is_error_occured() ? -1 : 0;
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*G)(const Ptr<const E> &, Tuples &) const, void (O::*S)(const Ptr<E> &, Tuples &) const> PyObject *TypeSmart<T, E, C, I>::m_getter_paired(PyObject *self, void */*closure*/)
{
  tuples_auto_release_t tuples(Tuples::factory(__smart_internal::data<E, O, G, S>(reinterpret_cast<size_t>(m_getter_paired<O, G, S>))));
  call_ptr_const<O, G> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, NULL, NULL, !self, Tuples::ctGetter);
}

template <typename T, class E, E* (*C)(T *), class I> template <typename O, void (O::*G)(const Ptr<const E> &, Tuples &) const, void (O::*S)(const Ptr<E> &, Tuples &) const> int TypeSmart<T, E, C, I>::m_setter_paired(PyObject *self, PyObject *value, void */*closure*/)
{
  PyObject *value_packed = pack_tuple(value);
  if (value && !value_packed)
  {
    return -1;
  }
  tuples_auto_release_t tuples(Tuples::factory(__smart_internal::data<E, O, G, S>(reinterpret_cast<size_t>(m_setter_paired<O, G, S>))));
  call_ptr<O, S> c(*tuples, self, __PRETTY_FUNCTION__ );
  tuples->ubiquitous_caller(c, value_packed, NULL, !self, Tuples::ctSetter);
  Py_DecRef(value_packed);
  return is_error_occured() ? -1 : 0;
}

} //namespace pyhrol

#endif //__pyhrol_type_smart_hpp__
