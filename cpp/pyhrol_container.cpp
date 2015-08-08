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

// $Date: 2015-08-08 11:22:54 +0300 (Сб., 08 авг. 2015) $
// $Revision: 1052 $

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iomanip>

#include <Python.h>

#include "pyhrol_tuple_format.h"
#include "pyhrol_container.h"
#include "pyhrol_common.h"
#include "pyhrol_type.h"
#include "pyhrol_trace.h"
#include "pyhrol_tuple_describer.h"
#include "pyhrol_tuples_default_impl.h"
#include "self_addressed_cstring.hpp"
#include "smart_flag.hpp"
#include "nullable.hpp"
#include "pyhrol_index.h"
#ifndef PYHROL_SAFE_MODE
#include "pyhrol_tuple_format.h"
#endif //PYHROL_SAFE_MODE

using namespace std;

#ifndef PYHROL_SAFE_MODE
extern bool _G_signature_hack_enabled;
#endif //PYHROL_SAFE_MODE

namespace pyhrol
{

const string Container::ctor_caption = "<ctor>";

Container::WrappersReused::WrappersReused()
  : logic_error("One or more static wrappers reused")
{
}

const std::string Container::TypeDeleted::m_describe(const size_t pos)
{
  ostringstream ostr;
  ostr << "Type object at offset " << pos << " unexpectedly deleted. It's name and properties has been completely lost";
  return ostr.str();
}

Container::TypeDeleted::TypeDeleted(const size_t pos)
  : logic_error(m_describe(pos))
{
}


const std::string Container::InvalidLink::m_describe(const void *address)
{
  if (address)
  {
    ostringstream ostr;
    ostr << "Wrapper at address 0x" << hex << reinterpret_cast<size_t>(address) << dec << " not found";
    return ostr.str();
  }
  else
  {
    return "Wrapper not found";
  }
}

Container::InvalidLink::InvalidLink()
  : logic_error(m_describe(NULL))
{
}

Container::InvalidLink::InvalidLink(const void *address)
  : logic_error(m_describe(address))
{
}


Container::Container()
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
  m_index.reset(new Index);

  PyMethodDef md_sentinel;
  bzero(&md_sentinel, sizeof(PyMethodDef));
  module_functions_t *mf = new module_functions_t;
  mf->push_back(md_sentinel);
  m_functions.push_back(mf);
}

Container::~Container()
{
  PYHROL_TRACE(tpInternal, this, __PRETTY_FUNCTION__);
  for (functions_t::const_iterator iter = m_functions.begin(); iter != m_functions.end(); iter ++)
  {
    const module_functions_t *mf = (*iter);
    for (module_functions_t::const_iterator iter_sub = mf->begin(); iter_sub != mf->end(); iter_sub ++)
    {
      if (iter_sub->ml_doc)
      {
        delete TupleDescriber::from_string(iter_sub->ml_doc);
      }
    }
    delete mf;
  }
  for (artificials_t::const_iterator iter = m_artificials.begin(), iter_end = m_artificials.end(); iter != iter_end; iter ++)
  {
    if (iter->second.doc)
    {
      delete TupleDescriber::from_string(iter->second.doc);
    }
  }
}

Container &Container::container()
{
  static Container c;
  return c;
}

#ifndef PYHROL_SAFE_MODE
const TupleFormat &Container::tupleFormat()
{
  if (!m_tuple_format.get())
  {
    m_tuple_format.reset(new TupleFormat(TupleFormat::initer()));
  }
  return *m_tuple_format;
}

void Container::setTupleFormat(const char *th, const char *tb, const char *eh, const char *eb)
{
  if (!m_tuple_format.get())
  {
    m_tuple_format.reset(new TupleFormat(TupleFormat::initer()));
  }

  if (th)
  {
    m_tuple_format->setTupleHeaderFormat(th);
  }
  if (tb)
  {
    m_tuple_format->setTupleBodyFormat(tb);
  }
  if (eh)
  {
    m_tuple_format->setErrorHeaderFormat(eh);
  }
  if (eb)
  {
    m_tuple_format->setErrorBodyFormat(eb);
  }

  if (_G_signature_hack_enabled && (th || tb || eh || eb))
  {
    for (modules_t::const_iterator iter = m_modules.begin(), iter_end = m_modules.end(); iter != iter_end; iter ++)
    {
      m_describe_tuples(*iter);
    }
  }
}

#endif //PYHROL_SAFE_MODE

PyMethodDef *Container::functions()
{
  return &(m_functions.at(m_modules.size() - 1))->at(0);
}

const PyMethodDef *Container::functions(const size_t module_id) const
{
  return &(m_functions.at(module_id))->at(0);
}

Container::types_range_t Container::types() const
{
  const module &m = *m_modules.rbegin();
  return types_range_t(m_types.begin() + m.types_begin, m_types.begin() + m.types_end);
}

const string &Container::module_name(const size_t module_id) const
{
  return m_modules[module_id].name;
}

const Index &Container::index() const
{
  return *m_index;
}

void Container::m_build_meta_index(Index &idx, const module &m) const
{
  size_t offset = 0;

  const PyMethodDef *pm;
  idx.add(Index::etModule, m.name, m.offset_self);

  if ((pm = &(*m_functions[m.offset_self])[0]))
  {
    offset = 0;
    while (pm->ml_meth)
    {
      idx.add(Index::etFunction, pm->ml_name, offset);
      pm ++;
      offset ++;
    }
  }
  for (size_t pos = m.types_begin; pos != m.types_end; pos ++)
  {
    const PyTypeObject *pt = m_types[pos];
    idx.add(Index::etType, isolate_type_name(pt->tp_name), pos);
    if (pt->tp_new)
    {
      idx.add(Index::etCtor, ctor_caption, 0);
    }
    if ((pm = pt->tp_methods))
    {
      offset = 0;
      while (pm->ml_meth)
      {
        idx.add(Index::etMethod, pm->ml_name, offset);
        pm ++;
        offset ++;
      }
    }
    const PyGetSetDef *pgs = pt->tp_getset;
    if (pgs)
    {
      offset = 0;
      while (pgs->get || pgs->set)
      {
        idx.add(Index::etGetsetter, pgs->name, offset);
        pgs ++;
        offset ++;
      }
    }
    artificials_t::const_iterator iter_af = m_artificials.find(artificials_t::key_type(pos, 0));
    while (iter_af != m_artificials.end() && iter_af->first.first == pos)
    {
      idx.add(Index::etSpecialCall, iter_af->second.name, iter_af->first.second);
      iter_af ++;
    }
  }
}

const PyTypeObject *Container::type(const size_t offset) const
{
  return m_types[offset];
}

const char *Container::protof_name(const size_t offset_type, const size_t offset_call) const
{
  return m_artificials.at(artificials_t::key_type(offset_type, offset_call)).name;
}

const char *Container::protof_doc(const size_t offset_type, const size_t offset_call) const
{
  return m_artificials.at(artificials_t::key_type(offset_type, offset_call)).doc;
}

const char **Container::doc_by_address(const size_t address) const
{
  address_idx_t::const_iterator iter = m_address_idx.find(address);
  if (iter != m_address_idx.end())
  {
    const call_position &p = iter->second;
    if (p.ct == ctGetter || p.ct == ctSetter)
    {
      return const_cast<const char **>(&m_types[p.offset_type]->tp_getset[p.offset_call].doc);
    }
    else if (p.ct == ctNumberCall || p.ct == ctSequenceCall || p.ct == ctMapCall)
    {
      return const_cast<const char **>(&m_artificials.at(artificials_t::key_type(p.offset_type, p.offset_call)).doc);
    }
    else if (p.ct == ctFunction)
    {
      return &m_functions.at(p.offset_module)->at(p.offset_call).ml_doc;
    }
    else if (p.ct == ctCtor)
    {
      return &m_types[p.offset_type]->tp_doc;
    }
    else if (p.ct == ctMethod)
    {
      return &m_types[p.offset_type]->tp_methods[p.offset_call].ml_doc;
    }
  }

  throw InvalidLink();
}

const std::string Container::context_by_address(const size_t address) const throw()
{
  bool valid = false;
  string retval;
  address_idx_t::const_iterator iter = m_address_idx.find(address);

  if (iter != m_address_idx.end())
  {
    try
    {
      const call_position &p = iter->second;

      if (m_modules.size() > 1)
      {
        retval += m_modules.at(p.offset_module).name;
        retval += '.';
      }
      if (p.ct == ctFunction)
      {
        retval += m_functions.at(p.offset_module)->at(p.offset_call).ml_name;
        valid = true;
      }
      else
      {
        const PyTypeObject *ptype = m_types[p.offset_type];
        retval += string(isolate_type_name(ptype->tp_name));
        retval += ".";
        if (p.ct == ctCtor)
        {
          retval += ctor_caption;
          valid = true;
        }
        else
        {
          if (p.ct == ctGetter || p.ct == ctSetter)
          {
            retval += ptype->tp_getset[p.offset_call].name;
            valid = true;
          }
          else if (p.ct == ctNumberCall || p.ct == ctSequenceCall || p.ct == ctMapCall)
          {
            retval += m_artificials.at(artificials_t::key_type(p.offset_type, p.offset_call)).name;
            valid = true;
          }
          else if (p.ct == ctMethod)
          {
            retval += ptype->tp_methods[p.offset_call].ml_name;
            valid = true;
          }
          else if (p.ct == ctOther)
          {
            retval += m_others.at(others_t::key_type(p.offset_type, p.offset_call)).first;
            valid = true;
          }
        }
      }
    }
    catch (const exception &ex)
    {
      retval = ex.what();
      retval += "; ";
    }
  }

  if (!valid)
  {
    ostringstream ostr;
    ostr << "0x" << hex << address << dec;
    retval += ostr.str();
  }
  return retval;
}

void Container::show_calls(ostream &os) const
{
  m_show_addresses_and_names(os, m_address_idx, m_name_idx);
}

void Container::indexate(const char *module_name)
{
  module m_new = {m_modules.size(), 0, m_types.size(), module_name};

  if (m_modules.size())
  {
    const module &m_prev = m_modules[m_modules.size() - 1];
    m_new.types_begin = m_prev.types_end;
  }
  m_modules.push_back(m_new);

  module_functions_t *mf = new module_functions_t;
  PyMethodDef md_sentinel;
  bzero(&md_sentinel, sizeof(PyMethodDef));
  mf->push_back(md_sentinel);
  m_functions.push_back(mf);

  m_qualify_types_with_module_name(m_new);
  m_fill_artificials(m_artificials, m_new);
  m_fill_others(m_others, m_new);
  m_indexate_addresses_and_names(m_address_idx, m_name_idx, m_new);
  m_build_meta_index(*m_index, m_new);
}


void Container::check_calls() const
{
  address_idx_t address_idx;
  name_idx_t name_idx;
  m_indexate_addresses_and_names(address_idx, name_idx, *m_modules.rbegin());

  const bool success = m_check_addresses(address_idx);
  m_check_names(name_idx);

  if (!success)
  {
    throw WrappersReused();
  }
}

#ifndef PYHROL_SAFE_MODE
void Container::describe_tuples()
{
  m_describe_tuples(*m_modules.rbegin());
}
#endif //PYHROL_SAFE_MODE

#ifdef _DEBUG_PYTHON_CONTAINER
void Container::__add_dummy_function(const char *name, const size_t address)
{
  m_add_function(name, reinterpret_cast<PyCFunction>(address), METH_VARARGS, NULL);
}
#endif //_DEBUG_PYTHON_CONTAINER

const size_t Container::register_type(PyTypeObject &type)
{
  m_types.push_back(&type);
  return m_types.size() - 1;
}

void Container::unregister_type(const size_t id)
{
  m_types[id] = NULL;
}

void Container::artificial::call() const
{
  switch (signature)
  {
    case 'A':
      (*callA)(NULL, NULL);
      break;
    case 'B':
      (*callB)(NULL, NULL, NULL);
      break;
    case 'C':
      (*callC)(NULL, NULL);
      break;
    case 'D':
      (*callD)(NULL, 0, NULL);
      break;
    case 'E':
      (*callE)(NULL, 0, 0, NULL);
      break;
    case 'F':
      (*callF)(NULL, NULL, NULL);
      break;
  }
}

size_t Container::artificial::address() const
{
  return reinterpret_cast<size_t>(callA);
}

Container::artificial::artificial(binaryfunc f, const char *_name, const call_types _ct)
  : signature('A')
  , name(_name)
  , doc(NULL)
  , ct(_ct)
  , callA(f)
{
}

Container::artificial::artificial(ternaryfunc f, const char *_name, const call_types _ct)
  : signature('B')
  , name(_name)
  , doc(NULL)
  , ct(_ct)
  , callB(f)
{
}

Container::artificial::artificial(objobjproc f, const char *_name, const call_types _ct)
  : signature('C')
  , name(_name)
  , doc(NULL)
  , ct(_ct)
  , callC(f)
{
}

Container::artificial::artificial(ssizeobjargproc f, const char *_name, const call_types _ct)
  : signature('D')
  , name(_name)
  , doc(NULL)
  , ct(_ct)
  , callD(f)
{
}

Container::artificial::artificial(ssizessizeobjargproc f, const char *_name, const call_types _ct)
  : signature('E')
  , name(_name)
  , doc(NULL)
  , ct(_ct)
  , callE(f)
{
}

Container::artificial::artificial(objobjargproc f, const char *_name, const call_types _ct)
  : signature('F')
  , name(_name)
  , doc(NULL)
  , ct(_ct)
  , callF(f)
{
}

const Container::reserved_t Container::m_init_reserved()
{
  reserved_t retval;

  const char
      *words[] = {"and", "del", "from", "not", "while", "as", "elif", "global", "or", "with", "assert", "else", "if", "pass", "yield", "break", "except", "import", "print", "class", "exec", "in", "raise", "continue", "finally", "is", "return", "def", "for", "lambda", "try", NULL}
    , **word = words
  ;

  while (*word)
  {
    retval.insert(*word);
    word ++;
  }
  return retval;
}

const char Container::m_call_type_2_char(const call_types ct) const
{
  char ch;

  switch (ct)
  {
    case ctFunction:
      ch = 'f';
      break;
    case ctMethod:
      ch = 'm';
      break;
    case ctGetter:
      ch = '<';
      break;
    case ctSetter:
      ch = '>';
      break;
    case ctCtor:
      ch = '+';
      break;
    case ctNumberCall:
      ch = '#';
      break;
    case ctSequenceCall:
      ch = '$';
      break;
    case ctMapCall:
      ch = '=';
      break;
    case ctOther:
      ch = '*';
      break;
   }
  return ch;
}

PyTypeObject *Container::m_get_type(const size_t pos)
{
  PyTypeObject *retval = m_types[pos];
  if (!retval)
  {
    throw TypeDeleted(pos);
  }
  return retval;
}

const PyTypeObject *Container::m_get_type(const size_t pos) const
{
  const PyTypeObject *retval = m_types[pos];
  if (!retval)
  {
    throw TypeDeleted(pos);
  }
  return retval;
}

void Container::m_get_name_props(name_props &t, name_props &c, const call_position &p) const
{
  static const reserved_t rl = m_init_reserved();

  t = npOK;
  c = npOK;

  if (p.ct == ctFunction)
  {
    const char *fname = m_functions.at(p.offset_module)->at(p.offset_call).ml_name;
    if (fname[0] == '_')
    {
      c = npUnderscorePrefixed;
    }
    else if (rl.count(fname))
    {
      c = npIsReservedWord;
    }
  }
  else
  {
    const PyTypeObject &tobj = *m_get_type(p.offset_type);
    if (*isolate_type_name(tobj.tp_name) == '_')
    {
      t = npUnderscorePrefixed;
    }
    else if (rl.count(isolate_type_name(tobj.tp_name)))
    {
      t = npIsReservedWord;
    }

    if (p.ct == ctMethod)
    {
      if (tobj.tp_methods[p.offset_call].ml_name[0] == '_')
      {
        c = npUnderscorePrefixed;
      }
      else if (rl.count(tobj.tp_methods[p.offset_call].ml_name))
      {
        c = npIsReservedWord;
      }
    }
    else if (p.ct == ctGetter || p.ct == ctSetter)
    {
      if (tobj.tp_getset[p.offset_call].name[0] == '_')
      {
        c = npUnderscorePrefixed;
      }
      else if (rl.count(tobj.tp_getset[p.offset_call].name))
      {
        c = npIsReservedWord;
      }
    }
  }
}

const string Container::m_get_call_name(const call_position &p, const bool with_type) const
{
  string retval;

  if (p.ct == ctFunction)
  {
    retval += m_functions.at(p.offset_module)->at(p.offset_call).ml_name;
  }
  else
  {
    const PyTypeObject &t = *m_get_type(p.offset_type);
    if (with_type)
    {
      retval += isolate_type_name(t.tp_name);
      retval += '.';
    }
    if (p.ct == ctMethod)
    {
      retval += t.tp_methods[p.offset_call].ml_name;
    }
    else if (p.ct == ctGetter || p.ct == ctSetter)
    {
      retval += t.tp_getset[p.offset_call].name;
    }
    else if (p.ct == ctCtor)
    {
      retval += ctor_caption;
    }
    else if (p.ct == ctNumberCall || p.ct == ctSequenceCall || p.ct == ctMapCall)
    {
      retval += m_artificials.at(artificials_t::key_type(p.offset_type, p.offset_call)).name;
    }
    else if (p.ct == ctOther)
    {
      retval += m_others.at(others_t::key_type(p.offset_type, p.offset_call)).first;
    }
  }
  return retval;
}

void Container::m_qualify_types_with_module_name(const module &m)
{
  for (size_t offset_type = m.types_begin; offset_type != m.types_end; offset_type ++)
  {
    PyTypeObject *ptype = m_get_type(offset_type);

    SelfAddressedCstring *pstr = SelfAddressedCstring::construct(ptype->tp_name);
    const size_t len_name = strlen(ptype->tp_name);
    char *ptr = pstr->allocate(m.name.length() + 1 + len_name + 1);
    memmove(ptr + m.name.length() + 1, ptr, len_name + 1);
    memcpy(ptr, m.name.c_str(), m.name.length());
    ptr[m.name.length()] = '.';
    pstr->get(ptype->tp_name);
  }
}

void Container::m_fill_artificials(artificials_t &artificials, const module &m) const
{
  for (size_t offset_type = m.types_begin; offset_type != m.types_end; offset_type ++)
  {
    const PyTypeObject *ptype = m_get_type(offset_type);

    size_t offset_call = 0;

    if (ptype->tp_as_number && (ptype->tp_flags & Py_TPFLAGS_CHECKTYPES))
    {
      const PyNumberMethods *pmethods = ptype->tp_as_number;

      if (pmethods->nb_add)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_add, "<nb_add>", ctNumberCall)));
      }
      if (pmethods->nb_subtract)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_subtract, "<nb_subtract>", ctNumberCall)));
      }
      if (pmethods->nb_multiply)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_multiply, "<nb_multiply>", ctNumberCall)));
      }
      if (pmethods->nb_divide)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_divide, "<nb_divide>", ctNumberCall)));
      }
      if (pmethods->nb_remainder)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_remainder, "<nb_remainder>", ctNumberCall)));
      }
      if (pmethods->nb_divmod)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_divmod, "<nb_divmod>", ctNumberCall)));
      }
      if (pmethods->nb_power)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_power, "<nb_power>", ctNumberCall)));
      }
      if (pmethods->nb_lshift)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_lshift, "<nb_lshift>", ctNumberCall)));
      }
      if (pmethods->nb_rshift)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_rshift, "<nb_rshift>", ctNumberCall)));
      }
      if (pmethods->nb_and)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_and, "<nb_and>", ctNumberCall)));
      }
      if (pmethods->nb_xor)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_xor, "<nb_xor>", ctNumberCall)));
      }
      if (pmethods->nb_or)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_or, "<nb_or>", ctNumberCall)));
      }
      if (pmethods->nb_floor_divide)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_floor_divide, "<nb_floor_divide>", ctNumberCall)));
      }
      if (pmethods->nb_true_divide)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_true_divide, "<nb_true_divide>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_add)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_add, "<nb_inplace_add>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_subtract)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_subtract, "<nb_inplace_subtract>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_multiply)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_multiply, "<nb_inplace_multiply>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_divide)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_divide, "<nb_inplace_divide>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_remainder)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_remainder, "<nb_inplace_remainder>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_power)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_power, "<nb_inplace_power>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_lshift)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_lshift, "<nb_inplace_lshift>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_rshift)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_rshift, "<nb_inplace_rshift>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_and)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_and, "<nb_inplace_and>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_xor)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_xor, "<nb_inplace_xor>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_or)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_or, "<nb_inplace_or>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_floor_divide)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_floor_divide, "<nb_inplace_floor_divide>", ctNumberCall)));
      }
      if (pmethods->nb_inplace_true_divide)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->nb_inplace_true_divide, "<nb_inplace_true_divide>", ctNumberCall)));
      }
    }
    if (ptype->tp_as_sequence)
    {
      const PySequenceMethods *pmethods = ptype->tp_as_sequence;

      if (pmethods->sq_concat)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->sq_concat, "<sq_concat>", ctSequenceCall)));
      }
      if (pmethods->sq_inplace_concat)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->sq_inplace_concat, "<sq_inplace_concat>", ctSequenceCall)));
      }
      if (pmethods->sq_contains)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->sq_contains, "<sq_contains>", ctSequenceCall)));
      }
      if (pmethods->sq_ass_item)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->sq_ass_item, "<sq_ass_item>", ctSequenceCall)));
      }
      if (pmethods->sq_ass_slice)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->sq_ass_slice, "<sq_ass_slice>", ctSequenceCall)));
      }
    }
    if (ptype->tp_as_mapping)
    {
      const PyMappingMethods *pmethods = ptype->tp_as_mapping;

      if (pmethods->mp_subscript)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->mp_subscript, "<mp_subscript>", ctMapCall)));
      }
      if (pmethods->mp_ass_subscript)
      {
        artificials.insert(artificials_t::value_type(artificials_t::key_type(offset_type, offset_call ++), artificial(pmethods->mp_ass_subscript, "<mp_ass_subscript>", ctMapCall)));
      }
    }
  }
}

void Container::m_fill_others(others_t &others, const module &m) const
{
  for (size_t offset_type = m.types_begin; offset_type != m.types_end; offset_type ++)
  {
    const PyTypeObject *ptype = m_get_type(offset_type);

    size_t offset_call = 0;
    // TODO Use macro with __STRING() here
    if (ptype->tp_as_number)
    {
      const PyNumberMethods *pmethods = ptype->tp_as_number;
      if (pmethods->nb_negative)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_negative>", reinterpret_cast<size_t>(pmethods->nb_negative));
      }
      if (pmethods->nb_positive)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_positive>", reinterpret_cast<size_t>(pmethods->nb_positive));
      }
      if (pmethods->nb_absolute)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_absolute>", reinterpret_cast<size_t>(pmethods->nb_absolute));
      }
      if (pmethods->nb_nonzero)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_nonzero>", reinterpret_cast<size_t>(pmethods->nb_nonzero));
      }
      if (pmethods->nb_invert)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_invert>", reinterpret_cast<size_t>(pmethods->nb_invert));
      }
      if (pmethods->nb_int)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_int>", reinterpret_cast<size_t>(pmethods->nb_int));
      }
      if (pmethods->nb_long)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_long>", reinterpret_cast<size_t>(pmethods->nb_long));
      }
      if (pmethods->nb_float)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_float>", reinterpret_cast<size_t>(pmethods->nb_float));
      }
      if (pmethods->nb_oct)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_oct>", reinterpret_cast<size_t>(pmethods->nb_oct));
      }
      if (pmethods->nb_hex)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_hex>", reinterpret_cast<size_t>(pmethods->nb_hex));
      }
      if (pmethods->nb_index)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_index>", reinterpret_cast<size_t>(pmethods->nb_index));
      }
      if (!(ptype->tp_flags & Py_TPFLAGS_CHECKTYPES))
      {
        if (pmethods->nb_add)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_add>", reinterpret_cast<size_t>(pmethods->nb_add));
        }
        if (pmethods->nb_subtract)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_subtract>", reinterpret_cast<size_t>(pmethods->nb_subtract));
        }
        if (pmethods->nb_multiply)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_multiply>", reinterpret_cast<size_t>(pmethods->nb_multiply));
        }
        if (pmethods->nb_divide)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_divide>", reinterpret_cast<size_t>(pmethods->nb_divide));
        }
        if (pmethods->nb_remainder)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_remainder>", reinterpret_cast<size_t>(pmethods->nb_remainder));
        }
        if (pmethods->nb_divmod)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_divmod>", reinterpret_cast<size_t>(pmethods->nb_divmod));
        }
        if (pmethods->nb_power)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_power>", reinterpret_cast<size_t>(pmethods->nb_power));
        }
        if (pmethods->nb_lshift)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_lshift>", reinterpret_cast<size_t>(pmethods->nb_lshift));
        }
        if (pmethods->nb_rshift)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_rshift>", reinterpret_cast<size_t>(pmethods->nb_rshift));
        }
        if (pmethods->nb_and)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_and>", reinterpret_cast<size_t>(pmethods->nb_and));
        }
        if (pmethods->nb_xor)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_xor>", reinterpret_cast<size_t>(pmethods->nb_xor));
        }
        if (pmethods->nb_or)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_or>", reinterpret_cast<size_t>(pmethods->nb_or));
        }
        if (pmethods->nb_floor_divide)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_floor_divide>", reinterpret_cast<size_t>(pmethods->nb_floor_divide));
        }
        if (pmethods->nb_true_divide)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_true_divide>", reinterpret_cast<size_t>(pmethods->nb_true_divide));
        }
        if (pmethods->nb_inplace_add)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_add>", reinterpret_cast<size_t>(pmethods->nb_inplace_add));
        }
        if (pmethods->nb_inplace_subtract)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_subtract>", reinterpret_cast<size_t>(pmethods->nb_inplace_subtract));
        }
        if (pmethods->nb_inplace_multiply)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_multiply>", reinterpret_cast<size_t>(pmethods->nb_inplace_multiply));
        }
        if (pmethods->nb_inplace_divide)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_divide>", reinterpret_cast<size_t>(pmethods->nb_inplace_divide));
        }
        if (pmethods->nb_inplace_remainder)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_remainder>", reinterpret_cast<size_t>(pmethods->nb_inplace_remainder));
        }
        if (pmethods->nb_inplace_power)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_power>", reinterpret_cast<size_t>(pmethods->nb_inplace_power));
        }
        if (pmethods->nb_inplace_lshift)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_lshift>", reinterpret_cast<size_t>(pmethods->nb_inplace_lshift));
        }
        if (pmethods->nb_inplace_rshift)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_rshift>", reinterpret_cast<size_t>(pmethods->nb_inplace_rshift));
        }
        if (pmethods->nb_inplace_and)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_and>", reinterpret_cast<size_t>(pmethods->nb_inplace_and));
        }
        if (pmethods->nb_inplace_xor)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_xor>", reinterpret_cast<size_t>(pmethods->nb_inplace_xor));
        }
        if (pmethods->nb_inplace_or)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_or>", reinterpret_cast<size_t>(pmethods->nb_inplace_or));
        }
        if (pmethods->nb_inplace_floor_divide)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_floor_divide>", reinterpret_cast<size_t>(pmethods->nb_inplace_floor_divide));
        }
        if (pmethods->nb_inplace_true_divide)
        {
          others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<nb_inplace_true_divide>", reinterpret_cast<size_t>(pmethods->nb_inplace_true_divide));
        }
      }
    }
    if (ptype->tp_as_sequence)
    {
      const PySequenceMethods *pmethods = ptype->tp_as_sequence;
      if (pmethods->sq_length)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<sq_length>", reinterpret_cast<size_t>(pmethods->sq_length));
      }
      if (pmethods->sq_repeat)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<sq_repeat>", reinterpret_cast<size_t>(pmethods->sq_repeat));
      }
      if (pmethods->sq_item)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<sq_item>", reinterpret_cast<size_t>(pmethods->sq_item));
      }
      if (pmethods->sq_slice)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<sq_slice>", reinterpret_cast<size_t>(pmethods->sq_slice));
      }
      if (pmethods->sq_inplace_repeat)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<sq_inplace_repeat>", reinterpret_cast<size_t>(pmethods->sq_inplace_repeat));
      }
    }
    if (ptype->tp_as_mapping)
    {
      const PyMappingMethods *pmethods = ptype->tp_as_mapping;

      if (pmethods->mp_length)
      {
        others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<mp_length>", reinterpret_cast<size_t>(pmethods->mp_length));
      }
    }

    if (ptype->tp_dealloc)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_dealloc>", reinterpret_cast<size_t>(ptype->tp_dealloc));
    }
    if (ptype->tp_print)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_print>", reinterpret_cast<size_t>(ptype->tp_print));
    }
    if (ptype->tp_getattr)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_getattr>", reinterpret_cast<size_t>(ptype->tp_getattr));
    }
    if (ptype->tp_setattr)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_setattr>", reinterpret_cast<size_t>(ptype->tp_setattr));
    }
    if (ptype->tp_compare)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_compare>", reinterpret_cast<size_t>(ptype->tp_compare));
    }
    if (ptype->tp_repr)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_repr>", reinterpret_cast<size_t>(ptype->tp_repr));
    }
    if (ptype->tp_hash)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_hash>", reinterpret_cast<size_t>(ptype->tp_hash));
    }
    if (ptype->tp_call)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_call>", reinterpret_cast<size_t>(ptype->tp_call));
    }
    if (ptype->tp_str)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_str>", reinterpret_cast<size_t>(ptype->tp_str));
    }
    if (ptype->tp_getattro)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_getattro>", reinterpret_cast<size_t>(ptype->tp_getattro));
    }
    if (ptype->tp_traverse)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_traverse>", reinterpret_cast<size_t>(ptype->tp_traverse));
    }
    if (ptype->tp_clear)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_clear>", reinterpret_cast<size_t>(ptype->tp_clear));
    }
    if (ptype->tp_richcompare)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_richcompare>", reinterpret_cast<size_t>(ptype->tp_richcompare));
    }
    if (ptype->tp_iter)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_iter>", reinterpret_cast<size_t>(ptype->tp_iter));
    }
    if (ptype->tp_iternext)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_iternext>", reinterpret_cast<size_t>(ptype->tp_iternext));
    }
    if (ptype->tp_descr_get)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_descr_get>", reinterpret_cast<size_t>(ptype->tp_descr_get));
    }
    if (ptype->tp_descr_set)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_descr_set>", reinterpret_cast<size_t>(ptype->tp_descr_set));
    }
    if (ptype->tp_init)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_init>", reinterpret_cast<size_t>(ptype->tp_init));
    }
    if (ptype->tp_alloc)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_alloc>", reinterpret_cast<size_t>(ptype->tp_alloc));
    }
    if (ptype->tp_free)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_free>", reinterpret_cast<size_t>(ptype->tp_free));
    }
    if (ptype->tp_is_gc)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_is_gc>", reinterpret_cast<size_t>(ptype->tp_is_gc));
    }
    if (ptype->tp_del)
    {
      others[others_t::key_type(offset_type, offset_call ++)] = others_t::mapped_type("<tp_del>", reinterpret_cast<size_t>(ptype->tp_del));
    }
  }
}

void Container::m_indexate_addresses_and_names(address_idx_t &address_idx, name_idx_t &name_idx, const module &m) const
{
  const PyMethodDef *pmethod;
  const PyGetSetDef *pgetseter;

  if ((pmethod = &m_functions.at(m.offset_self)->at(0)))
  {
    const PyMethodDef *pbegin = pmethod;
    while (pmethod->ml_meth)
    {
      call_position p = {ctFunction, 0, pmethod - pbegin, m.offset_self};
      address_idx.insert(address_idx_t::value_type(reinterpret_cast<size_t>(pmethod->ml_meth), p));
      name_idx.insert(name_idx_t::value_type(pmethod->ml_name, p));
      pmethod ++;
    }
  }

  for (size_t pos = m.types_begin; pos != m.types_end; pos ++)
  {
    const PyTypeObject *ptype = m_get_type(pos);
    const string ptype_name = isolate_type_name(ptype->tp_name);

    if ((pmethod = ptype->tp_methods))
    {
      while (pmethod->ml_meth)
      {
        call_position p = {ctMethod, pos, pmethod - ptype->tp_methods, m.offset_self};
        address_idx.insert(address_idx_t::value_type(reinterpret_cast<size_t>(pmethod->ml_meth), p));
        name_idx.insert(name_idx_t::value_type(ptype_name + '.' + pmethod->ml_name, p));
        pmethod ++;
      }
    }

    if ((pgetseter = ptype->tp_getset))
    {
      while (pgetseter->set || pgetseter->get)
      {
        if (pgetseter->set)
        {
          call_position p = {ctSetter, pos, pgetseter - ptype->tp_getset, m.offset_self};
          address_idx.insert(address_idx_t::value_type(reinterpret_cast<size_t>(pgetseter->set), p));
          name_idx.insert(name_idx_t::value_type(ptype_name + '.' + pgetseter->name, p));
        }
        if (pgetseter->get)
        {
          call_position p = {ctGetter, pos, pgetseter - ptype->tp_getset, m.offset_self};
          address_idx.insert(address_idx_t::value_type(reinterpret_cast<size_t>(pgetseter->get), p));
          name_idx.insert(name_idx_t::value_type(ptype_name + '.' + pgetseter->name, p));
        }
        pgetseter ++;
      }
    }

    if (ptype->tp_new)
    {
      call_position p = {ctCtor, pos, 0, m.offset_self};
      address_idx.insert(address_idx_t::value_type(reinterpret_cast<size_t>(ptype->tp_new), p));
      name_idx.insert(name_idx_t::value_type(ptype_name + '.' + ctor_caption, p));
    }


    artificials_t::const_iterator iter = m_artificials.find(artificials_t::key_type(pos, 0));
    while (iter != m_artificials.end() && iter->first.first == pos)
    {
      call_position p = {iter->second.ct, iter->first.first, iter->first.second, m.offset_self};
      address_idx.insert(address_idx_t::value_type(iter->second.address(), p));
      iter ++;
    }

    others_t::const_iterator iter_others = m_others.find(others_t::key_type(pos, 0));
    while (iter_others != m_others.end() && iter_others->first.first == pos)
    {
      call_position p = {ctOther, iter_others->first.first, iter_others->first.second, m.offset_self};
      address_idx.insert(address_idx_t::value_type(iter_others->second.second, p));
      iter_others ++;
    }
  }
}

const std::string Container::m_show_addresses_and_names(const address_idx_t &addrs, const name_idx_t &names) const
{
  ostringstream ostr;
  m_show_addresses_and_names(ostr, addrs, names);
  return ostr.str();
}

void Container::m_show_addresses_and_names(ostream &os, const address_idx_t &addrs, const name_idx_t &names) const
{
  size_t cnt = 0;
  address_idx_t::const_iterator
      iter_address = addrs.begin()
    , iter_address_prev
  ;
  name_idx_t::const_iterator
      iter_name = names.begin()
    , iter_name_prev
  ;

  while (true)
  {
    const bool
        eof_addrs = (iter_address == addrs.end())
      , eof_names = (iter_name == names.end())
    ;
    if (eof_addrs && eof_names)
    {
      break;
    }

    if (!eof_addrs)
    {
      if (!cnt || iter_address_prev->first != iter_address->first)
      {
        iter_address_prev = iter_address;
        cnt = 0;
      }
    }
    else if (!cnt || iter_name_prev->first != iter_name->first) // !eof_names
    {
      iter_name_prev = iter_name;
      cnt = 0;
    }
    cnt ++;

    const call_position *p;
    if (!eof_addrs)
    {
      p = &iter_address->second;
    }
    else // !eof_names
    {
      p = &iter_name->second;
    }
    const char ch = m_call_type_2_char (p->ct);

    os << setw(50);
    if (!eof_addrs)
    {
      os << hex << iter_address->first << dec;
    }
    else // !eof_names
    {
      os << left << iter_name->first << right;
    }
    name_props t, c;
    m_get_name_props(t, c, *p);
    os << ' ' << (t != npOK ? '!' : ' ') << (c != npOK ? '!' : ' ');
    if (cnt > 1)
    {
      os << " (" << cnt << ")";
    }
    else
    {
      os << setw(4) << ' ';
    }
    os << setw(2) << ch << setw(3);
    if (p->ct == ctFunction)
    {
      os << ' ';
    }
    else
    {
      os << p->offset_type;
    }
    os << setw(3) << p->offset_call << ' ' << setw(50);
    if (!eof_addrs)
    {
      os << left << m_get_call_name(*p) << right;
    }
    else
    {
      os << ' ';
    }
    os
      << m_modules[p->offset_module].name << endl
    ;
    if (!eof_addrs)
    {
      iter_address ++;
      if (iter_address == addrs.end())
      {
        cnt = 0;
        os
          << "---" << endl
        ;
      }
    }
    else // !eof_names
    {
      iter_name ++;
    }
  }
}

bool Container::m_check_addresses(const address_idx_t &addrs) const
{
  bool retval = true;
  address_idx_t::const_iterator
      iter = addrs.begin()
    , iter_prev
  ;
  size_t cnt = 0;

  while (true)
  {
    const bool eof = (iter == addrs.end());
    if (!cnt || eof || iter->first != iter_prev->first)
    {
      if (cnt > 1)
      {
        string calls;
        for (address_idx_t::const_iterator iter_dup = iter_prev; iter_dup != iter; iter_dup ++)
        {
          if (iter_dup != iter_prev)
          {
            calls += ", ";
          }
          calls += m_get_call_name(iter_dup->second);
        }
        PyErr_Format(PyExc_AttributeError, "Following calls share the same static wrapper {%s}", calls.c_str());
        PyErr_PrintEx(0);
        PyErr_Clear();

        retval = false;
      }
      cnt = 0;
      iter_prev = iter;
    }
    if (eof)
    {
      break;
    }
    cnt ++;
    iter ++;
  }
  return retval;
}

void Container::m_check_names(const name_idx_t &names) const
{
  name_idx_t::const_iterator
      iter = names.begin()
    , iter_prev
  ;
  string
      type_name_prev
    , call_name_prev
  ;
  uint32_t cnt = 0;

  while (true)
  {
    const bool eof = (iter == names.end());
    if (!cnt || eof || iter->first != iter_prev->first)
    {
      if (cnt > 1)
      {
        size_t
            cnt_getter = 0
          , cnt_setter = 0
        ;
        for (name_idx_t::const_iterator iter_dup = iter_prev; iter_dup != iter; iter_dup ++)
        {
          const call_position &p = iter_dup->second;
          if (p.ct == ctGetter)
          {
            cnt_getter ++;
          }
          else if (p.ct == ctSetter)
          {
            cnt_setter ++;
          }
        }
        if (cnt != 2 || cnt_getter != 1 && cnt_setter != 1)
        {
          cnt -= (cnt_getter && cnt_setter ? 1 : 0);
          PyErr_Format(PyExc_AttributeError, "Call name \"%s\" non unique and used %u times", iter_prev->first.c_str(), cnt);
          PyErr_PrintEx(0);
          PyErr_Clear();
        }
      }
      cnt = 0;
      iter_prev = iter;
    }
    if (eof)
    {
      break;
    }
    else
    {
      const string type_name = (iter->second.ct != ctFunction ? isolate_type_name(m_get_type(iter->second.offset_type)->tp_name) : "");
      const string call_name = m_get_call_name(iter->second, false);

      name_props t, c;
      m_get_name_props(t, c, iter->second);
      if (!type_name_prev.length() || type_name != type_name_prev)
      {
        if (t == npUnderscorePrefixed)
        {
          PyErr_Format(PyExc_AttributeError, "Type name \"%s\" starts from underscore, therefore will be ignored by interpreter", type_name.c_str());
          PyErr_PrintEx(0);
          PyErr_Clear();
        }
        else if (t == npIsReservedWord)
        {
          PyErr_Format(PyExc_AttributeError, "Type name \"%s\" invalid because it is language keyword", type_name.c_str());
          PyErr_PrintEx(0);
          PyErr_Clear();
        }
      }
      if (!call_name_prev.length() || call_name != call_name_prev)
      {
        if (c == npUnderscorePrefixed)
        {
          if (iter->second.ct != ctFunction)
          {
            PyErr_Format(PyExc_AttributeError, "Method name \"%s\" starts from underscore, therefore will be ignored by interpreter (%s)", call_name.c_str(), m_get_call_name(iter->second).c_str());
          }
          else
          {
            PyErr_Format(PyExc_AttributeError, "Function name \"%s\" starts from underscore, therefore will be ignored by interpreter", call_name.c_str());
          }
          PyErr_PrintEx(0);
          PyErr_Clear();
        }
        else if (c == npIsReservedWord)
        {
          if (iter->second.ct != ctFunction)
          {
            PyErr_Format(PyExc_AttributeError, "Method name \"%s\" invalid because it is language keyword (%s)", call_name.c_str(), m_get_call_name(iter->second).c_str());
          }
          else
          {
            PyErr_Format(PyExc_AttributeError, "Function name \"%s\" invalid because it is language keyword", call_name.c_str());
          }
          PyErr_PrintEx(0);
          PyErr_Clear();
        }
      }
      type_name_prev = type_name;
      call_name_prev = call_name;
    }
    cnt ++;
    iter ++;
  }
}

void Container::m_describe_tuples(const module &m)
{
  PyMethodDef *current_method;
  PyObject dummy;

  if ((current_method = &m_functions.at(m.offset_self)->at(0)))
  {
    while (current_method->ml_meth)
    {
      if (current_method->ml_flags == METH_VARARGS)
      {
        (*current_method->ml_meth)(&dummy, NULL);
      }
      else if (current_method->ml_flags == (METH_KEYWORDS | METH_VARARGS))
      {
        (*reinterpret_cast<PyCFunctionWithKeywords>(current_method->ml_meth))(&dummy, NULL, NULL);
      }
      else if (current_method->ml_flags == METH_NOARGS)
      {
        (*reinterpret_cast<PyNoArgsFunction>(current_method->ml_meth))(&dummy);
      }
      current_method ++;
    }
  }

  for (size_t pos = m.types_begin; pos != m.types_end; pos ++)
  {
    const PyTypeObject *current_type = m_get_type(pos);

    if ((current_method = current_type->tp_methods))
    {
      while (current_method->ml_meth)
      {
        if (current_method->ml_flags & METH_VARARGS)
        {
          (*current_method->ml_meth)(current_method->ml_flags & METH_STATIC ? &dummy : NULL, NULL);
        }
        else if (current_method->ml_flags & (METH_KEYWORDS | METH_VARARGS))
        {
          (*reinterpret_cast<PyCFunctionWithKeywords>(current_method->ml_meth))(current_method->ml_flags & METH_STATIC ? &dummy : NULL, NULL, NULL);
        }
        else if (current_method->ml_flags & METH_NOARGS)
        {
          (*reinterpret_cast<PyNoArgsFunction>(current_method->ml_meth))(current_method->ml_flags & METH_STATIC ? &dummy : NULL);
        }
        current_method ++;
      }
      current_method = NULL;
    }

    PyGetSetDef *current_getseter;
    if ((current_getseter = current_type->tp_getset))
    {
      while (current_getseter->set || current_getseter->get)
      {
        if (current_getseter->set)
        {
          (*current_getseter->set)(NULL, NULL, NULL);
        }
        if (current_getseter->get)
        {
          (*current_getseter->get)(NULL, NULL);
        }
        current_getseter ++;
      }
      current_getseter = NULL;
    }

    if (current_type->tp_new)
    {
      current_type->tp_new(NULL, NULL, NULL);
    }

    artificials_t::const_iterator iter = m_artificials.find(artificials_t::key_type(pos, 0));
    while (iter != m_artificials.end() && iter->first.first == pos)
    {
      iter->second.call();
      iter ++;
    }
  }
}

void Container::m_add_function(const char *name, const PyCFunction &function, const int flags, const char *help)
{
  PyMethodDef md;

  md.ml_name = name;
  md.ml_meth = function;
  md.ml_flags = flags;
  md.ml_doc = NULL;
  if (help)
  {
    TupleDescriber *pdescr = TupleDescriber::factory();
    if (pdescr)
    {
      pdescr->help(help);
      md.ml_doc = pdescr->to_string();
    }
  }

  module_functions_t &mf = **m_functions.rbegin();
  mf.insert(-- mf.end(), md);
}

} //namespace pyhrol
