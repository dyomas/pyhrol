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

// $Date: 2014-01-08 14:43:52 +0400 (Ср., 08 янв. 2014) $
// $Revision: 867 $

#ifndef __pyhrol_macro_h__
#define __pyhrol_macro_h__

#include "pyhrol_fmt_in_macro.h"

#define PYHROL_AFTER_PARSE_TUPLE(container)\
\
while (true)\
{\
  if (container.before_build())\
  {

#include "pyhrol_fmt_out_macro.h"

#define PYHROL_AFTER_BUILD_VALUE(container)\
    if (container.after_build())\
    {\
      return;\
    }\
  }


#define PYHROL_AFTER_EXECUTE_DEFAULT(container)\
  container.choose_build_variant(0);\
};

#define PYHROL_AFTER_EXECUTE(container, variant)\
  container.choose_build_variant(variant);\
};

#define PYHROL_REGISTER_FUNCTION(function, help)\
pyhrol::Container::container().add_function<function>(__STRING(function), help);

#define PYHROL_REGISTER_FUNCTION_WITH_KEYWORDS(function, help)\
pyhrol::Container::container().add_function_with_keywords<function>(__STRING(function), help);

#define PYHROL_REGISTER_FUNCTION_NO_ARGS(function, help)\
pyhrol::Container::container().add_function_no_args<function>(__STRING(function), help);

#define PYHROL_REGISTER_METHOD(type, method, help)\
m_add_method<type, &type::method>(__STRING(method), help);

#define PYHROL_REGISTER_METHOD_WITH_KEYWORDS(type, method, help)\
m_add_method_with_keywords<type, &type::method>(__STRING(method), help);

#define PYHROL_REGISTER_METHOD_NO_ARGS(type, method, help)\
m_add_method_no_args<type, &type::method>(__STRING(method), help);

#define PYHROL_REGISTER_GETSETER(type, name, getter, setter, help)\
m_add_getseter<type, &type::getter, &type::setter>(name, help);

#define PYHROL_REGISTER_GETTER(type, getter, help)\
m_add_getter<type, &type::getter>(__STRING(getter), help);

#define PYHROL_REGISTER_SETTER(type, setter, help)\
m_add_setter<type, &type::setter>(__STRING(setter), help);

#endif //__pyhrol_macro_h__
