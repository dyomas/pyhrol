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

#ifndef __pyhrol_container_hpp__
#define __pyhrol_container_hpp__

#include <tr1/memory>

#include "pyhrol_type.h"
#include "pyhrol_trace.h"
#include "pyhrol_tuples.h"
#include "pyhrol_container.h"
#include "pyhrol_exception_handler.h"

namespace pyhrol
{

namespace __container_internal
{

template <void (*F)(Tuples &)> struct call_function: exceptionHandler
{
  const bool is_probe;
  Tuples &args;
  const char * const signature;
  virtual void call()
  {
    if (!is_probe)
    {
      PYHROL_TRACE(tpMediator, NULL, signature)
    }
    (*F)(args);
  }
  call_function(Tuples &_args, const char *_signature, const bool _is_probe)
    : is_probe(_is_probe)
    , args(_args)
    , signature(_signature)
  {
  }
};

} //namespace __wrap_internal


template <void (*F)(Tuples &)> void Container::add_function(const char *name, const char *help)
{
  m_add_function(name, m_function2<F>, METH_VARARGS, help);
}

template <void (*F)(Tuples &)> void Container::add_function_with_keywords(const char *name, const char *help)
{
  PyCFunctionWithKeywords f = m_function3<F>;
  m_add_function(name, reinterpret_cast<const PyCFunction &>(f), METH_KEYWORDS | METH_VARARGS, help);
}

template <void (*F)(Tuples &)> void Container::add_function_no_args(const char *name, const char *help)
{
  PyNoArgsFunction f = m_function<F>;
  m_add_function(name, reinterpret_cast<const PyCFunction &>(f), METH_NOARGS, help);
}


template <void (*F)(Tuples &)> PyObject *Container::m_function(PyObject *self)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_function_no_arg, reinterpret_cast<size_t>(m_function<F>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  __container_internal::call_function<F> c(*tuples, __PRETTY_FUNCTION__, self);
  return tuples->ubiquitous_caller(c, NULL, NULL, self);
}

template <void (*F)(Tuples &)> PyObject *Container::m_function2(PyObject *self, PyObject *args)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_function, reinterpret_cast<size_t>(m_function2<F>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  __container_internal::call_function<F> c(*tuples, __PRETTY_FUNCTION__, self);
  return tuples->ubiquitous_caller(c, args, NULL, self);
}

template <void (*F)(Tuples &)> PyObject *Container::m_function3(PyObject *self, PyObject *args, PyObject *kwds)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_function_kw, reinterpret_cast<size_t>(m_function3<F>)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  __container_internal::call_function<F> c(*tuples, __PRETTY_FUNCTION__, self);
  return tuples->ubiquitous_caller(c, args, kwds, self);
}

} //namespace pyhrol

#endif //__pyhrol_container_hpp__
