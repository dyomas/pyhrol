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

#ifndef __pyhrol_type_base_hpp__
#define __pyhrol_type_base_hpp__

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <tr1/memory>
#include <objimpl.h>
#include <stdlib.h>
#include <string.h>

#include "pyhrol_common.h"
#include "pyhrol_macro.h"
#include "pyhrol_type_base.h"
#include "pyhrol_tuples.h"
#include "pyhrol_tuple_describer.h"
#include "pyhrol_trace.h"
#include "self_addressed_cstring.h"

namespace pyhrol
{

const size_t register_type(PyTypeObject &type);
void unregister_type(size_t);
bool is_error_occured();
PyObject *clear_on_error(PyObject *);
PyObject *pack_tuple(PyObject *, PyObject * = NULL);
const std::string demangle(const std::type_info &ti);

template <class T> const size_t TypeBase<T>::endosome_offset = offsetof(struct TypeBase<T>::T_struct, endosome);

template <class T> TypeBase<T>::TypeBase(const char *name, const char *doc)
  : id (register_type(m_type_object))
{
  PYHROL_TRACE(pyhrol::tpInternal, this, __PRETTY_FUNCTION__);

  m_init(name, doc);
}

template <class T> TypeBase<T>::TypeBase()
  : id (-1)
{
  throw std::logic_error("Default constructor for \"TypeBase<" + demangle(typeid(T)) + ">\" should never called");
}

template <class T> TypeBase<T>::~TypeBase()
{
  PYHROL_TRACE(pyhrol::tpInternal, this, __PRETTY_FUNCTION__);

  for (methods_t::const_iterator iter = m_methods.begin(); iter != m_methods.end(); iter ++)
  {
    if (iter->ml_doc)
    {
      delete TupleDescriber::from_string(iter->ml_doc);
    }
  }
  for (getseters_t::const_iterator iter = m_getseters.begin(); iter != m_getseters.end(); iter ++)
  {
    if (iter->doc)
    {
      delete TupleDescriber::from_string(iter->doc);
    }
  }
  delete SelfAddressedCstring::construct(m_type_object.tp_name);
  if (m_type_object.tp_doc)
  {
    delete TupleDescriber::from_string(m_type_object.tp_doc);
  }

  unregister_type(id);
}

template <class T> const char *TypeBase<T>::name() const
{
  return m_type_object.tp_name;
}

template <class T> TypeBase<T>::operator PyTypeObject &()
{
  return m_type_object;
}

template <class T> TypeBase<T>::operator const PyTypeObject &() const
{
  return m_type_object;
}

template <class T> const PyTypeObject *TypeBase<T>::operator ->() const
{
  return &m_type_object;
}

template <class T> void TypeBase<T>::constructor(T &/*pres*/, Tuples &_args) const
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  throw ImplementationException(ImplementationException::cpntConstructor);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args);
}

template <class T> void TypeBase<T>::m_add_method(const char *name, const PyCFunction &_method, const char *help, const int flags)
{
  PyMethodDef md;

  md.ml_name = name;
  md.ml_meth = _method;
  md.ml_flags = METH_VARARGS | flags;
  md.ml_doc = NULL;
  if (help)
  {
    TupleDescriber *pdescr = TupleDescriber::factory();
    if (pdescr)
    {
      pdescr->help(help);
      md.ml_doc = pdescr->to_string();
    }
  }

  m_methods.insert(-- m_methods.end(), md);
  m_type_object.tp_methods = &m_methods[0];
}

template <class T> void TypeBase<T>::m_add_method(const char *name, const PyCFunctionWithKeywords &_method, const char *help, const int flags)
{
  PyMethodDef md;

  md.ml_name = name;
  md.ml_meth = reinterpret_cast<PyCFunction>(_method);
  md.ml_flags = METH_VARARGS | METH_KEYWORDS | flags;
  md.ml_doc = NULL;
  if (help)
  {
    TupleDescriber *pdescr = TupleDescriber::factory();
    if (pdescr)
    {
      pdescr->help(help);
      md.ml_doc = pdescr->to_string();
    }
  }

  m_methods.insert(-- m_methods.end(), md);
  m_type_object.tp_methods = &m_methods[0];
}

template <class T> void TypeBase<T>::m_add_method(const char *name, const PyNoArgsFunction &_method, const char *help, const int flags)
{
  PyMethodDef md;

  md.ml_name = name;
  md.ml_meth = reinterpret_cast<PyCFunction>(_method);
  md.ml_flags = METH_NOARGS | flags;
  md.ml_doc = NULL;
  if (help)
  {
    TupleDescriber *pdescr = TupleDescriber::factory();
    if (pdescr)
    {
      pdescr->help(help);
      md.ml_doc = pdescr->to_string();
    }
  }

  m_methods.insert(-- m_methods.end(), md);
  m_type_object.tp_methods = &m_methods[0];
}

template <class T> void TypeBase<T>::m_add_getseter(const char *name, const getter &_get, const setter &_set, const char *doc)
{
  PyGetSetDef gsd;
  gsd.name = const_cast<char *>(name);
  gsd.get = _get;
  gsd.set = _set;
  gsd.doc = NULL;
  if (doc)
  {
    TupleDescriber *pdescr = TupleDescriber::factory();
    if (pdescr)
    {
      pdescr->help(doc);
      gsd.doc = const_cast<char *>(pdescr->to_string());
    }
  }
  gsd.closure = NULL;

  m_getseters.insert(-- m_getseters.end(), gsd);
  m_type_object.tp_getset = &m_getseters[0];
}

template <class T> TypeBase<T> &TypeBase<T>::m_get(TypeBase<T> *arg)
{
  static
#if __cplusplus > 201100L
  std::unique_ptr<TypeBase<T>>
#else
  std::auto_ptr<TypeBase<T> >
#endif
  t;
  if (arg)
  {
    if (t.get())
    {
      throw TypeInitException(t.get()->m_type_object.tp_name);
    }
    t.reset(arg);
  }
  TypeBase<T> *retval = t.get();
  if (!retval)
  {
    throw TypeInitException(typeid(T));
  }
  return *retval;
}

template <class T> typename TypeBase<T>::T_struct *TypeBase<T>::allocate()
{
  T_struct *pstruct = PyObject_New(T_struct, &m_type_object);

  if (!pstruct)
  {
    throw std::bad_alloc();
  }

  PYHROL_TRACE(pyhrol::tpInternal, address(pstruct->endosome), __PRETTY_FUNCTION__);

  return pstruct;
}

template <class T> void TypeBase<T>::free(T_struct *pstruct) const throw()
{
  if (pstruct)
  {
    PYHROL_TRACE(pyhrol::tpInternal, address(pstruct->endosome), __PRETTY_FUNCTION__);
    PyObject *pobj = *pstruct;
    pobj->ob_type->tp_free(pobj);
  }
}

template <class T> typename TypeBase<T>::T_struct *TypeBase<T>::allocate_static()
{
  TypeBase &type = m_get();
  T_struct *pstruct = PyObject_New(T_struct, &type.m_type_object);

  if (!pstruct)
  {
    throw std::bad_alloc();
  }

  PYHROL_TRACE(pyhrol::tpInternal, type.address(pstruct->endosome), __PRETTY_FUNCTION__);

  return pstruct;
}

template <class T> void TypeBase<T>::free_static(T_struct *pstruct) throw()
{
  if (pstruct)
  {
    PYHROL_TRACE(pyhrol::tpInternal, m_get().address(pstruct->endosome), __PRETTY_FUNCTION__);
    PyObject *pobj = *pstruct;
    pobj->ob_type->tp_free(pobj);
  }
}

template <class T> void TypeBase<T>::m_init(const char *name, const char *doc)
{
  bzero(&m_type_object, sizeof(m_type_object));
  PyTypeObject _t_template = {PyObject_HEAD_INIT(NULL)};
  memcpy(&m_type_object, &_t_template, sizeof(PyVarObject));

  SelfAddressedCstring *pstr = new SelfAddressedCstring;
  const size_t len_name = strlen(name);
  char *ptr = pstr->allocate(len_name + 1);
  memcpy(ptr, name, len_name + 1);

  /* NOTE
  Pointer to PyTypeObject::tp_name points to SelfAddressedCstring member
  This fact used by:
    -- Container while prepending module name (m_qualify_types_with_module_name)
    -- destructor of this object (~TypeBase)
  */
  pstr->get(m_type_object.tp_name);
  m_type_object.tp_basicsize = sizeof(T_struct);
  m_type_object.tp_dealloc = mediator_destructor;
  m_type_object.tp_print = mediator_print;
  m_type_object.tp_repr = mediator_repr;
  m_type_object.tp_str = mediator_str;
  m_type_object.tp_flags = Py_TPFLAGS_DEFAULT;
  if (doc)
  {
    TupleDescriber *pdescr = TupleDescriber::factory();
    if (pdescr)
    {
      pdescr->help(doc);
      m_type_object.tp_doc = pdescr->to_string();
    }
  }
  m_type_object.tp_new = mediator_constructor;

  PyMethodDef md_sentinel;
  PyGetSetDef gs_sentinel;

  bzero(&md_sentinel, sizeof(PyMethodDef));
  bzero(&gs_sentinel, sizeof(PyGetSetDef));
  m_methods.push_back(md_sentinel);
  m_getseters.push_back(gs_sentinel);
}


template <class T> PyObject *TypeBase<T>::mediator_constructor(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  static tuples_data_auto_release_t data(TuplesData::factory(v_ctor, reinterpret_cast<size_t>(mediator_constructor)));
  tuples_auto_release_t tuples(Tuples::factory(*data));
  call_constructor c(*tuples, type, __PRETTY_FUNCTION__, !type);
  tuples->ubiquitous_caller(c, args, kwds, !type);
  return clear_on_error(c.retval);
}

template <class T> void TypeBase<T>::call_constructor::call()
{
  const TypeBase<T> &type = m_get();

  if (is_probe)
  {
    type.constructor(*reinterpret_cast<T *>(NULL), args);
  }
  else
  {
    T_struct *pstruct = reinterpret_cast<T_struct *>(ptype->tp_alloc(ptype, 0));
    if (pstruct)
    {
      try
      {
        type.constructor(pstruct->endosome, args);
        PYHROL_TRACE (pyhrol::tpMediator, type.address(pstruct->endosome), signature);
        retval = *pstruct;
      }
      catch (...)
      {
        ptype->tp_free(pstruct);
        throw;
      }
    }
  }
}

template <class T> TypeBase<T>::call_constructor::call_constructor(Tuples &_args, PyTypeObject *_type, const char *_signature, const bool _is_probe)
  : is_probe(_is_probe)
  , args(_args)
  , ptype(_type)
  , retval(NULL)
  , signature(_signature)
{
}

template <class T> void TypeBase<T>::call_destructor::call()
{
  T &arg = reinterpret_cast<T_struct *>(self)->endosome;
  PYHROL_TRACE (pyhrol::tpMediator, type.address(arg), signature);
  try
  {
    type.destructor(arg);
    self->ob_type->tp_free(self);
  }
  catch (...)
  {
    self->ob_type->tp_free(self);
    throw;
  }
}

template <class T> TypeBase<T>::call_destructor::call_destructor(PyObject *_self, const char *_signature)
  : self(_self)
  , signature(_signature)
  , type(m_get())
{
}

template <class T> void TypeBase<T>::call_print::call()
{
  PYHROL_TRACE (pyhrol::tpMediator, type.address(self), signature);
  std::ostringstream ostr;
  type.print(ostr, self);
  fwrite(ostr.str().c_str(), ostr.str().length(), 1, fp);
  fsync(fileno(fp));
  retval = 0;
}

template <class T> TypeBase<T>::call_print::call_print(const PyObject *_self, FILE *_fp, const char *_signature)
  : self(reinterpret_cast<const T_struct *>(_self)->endosome)
  , fp(_fp)
  , signature(_signature)
  , type(m_get())
  , retval(1)
{
}

template <class T> void TypeBase<T>::call_repr::call()
{
  PYHROL_TRACE (pyhrol::tpMediator, type.address(self), signature);
  retval = PyString_FromFormat("<%s>", type.repr(self).c_str());
}

template <class T> TypeBase<T>::call_repr::call_repr(const PyObject *_self, const char *_signature)
  : self(reinterpret_cast<const T_struct *>(_self)->endosome)
  , signature(_signature)
  , type(m_get())
  , retval(NULL)
{
}

template <class T> void TypeBase<T>::call_str::call()
{
  PYHROL_TRACE (pyhrol::tpMediator, type.address(self), signature);
  std::ostringstream ostr;
  type.str(ostr, self);
  retval = PyString_FromStringAndSize(ostr.str().c_str(), ostr.str().length());
}

template <class T> TypeBase<T>::call_str::call_str(const PyObject *_self, const char *_signature)
  : self(reinterpret_cast<const T_struct *>(_self)->endosome)
  , signature(_signature)
  , type(m_get())
  , retval(NULL)
{
}

template <class T> void TypeBase<T>::mediator_destructor(PyObject* self)
{
  call_destructor cd(self, __PRETTY_FUNCTION__);
  exceptionHandler::call(cd, reinterpret_cast<size_t>(mediator_destructor));
}

template <class T> int TypeBase<T>::mediator_print(PyObject *self, FILE *fp, int /*flags*/)
{
  call_print cp(self, fp, __PRETTY_FUNCTION__);
  exceptionHandler::call(cp, reinterpret_cast<size_t>(mediator_print));
  return cp.retval;
}

template <class T> PyObject *TypeBase<T>::mediator_repr(PyObject *self)
{
  call_repr cr(self, __PRETTY_FUNCTION__);
  exceptionHandler::call(cr, reinterpret_cast<size_t>(mediator_repr));
  return cr.retval;
}

template <class T> PyObject *TypeBase<T>::mediator_str(PyObject *self)
{
  call_str cs(self, __PRETTY_FUNCTION__);
  exceptionHandler::call(cs, reinterpret_cast<size_t>(mediator_str));
  return cs.retval;
}

} //namespace pyhrol

#endif //__pyhrol_type_base_hpp__
