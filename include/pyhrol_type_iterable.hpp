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

#ifndef __pyhrol_type_iterable_hpp__
#define __pyhrol_type_iterable_hpp__

#include "pyhrol_type_iterable.h"
#include "pyhrol_type_base.hpp"
#include "pyhrol_exception_handler.h"

namespace pyhrol
{

namespace __iter_internal
{

void stop_iteration_error(const char *type_name);

} //namespace __iter_internal

template <typename T, typename I> void TypeIterable<T, I>::call_iter::call()
{
  typename TypeBase<I>::T_struct *pres = NULL;
  try
  {
    const TypeIterable &type = dynamic_cast<const TypeIterable &>(TypeBase<T>::m_get());
    pres = TypeBase<I>::allocate_static();
    T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(pyhrol::tpMediator, type.address(obj), signature);
    retval = *pres;
    type.iter(Ptr<I>(&pres->endosome, retval), Ptr<const T>(&obj, self));
  }
  catch (...)
  {
    TypeBase<I>::free_static(pres);
    retval = NULL;
    throw;
  }
}

template <typename T, typename I> TypeIterable<T, I>::call_iter::call_iter(PyObject *_self, const char *_signature)
  :self(_self)
  , signature(_signature)
  , retval(NULL)
{
}

template <typename T, typename I> void TypeIterable<T, I>::call_next::call()
{
  typename TypeBase<I>::T_struct *pres = NULL;

  try
  {
    const TypeIterable &type = dynamic_cast<const TypeIterable &>(TypeBase<T>::m_get());
    pres = TypeBase<I>::allocate_static();
    T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
    PYHROL_TRACE(pyhrol::tpMediator, type.address(obj), signature);
    retval = *pres;
    if (!type.next(Ptr<I>(&pres->endosome, retval), Ptr<T>(&obj, self)))
    {
      TypeBase<I>::free_static(pres);
      retval = NULL;
      __iter_internal::stop_iteration_error(type->tp_name);
    }
  }
  catch (...)
  {
    TypeBase<I>::free_static(pres);
    retval = NULL;
    throw;
  }
}

template <typename T, typename I> TypeIterable<T, I>::call_next::call_next(PyObject *_self, const char *_signature)
  : self(_self)
  , signature(_signature)
  , retval(NULL)
{
}

template <typename T, typename I> TypeIterable<T, I>::TypeIterable()
{
  m_init();
}

template <typename T, typename I> void TypeIterable<T, I>::m_init()
{
  TypeBase<T>::m_type_object.tp_iter = mediator_tp_iter;
  TypeBase<T>::m_type_object.tp_iternext = mediator_tp_iternext;
}

template <typename T, typename I> void TypeIterable<T, I>::iter(const Ptr<I> &, const Ptr<const T> &) const
{
  throw ImplementationException();
}

template <typename T, typename I> bool TypeIterable<T, I>::next(const Ptr<I> &, const Ptr<T> &) const
{
  throw ImplementationException();
}

template <typename T, typename I> PyObject *TypeIterable<T, I>::mediator_tp_iter(PyObject *self)
{
  call_iter c(self, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<size_t>(mediator_tp_iter));
  return c.retval;
}

template <typename T, typename I> PyObject *TypeIterable<T, I>::mediator_tp_iternext(PyObject *self)
{
  call_next c(self, __PRETTY_FUNCTION__);
  exceptionHandler::call(c, reinterpret_cast<size_t>(mediator_tp_iternext));
  return c.retval;
}

} //namespace pyhrol

#endif //__pyhrol_type_iterable_hpp__
