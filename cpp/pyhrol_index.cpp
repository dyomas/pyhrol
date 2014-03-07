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

#include <iomanip>
#include <iostream>
#include <sstream>

#include "pyhrol_index.h"
#include "smart_flag.hpp"
#include "nullable.hpp"

using namespace std;

namespace pyhrol
{

const size_t Index::num_width = 5;

const string Index::NotFoundException::m_describe(const char *name, const elementTypes et)
{
  ostringstream ostr;
  switch (et)
  {
    case etModule:
      ostr << "Module";
      break;
    case etType:
      ostr << "Type";
      break;
    case etCtor:
      ostr << "Constructor";
      break;
    case etMethod:
    case etGetsetter:
    case etSpecialCall:
      ostr << "Method";
      break;
    case etFunction:
      ostr << "Function";
      break;
  };

  ostr <<  " `" << name << "` not found";
  return ostr.str();
}

Index::NotFoundException::NotFoundException(const char *name)
  : KeyException(string("Element `") + name + "` not found")
{
}

Index::NotFoundException::NotFoundException(const char *name, const elementTypes et)
  : KeyException(m_describe(name, et))
{
}

const string Index::NonUniqueException::m_describe(const char *name, const size_t cnt)
{
  ostringstream ostr;
  ostr << "Element with name `" << name << "` is not unique (" << cnt << " occurrences)";
  return ostr.str();
}

Index::NonUniqueException::NonUniqueException(const char *name, const size_t cnt)
  : KeyException(m_describe(name, cnt))
{
}

const string Index::WrongTypeException::m_describe(const char *name, const elementTypes real, const elementTypes expected)
{
  ostringstream ostr;
  ostr << "Element `" << name << "` has wrong type `" << m_element_typename(real) << "`; expected `" << m_element_typename(expected) << "`";
  return ostr.str();
}

const string Index::WrongTypeException::m_describe(const char *name, const elementTypes real)
{
  ostringstream ostr;
  ostr << "Element `" << name << "` has wrong type `" << m_element_typename(real) << "`;";
  return ostr.str();
}

Index::WrongTypeException::WrongTypeException(const char *name, const elementTypes real)
  : TypeException(m_describe(name, real))
{
}

Index::WrongTypeException::WrongTypeException(const char *name, const elementTypes real, const elementTypes expected)
  : TypeException(m_describe(name, real, expected))
{
}

void Index::add(const elementTypes et, const std::string &name, const size_t offset)
{
  const size_t pos_new = m_idx.size();
  ssize_t
      next = 0
    , prev = 0
    , parent = 0
  ;

  m_names.insert(names_t::value_type(name, pos_new));

  if (et == etType)
  {
    const size_t module_id = m_module_notch.getValue() + m_idx[m_module_notch.getValue()].prev;
    item *module = &m_idx[module_id];
    if (!module->child)
    {
      module->child = pos_new - module_id;
    }
    module->children ++;
    parent = module_id - pos_new;
    m_names.insert(names_t::value_type(module->name + '.' + name, pos_new));
  }
  else if (et == etCtor || et == etMethod || et == etGetsetter || et == etSpecialCall)
  {
    const size_t type_id = m_type_notch.getValue() + m_idx[m_type_notch.getValue()].prev;
    item *type = &m_idx[type_id];
    if (!type->child)
    {
      type->child = pos_new - type_id;
    }
    type->children ++;
    parent = type_id - pos_new;
    m_names.insert(names_t::value_type(type->name + '.' + name, pos_new));
    m_names.insert(names_t::value_type(m_idx[m_module_notch.getValue() + m_idx[m_module_notch.getValue()].prev].name + '.' + type->name + '.' + name, pos_new));
  }
  else if (et == etFunction)
  {
    const size_t module_id = m_module_notch.getValue() + m_idx[m_module_notch.getValue()].prev;
    item *module = &m_idx[module_id];
    if (!module->parent)
    {
      module->parent = pos_new - module_id;
    }
    module->children ++;
    parent = module_id - pos_new;
    m_names.insert(names_t::value_type(module->name + '.' + name, pos_new));
  }


  Nullable<size_t> *pnotch;
  switch (et)
  {
    case etModule:
      pnotch = &m_module_notch;
      break;
    case etType:
      pnotch = &m_type_notch;
      break;
    case etCtor:
    case etMethod:
    case etGetsetter:
    case etSpecialCall:
      pnotch = &m_method_notch;
      break;
    case etFunction:
      pnotch = &m_function_notch;
      break;
  }

  if (pnotch->assigned())
  {
    item
        *pi_first = &m_idx[pnotch->getValue()]
      , *pi_last = pi_first + pi_first->prev
    ;
    next = pnotch->getValue() - pos_new;
    prev = pnotch->getValue() + pi_first->prev - pos_new;
    pi_first->prev = - next;
    pi_last->next = - prev;
  }
  else
  {
    *pnotch = pos_new;
  }

  item i = {et, name, next, prev, 0, parent, offset, 0};
  m_idx.push_back(i);
}

void Index::show(std::ostream &os) const
{
  size_t pos = 0;

  if (m_idx.size())
  {
    os
      << "    "
      << setw(32) << left << "name" << right
      << setw(num_width) << "next"
      << setw(num_width) << "prev"
      << setw(num_width) << "chld"
      << setw(num_width) << "prnt"
      << setw(num_width) << "chdn"
      << setw(num_width) << "off"
      << endl
    ;
  }

  for (idx_t::const_iterator iter = m_idx.begin(), iter_end = m_idx.end(); iter != iter_end; iter ++)
  {
    const item &i = *iter;
    if
    (
         i.et == etModule && m_module_notch.assigned() && pos == m_module_notch.getValue()
      || i.et == etType && m_type_notch.assigned() && pos == m_type_notch.getValue()
      || (i.et == etCtor || i.et == etMethod || i.et == etGetsetter || i.et == etSpecialCall) && m_method_notch.assigned() && pos == m_method_notch.getValue()
      || i.et == etFunction && m_function_notch.assigned() && pos == m_function_notch.getValue()
    )
    {
      os << '*';
    }
    else
    {
      os << ' ';
    }

    os
      << ' ' << m_element_letter(i.et) << ' '
      << setw(32) << left << i.name << right << setw(num_width);
    if (i.next)
    {
      os << i.next;
    }
    else
    {
      os << ' ';
    }
    os << setw(num_width);
    if (i.prev)
    {
      os << i.prev;
    }
    else
    {
      os << ' ';
    }
    os << setw(num_width);
    if (i.child)
    {
      os << i.child;
    }
    else
    {
      os << ' ';
    }
    os << setw(num_width);
    if (i.parent)
    {
      os << i.parent;
    }
    else
    {
      os << ' ';
    }
    os << setw(num_width);
    if (i.children)
    {
      os << i.children;
    }
    else
    {
      os << ' ';
    }
    os
      << setw(num_width) << i.offset << endl
    ;
    pos ++;
  }
}

void Index::show_names(std::ostream &os) const
{
  for (names_t::const_iterator iter = m_names.begin(), iter_end = m_names.end(); iter != iter_end; iter ++)
  {
    const item &i = m_idx[iter->second];
    os
      << m_element_letter(i.et) << setw(5) << iter->second << ' ' << iter->first << endl
    ;
  }
}

Index::iterator::iterator(const elementTypes _et)
  : et(_et)
{
}

Index::iterator::iterator(const elementTypes _et, const size_t _filter)
  : et(_et)
  , filter(_filter)
{
}

bool Index::iterate(iterator &iter) const
{
  bool retval = false;

  //modules, types_all, types_by_module, methods_all, methods_by_type, functions_all, functions_by_module

  if (iter.pos.assigned())
  {
    retval = m_next(iter.pos, iter.filter.assigned());
  }
  else
  {
    switch (iter.et)
    {
      case etModule:
        if (m_module_notch.assigned())
        {
          iter.pos = m_module_notch;
          retval = true;
        }
        break;
      case etType:
        if (m_type_notch.assigned())
        {
          if (iter.filter.assigned())
          {
            retval = m_find(iter);
          }
          else
          {
            iter.pos = m_type_notch;
            retval = true;
          }
        }
        break;
      case etCtor:
      case etMethod:
      case etGetsetter:
      case etSpecialCall:
        if (m_method_notch.assigned())
        {
          if (iter.filter.assigned())
          {
            retval = m_find(iter);
          }
          else
          {
            iter.pos = m_method_notch;
            retval = true;
          }
        }
        break;
      case etFunction:
        if (m_function_notch.assigned())
        {
          if (iter.filter.assigned())
          {
            retval = m_find(iter);
          }
          else
          {
            iter.pos = m_function_notch;
            retval = true;
          }
        }
        break;
    }
  }

  return retval;
}

const Index::item *Index::get(const size_t pos) const
{
  return &m_idx[pos];
}

size_t Index::find(const char *name) const
{
  size_t retval;

  const names_t::const_iterator iter = m_names.find(name);
  if (iter == m_names.end())
  {
    throw NotFoundException(name);
  }
  else
  {
    const size_t cnt = m_names.count(name);
    if (cnt != 1)
    {
      throw NonUniqueException(name, cnt);
    }
    else
    {
      retval = iter->second;
    }
  }
  return retval;
}

size_t Index::find(const char *name, const size_t parent) const
{
  size_t retval, cnt = 0;

  for (names_t::const_iterator iter = m_names.lower_bound(name), iter_end = m_names.upper_bound(name); iter != iter_end; iter ++)
  {
    retval = iter->second;
    const item *i = &m_idx[retval];
    if (i->et != etModule && i->parent + retval == parent)
    {
      cnt ++;
    }
  }

  if (!cnt)
  {
    throw NotFoundException(name);
  }
  else if (cnt != 1)
  {
    throw NonUniqueException(name, cnt);
  }
  return retval;
}

size_t Index::find(const char *name, const elementTypes et) const
{
  const item *i;
  size_t retval, cnt = 0, cnt_total = 0;

  for (names_t::const_iterator iter = m_names.lower_bound(name), iter_end = m_names.upper_bound(name); iter != iter_end; iter ++)
  {
    retval = iter->second;
    if ((i = &m_idx[retval])->et == et)
    {
      cnt ++;
    }
    cnt_total ++;
  }
/*
cnt_total cnt
        0   0 NotFoundException
        1   0 WrongTypeException
        1   1 +
        2   0 (WrongType|NonUnique)Exception
        2   1 +
*/
  if (!cnt_total)
  {
    throw NotFoundException(name);
  }
  else if (cnt_total == 1 && !cnt)
  {
    throw WrongTypeException(name, i->et, et);
  }
  else if (cnt != 1)
  {
    throw NonUniqueException(name, cnt_total);
  }
  return retval;
}

size_t Index::find(const char *name, const elementTypes et, const size_t parent) const
{
  const item *i;
  size_t retval, cnt = 0, cnt_total = 0;

  for (names_t::const_iterator iter = m_names.lower_bound(name), iter_end = m_names.upper_bound(name); iter != iter_end; iter ++)
  {
    retval = iter->second;
    if ((i = &m_idx[retval])->et != etModule && i->parent + retval == parent)
    {
      if (i->et == et)
      {
        cnt ++;
      }
      cnt_total ++;
    }
  }

  if (!cnt_total)
  {
    throw NotFoundException(name);
  }
  else if (cnt_total == 1 && !cnt)
  {
    throw WrongTypeException(name, i->et, et);
  }
  else if (cnt != 1)
  {
    throw NonUniqueException(name, cnt_total);
  }
  return retval;
}

size_t Index::find(const char *name, const elementTypes *pet, const size_t et_len) const
{
  const item *i;
  size_t retval, cnt = 0, cnt_total = 0;

  for (names_t::const_iterator iter = m_names.lower_bound(name), iter_end = m_names.upper_bound(name); iter != iter_end; iter ++)
  {
    retval = iter->second;
    i = &m_idx[retval];
    for (size_t pos = 0; pos != et_len; pos ++)
    {
      if (i->et == pet[pos])
      {
        cnt ++;
        break;
      }
    }
    cnt_total ++;
  }

  if (!cnt_total)
  {
    throw NotFoundException(name);
  }
  else if (cnt_total == 1 && !cnt)
  {
    throw WrongTypeException(name, i->et);
  }
  else if (cnt != 1)
  {
    throw NonUniqueException(name, cnt_total);
  }
  return retval;
}

size_t Index::find(const char *name, const elementTypes *pet, const size_t et_len, const size_t parent) const
{
  const item *i;
  size_t retval, cnt = 0, cnt_total = 0;

  for (names_t::const_iterator iter = m_names.lower_bound(name), iter_end = m_names.upper_bound(name); iter != iter_end; iter ++)
  {
    retval = iter->second;
    if ((i = &m_idx[retval])->et != etModule && i->parent + retval == parent)
    {
      for (size_t pos = 0; pos != et_len; pos ++)
      {
        if (i->et == pet[pos])
        {
          cnt ++;
          break;
        }
      }
      cnt_total ++;
    }
  }

  if (!cnt_total)
  {
    throw NotFoundException(name);
  }
  else if (cnt_total == 1 && !cnt)
  {
    throw WrongTypeException(name, i->et);
  }
  else if (cnt != 1)
  {
    throw NonUniqueException(name, cnt_total);
  }
  return retval;
}

bool Index::m_find(iterator &iter) const
{
  bool retval = false;
  const item * pi = &m_idx[iter.filter.getValue()];

  if (iter.et == etFunction)
  {
    if (pi->parent)
    {
      pi += pi->parent;
      retval = true;
    }
  }
  else
  {
    if (pi->child)
    {
      pi += pi->child;
      retval = true;
    }
  }

  if (retval)
  {
    iter.pos = pi - &m_idx[0];
  }

  return retval;
}

bool Index::m_next(Nullable<size_t> &pos, const bool filtered) const
{
  bool retval = false;
  const item *pi = &m_idx[pos.getValue()];

  if (pi->et == etModule)
  {
    if (pi->next > 0)
    {
      pi += pi->next;
      pos = pi - &m_idx[0];
      retval = true;
    }
  }
  else
  {
    const item * const pi_parent = pi + pi->parent;
    if (pi->next > 0)
    {
      pi += pi->next;
      if (!filtered || (pi + pi->parent == pi_parent))
      {
        pos = pi - &m_idx[0];
        retval = true;
      }
    }
  }

  return retval;
}

char Index::m_element_letter(const elementTypes et)
{
  char retval;
  switch (et)
  {
    case etModule:
      retval = 'M';
      break;
    case etType:
      retval = 'T';
      break;
    case etCtor:
      retval = '+';
      break;
    case etMethod:
      retval = 'm';
      break;
    case etGetsetter:
      retval = 'p';
      break;
    case etSpecialCall:
      retval = 'i';
      break;
    case etFunction:
      retval = 'f';
      break;
  }
  return retval;
}

const string Index::m_element_typename(const elementTypes et)
{
  string retval;
  switch (et)
  {
    case etModule:
      retval = "module";
      break;
    case etType:
      retval = "type";
      break;
    case etCtor:
      retval = "ctor";
      break;
    case etMethod:
    case etGetsetter:
    case etSpecialCall:
      retval = "method";
      break;
    case etFunction:
      retval = "function";
      break;
  }
  return retval;
}

}; //namespace pyhrol

using namespace pyhrol;


SMART_FLAG_INSTANTIATION_BEGIN(Index::elementTypes, t)
{
  t.add(npOr, Index::etModule, "module");
  t.add(npOr, Index::etType, "type", Index::etModule);
  t.add(npOr, Index::etCtor, "ctor", Index::etType);
  t.add(npOr, Index::etMethod, "method", Index::etCtor);
  t.add(npOr, Index::etGetsetter, "getsetter", Index::etMethod);
  t.add(npOr, Index::etSpecialCall, "specialCall", Index::etGetsetter);
  t.add(npOr, Index::etFunction, "function", Index::etSpecialCall);
}
SMART_FLAG_INSTANTIATION_END(t);
