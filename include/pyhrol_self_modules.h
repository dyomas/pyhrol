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

// $Date: 2013-12-10 16:05:29 +0400 (Вт., 10 дек. 2013) $
// $Revision: 862 $

#ifndef __pyhrol_self_modules_h__
#define __pyhrol_self_modules_h__

#include <stdint.h>
#include <string>
#include <set>

#include "pyhrol_type.h"
#include "pyhrol_auto_holder.h"
#include "pyhrol_index.h"
#include "pyhrol_type_iterable.h"
#include "pyhrol_type_map.h"

namespace pyhrol
{

struct module: pyhrol::AutoHolder
{
  const size_t pos;
  module(const size_t, PyObject *);
};

struct modules_iterator: pyhrol::AutoHolder
{
  Index::iterator iter;
  modules_iterator(PyObject *);
};

struct modules
{
};

class Module: public pyhrol::TypeWrapper<module, Module>
{
  Module();

  virtual void destructor(module &) const;
  void get_name(const pyhrol::Ptr<const module> &, pyhrol::Tuples &) const;
  void get_types(const pyhrol::Ptr<const module> &, pyhrol::Tuples &) const;
  void get_functions(const pyhrol::Ptr<const module> &, pyhrol::Tuples &) const;

public:
  static void init();
};

class ModulesIterator: public pyhrol::TypeWrapper<modules_iterator>, public pyhrol::TypeIterable<modules_iterator, module>
{
  ModulesIterator();

  virtual void destructor(modules_iterator &) const;
  virtual bool next(const pyhrol::Ptr<module> &, const pyhrol::Ptr<modules_iterator> &) const;

public:
  static void init();
};

class Modules: public pyhrol::TypeWrapper<modules>, public pyhrol::TypeIterable<modules, modules_iterator>, public TypeMap<modules>
{
  Modules();

  virtual void constructor(endosome_t &, pyhrol::Tuples &) const;
  virtual void destructor(endosome_t &) const;
  virtual void iter(const pyhrol::Ptr<modules_iterator> &, const pyhrol::Ptr<const modules> &) const;
  virtual void get(const pyhrol::Ptr<const endosome_t> &, pyhrol::Tuples &) const;

public:
  static void init();
};

} //namespace pyhrol

#endif //__pyhrol_self_modules_h__
