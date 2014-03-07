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

// $Date: 2013-12-10 16:05:29 +0400 (Tue, 10 Dec 2013) $
// $Revision: 862 $

#ifndef __pyhrol_self_methods_h__
#define __pyhrol_self_methods_h__

#include <stdint.h>
#include <string>
#include <set>

#include "nullable.h"
#include "pyhrol_auto_holder.h"
#include "pyhrol_type.h"
#include "pyhrol_type_pointer.h"
#include "pyhrol_type_iterable.h"
#include "pyhrol_type_map.h"
#include "pyhrol_index.h"
#include "smart_flag.h"

namespace pyhrol
{

enum methodKinds
{
    mkCtor = pyhrol::Index::etCtor
  , mkMethod = pyhrol::Index::etMethod
  , mkGetsetter = pyhrol::Index::etGetsetter
  , mkSpecialCall = pyhrol::Index::etSpecialCall
};

struct method: pyhrol::AutoHolder
{
  const size_t pos;
  method(const size_t, PyObject *);
};

struct methods_iterator: pyhrol::AutoHolder
{
  const Nullable<methodKinds> kind;
  Index::iterator iter;
  methods_iterator(PyObject *);
  methods_iterator(const methodKinds, PyObject *);
  methods_iterator(const size_t, PyObject *);
};

struct methods: pyhrol::AutoHolder
{
  const Nullable<methodKinds> kind;
  const Nullable<size_t> type_pos;
  methods();
  methods(const methodKinds);
  methods(const size_t, PyObject *);
};


class Method: public pyhrol::TypeWrapper<method>
{
  Method();

  virtual void destructor(method &) const;
  void get_name(const pyhrol::Ptr<const method> &, pyhrol::Tuples &) const;
  void get_doc(const pyhrol::Ptr<const method> &, pyhrol::Tuples &) const;
  void get_type(const pyhrol::Ptr<const method> &, pyhrol::Tuples &) const;
  void get_module(const pyhrol::Ptr<const method> &, pyhrol::Tuples &) const;
  void get_kind(const pyhrol::Ptr<const method> &, pyhrol::Tuples &) const;
  void get_flags(const pyhrol::Ptr<const method> &, pyhrol::Tuples &) const;

public:
  static void init();
};

class MethodsIterator: public pyhrol::TypeWrapper<methods_iterator>, public pyhrol::TypeIterable<methods_iterator, method>
{
  MethodsIterator();

  virtual void destructor(methods_iterator &) const;
  virtual bool next(const pyhrol::Ptr<method> &, const pyhrol::Ptr<methods_iterator> &) const;

public:
  static void init();
};

class Methods: public pyhrol::TypeWrapper<methods>, public pyhrol::TypeIterable<methods, methods_iterator>, public TypeMap<methods>
{
  Methods();

  virtual void constructor(endosome_t &, pyhrol::Tuples &) const;
  virtual void destructor(endosome_t &) const;
  virtual void iter(const pyhrol::Ptr<methods_iterator> &, const pyhrol::Ptr<const methods> &) const;
  virtual void get(const pyhrol::Ptr<const endosome_t> &, pyhrol::Tuples &) const;

public:
  static void init();
};

} //namespace pyhrol

typedef SmartFlag<pyhrol::methodKinds> method_kinds_flag_t;

#endif //__pyhrol_self_methods_h__
