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

// $Date: 2014-02-27 22:19:34 +0400 (Thu, 27 Feb 2014) $
// $Revision: 896 $

#include "pyhrol.h"
#include "pyhrol_self_modules.h"
#include "pyhrol_self_functions.h"
#include "pyhrol_trace.h"
#include "pyhrol_self.h"
#include "pyhrol_flags.h"
#include "nullable.hpp"


using namespace std;

namespace pyhrol
{

function::function(const size_t _pos, PyObject *obj)
  : AutoHolder(obj)
  , pos(_pos)
{
}

functions_iterator::functions_iterator(PyObject *obj)
  : AutoHolder(obj)
  , iter(Index::etFunction)
{
}

functions_iterator::functions_iterator(const size_t module_pos, PyObject *obj)
  : AutoHolder(obj)
  , iter(Index::etFunction, module_pos)
{
}

functions::functions()
  : AutoHolder(NULL)
  , all(true)
  , module_pos()
{
}

functions::functions(const size_t _pos, PyObject *obj)
  : AutoHolder(obj)
  , all(false)
  , module_pos(_pos)
{
}


Function::Function()
  : TypeBase<function>("Function", NULL)
{
  m_add_member<const size_t, &function::pos>("pos", NULL);
  m_add_getter<Function, &Function::get_name>("name", NULL);
  m_add_getter<Function, &Function::get_doc>("doc", NULL);
  m_add_getter<Function, &Function::get_module>("module", NULL);
  m_add_getter<Function, &Function::get_flags>("flags", NULL);
}

void Function::init()
{
  m_get(new Function);
}

void Function::destructor(function &obj) const
{
  obj.~function();
}

void Function::get_name(const Ptr<const function> &obj, Tuples &_args) const
{
  const char *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Container &c = Container::container();
  const Index::item
      *pfunction = c.index().get(obj->pos)
    , *pmodule = pfunction + pfunction->parent
  ;

  retval = c.functions(pmodule->offset)[pfunction->offset].ml_name;

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Function::get_doc(const Ptr<const function> &obj, Tuples &_args) const
{
  const char *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Container &c = Container::container();
  const Index::item
      *pfunction = c.index().get(obj->pos)
    , *pmodule = pfunction + pfunction->parent
  ;

  retval = c.functions(pmodule->offset)[pfunction->offset].ml_doc;

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Function::get_module(const Ptr<const function> &obj, Tuples &_args) const
{
  PyObject *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Module::T_struct *pobj = Module::allocate_static();
  retval = *pobj;

  new (&pobj->endosome) Module::endosome_t(obj->pos + Container::container().index().get(obj->pos)->parent, NULL);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Function::get_flags(const Ptr<const function> &obj, Tuples &_args) const
{
  method_flags_struct retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  _args.set_options(optTuple);
  PYHROL_BUILD_VALUE_2((string("\nstring equivalent: ") + method_flags_struct::init()->statement()).c_str(), _args, retval.get(), retval.as_string().c_str())
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Container &c = Container::container();
  const Index::item
      *pfunction = c.index().get(obj->pos)
    , *pmodule = pfunction + pfunction->parent
  ;

  static_cast<SmartFlagInt &>(retval) = c.functions(pmodule->offset)[pfunction->offset].ml_flags;

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

FunctionsIterator::FunctionsIterator()
  : TypeBase<functions_iterator>("FunctionsIterator", NULL)
{
  m_type_object.tp_iter = NULL;
}

void FunctionsIterator::init()
{
  m_get(new FunctionsIterator);
}

void FunctionsIterator::destructor(functions_iterator &obj) const
{
  obj.~endosome_t();
}

bool FunctionsIterator::next(const Ptr<function> &itm, const Ptr<functions_iterator> &i) const
{
  const Index &idx = Container::container().index();
  Index::iterator &iter = i->iter;

  const bool retval = idx.iterate(iter);

  if (retval)
  {
    new (&*itm) function(iter.pos.getValue(), i);
  }
  return retval;
}


Functions::Functions()
  : TypeBase<functions>("Functions", NULL)
{
  m_type_object.tp_iternext = NULL;
}

void Functions::init()
{
  m_get(new Functions);
}

void Functions::constructor(endosome_t &res, Tuples &_args) const
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  new(&res) endosome_t();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Functions::destructor(endosome_t &res) const
{
  res.~endosome_t();
}

void Functions::iter(const Ptr<functions_iterator> &m, const Ptr<const functions> &ms) const
{
  if (ms->all)
  {
    new (&*m) functions_iterator(ms);
  }
  else
  {
    new (&*m) functions_iterator(ms->module_pos, ms);
  }
}

void Functions::get(const Ptr<const endosome_t> &funcs, Tuples &_args) const
{
  PyObject *retval;
  const char *name;

  PYHROL_PARSE_TUPLE_1(NULL, _args, name)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Index &idx = Container::container().index();
  size_t pos;
  if (funcs->all)
  {
    pos = idx.find(name, Index::etFunction);
  }
  else
  {
    pos = idx.find(name, Index::etFunction, funcs->module_pos);
  }

  Function::T_struct *pobj = Function::allocate_static();
  retval = *pobj;
  new(&pobj->endosome) function(pos, funcs);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

} //namespace pyhrol
