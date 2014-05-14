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

#ifndef __pyhrol_self_types_h__
#define __pyhrol_self_types_h__

#include <stdint.h>
#include <string>
#include <set>

#include "pyhrol_auto_holder.h"
#include "pyhrol_type.h"
#include "pyhrol_type_iterable.h"
#include "pyhrol_type_map.h"
#include "pyhrol_index.h"
#include "smart_flag_int.h"

namespace pyhrol
{

struct type: pyhrol::AutoHolder
{
  const size_t pos;
  type(const size_t, PyObject *);
};

struct types_iterator: pyhrol::AutoHolder
{
  Index::iterator iter;
  types_iterator(PyObject *);
  types_iterator(const size_t, PyObject *);
};

struct types: pyhrol::AutoHolder
{
  const bool all;
  const size_t module_pos;
  types();
  types(const size_t, PyObject *);
};

class Type: public pyhrol::TypeWrapper<type>
{
  Type();

  virtual void destructor(type &) const;
  void get_name(const pyhrol::Ptr<const type> &, pyhrol::Tuples &) const;
  void get_doc(const pyhrol::Ptr<const type> &, pyhrol::Tuples &) const;
  void get_methods(const pyhrol::Ptr<const type> &, pyhrol::Tuples &) const;
  void get_module(const pyhrol::Ptr<const type> &, pyhrol::Tuples &) const;
  void get_flags(const pyhrol::Ptr<const type> &, pyhrol::Tuples &) const;

public:
  static void init();
};

class TypesIterator: public pyhrol::TypeWrapper<types_iterator>, public pyhrol::TypeIterable<types_iterator, type>
{
  TypesIterator();

  virtual void destructor(types_iterator &) const;
  virtual bool next(const pyhrol::Ptr<type> &, const pyhrol::Ptr<types_iterator> &) const;

public:
  static void init();
};

class Types: public pyhrol::TypeWrapper<types>, public pyhrol::TypeIterable<types, types_iterator>, public TypeMap<types>
{
  Types();

  virtual void constructor(endosome_t &, pyhrol::Tuples &) const;
  virtual void destructor(endosome_t &) const;
  virtual void iter(const pyhrol::Ptr<types_iterator> &, const pyhrol::Ptr<const types> &) const;
  virtual void get(const pyhrol::Ptr<const endosome_t> &, pyhrol::Tuples &) const;

public:
  static void init();
};

} //namepace pyhrol

struct type_flags_struct: public SmartFlagUint
{
  type_flags_struct();
  static const ftree_t *init();
private:
  static ftree_t m_ftree;
};

#endif //__pyhrol_self_types_h__
