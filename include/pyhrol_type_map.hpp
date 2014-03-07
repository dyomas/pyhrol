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

#ifndef __pyhrol_type_map_hpp__
#define __pyhrol_type_map_hpp__

#include <iostream>

#include "pyhrol_type_map.h"
#include "pyhrol_type_base.hpp"
#include "pyhrol_macro.h"
#include "pyhrol_tuple_in.h"

namespace pyhrol
{

template <typename T> void TypeMap<T>::mp_call_length::call()
{
  const TypeMap &type = dynamic_cast<const TypeMap &>(TypeBase<T>::m_get());
  const T &obj = reinterpret_cast<const typename TypeBase<T>::T_struct *>(self)->endosome;
  PYHROL_TRACE(tpMediator, type.address(obj), signature);
  retval = type.length(Ptr<const T>(&obj, self));
}

template <typename T> TypeMap<T>::mp_call_length::mp_call_length(PyObject *_self, const char *_signature)
  : self(_self)
  , signature(_signature)
  , retval(-1)
{
}

template <typename T> void TypeMap<T>::mp_call_get::call()
{
  const TypeMap &type = dynamic_cast<const TypeMap &>(TypeBase<T>::m_get());
  if (self)
  {
    T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(tpMediator, type.address(obj), signature);
    type.get(Ptr<const T>(&obj, self), args);
  }
  else
  {
    type.get(Ptr<const T>(), args);
  }
}

template <typename T> TypeMap<T>::mp_call_get::mp_call_get(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , signature(_signature)
{
}

template <typename T> template <void (TypeMap<T>::*M)(const Ptr<T> &, Tuples &) const> void TypeMap<T>::mp_call_change<M>::call()
{
  const TypeMap &type = dynamic_cast<const TypeMap &>(TypeBase<T>::m_get());
  if (self)
  {
    T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(tpMediator, type.address(obj), signature);
    (type.*M)(Ptr<T>(&obj, self), args);
  }
  else
  {
    (type.*M)(Ptr<T>(), args);
  }
}

template <typename T> template <void (TypeMap<T>::*M)(const Ptr<T> &, Tuples &) const> TypeMap<T>::mp_call_change<M>::mp_call_change(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , signature(_signature)
{
}

template <typename T> TypeMap<T>::TypeMap()
{
  m_init();
}

template <typename T> void TypeMap<T>::m_init()
{
  m_methods.mp_length = mediator_mp_length;
  m_methods.mp_subscript = mediator_mp_subscript;
  m_methods.mp_ass_subscript = mediator_mp_ass_subscript;
  TypeBase<T>::m_type_object.tp_as_mapping = &m_methods;
}

template <typename T> Py_ssize_t TypeMap<T>::length(const Ptr<const T> &) const
{
  throw ImplementationException(__func__);
}

template <typename T> void TypeMap<T>::get(const Ptr<const T> &, Tuples &_args) const
{
  soother key;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, key)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException(__func__);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeMap<T>::del(const Ptr<T> &, Tuples &_args) const
{
  soother key;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, key)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException(__func__);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeMap<T>::assign(const Ptr<T> &, Tuples &_args) const
{
  soother key, value;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, key, soother::converter, value)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException(__func__);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> Py_ssize_t TypeMap<T>::mediator_mp_length(PyObject *self)
{
  mp_call_length c(self, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_mp_length));
  return c.retval;
}

template <typename T> PyObject *TypeMap<T>::mediator_mp_subscript(PyObject *self, PyObject *key)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof_retval, reinterpret_cast<void *>(mediator_mp_subscript)));
  PyObject *key_packed = pack_tuple(key);
  if (key && !key_packed)
  {
    return NULL;
  }
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  mp_call_get c(*tuples, self, __PRETTY_FUNCTION__);
  PyObject *retval = tuples->ubiquitous_caller(c, key_packed, NULL, !self);
  Py_DecRef(key_packed);
  return retval;
}

template <typename T> int TypeMap<T>::mediator_mp_ass_subscript(PyObject *self, PyObject *key, PyObject *value)
{
  static std::auto_ptr<TuplesData>
      data_assign(TuplesData::factory(v_protof, reinterpret_cast<void *>(mediator_mp_ass_subscript)))
    , data_del(TuplesData::factory(v_protof, reinterpret_cast<void *>(mediator_mp_ass_subscript)))
  ;
  PyObject *args_packed = pack_tuple(key, value);
  if (key && !args_packed)
  {
    return -1;
  }
  if (value)
  {
    std::auto_ptr<Tuples> tuples(Tuples::factory(*data_assign));
    mp_call_change<&TypeMap<T>::assign> c(*tuples, self, __PRETTY_FUNCTION__);
    tuples->ubiquitous_caller(c, args_packed, NULL, !self);
  }
  else
  {
    std::auto_ptr<Tuples> tuples(Tuples::factory(*data_del));
    mp_call_change<&TypeMap<T>::del> c(*tuples, self, __PRETTY_FUNCTION__);
    tuples->ubiquitous_caller(c, args_packed, NULL, !self);
  }
  Py_DecRef(args_packed);
  return is_error_occured() ? -1 : 0;
}

}; //namespace pyhrol

#endif //__pyhrol_type_map_hpp__

