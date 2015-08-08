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

// $Date: 2015-04-25 01:51:34 +0300 (Сб., 25 апр. 2015) $
// $Revision: 1033 $

#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <pyport.h>
#include <object.h>
#include <unicodeobject.h>
#include <pyerrors.h>
#include <pyarena.h>
#include <pystate.h>
#include <pythonrun.h>

#include "pyhrol_trace.h"
#include "signature_parser.h"
#include "smart_flag.hpp"

using namespace std;

namespace pyhrol
{

static traceParts m_init_trace_parts(const traceParts *new_value = NULL)
{
  static trace_parts_flag_t ret_val (tpNo);

  if (new_value)
  {
    ret_val = *new_value;
  }
  else if (!ret_val.assigned())
  {
    const char *const env_name = "PYHROL_TRACE";
    const char *val = getenv(env_name);

    if (val && *val)
    {
      try
      {
        ret_val = val;
      }
      catch(const exception &ex)
      {
        ret_val = tpNo;
        PyErr_Format(PyExc_UserWarning, "Environment variable \"%s\" has invalid value, tracing not started\n  %s\n  Possible values: %s", env_name, ex.what(), ret_val->statement().c_str());
        PyErr_PrintEx(0);
        PyErr_Clear();
      }
    }
    else
    {
      ret_val = tpNo;
    }
  }
  return ret_val;
}

typedef map<size_t, size_t> addresses_t;
static addresses_t &addresses()
{
  static addresses_t retval;
  return retval;
}

static void trace_head(const traceParts tp, const traceParts show_parts, const void *ptr, const char *signature)
{
  if (tp & tpInternal)
  {
    fprintf(stderr, "I");
  }
  else if (tp & tpMediator)
  {
    fprintf(stderr, "M");
  }
  else if (tp & tpUser)
  {
    fprintf(stderr, "U");
  }

  const size_t address = reinterpret_cast<size_t>(ptr);

  if (!(show_parts & tpHideAddress))
  {
  #ifdef __LP64__
    if (address)
    {
      fprintf(stderr, " 0x%016lX", address);
    }
    else
    {
      fprintf(stderr, "%*c", 19, ' ');
    }
  #else
    if (address)
    {
      fprintf(stderr, " 0x%08X", address);
    }
    else
    {
      fprintf(stderr, "%*c", 11, ' ');
    }
  #endif
  }

  if (show_parts & tpShowNum)
  {
    if (address)
    {
      const addresses_t::const_iterator iter = addresses().find(address);
      size_t num;
      if (iter == addresses().end())
      {
        num = addresses().size();
        addresses()[address] = num;
      }
      else
      {
        num = iter->second;
      }
#ifdef __LP64__
      fprintf(stderr, "%5lu", num);
#else
      fprintf(stderr, "%5u", num);
#endif
    }
    else
    {
      fprintf(stderr, "%*c", 5, ' ');
    }
  }

  if (signature)
  {
    ParserState st;
    const char
        *begin
      , *end = signature
    ;
    bool
        with = false
      , filtered = false
    ;
    while ((begin = ParserState::parse(st, end)))
    {
      switch (st.part)
      {
        case ParserState::pSignature:
          fprintf(stderr, " %.*s", static_cast<int>(end - begin), begin);
          break;
        case ParserState::pTemplateName:
          {
            const string name(begin, end);
            filtered = (name == "PyObject" || name == "PyTypeObject" || name == "FILE");
            if (!filtered)
            {
              if (with)
              {
                fprintf(stderr, ", ");
              }
              else
              {
                fprintf(stderr, " [with ");
                with = true;
              }
              fprintf(stderr, "%.*s", static_cast<int>(end - begin), begin);
            }
          }
          break;
        case ParserState::pTemplateValue:
          if (!filtered)
          {
            fprintf(stderr, " = %.*s", static_cast<int>(end - begin), begin);
          }
          break;
        case ParserState::pTemplates:
          fprintf(stderr, "%.*s", static_cast<int>(end - begin), begin);
          break;
        default:
          fprintf(stderr, "<?parse error>");
      }
    }
    if (with)
    {
      fprintf(stderr, "]");
    }
  }
}

traceParts tracer_state()
{
  return m_init_trace_parts();
}

void reset_tracer(const traceParts tp)
{
  m_init_trace_parts(&tp);
}

void trace(const traceParts tp, const void *address, const char *signature)
{
  const traceParts show_parts = m_init_trace_parts();
  if (!(tp & show_parts))
  {
    return;
  }

  trace_head(tp, show_parts, address, signature);

  fprintf(stderr, "\n");
}

bool user_trace_enbled()
{
  return m_init_trace_parts() & tpUser;
}

ostream &user_trace(const void *address, const char *signature)
{
  trace_head(tpUser, m_init_trace_parts(), address, signature);
  cerr << ' ';
  return cerr;
}

} //namespace


using namespace pyhrol;

SMART_FLAG_INSTANTIATION_BEGIN(traceParts, t)
{
  t.add(npMask, tpOptionsMask);
  t.add(static_cast<node_props>(npAnd | npTrue), tpHideAddress, "hide_address", tpOptionsMask);
  t.add(npAnd, tpShowNum, "show_num", tpHideAddress);
  t.add(npOr, tpNo, "no", tpOptionsMask);
  t.add(npOr, tpAll, "all", tpNo);
  t.add(npAnd, tpInternal, "internal", tpAll);
  t.add(npAnd, tpMediator, "mediator", tpInternal);
  t.add(npAnd, tpUser, "user", tpMediator);
}
SMART_FLAG_INSTANTIATION_END(traceParts, t)
