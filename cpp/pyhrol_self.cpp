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

// $Date: 2015-07-20 23:24:41 +0300 (Пн., 20 июля 2015) $
// $Revision: 1047 $

#include "pyhrol.h"
#include "pyhrol_self.h"
#include "pyhrol_self_modules.h"
#include "pyhrol_self_functions.h"
#include "pyhrol_self_types.h"
#include "pyhrol_self_methods.h"
#include "pyhrol_trace.h"
#include "pyhrol_tuple_format.h"
#include "pyhrol_index.h"
#include "enum_shell.hpp"

using namespace std;

#ifndef PYHROL_SAFE_MODE
extern bool _G_signature_hack_enabled;
#endif //PYHROL_SAFE_MODE

namespace pyhrol
{

void ContainerObject::show_index(pyhrol::Tuples &_args)
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Container::container().index().show(cout);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void ContainerObject::show_index_names(pyhrol::Tuples &_args)
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Container::container().index().show_names(cout);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void ContainerObject::set_trace_options(Tuples &_args)
{
  static string src_default = trace_parts_flag_t(tpNo).to_string();
  const char
      *src = &src_default[0]
    , *graph = NULL
  ;
  string res;

  _args.set_options(0);
  PYHROL_PARSE_TUPLE_1(string("\n" + trace_parts_flag_t()->statement()).c_str(), _args, src)
  _args.set_options(0);
  PYHROL_PARSE_TUPLE_1("secret undocumented signature; generates graph with options rules; use the following code to see it:\n  import os\n  f = os.popen(\"dotty -\", \'w\')\n  f.write(Container.set_trace_options(graph = \'\'))\n  f.flush()\n", _args, graph)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_0(NULL, _args)
  PYHROL_BUILD_VALUE_1(NULL, _args, res.c_str())
  PYHROL_AFTER_BUILD_VALUE(_args)

  uint8_t build_variant = 0;
  switch (_args.parsed_variant())
  {
    case 0:
      if (src)
      {
        reset_tracer(trace_parts_flag_t(src));
      }
      else
      {
        reset_tracer(tpNo);
      }
      break;
    case 1:
      {
        ostringstream ostr;
        trace_parts_flag_t()->graph(ostr);
        res = ostr.str();
      }
      build_variant = 1;
      break;
  }

  PYHROL_AFTER_EXECUTE(_args, build_variant)
}

void ContainerObject::get_trace_options(Tuples &_args)
{
  string res;
  uint32_t res_int;

  PYHROL_AFTER_PARSE_TUPLE(_args)
  _args.set_options(optTuple);
  PYHROL_BUILD_VALUE_2("\nas string\nas numeric", _args, res.c_str(), res_int)
  PYHROL_AFTER_BUILD_VALUE(_args)

  res = trace_parts_flag_t(tracer_state()).to_string();
  res_int = tracer_state();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

const string replace_newlines(string src)
{
  size_t pos, pos_prev = 0;
  while ((pos = src.find('\n', pos_prev)) != string::npos)
  {
    src.replace(pos, 1, "\\n");
    pos_prev = pos;
  }
  return src;
}

#ifndef PYHROL_SAFE_MODE
void ContainerObject::is_signature_hack_enabled(pyhrol::Tuples &_args)
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_BUILD_VALUE_1(NULL, _args, _G_signature_hack_enabled)
  PYHROL_AFTER_BUILD_VALUE(_args)

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void ContainerObject::enable_signature_hack(pyhrol::Tuples &_args)
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  _G_signature_hack_enabled = true;

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void ContainerObject::disable_signature_hack(pyhrol::Tuples &_args)
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  _G_signature_hack_enabled = false;

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void ContainerObject::set_help_format(Tuples &_args)
{
  static const string help("\ntuple header format, \'\' -- restore default \"" + replace_newlines(TupleFormat::defaultTupleHeaderFormat) + "\"\ntuple body format, \'\' -- restore default \"" + replace_newlines(TupleFormat::defaultTupleBodyFormat) + "\"\nerror header format, \'\' -- restore default \"" + replace_newlines(TupleFormat::defaultErrorHeaderFormat) + "\"\nerror body format, \'\' -- restore default \"" + replace_newlines(TupleFormat::defaultErrorBodyFormat) + "\"");
  const char *th = NULL, *tb = NULL, *eh = NULL, *eb = NULL;
  int32_t tb_len = 0, th_len = 0, eh_len = 0, eb_len = 0;

  _args.set_options(0);
  PYHROL_PARSE_TUPLE_8(help.c_str(), _args, th, th_len, tb, tb_len, eh, eh_len, eb, eb_len)
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Container::container().setTupleFormat
  (
      th ? (th_len ? th : TupleFormat::defaultTupleHeaderFormat.c_str()) : NULL
    , tb ? (tb_len ? tb : TupleFormat::defaultTupleBodyFormat.c_str()) : NULL
    , eh ? (th_len ? eh : TupleFormat::defaultErrorHeaderFormat.c_str()) : NULL
    , eb ? (th_len ? eb : TupleFormat::defaultErrorBodyFormat.c_str()) : NULL
  );

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

void ContainerObject::get_help_format(Tuples &_args)
{
  const char *th, *tb, *eh, *eb;
  PYHROL_AFTER_PARSE_TUPLE(_args)
  _args.set_options(optDictionary);
  PYHROL_BUILD_VALUE_8("\n\ntuple header format\n\ntuple body format\n\nerror header format\n\nerror body format", _args, "tupleHeaderFormat", th, "tupleBodyFormat", tb, "errorHeaderFormat", eh, "errorBodyFormat", eb)
  PYHROL_AFTER_BUILD_VALUE(_args)

  const TupleFormat &tf = Container::container().tupleFormat();
  th = tf.tupleHeaderFormat().c_str();
  tb = tf.tupleBodyFormat().c_str();
  eh = tf.errorHeaderFormat().c_str();
  eb = tf.errorBodyFormat().c_str();

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}
#endif //PYHROL_SAFE_MODE

void ContainerObject::show_calls(Tuples &_args)
{
  PYHROL_AFTER_PARSE_TUPLE(_args)
  PYHROL_AFTER_BUILD_VALUE(_args)

  Container::container().show_calls(cout);

  PYHROL_AFTER_EXECUTE_DEFAULT(_args)
}

ContainerClass::ContainerClass()
  : TypeBase<pointer<ContainerObject> >("Container", NULL)
{
  m_type_object.tp_new = NULL;
}

void ContainerClass::destructor(pointer<ContainerObject> &) const
{
}

void ContainerClass::init()
{
  ContainerClass *cls = new ContainerClass;
  m_get(cls);

  cls->m_add_static_method_no_args<ContainerObject::show_index>("show_index", NULL);
  cls->m_add_static_method_no_args<ContainerObject::show_index_names>("show_index_names", NULL);
  cls->m_add_static_method_with_keywords<ContainerObject::set_trace_options>("set_trace_options", NULL);
  cls->m_add_static_method_no_args<ContainerObject::get_trace_options>("get_trace_options", NULL);
#ifndef PYHROL_SAFE_MODE
  cls->m_add_static_method<ContainerObject::is_signature_hack_enabled>("is_signature_hack_enabled", NULL);
  cls->m_add_static_method<ContainerObject::enable_signature_hack>("enable_signature_hack", NULL);
  cls->m_add_static_method<ContainerObject::disable_signature_hack>("disable_signature_hack", NULL);
  cls->m_add_static_method_with_keywords<ContainerObject::set_help_format>("set_help_format", "set help format and rebuild help info for all loaded pyhrol dynamically linked modules\n\
help format is printf like format strings with any specifications of the form \'%[<flag>][<width>][.<precision>][<modifier>]<part>\', where:\n\
  <flag> may be only \'-\', means left alignment\n\
  <width> is minimum field width, shorter fields padded with spaces\n\
  <precision> used only with \'n\' part, numbers less than one are ommited\n\
  <modifier> may be only \'a\', means show all of grouped parameters, such as \'O!\', otherwise shown most significant one\n\
  <part> is one of following:\n\
  th tb eh eb\n\
   +     +       d -- tuple direction: \'->\' -- input, argument, \'<-\' -- output, result\n\
   +  +  +  +    n -- serial number of tuple or parameter in tuple \n\
   +  +       [a]S -- C++ signature\n\
   +  +       [a]F -- Python format string\n\
   +  +          P -- Python signature\n\
   +  +          D -- description of tuple or parameter in tuple \n\
      +          N -- tuple parameter name, only for input tuples with keywords\n\
      +       [a]V -- tuple parameter default value, only for optional parameteres in input tuples\n\
      +       [a]A -- tuple parameter address\n\
         +  +    E -- error description, only for invalid tuples\n\
th -- tuple header, tb -- tuple body (per parameter table), eh -- error header (only for invalid tuples), eb -- error body\n");
  cls->m_add_static_method_no_args<ContainerObject::get_help_format>("get_help_format", NULL);
#endif //PYHROL_SAFE_MODE
  cls->m_add_static_method_no_args<ContainerObject::show_calls>("show_calls", NULL);
}

ContainerClass &ContainerClass::get()
{
  return dynamic_cast<ContainerClass &>(m_get());
}

} //namespace pyhrol


using namespace pyhrol;

static void __attribute__ ((constructor)) __on_load_self()
{
  /* NOTE
  Порядок инициализации классов принципиален для прохождения тестов,
  поэтому все классы инициализируются здесь
  */
  Module::init();
  ModulesIterator::init();
  Modules::init();
  Type::init();
  TypesIterator::init();
  Types::init();
  Method::init();
  MethodsIterator::init();
  Methods::init();
  Function::init();
  FunctionsIterator::init();
  Functions::init();
}
