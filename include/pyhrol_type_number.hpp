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

#ifndef __pyhrol_type_number_hpp__
#define __pyhrol_type_number_hpp__

#include <structmember.h>
#include <modsupport.h>
#include <intobject.h>
#include <floatobject.h>
#include <stringobject.h>
#include <sstream>

#include "pyhrol_type_number.h"
#include "pyhrol_type_base.hpp"
#include "pyhrol_type_number_common.hpp"


namespace pyhrol
{

template <typename T> TypeNumber<T>::TypeNumber()
{
  m_init();
}

template <typename T> void TypeNumber<T>::add(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::subtract(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::multiply(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::divide(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::remainder(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::divmod(const Ptr<T> &, const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::power(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::power(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::lshift(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::rshift(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::nb_and(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::nb_xor(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::nb_or(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_add(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_subtract(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_multiply(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_divide(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_remainder(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_power(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_power(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_lshift(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_rshift(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_and(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_xor(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_or(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::floor_divide(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::true_divide(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_floor_divide(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::inplace_true_divide(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumber<T>::m_init()
{
  TypeNumberCommon<T>::m_methods.nb_add = mediator_nb_add;
  TypeNumberCommon<T>::m_methods.nb_subtract = mediator_nb_subtract;
  TypeNumberCommon<T>::m_methods.nb_multiply = mediator_nb_multiply;
  TypeNumberCommon<T>::m_methods.nb_divide = mediator_nb_divide;
  TypeNumberCommon<T>::m_methods.nb_remainder = mediator_nb_remainder;
  TypeNumberCommon<T>::m_methods.nb_divmod = mediator_nb_divmod;
  TypeNumberCommon<T>::m_methods.nb_power = mediator_nb_power;
  TypeNumberCommon<T>::m_methods.nb_lshift = mediator_nb_lshift;
  TypeNumberCommon<T>::m_methods.nb_rshift = mediator_nb_rshift;
  TypeNumberCommon<T>::m_methods.nb_and = mediator_nb_and;
  TypeNumberCommon<T>::m_methods.nb_xor = mediator_nb_xor;
  TypeNumberCommon<T>::m_methods.nb_or = mediator_nb_or;
  TypeNumberCommon<T>::m_methods.nb_inplace_add = mediator_nb_inplace_add;
  TypeNumberCommon<T>::m_methods.nb_inplace_subtract = mediator_nb_inplace_subtract;
  TypeNumberCommon<T>::m_methods.nb_inplace_multiply = mediator_nb_inplace_multiply;
  TypeNumberCommon<T>::m_methods.nb_inplace_divide = mediator_nb_inplace_divide;
  TypeNumberCommon<T>::m_methods.nb_inplace_remainder = mediator_nb_inplace_remainder;
  TypeNumberCommon<T>::m_methods.nb_inplace_power = mediator_nb_inplace_power;
  TypeNumberCommon<T>::m_methods.nb_inplace_lshift = mediator_nb_inplace_lshift;
  TypeNumberCommon<T>::m_methods.nb_inplace_rshift = mediator_nb_inplace_rshift;
  TypeNumberCommon<T>::m_methods.nb_inplace_and = mediator_nb_inplace_and;
  TypeNumberCommon<T>::m_methods.nb_inplace_xor = mediator_nb_inplace_xor;
  TypeNumberCommon<T>::m_methods.nb_inplace_or = mediator_nb_inplace_or;
  TypeNumberCommon<T>::m_methods.nb_floor_divide = mediator_nb_floor_divide;
  TypeNumberCommon<T>::m_methods.nb_true_divide = mediator_nb_true_divide;
  TypeNumberCommon<T>::m_methods.nb_inplace_floor_divide = mediator_nb_inplace_floor_divide;
  TypeNumberCommon<T>::m_methods.nb_inplace_true_divide = mediator_nb_inplace_true_divide;
}

template <typename T> void TypeNumber<T>::call_divmod::call()
{
  TypeNumber &type = dynamic_cast<TypeNumber &>(TypeBase<T>::m_get());
  typename TypeBase<T>::T_struct *pres1 = NULL, *pres2 = NULL;

  try
  {
    const T &obj = reinterpret_cast<const typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(pyhrol::tpMediator, type.address(obj), signature);
    pres1 = type.allocate();
    pres2 = type.allocate();
    type.divmod
    (
        Ptr<T>(&pres1->endosome, *pres1)
      , Ptr<T>(&pres2->endosome, *pres2)
      , Ptr<const T>(&obj, arg)
      , Ptr<const T>(&reinterpret_cast<const typename TypeBase<T>::T_struct *>(arg)->endosome, arg)
    );
    retval = Py_BuildValue("(OO)", pres1, pres2);
  }
  catch (const ImplementationException &)
  {
    type.free(pres1);
    type.free(pres2);
    Py_IncRef(Py_NotImplemented);
    retval = Py_NotImplemented;
  }
  catch (...)
  {
    type.free(pres1);
    type.free(pres2);
    throw;
  }
}

template <typename T> TypeNumber<T>::call_divmod::call_divmod(PyObject *_self, PyObject *_arg, const char *_signature)
  : self(_self)
  , arg(_arg)
  , signature(_signature)
  , retval(NULL)
{
}

template <typename T> template <void (TypeNumber<T>::*M)(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const> void TypeNumber<T>::call_binary<M>::call()
{
  TypeNumber &type = dynamic_cast<TypeNumber &>(TypeBase<T>::m_get());
  typename TypeBase<T>::T_struct *pres = NULL;

  try
  {
    pres = type.allocate();
    retval = *pres;
    const T &obj = reinterpret_cast<const typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(pyhrol::tpMediator, type.address(obj), signature);
    (type.*M)(Ptr<T>(&pres->endosome, retval), Ptr<const T>(&obj, self), Ptr<const T>(&reinterpret_cast<const typename TypeBase<T>::T_struct *>(arg)->endosome, arg));
  }
  catch (const ImplementationException &)
  {
    type.free(pres);
    if (bh == returnNotImplemented)
    {
      Py_IncRef(Py_NotImplemented);
      retval = Py_NotImplemented;
    }
    else //raiseError
    {
      retval = NULL;
      throw;
    }
  }
  catch (...)
  {
    type.free(pres);
    retval = NULL;
    throw;
  }
}

template <typename T> template <void (TypeNumber<T>::*M)(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const> TypeNumber<T>::call_binary<M>::call_binary(PyObject *_self, PyObject *_arg, const char *_signature, const notImplementedBehavior _bh)
  : self(_self)
  , arg(_arg)
  , signature(_signature)
  , bh(_bh)
  , retval(NULL)
{
}

template <typename T> template <void (TypeNumber<T>::*M)(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &, const Ptr<const T> &) const> void TypeNumber<T>::call_ternary<M>::call()
{
  TypeNumber &type = dynamic_cast<TypeNumber &>(TypeBase<T>::m_get());
  typename TypeBase<T>::T_struct *pres = NULL;

  try
  {
    pres = type.allocate();
    retval = *pres;
    const T &obj = reinterpret_cast<const typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(pyhrol::tpMediator, type.address(obj), signature);
    (type.*M)
    (
        Ptr<T>(&pres->endosome, retval)
      , Ptr<const T>(&obj, self)
      , Ptr<const T>(&reinterpret_cast<const typename TypeBase<T>::T_struct *>(arg)->endosome, arg)
      , Ptr<const T>(&reinterpret_cast<const typename TypeBase<T>::T_struct *>(arg2)->endosome, arg2));
  }
  catch (const ImplementationException &)
  {
    type.free(pres);
    if (bh == returnNotImplemented)
    {
      Py_IncRef(Py_NotImplemented);
      retval = Py_NotImplemented;
    }
    else //raiseError
    {
      retval = NULL;
      throw;
    }
  }
  catch (...)
  {
    type.free(pres);
    retval = NULL;
    throw;
  }
}

template <typename T> template <void (TypeNumber<T>::*M)(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &, const Ptr<const T> &) const> TypeNumber<T>::call_ternary<M>::call_ternary(PyObject *_self, PyObject *_arg, PyObject *_arg2, const char *_signature, const notImplementedBehavior _bh)
  : self(_self)
  , arg(_arg)
  , arg2(_arg2)
  , signature(_signature)
  , bh(_bh)
  , retval(NULL)
{
}

template <typename T> template <void (TypeNumber<T>::*M)(const Ptr<T> &, const Ptr<const T> &) const> void TypeNumber<T>::call_inplace_unary<M>::call()
{
  try
  {
    TypeNumber &type = dynamic_cast<TypeNumber &>(TypeBase<T>::m_get());
    T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(pyhrol::tpMediator, type.address(obj), signature);
    (type.*M)(Ptr<T>(&obj, self), Ptr<const T>(&reinterpret_cast<typename TypeBase<T>::T_struct *>(arg)->endosome, arg));
    Py_IncRef(self);
  }
  catch (const ImplementationException &)
  {
    if (bh == returnNotImplemented)
    {
      Py_IncRef(Py_NotImplemented);
      self = Py_NotImplemented;
    }
    else //raiseError
    {
      self = NULL;
      throw;
    }
  }
  catch (...)
  {
    self = NULL;
    throw;
  }
}

template <typename T> template <void (TypeNumber<T>::*M)(const Ptr<T> &, const Ptr<const T> &) const> TypeNumber<T>::call_inplace_unary<M>::call_inplace_unary(PyObject *_self, PyObject *_arg, const char *_signature, const notImplementedBehavior _bh)
  : self(_self)
  , arg(_arg)
  , signature(_signature)
  , bh(_bh)
{
}

template <typename T> template <void (TypeNumber<T>::*M)(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const> void TypeNumber<T>::call_inplace_binary<M>::call()
{
  try
  {
    TypeNumber &type = dynamic_cast<TypeNumber &>(TypeBase<T>::m_get());
    T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(pyhrol::tpMediator, type.address(obj), signature);
    (type.*M)(Ptr<T>(&obj, self), Ptr<const T>(&reinterpret_cast<typename TypeBase<T>::T_struct *>(arg)->endosome, arg), Ptr<const T>(&reinterpret_cast<typename TypeBase<T>::T_struct *>(arg2)->endosome, arg2));
    Py_IncRef(self);
  }
  catch (const ImplementationException &)
  {
    if (bh == returnNotImplemented)
    {
      Py_IncRef(Py_NotImplemented);
      self = Py_NotImplemented;
    }
    else //raiseError
    {
      self = NULL;
      throw;
    }
  }
  catch (...)
  {
    self = NULL;
    throw;
  }
}

template <typename T> template <void (TypeNumber<T>::*M)(const Ptr<T> &, const Ptr<const T> &, const Ptr<const T> &) const> TypeNumber<T>::call_inplace_binary<M>::call_inplace_binary(PyObject *_self, PyObject *_arg, PyObject *_arg2, const char *_signature, const notImplementedBehavior _bh)
  : self(_self)
  , arg(_arg)
  , arg2(_arg2)
  , signature(_signature)
  , bh(_bh)
{
}


template <typename T> PyObject *TypeNumber<T>::mediator_nb_add(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::add> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_add));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_subtract(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::subtract> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_subtract));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_multiply(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::multiply> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_multiply));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_divide(PyObject *left, PyObject *right)
{
   call_binary<&TypeNumber<T>::divide> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_divide));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_remainder(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::remainder> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_remainder));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_divmod(PyObject *left, PyObject *right)
{
  call_divmod cd(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(cd, reinterpret_cast<void *>(mediator_nb_divmod));
  return cd.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_power(PyObject *self, PyObject *arg, PyObject *arg2)
{
  if (arg2 == Py_None)
  {
    call_binary<&TypeNumber<T>::power> c(self, arg, __PRETTY_FUNCTION__);
    exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_power));
    return c.retval;
  }
  else
  {
    call_ternary<&TypeNumber<T>::power> c(self, arg, arg2, __PRETTY_FUNCTION__);
    exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_power));
    return c.retval;
  }
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_lshift(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::lshift> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_lshift));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_rshift(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::rshift> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_rshift));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_and(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::nb_and> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_and));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_xor(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::nb_xor> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_xor));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_or(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::nb_or> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_or));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_add(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_add> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_add));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_subtract(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_subtract> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_subtract));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_multiply(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_multiply> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_multiply));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_divide(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_divide> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_divide));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_remainder(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_remainder> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_remainder));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_power(PyObject *self, PyObject *arg, PyObject *arg2)
{
  if (arg2 == Py_None)
  {
    call_inplace_unary<&TypeNumber<T>::inplace_power> c(self, arg, __PRETTY_FUNCTION__, raiseError);
    exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_power));
    return c.self;
  }
  else
  {
    call_inplace_binary<&TypeNumber<T>::inplace_power> c(self, arg, arg2, __PRETTY_FUNCTION__, raiseError);
    exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_power));
    return c.self;
  }
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_lshift(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_lshift> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_lshift));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_rshift(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_rshift> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_rshift));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_and(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_and> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_and));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_xor(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_xor> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_xor));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_or(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_or> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_or));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_floor_divide(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::floor_divide> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_floor_divide));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_true_divide(PyObject *left, PyObject *right)
{
  call_binary<&TypeNumber<T>::true_divide> c(left, right, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_true_divide));
  return c.retval;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_floor_divide(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_floor_divide> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_floor_divide));
  return c.self;
}

template <typename T> PyObject *TypeNumber<T>::mediator_nb_inplace_true_divide(PyObject *self, PyObject *arg)
{
  call_inplace_unary<&TypeNumber<T>::inplace_true_divide> c(self, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<void *>(mediator_nb_inplace_true_divide));
  return c.self;
}

} //namespace pyhrol

#endif //__pyhrol_type_map_hpp__
