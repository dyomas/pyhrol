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

// $Date: 2014-02-13 01:38:33 +0400 (Чт., 13 февр. 2014) $
// $Revision: 888 $

#ifndef __pyhrol_common_h__
#define __pyhrol_common_h__

#include <sys/types.h>
#include <stdexcept>
#include <typeinfo>

#include "smart_flag.h"

namespace pyhrol
{
  enum options
  {
      optNo = 0
    , optTuple = 0x010 //исходный в TupleIn или результирующий в TupleOut тип -- tuple ("(форматы)")
    , optList = 0x020 //результирующий тип -- list ("[форматы]"), игнорируется в TupleIn
    , optDictionary = 0x040 //результирующий тип -- dictionary ("{форматы}"), игнорируется в TupleIn
    , optItemsMask = 0x070 //optTuple | optList | optDictionary
    , optNoneEnabled = 0x100 //формат "s" заменяется на "z", т. е. допускается значение None (аналог NULL)
    , optIncrementReferenceCount = 0x400 //формат "N" заменяется на "O"
    , optNotRecode = 0x800 //формат "s" заменяется на формат "t"
  };

  enum callModifiers
  {
      f_no = 0
    , f_noarg = 8
    , f_void = 16
    , f_const = 32
    , f_keywords = 64
  };

  enum callContexts
  {
      c_function = 1
    , c_method = 2
    , c_static_method = 3
  };

  enum callPurposes
  {
      p_getter = 256
    , p_setter = 512
    , p_ctor = 1024
    , p_protof = 2048
  };

  enum callVariants
  {
      v_function = c_function
    , v_function_no_arg = c_function + f_noarg
    , v_function_void = c_function + f_void
    , v_function_no_arg_void = c_function + f_noarg + f_void
    , v_function_kw = c_function + f_keywords
    , v_function_void_kw = c_function + f_void + f_keywords
    , v_static_method = c_static_method
    , v_static_method_no_arg = c_static_method + f_noarg
    , v_static_method_void = c_static_method + f_void
    , v_static_method_no_arg_void = c_static_method + f_noarg + f_void
    , v_static_method_kw = c_static_method + f_keywords
    , v_static_method_void_kw = c_static_method + f_void + f_keywords
    , v_method = c_method
    , v_method_no_arg = c_method + f_noarg
    , v_method_void = c_method + f_void
    , v_method_no_arg_void = c_method + f_noarg + f_void
    , v_method_kw = c_method + f_keywords
    , v_method_void_kw = c_method + f_void + f_keywords
    , v_method_const = c_method + f_const
    , v_method_no_arg_const = c_method + f_noarg + f_const
    , v_method_void_const = c_method + f_void + f_const
    , v_method_no_arg_void_const = c_method + f_noarg + f_void + f_const
    , v_method_kw_const = c_method + f_const + f_keywords
    , v_method_void_kw_const = c_method + f_void + f_const + f_keywords
    , v_getter = c_method + f_noarg + f_const + p_getter
    , v_setter = c_method + f_void + p_setter
    , v_getsetter = c_method + p_getter + p_setter
    , v_ctor = c_method + f_void + f_keywords + p_ctor
    , v_protof = c_method + f_void + f_const + p_protof
    , v_protof_retval = c_method + f_const + p_protof
  };

  extern const int8_t requiredAll;

  /**
  Get last part of dot separated type name:
  */
  const char *isolate_type_name(const char *);

  class ImplementationException: public std::logic_error
  {
  public:
    enum components
    {
        cpntMethod
      , cpntConstructor
    };
    ImplementationException(const components = cpntMethod);
    ImplementationException(const char *);

  private:
    static const std::string m_describe(const components, const char * = NULL);
  };

  class NullPointer: public std::domain_error
  {
  public:
    NullPointer();
  };

  class TypeInitException: public std::logic_error
  {
  public:
    TypeInitException(const char *tp_name);
    TypeInitException(const std::type_info &);

  private:
    static const std::string m_describe(const char *tp_name);
    static const std::string m_describe(const std::type_info &);
  };

  class IndexException: public std::range_error
  {
  public:
    IndexException();
    IndexException(size_t limit, ssize_t cnt);
    IndexException(size_t limit, ssize_t begin, ssize_t end);

  private:
    static const std::string m_describe(size_t limit, ssize_t cnt);
    static const std::string m_describe(size_t limit, ssize_t begin, ssize_t end);
  };

  class AttributeException: public std::logic_error
  {
  public:
    AttributeException(const std::string &);
  };

  class KeyException: public std::runtime_error
  {
  public:
    KeyException(const std::string &);
  };

  class TypeException: public std::runtime_error
  {
  public:
    TypeException(const std::string &);
  };
} //namespace pyhrol

typedef SmartFlag<pyhrol::options> tuples_options_flag_t;
typedef SmartFlag<pyhrol::callModifiers> call_modifiers_flag_t;
typedef SmartFlag<pyhrol::callContexts> call_contexts_flag_t;
typedef SmartFlag<pyhrol::callPurposes> call_purposes_flag_t;
typedef SmartFlag<pyhrol::callVariants> call_variants_flag_t;

#endif //__pyhrol_common_h__
