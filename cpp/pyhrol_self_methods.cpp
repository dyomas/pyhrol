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

// $Date: 2015-05-29 00:10:31 +0300 (Пт., 29 мая 2015) $
// $Revision: 1034 $

#include "pyhrol.h"
#include "pyhrol_self_modules.h"
#include "pyhrol_self_types.h"
#include "pyhrol_self_methods.h"
#include "pyhrol_trace.h"
#include "pyhrol_flags.h"
#include "pyhrol_flags.h"
#include "nullable.hpp"
#include "smart_flag.hpp"

using namespace std;

namespace pyhrol
{

method::method(const size_t _pos, PyObject *obj)
  : AutoHolder(obj)
  , pos(_pos)
{
}

methods_iterator::methods_iterator(PyObject *obj)
  : AutoHolder(obj)
  , iter(Index::etMethod)
{
}

methods_iterator::methods_iterator(const methodKinds _kind, PyObject *obj)
  : AutoHolder(obj)
  , kind(_kind)
  , iter(Index::etMethod)
{
}

methods_iterator::methods_iterator(const size_t type_pos, PyObject *obj)
  : AutoHolder(obj)
  , iter(Index::etMethod, type_pos)
{
}

methods::methods()
  : AutoHolder(NULL)
{
}

methods::methods(const methodKinds _kind)
  : AutoHolder(NULL)
  , kind(_kind)
{
}

methods::methods(const size_t _pos, PyObject *obj)
  : AutoHolder(obj)
  , type_pos(_pos)
{
}


Method::Method()
  : TypeBase<method>("Method", NULL)
{
  m_add_member<const size_t, &method::pos>("pos", NULL);
  m_add_getter<Method, &Method::get_name>("name", NULL);
  m_add_getter<Method, &Method::get_doc>("doc", NULL);
  m_add_getter<Method, &Method::get_type>("type", NULL);
  m_add_getter<Method, &Method::get_module>("module", NULL);
  m_add_getter<Method, &Method::get_kind>("kind", NULL);
  m_add_getter<Method, &Method::get_flags>("flags", NULL);
}

void Method::init()
{
  m_get(new Method);
}

void Method::destructor(method &obj) const
{
  obj.~method();
}

void Method::get_name(const Ptr<const method> &obj, Tuples &_args) const
{
  const char *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Container &c = Container::container();
  const Index::item
      *pmethod = c.index().get(obj->pos)
    , *ptype = pmethod + pmethod->parent
  ;

  switch (pmethod->et)
  {
    case Index::etCtor:
      retval = Container::ctor_caption.c_str();
      break;
    case Index::etMethod:
      retval = c.type(ptype->offset)->tp_methods[pmethod->offset].ml_name;
      break;
    case Index::etGetsetter:
      retval = c.type(ptype->offset)->tp_getset[pmethod->offset].name;
      break;
    case Index::etSpecialCall:
      retval = c.protof_name(ptype->offset, pmethod->offset);
      break;
    default:
      {
        ostringstream ostr;
        ostr << "Object type `" << index_element_types_flag_t(pmethod->et) << "` unexpected at position " << obj->pos;
        throw logic_error(ostr.str());
      }
  }

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Method::get_doc(const Ptr<const method> &obj, Tuples &_args) const
{
  const char *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Container &c = Container::container();
  const Index::item
      *pmethod = c.index().get(obj->pos)
    , *ptype = pmethod + pmethod->parent
  ;

  switch (pmethod->et)
  {
    case Index::etCtor:
      retval = c.type(ptype->offset)->tp_doc;
      break;
    case Index::etMethod:
      retval = c.type(ptype->offset)->tp_methods[pmethod->offset].ml_doc;
      break;
    case Index::etGetsetter:
      retval = c.type(ptype->offset)->tp_getset[pmethod->offset].doc;
      break;
    case Index::etSpecialCall:
      retval = c.protof_doc(ptype->offset, pmethod->offset);
      break;
    default:
      {
        ostringstream ostr;
        ostr << "Object type `" << index_element_types_flag_t(pmethod->et) << "` unexpected at position " << obj->pos;
        throw logic_error(ostr.str());
      }
  }

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Method::get_type(const Ptr<const method> &obj, Tuples &_args) const
{
  PyObject *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Type::T_struct *pobj = Type::allocate_static();
  retval = *pobj;

  new (&pobj->endosome) Type::endosome_t(obj->pos + Container::container().index().get(obj->pos)->parent, NULL);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Method::get_module(const Ptr<const method> &obj, Tuples &_args) const
{
  PyObject *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Module::T_struct *pobj = Module::allocate_static();
  retval = *pobj;

  const Index &idx = Container::container().index();
  const size_t pos_type = obj->pos + idx.get(obj->pos)->parent;
  new (&pobj->endosome) Module::endosome_t(pos_type + idx.get(pos_type)->parent, NULL);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Method::get_kind(const Ptr<const method> &obj, Tuples &_args) const
{
  string retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1((string("one of the following: ") + method_kinds_flag_t()->statement().c_str()).c_str(), _args, retval.c_str())
  PYHROL_AFTER_BUILD_VALUE(_args)

  retval = method_kinds_flag_t(static_cast<methodKinds>(Container::container().index().get(obj->pos)->et)).to_string();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Method::get_flags(const Ptr<const method> &obj, Tuples &_args) const
{
  method_flags_struct retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  _args.set_options(optTuple);
  PYHROL_BUILD_VALUE_2((string("\nstring equivalent: ") + method_flags_struct::init()->statement()).c_str(), _args, retval.get(), retval.as_string().c_str())
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Container &c = Container::container();
  const Index::item
      *pmethod = c.index().get(obj->pos)
    , *ptype = pmethod + pmethod->parent
  ;

  if (pmethod->et == Index::etMethod)
  {
    static_cast<SmartFlagInt &>(retval) = c.type(ptype->offset)->tp_methods[pmethod->offset].ml_flags;
  }
  else
  {
    ostringstream ostr;
    ostr << "Object of type `" << index_element_types_flag_t(pmethod->et) << "` at position " << obj->pos << " unexpected";
    throw logic_error(ostr.str());
  }

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

MethodsIterator::MethodsIterator()
  : TypeBase<methods_iterator>("MethodsIterator", NULL)
{
  m_type_object.tp_iter = NULL;
}

void MethodsIterator::init()
{
  m_get(new MethodsIterator);
}

void MethodsIterator::destructor(methods_iterator &obj) const
{
  obj.~endosome_t();
}

bool MethodsIterator::next(const Ptr<method> &itm, const Ptr<methods_iterator> &i) const
{
  const Index &idx = Container::container().index();
  Index::iterator &iter = i->iter;

  bool retval;

  while ((retval = idx.iterate(iter)))
  {
    if (!i->kind.assigned() || i->kind.getValue() == static_cast<methodKinds>(idx.get(iter.pos.getValue())->et))
    {
      new (&*itm) method(iter.pos.getValue(), i);
      break;
    }
  }
  return retval;
}


Methods::Methods()
  : TypeBase<methods>("Methods", NULL)
{
  m_type_object.tp_iternext = NULL;
}

void Methods::init()
{
  m_get(new Methods);
}

void Methods::constructor(endosome_t &res, Tuples &_args) const
{
  const char *kind_str = NULL;

  _args.set_options(0);
  PYHROL_PARSE_TUPLE_1((string("filter by method kind: ") + method_kinds_flag_t()->statement()).c_str(), _args, kind_str)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  if (kind_str)
  {
    new(&res) endosome_t(method_kinds_flag_t(kind_str));
  }
  else
  {
    new(&res) endosome_t();
  }
  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Methods::destructor(endosome_t &res) const
{
  res.~endosome_t();
}

void Methods::iter(const Ptr<methods_iterator> &m, const Ptr<const methods> &ms) const
{
  if (ms->kind.assigned())
  {
    new (&*m) methods_iterator(ms->kind.getValue(), ms);
  }
  else if (ms->type_pos.assigned())
  {
    new (&*m) methods_iterator(ms->type_pos.getValue(), ms);
  }
  else
  {
    new (&*m) methods_iterator(ms);
  }
}

void Methods::get(const Ptr<const endosome_t> &methods, Tuples &_args) const
{
  PyObject *retval;
  const char *name;

  PYHROL_PARSE_TUPLE_1(NULL, _args, name)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Index &idx = Container::container().index();

  size_t pos;
  if (methods->kind.assigned())
  {
    if (methods->type_pos.assigned())
    {
      pos = idx.find(name, static_cast<Index::elementTypes>(methods->kind.getValue()), methods->type_pos.getValue());
    }
    else
    {
      pos = idx.find(name, static_cast<Index::elementTypes>(methods->kind.getValue()));
    }
  }
  else
  {
    const Index::elementTypes et[] = {Index::etCtor, Index::etGetsetter, Index::etMethod, Index::etSpecialCall};
    if (methods->type_pos.assigned())
    {
      pos = idx.find(name, et, 4, methods->type_pos.getValue());
    }
    else
    {
      pos = idx.find(name, et, 4);
    }
  }

  Method::T_struct *pobj = Method::allocate_static();
  retval = *pobj;
  new(&pobj->endosome) method(pos, methods);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

} //namespace pyhrol

using namespace pyhrol;


SMART_FLAG_INSTANTIATION_BEGIN(methodKinds, t)
{
  t.add(npOr, mkCtor, "ctor");
  t.add(npOr, mkMethod, "method", mkCtor);
  t.add(npOr, mkGetsetter, "getsetter", mkMethod);
  t.add(npOr, mkSpecialCall, "specialCall", mkGetsetter);
}
SMART_FLAG_INSTANTIATION_END(methodKinds, t)
