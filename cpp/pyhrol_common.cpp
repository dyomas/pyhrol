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

// $Date: 2014-02-13 01:38:33 +0400 (Thu, 13 Feb 2014) $
// $Revision: 888 $

#include <string.h>
#include <stdlib.h>
#include "pyhrol_common.h"
#include "smart_flag.hpp"
#include "demangling.h"

using namespace std;

namespace pyhrol
{

const int8_t requiredAll = -1;

const char *isolate_type_name(const char *name)
{
  const char *res = strchr(name, '.');
  return res ? res + 1 : name;
}

const string ImplementationException::m_describe(const components cpnt, const char *name)
{
  string retval;

  if (cpnt == cpntConstructor)
  {
    retval = "Dummy constructor not overridden";
  }
  else if (cpnt == cpntMethod)
  {
    if (name)
    {
      retval += "Method `";
      retval += name;
      retval += "` not implemented";
    }
    else
    {
      retval = "Method not implemented";
    }
  }

  return retval;
}

ImplementationException::ImplementationException(const components cpnt)
  : logic_error(m_describe(cpnt))
{
}

ImplementationException::ImplementationException(const char *name)
  : logic_error(m_describe(cpntMethod, name))
{
}

NullPointer::NullPointer()
  : domain_error("Null pointer")
{
}

TypeInitException::TypeInitException(const char *tp_name)
  : logic_error(m_describe(tp_name))
{
}

TypeInitException::TypeInitException(const type_info &t)
  : logic_error(m_describe(t))
{
}

const string TypeInitException::m_describe(const char *tp_name)
{
  return string("Reinitialization of type \"") + tp_name + "\"";
}

const string TypeInitException::m_describe(const type_info &t)
{
  return string("Type wrapper \"") + demangle(t) +"\" uninitialized";
}


IndexException::IndexException()
  : range_error("Out of range")
{
}

IndexException::IndexException(size_t limit, ssize_t cnt)
  : range_error(m_describe(limit, cnt))
{
}

IndexException::IndexException(size_t limit, ssize_t begin, ssize_t end)
  : range_error(m_describe(limit, begin, end))
{
}

const string IndexException::m_describe(size_t limit, ssize_t cnt)
{
  ostringstream ostr;
  if (cnt < 0)
  {
    ostr << "Negative index " << cnt << " not possible";
  }
  else
  {
    ostr << "Index " << cnt << " exceeds object size " << limit;
  }
  return ostr.str();
}

const string IndexException::m_describe(size_t limit, ssize_t begin, ssize_t end)
{
  ostringstream ostr;
  if (begin < 0 || end < 0)
  {
    ostr << "Negative range borders [" << begin << ", " << end << ") not possible";
  }
  else
  {
    ostr << "Range [" << begin << ", " << end << ") exceeds object size " << limit;
  }
  return ostr.str();
}

AttributeException::AttributeException(const std::string &msg)
  : logic_error(msg)
{
}

KeyException::KeyException(const std::string &msg)
  : runtime_error(msg)
{
}

TypeException::TypeException(const std::string &msg)
  : runtime_error(msg)
{
}

}

using namespace pyhrol;


SMART_FLAG_INSTANTIATION_BEGIN(options, t)
{
  t.add(npOr, optNo, "No");
  t.add(npFilter, optItemsMask, optNo);
  t.add(node_props(npOr | npTrue), optTuple, "Tuple", optItemsMask);
  t.add(npOr, optList, "List", optTuple);
  t.add(npOr, optDictionary, "Dictionary", optList);
  t.add(npAnd, optNoneEnabled, "NoneEnabled", optItemsMask);
  t.add(npAnd, optIncrementReferenceCount, "IncrementReferenceCount", optNoneEnabled);
  t.add(npAnd, optNotRecode, "NotRecode", optIncrementReferenceCount);
}
SMART_FLAG_INSTANTIATION_END(t);

SMART_FLAG_INSTANTIATION_BEGIN(callModifiers, t)
{
  t.add(npOr, f_no, "no");
  t.add(npOr, f_noarg, "noarg", f_no);
  t.add(npOr, f_void, "void", f_noarg);
  t.add(npOr, f_const, "const", f_void);
  t.add(npOr, f_keywords, "keywords", f_const);
}
SMART_FLAG_INSTANTIATION_END(t);

SMART_FLAG_INSTANTIATION_BEGIN(callContexts, t)
{
  t.add(npOr, c_function, "function");
  t.add(npOr, c_method, "method", c_function);
}
SMART_FLAG_INSTANTIATION_END(t);

SMART_FLAG_INSTANTIATION_BEGIN(callPurposes, t)
{
  t.add(npOr, p_ctor, "ctor");
  t.add(npOr, p_protof, "protof", p_ctor);
  t.add(npAnd, p_getter, "getter", p_protof);
  t.add(npAnd, p_setter, "setter", p_getter);
}
SMART_FLAG_INSTANTIATION_END(t);

SMART_FLAG_INSTANTIATION_BEGIN(callVariants, t)
{
  t.add(npOr, v_function, "function");
  t.add(npOr, v_function_no_arg, "function_no_arg", v_function);
  t.add(npOr, v_function_void, "function_void", v_function_no_arg);
  t.add(npOr, v_function_no_arg_void, "function_no_arg_void", v_function_void);
  t.add(npOr, v_function_kw, "function_kw", v_function_no_arg_void);
  t.add(npOr, v_function_void_kw, "function_void_kw", v_function_kw);
  t.add(npOr, v_static_method, "static_method", v_function_void_kw);
  t.add(npOr, v_static_method_no_arg, "static_method_no_arg", v_static_method);
  t.add(npOr, v_static_method_void, "static_method_void", v_static_method_no_arg);
  t.add(npOr, v_static_method_no_arg_void, "static_method_no_arg_void", v_static_method_void);
  t.add(npOr, v_static_method_kw, "static_method_kw", v_static_method_no_arg_void);
  t.add(npOr, v_static_method_void_kw, "static_method_void_kw", v_static_method_kw);
  t.add(npOr, v_method, "method", v_static_method_void_kw);
  t.add(npOr, v_method_no_arg, "method_no_arg", v_method);
  t.add(npOr, v_method_void, "method_void", v_method_no_arg);
  t.add(npOr, v_method_no_arg_void, "method_no_arg_void", v_method_void);
  t.add(npOr, v_method_kw, "method_kw", v_method_no_arg_void);
  t.add(npOr, v_method_void_kw, "method_void_kw", v_method_kw);
  t.add(npOr, v_method_const, "method_const", v_method_void_kw);
  t.add(npOr, v_method_no_arg_const, "method_no_arg_const", v_method_const);
  t.add(npOr, v_method_void_const, "method_void_const", v_method_no_arg_const);
  t.add(npOr, v_method_no_arg_void_const, "method_no_arg_void_const", v_method_void_const);
  t.add(npOr, v_method_kw_const, "method_kw_const", v_method_no_arg_void_const);
  t.add(npOr, v_method_void_kw_const, "method_void_kw_const", v_method_kw_const);
  t.add(npOr, v_getter, "getter", v_method_void_kw_const);
  t.add(npOr, v_setter, "setter", v_getter);
  t.add(npOr, v_getsetter, "getsetter", v_setter);
  t.add(npOr, v_ctor, "ctor", v_getsetter);
  t.add(npOr, v_protof, "protof", v_ctor);
  t.add(npOr, v_protof_retval, "protof_retval", v_protof);
}
SMART_FLAG_INSTANTIATION_END(t);
