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

// $Date: 2014-02-27 22:19:34 +0400 (Thu, 27 Feb 2014) $
// $Revision: 896 $

#include "pyhrol.h"
#include "pyhrol_self_modules.h"
#include "pyhrol_self_types.h"
#include "pyhrol_self_methods.h"
#include "pyhrol_trace.h"
#include "pyhrol_tuple_format.h"
#include "pyhrol_container.h"
#include "nullable.hpp"


using namespace std;

namespace pyhrol
{

type::type(const size_t _pos, PyObject *obj)
  : AutoHolder(obj)
  , pos(_pos)
{
}

types_iterator::types_iterator(PyObject *obj)
  : AutoHolder(obj)
  , iter(Index::etType)
{
}

types_iterator::types_iterator(const size_t module_pos, PyObject *obj)
  : AutoHolder(obj)
  , iter(Index::etType, module_pos)
{
}

types::types()
  : AutoHolder(NULL)
  , all(true)
  , module_pos()
{
}

types::types(const size_t pos, PyObject *obj)
  : AutoHolder(obj)
  , all(false)
  , module_pos(pos)
{
}

Type::Type()
  : TypeBase<type>("Type", NULL)
{
  m_add_getter<Type, &Type::get_name>("name", NULL);
  m_add_getter<Type, &Type::get_doc>("doc", NULL);
  m_add_getter<Type, &Type::get_methods>("methods", NULL);
  m_add_getter<Type, &Type::get_module>("module", NULL);
  m_add_getter<Type, &Type::get_flags>("flags", NULL);
  m_add_member<const size_t, &type::pos>("pos", NULL);
}

void Type::init()
{
  m_get(new Type);
}

void Type::destructor(type &obj) const
{
  obj.~type();
}

void Type::get_name(const Ptr<const type> &ptr, Tuples &_args) const
{
  const char *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Container &c = Container::container();
  retval = isolate_type_name(c.type(c.index().get(ptr->pos)->offset)->tp_name);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Type::get_doc(const Ptr<const type> &ptr, Tuples &_args) const
{
  const char *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Container &c = Container::container();
  retval = c.type(c.index().get(ptr->pos)->offset)->tp_doc;

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Type::get_methods(const Ptr<const type> &ptr, Tuples &_args) const
{
  PyObject *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Methods::T_struct *pobj = Methods::allocate_static();
  retval = *pobj;
  new (&pobj->endosome) Methods::endosome_t(ptr->pos, ptr);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Type::get_module(const Ptr<const type> &ptr, Tuples &_args) const
{
  PyObject *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Module::T_struct *pobj = Module::allocate_static();
  retval = *pobj;

  new (&pobj->endosome) Module::endosome_t(ptr->pos + Container::container().index().get(ptr->pos)->parent, NULL);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Type::get_flags(const pyhrol::Ptr<const type> &obj, pyhrol::Tuples &_args) const
{
  type_flags_struct retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  _args.set_options(optTuple);
  PYHROL_BUILD_VALUE_2((string("\nstring equivalent: ") + type_flags_struct::init()->statement()).c_str(), _args, retval.get(), retval.as_string().c_str())
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Container &c = Container::container();
  static_cast<SmartFlagInt &>(retval) = c.type(c.index().get(obj->pos)->offset)->tp_flags;

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

TypesIterator::TypesIterator()
  : TypeBase<types_iterator>("TypesIterator", NULL)
{
  m_type_object.tp_iter = NULL;
}

void TypesIterator::init()
{
  m_get(new TypesIterator);
}

void TypesIterator::destructor(types_iterator &obj) const
{
  obj.~types_iterator();
}

bool TypesIterator::next(const Ptr<type> &itm, const Ptr<types_iterator> &i) const
{
  Index::iterator &iter = i->iter;
  const bool retval = Container::container().index().iterate(iter);

  if (retval)
  {
    new (&*itm) type(iter.pos.getValue(), i);
  }
  return retval;
}


Types::Types()
  : TypeBase<types>("Types", NULL)
{
  m_type_object.tp_iternext = NULL;
  m_add_member<const size_t, &types::module_pos>("module_pos", NULL);
}

void Types::init()
{
  m_get(new Types);
}

void Types::constructor(endosome_t &res, pyhrol::Tuples &_args) const
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  new (&res) Types::endosome_t();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Types::destructor(endosome_t &obj) const
{
  obj.~endosome_t();
}

void Types::iter(const Ptr<types_iterator> &i, const Ptr<const types> &t) const
{
  if (t->all)
  {
    new (&*i) types_iterator(t);
  }
  else
  {
    new (&*i) types_iterator(t->module_pos, t);
  }
}

void Types::get(const Ptr<const endosome_t> &types, pyhrol::Tuples &_args) const
{
  PyObject *retval;
  const char *name;

  PYHROL_PARSE_TUPLE_1(NULL, _args, name)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Index &idx = Container::container().index();
  size_t pos;
  if (types->all)
  {
    pos = idx.find(name, Index::etType);
  }
  else
  {
    pos = idx.find(name, Index::etType, types->module_pos);
  }

  Type::T_struct *pobj = Type::allocate_static();
  retval = *pobj;
  new(&pobj->endosome) type(pos, types);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

} //namespace pyhrol

using namespace pyhrol;


SmartFlagInt::ftree_t type_flags_struct::m_ftree;

type_flags_struct::type_flags_struct()
  : SmartFlagInt(init())
{
}

const SmartFlagInt::ftree_t *type_flags_struct::init()
{
  if (!m_ftree.filled())
  {
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_GETCHARBUFFER, "HAVE_GETCHARBUFFER");
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_SEQUENCE_IN, "HAVE_SEQUENCE_IN", Py_TPFLAGS_HAVE_GETCHARBUFFER);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_INPLACEOPS, "HAVE_INPLACEOPS", Py_TPFLAGS_HAVE_SEQUENCE_IN);
    m_ftree.add(npAnd, Py_TPFLAGS_CHECKTYPES, "CHECKTYPES", Py_TPFLAGS_HAVE_INPLACEOPS);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_RICHCOMPARE, "HAVE_RICHCOMPARE", Py_TPFLAGS_CHECKTYPES);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_WEAKREFS, "HAVE_WEAKREFS", Py_TPFLAGS_HAVE_RICHCOMPARE);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_ITER, "HAVE_ITER", Py_TPFLAGS_HAVE_WEAKREFS);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_CLASS, "HAVE_CLASS",Py_TPFLAGS_HAVE_ITER );
    m_ftree.add(npAnd, Py_TPFLAGS_HEAPTYPE, "HEAPTYPE", Py_TPFLAGS_HAVE_CLASS);
    m_ftree.add(npAnd, Py_TPFLAGS_BASETYPE, "BASETYPE", Py_TPFLAGS_HEAPTYPE);
    m_ftree.add(npAnd, Py_TPFLAGS_READY, "READY", Py_TPFLAGS_BASETYPE);
    m_ftree.add(npAnd, Py_TPFLAGS_READYING, "READYING", Py_TPFLAGS_READY);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_GC, "HAVE_GC", Py_TPFLAGS_READYING);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_STACKLESS_EXTENSION, "HAVE_STACKLESS_EXTENSION", Py_TPFLAGS_HAVE_GC);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_INDEX, "HAVE_INDEX", Py_TPFLAGS_HAVE_STACKLESS_EXTENSION);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_VERSION_TAG, "HAVE_VERSION_TAG", Py_TPFLAGS_HAVE_INDEX);
    m_ftree.add(npAnd, Py_TPFLAGS_VALID_VERSION_TAG, "VALID_VERSION_TAG", Py_TPFLAGS_HAVE_VERSION_TAG);
    m_ftree.add(npAnd, Py_TPFLAGS_IS_ABSTRACT, "IS_ABSTRACT", Py_TPFLAGS_VALID_VERSION_TAG);
    m_ftree.add(npAnd, Py_TPFLAGS_HAVE_NEWBUFFER, "HAVE_NEWBUFFER", Py_TPFLAGS_IS_ABSTRACT);
    m_ftree.add(npAnd, Py_TPFLAGS_INT_SUBCLASS, "INT_SUBCLASS", Py_TPFLAGS_HAVE_NEWBUFFER);
    m_ftree.add(npAnd, Py_TPFLAGS_LONG_SUBCLASS, "LONG_SUBCLASS", Py_TPFLAGS_INT_SUBCLASS);
    m_ftree.add(npAnd, Py_TPFLAGS_LIST_SUBCLASS, "LIST_SUBCLASS", Py_TPFLAGS_LONG_SUBCLASS);
    m_ftree.add(npAnd, Py_TPFLAGS_TUPLE_SUBCLASS, "TUPLE_SUBCLASS", Py_TPFLAGS_LIST_SUBCLASS);
    m_ftree.add(npAnd, Py_TPFLAGS_STRING_SUBCLASS, "STRING_SUBCLASS", Py_TPFLAGS_TUPLE_SUBCLASS);
    m_ftree.add(npAnd, Py_TPFLAGS_UNICODE_SUBCLASS, "UNICODE_SUBCLASS", Py_TPFLAGS_STRING_SUBCLASS);
    m_ftree.add(npAnd, Py_TPFLAGS_DICT_SUBCLASS, "DICT_SUBCLASS", Py_TPFLAGS_UNICODE_SUBCLASS);
    m_ftree.add(npAnd, Py_TPFLAGS_BASE_EXC_SUBCLASS, "BASE_EXC_SUBCLASS", Py_TPFLAGS_DICT_SUBCLASS);
    m_ftree.add(npAnd, Py_TPFLAGS_TYPE_SUBCLASS, "TYPE_SUBCLASS", Py_TPFLAGS_BASE_EXC_SUBCLASS);
  }
  return &m_ftree;
}




