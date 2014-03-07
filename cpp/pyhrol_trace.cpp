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

// $Date: 2014-01-26 21:24:54 +0400 (Sun, 26 Jan 2014) $
// $Revision: 881 $

#include <iostream>
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

static traceParts m_init_trace_parts()
{
  const char *const env_name = "PYHROL_TRACE";
  trace_parts_flag_t ret_val (tpNo);
  const char *val = getenv(env_name);

  if (val && *val)
  {
    try
    {
      ret_val = val;
    }
    catch(const exception &ex)
    {
      PyErr_Format(PyExc_UserWarning, "Environment variable \"%s\" has invalid value, tracing not started\n  %s\n  Possible values: %s", env_name, ex.what(), ret_val->statement().c_str());
      PyErr_PrintEx(0);
      PyErr_Clear();
    }
  }
  return ret_val;
}

typedef map<size_t, size_t> addresses_t;
traceParts _G_show_parts = m_init_trace_parts();
addresses_t _G_addresses;

static void trace_head(const traceParts tp, const void *ptr, const char *signature)
{
  if (tp & tpInternal)
  {
    cerr << 'I';
  }
  else if (tp & tpMediator)
  {
    cerr << 'M';
  }
  else if (tp & tpUser)
  {
    cerr << 'U';
  }

  const size_t buff_size = 32;
  char buff[buff_size];
  const size_t address = reinterpret_cast<size_t>(ptr);

  if (!(_G_show_parts & tpHideAddress))
  {
  #ifdef __LP64__
    if (address)
    {
      snprintf(buff, buff_size, " 0x%016lX", address);
      cerr << buff;
    }
    else
    {
      cerr << setw(19) << ' ';
    }
  #else
    if (address)
    {
      snprintf(buff, buff_size, " 0x%08X", address);
      cerr << buff;
    }
    else
    {
      cerr << setw(11) << ' ';
    }
  #endif
  }

  if (_G_show_parts & tpShowNum)
  {
    cerr << setw(5);
    if (address)
    {
      addresses_t::const_iterator iter = _G_addresses.find(address);
      if (iter == _G_addresses.end())
      {
        const size_t num = _G_addresses.size();
        _G_addresses[address] = num;
        cerr << num;
      }
      else
      {
        cerr << iter->second;
      }
    }
    else
    {
      cerr << ' ';
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
          cerr << ' ' << string(begin, end);
          break;
        case ParserState::pTemplateName:
          {
            const string name(begin, end);
            filtered = (name == "PyObject" || name == "PyTypeObject" || name == "FILE");
            if (!filtered)
            {
              if (with)
              {
                cerr << ", ";
              }
              else
              {
                cerr << " [with ";
                with = true;
              }
              cerr << name;
            }
          }
          break;
        case ParserState::pTemplateValue:
          if (!filtered)
          {
            cerr << " = " << string(begin, end);
          }
          break;
        case ParserState::pTemplates:
          cerr << string(begin, end);
          break;
        default:
          cerr << "<?parse error>";
      }
    }
    if (with)
    {
      cerr << "]";
    }
  }
}

void trace(const traceParts tp, const void *address, const char *signature)
{
  if (!(tp & _G_show_parts))
  {
    return;
  }

  trace_head(tp, address, signature);

  cerr
    << endl
  ;
}

bool user_trace_enbled()
{
  return _G_show_parts & tpUser;
}

ostream &user_trace(const void *address, const char *signature)
{
  trace_head(tpUser, address, signature);
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
SMART_FLAG_INSTANTIATION_END(t);
