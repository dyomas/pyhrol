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

#ifndef __pyhrol_type_number_any_hpp__
#define __pyhrol_type_number_any_hpp__

#include <structmember.h>
#include <modsupport.h>
#include <intobject.h>
#include <floatobject.h>
#include <stringobject.h>
#include <pyport.h>
#include <object.h>
#include <tupleobject.h>
#include <sstream>

#include "pyhrol_type_number_any.h"
#include "pyhrol_type_base.hpp"
#include "pyhrol_type_number_common.hpp"
#include "pyhrol_macro.h"
#include "pyhrol_tuple_in.h"


namespace pyhrol
{

template <typename T> TypeNumberAny<T>::TypeNumberAny()
{
  m_init();
}

template <typename T> void TypeNumberAny<T>::add(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::subtract(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::multiply(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::divide(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::remainder(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::divmod(const Ptr<T> &, const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::power(const Ptr<T> &, Tuples &_args) const
{
  soother self, power, modulo;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, self, soother::converter, power)
  PYHROL_PARSE_TUPLE_6(NULL, _args, soother::converter, self, soother::converter, power, soother::converter, modulo)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::lshift(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::rshift(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::nb_and(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::nb_xor(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::nb_or(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::floor_divide(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::true_divide(const Ptr<T> &, Tuples &_args) const
{
  soother left, right;
  PYHROL_PARSE_TUPLE_4(NULL, _args, soother::converter, left, soother::converter, right)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}


template <typename T> void TypeNumberAny<T>::inplace_add(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_subtract(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_multiply(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_divide(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_remainder(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_power(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_lshift(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_rshift(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_and(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_xor(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_or(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_floor_divide(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> void TypeNumberAny<T>::inplace_true_divide(const Ptr<T> &, Tuples &_args) const
{
  soother arg;
  PYHROL_PARSE_TUPLE_2(NULL, _args, soother::converter, arg)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

template <typename T> template <void (TypeNumberAny<T>::*M)(const Ptr<T> &, Tuples &) const> void TypeNumberAny<T>::call_std<M>::call()
{
  TypeNumberAny &type = dynamic_cast<TypeNumberAny &>(TypeBase<T>::m_get());

  if (is_probe)
  {
    (type.*M)(Ptr<T>(), args);
  }
  else
  {
    bool not_implemented = false;
    typename TypeBase<T>::T_struct *pres = NULL;
    try
    {
      pres = type.allocate();
      PYHROL_TRACE(tpMediator, type.address(pres->endosome), signature);
      retval = *pres;
      (type.*M)(Ptr<T>(&pres->endosome, retval), args);
    }
    catch (const ImplementationException &)
    {
      not_implemented = true;
      type.free(pres);
      Py_IncRef(Py_NotImplemented);
      retval = Py_NotImplemented;
    }
    catch (...)
    {
      type.free(pres);
      retval = NULL;
      throw;
    }

    if (not_implemented)
    {
      args.choose_build_variant();
      args.before_build();
      args.after_build();
    }
  }
}

template <typename T> template <void (TypeNumberAny<T>::*M)(const Ptr<T> &, Tuples &) const> TypeNumberAny<T>::call_std<M>::call_std(Tuples &_args, const char *_signature, const bool _is_probe)
  : is_probe(_is_probe)
  , args(_args)
  , retval(NULL)
  , signature(_signature)
{
}

template <typename T> template <void (TypeNumberAny<T>::*M)(const Ptr<T> &, Tuples &) const> void TypeNumberAny<T>::call_inplace<M>::call()
{
  TypeNumberAny &type = dynamic_cast<TypeNumberAny &>(TypeBase<T>::m_get());

  if (self)
  {
    bool not_implemented = false;
    try
    {
      T &obj = reinterpret_cast<typename TypeBase<T>::T_struct *>(self)->endosome;
      PYHROL_TRACE(tpMediator, type.address(obj), signature);
      (type.*M)(Ptr<T>(&obj, self), args);
      Py_IncRef(self);
    }
    catch (const ImplementationException &)
    {
      not_implemented = true;
      Py_IncRef(Py_NotImplemented);
      self = Py_NotImplemented;
    }
    catch (...)
    {
      self = NULL;
      throw;
    }

    if (not_implemented)
    {
      args.choose_build_variant();
      args.before_build();
      args.after_build();
    }
  }
  else
  {
    (type.*M)(Ptr<T>(), args);
  }
}

template <typename T> template <void (TypeNumberAny<T>::*M)(const Ptr<T> &, Tuples &) const> TypeNumberAny<T>::call_inplace<M>::call_inplace(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , signature(_signature)
{
}

template <typename T> void TypeNumberAny<T>::call_divmod::call()
{
  TypeNumberAny &type = dynamic_cast<TypeNumberAny &>(TypeBase<T>::m_get());

  if (is_probe)
  {
    type.divmod(Ptr<T>(), Ptr<T>(), args);
  }
  else
  {
    bool not_implemented = false;
    typename TypeBase<T>::T_struct
        *pres1 = NULL
      , *pres2 = NULL
    ;
    try
    {
      pres1 = type.allocate();
      pres2 = type.allocate();
      PYHROL_TRACE(tpMediator, type.address(pres1->endosome), signature);
      type.divmod(Ptr<T>(&pres1->endosome, *pres1), Ptr<T>(&pres2->endosome, *pres2), args);
      retval = Py_BuildValue("(OO)", pres1, pres2);
    }
    catch (const ImplementationException &)
    {
      not_implemented = true;
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

    if (not_implemented)
    {
      args.choose_build_variant();
      args.before_build();
      args.after_build();
    }
  }
}

template <typename T> TypeNumberAny<T>::call_divmod::call_divmod(Tuples &_args, const char *_signature, const bool _is_probe)
  : is_probe(_is_probe)
  , args(_args)
  , retval(NULL)
  , signature(_signature)
{
}


template <typename T> void TypeNumberAny<T>::m_init()
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
  TypeBase<T>::m_type_object.tp_flags |= Py_TPFLAGS_CHECKTYPES;
}


bool actual_call(exceptionHandler &, Tuples &, PyObject *, PyObject *, PyObject * = NULL);
bool actual_call_inplace(exceptionHandler &, Tuples &, const bool is_probe, PyObject *, PyObject * = NULL);


template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_add(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_add)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::add> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_subtract(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_subtract)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::subtract> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_multiply(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_multiply)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::multiply> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_divide(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_divide)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::divide> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_remainder(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_remainder)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::remainder> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_divmod(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_remainder)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_divmod c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_power(PyObject *self, PyObject *power, PyObject *modulo)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_power)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::power> c(*tuples, __PRETTY_FUNCTION__, !self);
  return actual_call(c, *tuples, self, power, modulo) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_lshift(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_lshift)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::lshift> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_rshift(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_rshift)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::rshift> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_and(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_and)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::nb_and> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_xor(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_xor)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::nb_xor> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_or(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_or)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::nb_or> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_floor_divide(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_floor_divide)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::floor_divide> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_true_divide(PyObject *left, PyObject *right)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_true_divide)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_std<&TypeNumberAny<T>::true_divide> c(*tuples, __PRETTY_FUNCTION__, !left);
  return actual_call(c, *tuples, left, right) ? clear_on_error(c.retval) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_add(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_add)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_add> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_subtract(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_subtract)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_subtract> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_multiply(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_multiply)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_multiply> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_divide(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_divide)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_divide> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_remainder(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_remainder)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_remainder> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_power(PyObject *self, PyObject *power, PyObject *modulo)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_power)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_power> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, power, modulo) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_lshift(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_lshift)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_lshift> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_rshift(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_rshift)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_rshift> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_and(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_and)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_and> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_xor(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_xor)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_xor> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_or(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_or)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_or> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_floor_divide(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_floor_divide)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_floor_divide> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

template <typename T> PyObject *TypeNumberAny<T>::mediator_nb_inplace_true_divide(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_protof, reinterpret_cast<size_t>(mediator_nb_inplace_true_divide)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_inplace<&TypeNumberAny<T>::inplace_true_divide> c(*tuples, self, __PRETTY_FUNCTION__);
  return actual_call_inplace(c, *tuples, !self, args) ? clear_on_error(c.self) : NULL;
}

} //namespace pyhrol

#endif //__pyhrol_type_number_any_hpp__
