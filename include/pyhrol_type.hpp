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

// $Date: 2013-12-06 01:08:07 +0400 (Fri, 06 Dec 2013) $
// $Revision: 859 $

#ifndef __pyhrol_type_hpp__
#define __pyhrol_type_hpp__

#include <structmember.h>
#include <sstream>
#ifdef _DEBUG_PYTHON_PYMEMBERS
#include <iomanip>
#endif //_DEBUG_PYTHON_PYMEMBERS

#include "pyhrol_type.h"
#include "pyhrol_type_base.hpp"
#include "pyhrol_type_traits.hpp"
#include "pyhrol_trace.h"
#include "self_addressed_cstring.h"


namespace pyhrol
{

namespace __type_internal
{

template <typename T, typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const, void (O::*S)(const Ptr<T> &, Tuples &) const> TuplesData &data(const void *ptr)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_getsetter, ptr));
  return *data;
}

}; //namespace __type_internal


template <class T, class I> template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::call_ptr<O, F>::call()
{
  T *pobj = self ? &(reinterpret_cast<typename TypeBase<T>::T_struct *>(self))->endosome : NULL;
  if (pobj)
  {
    PYHROL_TRACE(tpMediator, pobj, signature);
  }
  (dynamic_cast<O &>(TypeBase<T>::m_get()).*F)(Ptr<T>(pobj, self), args);
}

template <class T, class I> template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> TypeWrapper<T, I>::call_ptr<O, F>::call_ptr(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , signature(_signature)
{
}


template <class T, class I> template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> void TypeWrapper<T, I>::call_ptr_const<O, F>::call()
{
  const T *pobj = self ? &(reinterpret_cast<const typename TypeBase<T>::T_struct *>(self))->endosome : NULL;
  if (pobj)
  {
    PYHROL_TRACE(tpMediator, pobj, signature);
  }
  (dynamic_cast<O &>(TypeBase<T>::m_get()).*F)(Ptr<const T>(pobj, self), args);
}

template <class T, class I> template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> TypeWrapper<T, I>::call_ptr_const<O, F>::call_ptr_const(Tuples &_args, PyObject *_self, const char *_signature)
  : args(_args)
  , self(_self)
  , signature(_signature)
{
}


template <class T, class I> template <typename P, P T::*M> void TypeWrapper<T, I>::m_add_member(const char *name, const char *doc)
{
  bool is_readonly;
  P *x = NULL;
  const int type = TypeTraits::get_type_id(x, is_readonly);

  PyMemberDef md = {
      NULL //name
    , type //type
    , TypeBase<T>::endosome_offset + reinterpret_cast<size_t>(&(reinterpret_cast<T*>(NULL)->*M)) //offset
    , is_readonly ? READONLY : 0 //flags
    , NULL //doc
  };

  if (name)
  {
    SelfAddressedCstring *pstr = new SelfAddressedCstring;
    const size_t len = strlen(name) + 1;
    memcpy(pstr->allocate(len), name, len);
    pstr->get(md.name);
  }

  std::ostringstream ostr;
  if (doc)
  {
    ostr << doc << ' ';
  }
  ostr << "(" << TypeTraits::to_string(type);
  if (is_readonly)
  {
    ostr << ", RO";
  }
  ostr <<  ")";
  SelfAddressedCstring *pstr = new SelfAddressedCstring;
  const size_t len = ostr.str().length() + 1;
  memcpy(pstr->allocate(len), ostr.str().c_str(), len);
  pstr->get(md.doc);

  m_members.insert(-- m_members.end(), md);
  TypeBase<T>::m_type_object.tp_members = &m_members[0];
}

template <class T, class I> const void *TypeWrapper<T, I>::address(const T &arg) const
{
  return &arg;
}

template <class T, class I> void TypeWrapper<T, I>::print(std::ostream &os, const T &obj) const
{
  os
    << TypeBase<T>::m_type_object.tp_name << '@' << std::hex << reinterpret_cast<size_t>(&obj) << std::dec << std::endl
  ;
}

template <class T, class I> const std::string TypeWrapper<T, I>::repr(const T &obj) const
{
  std::ostringstream ostr;
  ostr
    << TypeBase<T>::m_type_object.tp_name << '@' << std::hex << reinterpret_cast<size_t>(&obj) << std::dec
  ;
  return ostr.str();
}

template <class T, class I> void TypeWrapper<T, I>::str(std::ostream &os, const T &obj) const
{
  os
    << TypeBase<T>::m_type_object.tp_name << '@' << std::hex << reinterpret_cast<size_t>(&obj) << std::dec << std::endl
  ;
}

#ifdef _DEBUG_PYTHON_PYMEMBERS
template <class T, class I> void TypeWrapper<T, I>::m_show_members(std::ostream &os) const
{
  if (m_members.size())
  {
    os
      << std::left << std::setw(4) << "#"
      << std::setw(40) << "name"
      << std::setw(6) << "type"
      << std::setw(20) << "type name";
    if (getenv("__PY_SHOW_MEMBERS_OFFSET"))
    {
      os << std::setw(5) << "off";
    }
    os
      << std::setw(5) << "flags" << std::right << std::endl
    ;
  }

  for (members_t::const_iterator iter = m_members.begin(), end = -- m_members.end(); iter != end; iter ++)
  {
    const PyMemberDef &md = *iter;
    os
      << std::setw(3) << (iter - m_members.begin())
      << ' ' << std::setw(40) << std::left << md.name << std::right
      << std::setw(5) << md.type
      << ' ' << std::setw(20) << std::left << TypeTraits::to_string(md.type) << std::right;
    if (getenv("__PY_SHOW_MEMBERS_OFFSET"))
    {
      os << std::setw(5) << md.offset;
    }
    os
      << std::setw(5) << md.flags << std::endl
    ;
  }
}
#endif //_DEBUG_PYTHON_PYMEMBERS

template <class T, class I> void TypeWrapper<T, I>::m_init()
{
  PyMemberDef sentinel;

  bzero(&sentinel, sizeof(PyMemberDef));
  m_members.push_back(sentinel);
}

template <class T, class I> TypeWrapper<T, I>::TypeWrapper()
{
  m_init();
}

template <class T, class I> TypeWrapper<T, I>::~TypeWrapper()
{
  for (members_t::const_iterator iter = m_members.begin(), end = m_members.end(); iter != end; iter ++)
  {
    delete SelfAddressedCstring::construct(iter->name);
    delete SelfAddressedCstring::construct(iter->doc);
  }
}

template <class T, class I> PyObject *TypeWrapper<T, I>::convert(T &obj)
{
  PyObject *ret_val = NULL;

  typename TypeBase<T>::T_struct *pstruct = NULL;
  try
  {
    pstruct = TypeBase<T>::allocate();
    new (&pstruct->endosome) T(obj);
    ret_val = *pstruct;
  }
  catch (...)
  {
    TypeBase<T>::free(pstruct);
    throw;
  }

  PYHROL_TRACE(pyhrol::tpMediator, &pstruct->endosome, __PRETTY_FUNCTION__);

  return ret_val;
}

template <typename T, typename I> template <void (I::*F)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method2<I, F>, help);
}

template <typename T, typename I> template <void (I::*F)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method3<I, F>, help);
}

template <typename T, typename I> template <void (I::*F)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method<I, F>, help);
}

template <typename T, typename I> template <void (I::*F)(const Ptr<const T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method2<I, F>, help);
}

template <typename T, typename I> template <void (I::*F)(const Ptr<const T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method3<I, F>, help);
}

template <typename T, typename I> template <void (I::*F)(const Ptr<const T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method<I, F>, help);
}

template <typename T, typename I> template <void (I::*G)(const Ptr<const T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_getter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, m_getter<I, G>, NULL, doc);
}

template <typename T, typename I> template <void (I::*S)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_setter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, NULL, m_setter<I, S>, doc);
}

template <typename T, typename I> template <void (I::*G)(const Ptr<const T> &, Tuples &) const, void (I::*S)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_getseter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, m_getter_paired<I, G, S>, m_setter_paired<I, G, S>, doc);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method2<O, F>, help);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method3<O, F>, help);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method<O, F>, help);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method2<O, F>, help);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method_with_keywords(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method3<O, F>, help);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_method_no_args(const char *name, const char *help)
{
  TypeBase<T>::m_add_method(name, m_method<O, F>, help);
}

template <typename T, typename I> template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_getter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, m_getter<O, G>, NULL, doc);
}

template <typename T, typename I> template <typename O, void (O::*S)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_setter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, NULL, m_setter<O, S>, doc);
}

template <typename T, typename I> template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const, void (O::*S)(const Ptr<T> &, Tuples &) const> void TypeWrapper<T, I>::m_add_getseter(const char *name, const char *doc)
{
  TypeBase<T>::m_add_getseter(name, m_getter_paired<O, G, S>, m_setter_paired<O, G, S>, doc);
}


template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> PyObject *TypeWrapper<T, I>::m_method(PyObject *self)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_method_no_arg, reinterpret_cast<void *>(m_method<O, F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_ptr<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, NULL, NULL, !self);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> PyObject *TypeWrapper<T, I>::m_method2(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_method, reinterpret_cast<void *>(m_method2<O, F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_ptr<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, args, NULL, !self);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<T> &, Tuples &) const> PyObject *TypeWrapper<T, I>::m_method3(PyObject *self, PyObject *args, PyObject *kwds)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_method_kw, reinterpret_cast<void *>(m_method3<O, F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_ptr<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, args, kwds, !self);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> PyObject *TypeWrapper<T, I>::m_method(PyObject *self)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_method_no_arg_const, reinterpret_cast<void *>(m_method<O, F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_ptr_const<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, NULL, NULL, !self);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> PyObject *TypeWrapper<T, I>::m_method2(PyObject *self, PyObject *args)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_method_const, reinterpret_cast<void *>(m_method2<O, F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_ptr_const<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, args, NULL, !self);
}

template <typename T, typename I> template <typename O, void (O::*F)(const Ptr<const T> &, Tuples &) const> PyObject *TypeWrapper<T, I>::m_method3(PyObject *self, PyObject *args, PyObject *kwds)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_method_kw_const, reinterpret_cast<void *>(m_method3<O, F>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_ptr_const<O, F> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, args, kwds, !self);
}

template <typename T, typename I> template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const> PyObject *TypeWrapper<T, I>::m_getter(PyObject *self, void */*closure*/)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_getter, reinterpret_cast<void *>(m_getter<O, G>)));
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_ptr_const<O, G> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, NULL, NULL, !self);
}

template <typename T, typename I> template <typename O, void (O::*S)(const Ptr<T> &, Tuples &) const> int TypeWrapper<T, I>::m_setter(PyObject *self, PyObject *value, void */*closure*/)
{
  static std::auto_ptr<TuplesData> data(TuplesData::factory(v_setter, reinterpret_cast<void *>(m_setter<O, S>)));
  PyObject *value_packed = pack_tuple(value);
  if (value && !value_packed)
  {
    return -1;
  }
  std::auto_ptr<Tuples> tuples(Tuples::factory(*data));
  call_ptr<O, S> c(*tuples, self, __PRETTY_FUNCTION__ );
  tuples->ubiquitous_caller(c, value_packed, NULL, !self);
  Py_DecRef(value_packed);
  return is_error_occured() ? -1 : 0;
}

template <typename T, typename I> template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const, void (O::*S)(const Ptr<T> &, Tuples &) const> PyObject *TypeWrapper<T, I>::m_getter_paired(PyObject *self, void */*closure*/)
{
  std::auto_ptr<Tuples> tuples(Tuples::factory(__type_internal::data<T, O, G, S>(reinterpret_cast<void *>(m_getter_paired<O, G, S>))));
  call_ptr_const<O, G> c(*tuples, self, __PRETTY_FUNCTION__ );
  return tuples->ubiquitous_caller(c, NULL, NULL, !self, Tuples::ctGetter);
}

template <typename T, typename I> template <typename O, void (O::*G)(const Ptr<const T> &, Tuples &) const, void (O::*S)(const Ptr<T> &, Tuples &) const> int TypeWrapper<T, I>::m_setter_paired(PyObject *self, PyObject *value, void */*closure*/)
{
  PyObject *value_packed = pack_tuple(value);
  if (value && !value_packed)
  {
    return -1;
  }
  std::auto_ptr<Tuples> tuples(Tuples::factory(__type_internal::data<T, O, G, S>(reinterpret_cast<void *>(m_setter_paired<O, G, S>))));
  call_ptr<O, S> c(*tuples, self, __PRETTY_FUNCTION__ );
  tuples->ubiquitous_caller(c, value_packed, NULL, !self, Tuples::ctSetter);
  Py_DecRef(value_packed);
  return is_error_occured() ? -1 : 0;
}

} //namespace pyhrol

#endif //__pyhrol_type_hpp__

