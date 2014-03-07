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

#ifndef __pyhrol_type_sequence_hpp__
#define __pyhrol_type_sequence_hpp__

#include "pyhrol_type_sequence.h"
#include "pyhrol_type_base.hpp"
#include "pyhrol_macro.h"

namespace pyhrol
{

template <typename T> void TypeSequence<T>::call_del::call()
{
  const TypeSequence &type = dynamic_cast<const TypeSequence &>(TypeBase<T>::m_get());
  T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
  PYHROL_TRACE(tpMediator, type.address(obj), signature);
  type.del(Ptr<T>(&obj, self), begin, end);
}

template <typename T> TypeSequence<T>::call_del::call_del(PyObject *_self, Py_ssize_t _begin, Py_ssize_t _end, const char *_signature)
  : self(_self)
  , begin(_begin)
  , end(_end)
  , signature(_signature)
{
}


template <typename T> void TypeSequence<T>::call_length::call()
{
  const TypeSequence &type = dynamic_cast<const TypeSequence &>(TypeBase<T>::m_get());
  const T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
  PYHROL_TRACE(tpMediator, type.address(obj), signature);
  retval = type.length(Ptr<const T>(&obj, self));
}

template <typename T> TypeSequence<T>::call_length::call_length(PyObject *_self, const char *_signature)
  : self(_self)
  , signature(_signature)
  , retval(-1)
{
}


template <typename T> void TypeSequence<T>::call_repeat::call()
{
  TypeSequence &type = dynamic_cast<TypeSequence &>(TypeBase<T>::m_get());
  typename TypeBase<T>::T_struct *pres = NULL;
  try
  {
    pres = type.allocate();
    const T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(tpMediator, type.address(obj), signature);
    retval = *pres;
    type.repeat(Ptr<T>(&pres->endosome, retval), Ptr<const T>(&obj, self), cnt);
  }
  catch (...)
  {
    type.free(pres);
    retval = NULL;
    throw;
  }
}

template <typename T> TypeSequence<T>::call_repeat::call_repeat(PyObject *_self, Py_ssize_t _cnt, const char *_signature)
  : self(_self)
  , cnt(_cnt)
  , signature(_signature)
  , retval(NULL)
{
}


template <typename T> void TypeSequence<T>::call_repeat_inplace::call()
{
  try
  {
    const TypeSequence &type = dynamic_cast<const TypeSequence &>(TypeBase<T>::m_get());
    T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(tpMediator, type.address(obj), signature);
    type.repeat(Ptr<T>(&obj, self), cnt);
    Py_IncRef(self);
  }
  catch (...)
  {
    self = NULL;
    throw;
  }
}

template <typename T> TypeSequence<T>::call_repeat_inplace::call_repeat_inplace(PyObject *_self, Py_ssize_t _cnt, const char *_signature)
  : self(_self)
  , cnt(_cnt)
  , signature(_signature)
{
}


template <typename T> void TypeSequence<T>::call_get::call()
{
  TypeSequence &type = dynamic_cast<TypeSequence &>(TypeBase<T>::m_get());
  typename TypeBase<T>::T_struct *pres = NULL;

  try
  {
    pres = type.allocate();
    const T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(tpMediator, type.address(obj), signature);
    retval = *pres;
    type.get(Ptr<T>(&pres->endosome, retval), Ptr<const T>(&obj, self), begin, end);
  }
  catch (...)
  {
    type.free(pres);
    retval = NULL;
    throw;
  }
}

template <typename T> TypeSequence<T>::call_get::call_get(PyObject *_self, Py_ssize_t _begin, Py_ssize_t _end, const char *_signature)
  : self(_self)
  , begin(_begin)
  , end(_end)
  , signature(_signature)
  , retval(NULL)
{
}


template <typename T> void TypeSequence<T>::call_concat::call()
{
  TypeSequence &type = dynamic_cast<TypeSequence &>(TypeBase<T>::m_get());

  if (self)
  {
    typename TypeBase<T>::T_struct *pres = NULL;
    try
    {
      pres = type.allocate();
      const T &obj = reinterpret_cast<const typename TypeBase<T>::T_struct *>(self)->endosome;
      PYHROL_TRACE(tpMediator, type.address(obj), signature)
      retval = *pres;
      type.concat(Ptr<T>(&pres->endosome, retval), Ptr<const T>(&obj, self), args);
    }
    catch (...)
    {
      type.free(pres);
      retval = NULL;
      throw;
    }
  }
  else
  {
    type.concat(Ptr<T>(), Ptr<const T>(), args);
  }
}

template <typename T> TypeSequence<T>::call_concat::call_concat(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , signature(_signature)
  , retval(NULL)
{
}


template <typename T> void TypeSequence<T>::call_concat_inplace::call()
{
  TypeSequence &type = dynamic_cast<TypeSequence &>(TypeBase<T>::m_get());

  if (self)
  {
    try
    {
      T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
      PYHROL_TRACE(tpMediator, type.address(obj), signature)
      type.concat(Ptr<T>(&obj, self), args);
      Py_IncRef(self);
    }
    catch(...)
    {
      self = NULL;
      throw;
    }
  }
  else
  {
    type.concat(Ptr<T>(), args);
  }
}

template <typename T> TypeSequence<T>::call_concat_inplace::call_concat_inplace(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , signature(_signature)
{
}


template <typename T> void TypeSequence<T>::call_assign::call()
{
  const TypeSequence &type = dynamic_cast<const TypeSequence &>(TypeBase<T>::m_get());

  if (self)
  {
    T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(tpMediator, type.address(obj), signature)
    type.assign(Ptr<T>(&obj, self), args, begin, end);
  }
  else
  {
    type.assign(Ptr<T>(), args, 0, 0);
  }
}

template <typename T> TypeSequence<T>::call_assign::call_assign(Tuples &_args, PyObject *_self, const Py_ssize_t _begin, const Py_ssize_t _end, const char *_signature)
  : args(_args)
  , self(_self)
  , begin(_begin)
  , end(_end)
  , signature(_signature)
{
}


template <typename T> void TypeSequence<T>::call_contains::call()
{
  const TypeSequence &type = dynamic_cast<const TypeSequence &>(TypeBase<T>::m_get());

  if (self)
  {
    const T &obj = reinterpret_cast<const typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(tpMediator, type.address(obj), signature)
    type.contains(retval, Ptr<const T>(&obj, self), args);
  }
  else
  {
    bool _tmp;
    type.contains(_tmp, Ptr<const T>(), args);
  }
}

template <typename T> TypeSequence<T>::call_contains::call_contains(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , retval(false)
  , signature(_signature)
{
}


template <typename T> TypeSequence<T>::TypeSequence()
{
  m_init();
}

template <typename T> Py_ssize_t TypeSequence<T>::length(const Ptr<const T> &) const
{
  throw ImplementationException(__func__);
  return 0;
}

template <typename T> void TypeSequence<T>::concat(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException(__func__);
}

template <typename T> void TypeSequence<T>::concat(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException(__func__);
}

template <typename T> void TypeSequence<T>::repeat(const Ptr<T> &, Py_ssize_t) const
{
  throw ImplementationException(__func__);
}

template <typename T> void TypeSequence<T>::repeat(const Ptr<T> &, const Ptr<const T> &, Py_ssize_t) const
{
  throw ImplementationException(__func__);
}

template <typename T> void TypeSequence<T>::get(const Ptr<T> &, const Ptr<const T> &, Py_ssize_t, Py_ssize_t) const
{
  throw ImplementationException(__func__);
}

template <typename T> void TypeSequence<T>::assign(const Ptr<T> &, const Ptr<const T> &, Py_ssize_t, Py_ssize_t) const
{
  throw ImplementationException(__func__);
}

template <typename T> void TypeSequence<T>::del(const Ptr<T> &, Py_ssize_t, Py_ssize_t) const
{
  throw ImplementationException(__func__);
}

template <typename T> bool TypeSequence<T>::contains(const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException(__func__);
}

template <typename T> void TypeSequence<T>::concat(const Ptr<T> &res, Tuples &_args) const
{
  PyTypeObject &type = TypeBase<T>::m_get();
  PyObject *padd;

  PYHROL_PARSE_TUPLE_2(NULL, _args, type, padd)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  concat(res, Ptr<const T>(&reinterpret_cast<typename TypeBase<T>::T_struct *>(padd)->endosome, padd));

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeSequence<T>::concat(const Ptr<T> &dst, const Ptr<const T> &src, Tuples &_args) const
{
  PyTypeObject &type = TypeBase<T>::m_get();
  PyObject *padd;

  PYHROL_PARSE_TUPLE_2(NULL, _args, type, padd)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  concat(dst, src, Ptr<const T>(&reinterpret_cast<typename TypeBase<T>::T_struct *>(padd)->endosome, padd));

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeSequence<T>::assign(const Ptr<T> &res, Tuples &_args, Py_ssize_t begin, Py_ssize_t end) const
{
  PyTypeObject &type = TypeBase<T>::m_get();
  PyObject *padd;

  PYHROL_PARSE_TUPLE_2(NULL, _args, type, padd)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  assign(res, Ptr<const T>(&reinterpret_cast<typename TypeBase<T>::T_struct *>(padd)->endosome, padd), begin, end);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeSequence<T>::contains(bool &retval, const Ptr<const T> &data, Tuples &_args) const
{
  PyTypeObject &type = TypeBase<T>::m_get();
  PyObject *pkey;

  PYHROL_PARSE_TUPLE_2(NULL, _args, type, pkey)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  retval = contains(data, Ptr<const T>(&reinterpret_cast<typename TypeBase<T>::T_struct *>(pkey)->endosome, pkey));

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeSequence<T>::m_init()
{
  m_methods.sq_length = mediator_sq_length;
  m_methods.sq_concat = mediator_sq_concat;
  m_methods.sq_repeat = mediator_sq_repeat;
  m_methods.sq_item = mediator_sq_item;
  m_methods.sq_slice = mediator_sq_slice;
  m_methods.sq_ass_item = mediator_sq_ass_item;
  m_methods.sq_ass_slice = mediator_sq_ass_slice;
  m_methods.sq_contains = mediator_sq_contains;
  m_methods.sq_inplace_concat = mediator_sq_inplace_concat;
  m_methods.sq_inplace_repeat = mediator_sq_inplace_repeat;
  TypeBase<T>::m_type_object.tp_as_sequence = &m_methods;
}

template <typename T> Py_ssize_t TypeSequence<T>::mediator_sq_length(PyObject *self)
{
  call_length c(self, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_sq_length));
  return c.retval;
}

template <typename T> PyObject *TypeSequence<T>::mediator_sq_concat(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<void *>(mediator_sq_concat)));
  PyObject *args_packed = pack_tuple(args);
  if (args && !args_packed)
  {
    return NULL;
  }
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_concat c(*tuples, self, __PRETTY_FUNCTION__);
  tuples->ubiquitous_caller(c, args_packed, NULL, !self);
  Py_DecRef(args_packed);
  return clear_on_error(c.retval);
}

template <typename T> PyObject *TypeSequence<T>::mediator_sq_repeat(PyObject *self, Py_ssize_t cnt)
{
  call_repeat c(self, cnt, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_sq_repeat));
  return c.retval;
}

template <typename T> PyObject *TypeSequence<T>::mediator_sq_item(PyObject *self, Py_ssize_t idx)
{
  call_get c(self, idx, idx + 1, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_sq_item));
  return c.retval;
}

template <typename T> PyObject *TypeSequence<T>::mediator_sq_slice(PyObject *self, Py_ssize_t begin, Py_ssize_t end)
{
  call_get c(self, begin, end, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_sq_slice));
  return c.retval;
}

template <typename T> int TypeSequence<T>::mediator_sq_ass_item(PyObject *self, Py_ssize_t idx, PyObject *args)
{
  int retval;
  if (self && !args)
  {
    call_del c(self, idx, idx + 1, __PRETTY_FUNCTION__);
    retval = exceptionHandler::call(c, reinterpret_cast<void *>(mediator_sq_ass_item)) ? 0 : -1;
  }
  else
  {
    static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<void *>(mediator_sq_ass_item)));
    PyObject *args_packed = pack_tuple(args);
    if (args && !args_packed)
    {
      return -1;
    }
    std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
    call_assign c(*tuples, self, idx, idx + 1, __PRETTY_FUNCTION__);
    tuples->ubiquitous_caller(c, args_packed, NULL, !self);
    Py_DecRef(args_packed);
    retval = is_error_occured() ? -1 : 0;
  }
  return retval;
}

template <typename T> int TypeSequence<T>::mediator_sq_ass_slice(PyObject *self, Py_ssize_t begin, Py_ssize_t end, PyObject *args)
{
  int retval;
  if (self && !args)
  {
    call_del c(self, begin, end, __PRETTY_FUNCTION__);
    retval = exceptionHandler::call(c, reinterpret_cast<void *>(mediator_sq_ass_slice)) ? 0 : -1;
  }
  else
  {
    static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<void *>(mediator_sq_ass_slice)));
    PyObject *args_packed = pack_tuple(args);
    if (args && !args_packed)
    {
      return -1;
    }
    std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
    call_assign c(*tuples, self, begin, end, __PRETTY_FUNCTION__);
    tuples->ubiquitous_caller(c, args_packed, NULL, !self);
    Py_DecRef(args_packed);
    retval = is_error_occured() ? -1 : 0;
  }
  return retval;
}

template <typename T> int TypeSequence<T>::mediator_sq_contains(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<void *>(mediator_sq_contains)));
  PyObject *args_packed = pack_tuple(args);
  if (args && !args_packed)
  {
    return -1;
  }
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_contains c(*tuples, self, __PRETTY_FUNCTION__);
  tuples->ubiquitous_caller(c, args_packed, NULL, !self);
  Py_DecRef(args_packed);
  return is_error_occured() ? -1 : c.retval;
}

template <typename T> PyObject *TypeSequence<T>::mediator_sq_inplace_concat(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<void *>(mediator_sq_inplace_concat)));
  PyObject *args_packed = pack_tuple(args);
  if (args && !args_packed)
  {
    return NULL;
  }
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_concat_inplace c(*tuples, self, __PRETTY_FUNCTION__);
  tuples->ubiquitous_caller(c, args_packed, NULL, !self);
  Py_DecRef(args_packed);
  return clear_on_error(c.self);
}

template <typename T> PyObject *TypeSequence<T>::mediator_sq_inplace_repeat(PyObject *self, Py_ssize_t cnt)
{
  call_repeat_inplace c(self, cnt, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_sq_inplace_repeat));
  return c.self;
}

}; //namespace pyhrol

#endif //__pyhrol_type_sequence_hpp__
