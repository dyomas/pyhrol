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

// $Date: 2014-01-26 14:46:32 +0400 (Sun, 26 Jan 2014) $
// $Revision: 880 $

#ifndef __pyhrol_self_h__
#define __pyhrol_self_h__

#include <stdint.h>
#include <string>
#include <set>

#include "pyhrol_type.h"
#include "pyhrol_type_pointer.h"
#include "pyhrol_type_special.h"
#include "pyhrol_type_iterable.h"

namespace pyhrol
{

class ContainerObject
{
public:
  static void show_index(pyhrol::Tuples &);
  static void show_index_names(pyhrol::Tuples &);

  static void set_trace_options(pyhrol::Tuples &);
  static void get_trace_options(pyhrol::Tuples &);
#ifndef PYHROL_SAFE_MODE
  static void is_signature_hack_enabled(pyhrol::Tuples &);
  static void enable_signature_hack(pyhrol::Tuples &);
  static void disable_signature_hack(pyhrol::Tuples &);

  static void set_help_format(pyhrol::Tuples &);
  static void get_help_format(pyhrol::Tuples &);
#endif //PYHROL_SAFE_MODE
  static void show_calls(pyhrol::Tuples &);
};

class ContainerClass: public pyhrol::TypePointer<ContainerObject>, pyhrol::TypeSpecial<pointer<ContainerObject> >
{
  ContainerClass();

  virtual void destructor(pointer<ContainerObject> &) const;

public:
  static void init() __attribute__ ((constructor));
  static ContainerClass &get();
};

} //namespace pyhrol

#endif //__pyhrol_self_h__
