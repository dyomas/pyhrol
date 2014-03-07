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
#include "pyhrol_self_functions.h"
#include "pyhrol_trace.h"
#include "pyhrol_container.h"
#include "nullable.hpp"


using namespace std;

namespace pyhrol
{

module::module(const size_t _pos, PyObject *obj)
  : AutoHolder(obj)
  , pos(_pos)
{
}

modules_iterator::modules_iterator(PyObject *obj)
  : AutoHolder(obj)
  , iter(Index::etModule)
{
}

Module::Module()
  : TypeBase<module>("Module", NULL)
{
}

void Module::destructor(module &obj) const
{
  obj.~module();
}

void Module::get_name(const Ptr<const module> &obj, Tuples &_args) const
{
  const char *name;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, name)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const Container &c = Container::container();
  name = c.module_name(c.index().get(obj->pos)->offset).c_str();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Module::get_types(const Ptr<const module> &obj, Tuples &_args) const
{
  PyObject *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Types::T_struct *pobj = Types::allocate_static();
  retval = *pobj;
  new (&pobj->endosome) Types::endosome_t(obj->pos, retval);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Module::get_functions(const Ptr<const module> &obj, Tuples &_args) const
{
  PyObject *retval;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Functions::T_struct *pobj = Functions::allocate_static();
  retval = *pobj;
  new (&pobj->endosome) Functions::endosome_t(obj->pos, retval);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Module::init()
{
  Module *cls = new Module;
  m_get(cls);
  cls->m_add_getter<&Module::get_name>("name", NULL);
  cls->m_add_getter<&Module::get_types>("types", NULL);
  cls->m_add_getter<&Module::get_functions>("functions", NULL);
  cls->m_add_member<const size_t, &module::pos>("pos", NULL);
}


ModulesIterator::ModulesIterator()
  : TypeBase<modules_iterator>("ModulesIterator", NULL)
{
  m_type_object.tp_iter = NULL;
}

void ModulesIterator::init()
{
  m_get(new ModulesIterator);
}

void ModulesIterator::destructor(modules_iterator &obj) const
{
  obj.~modules_iterator();
}

bool ModulesIterator::next(const Ptr<module> &itm, const Ptr<modules_iterator> &i) const
{
  Index::iterator &iter = i->iter;
  const bool retval = Container::container().index().iterate(iter);

  if (retval)
  {
    new (&*itm) module(iter.pos.getValue(), i);
  }
  return retval;
}


Modules::Modules()
  : TypeBase<modules>("Modules", NULL)
{
  m_type_object.tp_iternext = NULL;
}

void Modules::init()
{
  m_get(new Modules);
}

void Modules::iter(const Ptr<modules_iterator> &m, const Ptr<const modules> &ms) const
{
  new (&*m) modules_iterator(ms);
}

void Modules::constructor(endosome_t &res, Tuples &_args) const
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  new (&res) Modules::endosome_t();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void Modules::destructor(endosome_t &obj) const
{
  obj.~endosome_t();
}

void Modules::get(const Ptr<const endosome_t> &modules, Tuples &_args) const
{
  PyObject *retval;
  const char *name;

  PYHROL_PARSE_TUPLE_1(NULL, _args, name)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, retval)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const size_t pos = Container::container().index().find(name, Index::etModule);
  Module::T_struct *pobj = Module::allocate_static();
  retval = *pobj;
  new(&pobj->endosome) module(pos, modules);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

} //namespace pyhrol

using namespace pyhrol;
