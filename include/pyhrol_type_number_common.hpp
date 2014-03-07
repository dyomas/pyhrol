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

#ifndef __pyhrol_type_number_common_hpp__
#define __pyhrol_type_number_common_hpp__

#include "pyhrol_type_number_common.h"

#include <structmember.h>
#include <modsupport.h>
#include <intobject.h>
#include <floatobject.h>
#include <stringobject.h>
#include <sstream>

#include "pyhrol_type_number.h"
#include "pyhrol_type_base.hpp"
#include "pyhrol_exception_handler.h"


namespace pyhrol
{

template <typename T> TypeNumberCommon<T>::TypeNumberCommon()
  : TypeBase<T>(NULL)
{
  m_init();
}

template <typename T> void TypeNumberCommon<T>::negative(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumberCommon<T>::positive(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumberCommon<T>::absolute(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> bool TypeNumberCommon<T>::nonzero(const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumberCommon<T>::invert(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> long TypeNumberCommon<T>::to_long(const Ptr<const T> &) const
{
  throw ImplementationException(__func__);
}

template <typename T> double TypeNumberCommon<T>::to_double(const Ptr<const T> &) const
{
  throw ImplementationException(__func__);
}

template <typename T> const std::string TypeNumberCommon<T>::to_oct(const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> const std::string TypeNumberCommon<T>::to_hex(const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumberCommon<T>::index(const Ptr<T> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T> void TypeNumberCommon<T>::m_init()
{
  m_methods.nb_negative = mediator_nb_negative;
  m_methods.nb_positive = mediator_nb_positive;
  m_methods.nb_absolute = mediator_nb_absolute;
  m_methods.nb_nonzero = mediator_nb_nonzero;
  m_methods.nb_invert = mediator_nb_invert;
  m_methods.nb_coerce = NULL;
  m_methods.nb_int = mediator_nb_int;
  m_methods.nb_long = mediator_nb_long;
  m_methods.nb_float = mediator_nb_float;
  m_methods.nb_oct = mediator_nb_oct;
  m_methods.nb_hex = mediator_nb_hex;
  m_methods.nb_index = mediator_nb_index;
  TypeBase<T>::m_type_object.tp_as_number = &m_methods;
}

template <typename T> template <void (TypeNumberCommon<T>::*M)(const Ptr<T> &, const Ptr<const T> &) const> void TypeNumberCommon<T>::call_wrapper<M>::call()
{
  TypeNumberCommon &type = dynamic_cast<TypeNumberCommon &>(TypeBase<T>::m_get());
  typename TypeBase<T>::T_struct *pres = NULL;
  const T &obj = reinterpret_cast<const typename TypeBase<T>::T_struct *>(self)->endosome;
  try
  {
    pres = type.allocate();
    PYHROL_TRACE(pyhrol::tpMediator, type.address(obj), signature);
    retval = *pres;
    (type.*M)(Ptr<T>(&pres->endosome, retval), Ptr<const T>(&obj, self));
  }
  catch (const ImplementationException &ex)
  {
    type.free(pres);
    retval = NULL;
    if (behavior == returnNotImplemented)
    {
      Py_IncRef(Py_NotImplemented);
      retval = Py_NotImplemented;
    }
    else //raiseError
    {
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

template <typename T> template <void (TypeNumberCommon<T>::*M)(const Ptr<T> &, const Ptr<const T> &) const> TypeNumberCommon<T>::call_wrapper<M>::call_wrapper(PyObject *_self, const char *_signature, const notImplementedBehavior _behavior)
  : behavior(_behavior)
  , self(_self)
  , signature(_signature)
  , retval(NULL)
{
}

template <typename T> TypeNumberCommon<T>::converter::~converter()
{
}

template <typename T> PyObject *TypeNumberCommon<T>::to_long_converter::convert(const Ptr<const T> &arg) const
{
  return PyInt_FromLong(dynamic_cast<const TypeNumberCommon &>(TypeBase<T>::m_get()).to_long(arg));
}

template <typename T> PyObject *TypeNumberCommon<T>::to_double_converter::convert(const Ptr<const T> &arg) const
{
  return PyFloat_FromDouble(dynamic_cast<const TypeNumberCommon &>(TypeBase<T>::m_get()).to_double(arg));
}

template <typename T> PyObject *TypeNumberCommon<T>::to_oct_converter::convert(const Ptr<const T> &arg) const
{
  return PyString_FromString(dynamic_cast<const TypeNumberCommon &>(TypeBase<T>::m_get()).to_oct(arg).c_str());
}

template <typename T> PyObject *TypeNumberCommon<T>::to_hex_converter::convert(const Ptr<const T> &arg) const
{
  return PyString_FromString(dynamic_cast<const TypeNumberCommon &>(TypeBase<T>::m_get()).to_hex(arg).c_str());
}

template <typename T> void TypeNumberCommon<T>::call_converter::call()
{
  const T &obj = reinterpret_cast<const typename TypeBase<T>::T_struct *>(self)->endosome;
  PYHROL_TRACE(pyhrol::tpMediator, dynamic_cast<const TypeNumberCommon &>(TypeBase<T>::m_get()).address(obj), signature);
  retval = c.convert(Ptr<const T>(&obj, self));
}

template <typename T> TypeNumberCommon<T>::call_converter::call_converter(const converter &_c, PyObject *_self, const char *_signature)
  : c(_c)
  , self(_self)
  , signature(_signature)
  , retval(NULL)
{
}

template <typename T> void TypeNumberCommon<T>::call_nonzero::call()
{
  const TypeNumberCommon &type = dynamic_cast<const TypeNumberCommon &>(TypeBase<T>::m_get());
  const T &obj = reinterpret_cast<const typename TypeBase<T>::T_struct *>(self)->endosome;
  PYHROL_TRACE(pyhrol::tpMediator, type.address(obj), signature);
  retval = (type.nonzero(Ptr<const T>(&obj, self)) ? 1 : 0);
}

template <typename T> TypeNumberCommon<T>::call_nonzero::call_nonzero(PyObject *_self, const char *_signature)
  : self(_self)
  , signature(_signature)
  , retval(-1)
{
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_negative(PyObject *arg)
{
  call_wrapper<&TypeNumberCommon<T>::negative> cw(arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cw, reinterpret_cast<void *>(mediator_nb_negative));
  return cw.retval;
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_positive(PyObject *arg)
{
  call_wrapper<&TypeNumberCommon<T>::positive> cw(arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cw, reinterpret_cast<void *>(mediator_nb_positive));
  return cw.retval;
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_absolute(PyObject *arg)
{
  call_wrapper<&TypeNumberCommon<T>::absolute> cw(arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cw, reinterpret_cast<void *>(mediator_nb_absolute));
  return cw.retval;
}

template <typename T> int TypeNumberCommon<T>::mediator_nb_nonzero(PyObject *arg)
{
  call_nonzero cn(arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cn, reinterpret_cast<void *>(mediator_nb_nonzero));
  return cn.retval;
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_invert(PyObject *arg)
{
  call_wrapper<&TypeNumberCommon<T>::invert> cw(arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cw, reinterpret_cast<void *>(mediator_nb_invert));
  return cw.retval;
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_int(PyObject *arg)
{
  to_long_converter ci;
  call_converter cc(ci, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cc, reinterpret_cast<void *>(mediator_nb_int));
  return cc.retval;
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_long(PyObject *arg)
{
  to_long_converter cl;
  call_converter cc(cl, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cc, reinterpret_cast<void *>(mediator_nb_long));
  return cc.retval;
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_float(PyObject *arg)
{
  to_double_converter cf;
  call_converter cc(cf, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cc, reinterpret_cast<void *>(mediator_nb_float));
  return cc.retval;
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_oct(PyObject *arg)
{
  to_oct_converter co;
  call_converter cc(co, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cc, reinterpret_cast<void *>(mediator_nb_oct));
  return cc.retval;
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_hex(PyObject *arg)
{
  to_hex_converter ch;
  call_converter cc(ch, arg, __PRETTY_FUNCTION__);
  exceptionHandler::call(cc, reinterpret_cast<void *>(mediator_nb_hex));
  return cc.retval;
}

template <typename T> PyObject *TypeNumberCommon<T>::mediator_nb_index(PyObject *arg)
{
  call_wrapper<&TypeNumberCommon<T>::index> cw(arg, __PRETTY_FUNCTION__, raiseError);
  exceptionHandler::call(cw, reinterpret_cast<void *>(mediator_nb_index));
  return cw.retval;
}

} //namespace pyhrol

#endif //__pyhrol_type_number_common_hpp__
