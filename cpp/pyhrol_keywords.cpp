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

// $Date: 2013-09-01 19:34:50 +0400 (Вс., 01 сент. 2013) $
// $Revision: 767 $

#include <string.h>
#include <sstream>
#ifdef _DEBUG_PYTHON_KEYWORDS
#include <iostream>
#include <iomanip>
#endif //_DEBUG_PYTHON_KEYWORDS

#include "pyhrol_keywords.h"
#include "array_square.h"

using namespace std;

namespace pyhrol
{

void Keywords::m_add_keyword(const char *begin, const char *end)
{
  //NOTE может содержать пробелы
  ssize_t id = -1;

  const char
      *cur = begin
    , *begin_word = NULL
    , *end_word = NULL
  ;
  keyword k;
  part p;
  p.nums_start = false;
  p.nums_end = false;

  while (true)
  {
    const char ch = (cur == end ? '\0' : *cur);
    if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
    {
      if (!begin_word || *begin_word == '_')
      {
        begin_word = cur;
      }
      end_word = NULL;
      p.nums_end = false;
    }
    else if (ch >= '0' && ch <= '9')
    {
      if (!begin_word || *begin_word == '_')
      {
        begin_word = cur;
        p.nums_start = true;
      }
      end_word = NULL;
      p.nums_end = true;
    }
    else if (ch == '_')
    {
      if (!begin_word)
      {
        begin_word = cur;
      }
      if (!end_word)
      {
        end_word = cur;
      }
    }
    else if (begin_word)
    {
      p.value.assign(begin_word, end_word ? end_word : cur);
      k.parts.push_back(p);
      begin_word = NULL;
      end_word = NULL;
      p.nums_start = false;
      p.nums_end = false;
    }

    if (cur == end)
    {
      break;
    }
    cur ++;
  }

  m_keywords.push_back(k);
}

void Keywords::m_assign_variants(const parts_idx_t &idx)
{
  const parts_idx_t::const_iterator iter_end = idx.end();
  parts_idx_t::const_iterator iter = idx.begin();
  string part_prev;
  idx_item idx_prev;
  size_t
      num = 0
    , num_whole = 0
    , offset_prev
  ;
  bool offset_unique = false;

  while (true)
  {
    const bool
        last_iteration = (iter == iter_end)
      , new_value = last_iteration ? false : (!num || iter->first != part_prev)
      , is_subtotal = last_iteration ? true : (new_value && num)
    ;

    if (is_subtotal && ((num == 1 || offset_unique) && num_whole == 0 || num_whole == 1))
    {
      m_keywords[idx_prev.offset].variants.insert(variants_t::value_type(idx_prev.end - idx_prev.begin, idx_prev.begin));
    }

    if (new_value)
    {
    #ifdef _DEBUG_PYTHON_KEYWORDS
      cout
        << iter->first << ":" << endl
      ;
    #endif //_DEBUG_PYTHON_KEYWORDS
      num = 0;
      num_whole = 0;
      idx_prev.is_whole = false;
      offset_unique = true;
    }

    if (last_iteration)
    {
      break;
    }

  #ifdef _DEBUG_PYTHON_KEYWORDS
    cout
      << "  " << num << ' ' << (iter->second.is_whole ? '$' : ' ') << "["<< iter->second.begin << ", " << iter->second.end << ")@" << iter->second.offset << endl
    ;
  #endif //_DEBUG_PYTHON_KEYWORDS

    if (iter->second.is_whole)
    {
      num_whole ++;
    }
    else if (!num)
    {
      offset_prev = iter->second.offset;
    }
    else if (iter->second.offset != offset_prev)
    {
      offset_unique = false;
    }
    if (!idx_prev.is_whole)
    {
      idx_prev = iter->second;
    }
    part_prev = iter->first;
    num ++;
    iter ++;
  }

#ifdef _DEBUG_PYTHON_KEYWORDS
  cout
    << endl
  ;
#endif //_DEBUG_PYTHON_KEYWORDS
}

void Keywords::m_build_idx(parts_idx_t &idx) const
{
  size_t keyword_num = 0;

  for (keywords_t::const_iterator iter = m_keywords.begin(); iter != m_keywords.end(); iter ++)
  {
    const keyword &k = *iter;
    const size_t cnt = k.parts.size();
    size_t group_size = 1;
  #ifdef _DEBUG_PYTHON_KEYWORDS
    cout
      << keyword_num << ": cnt = " << cnt << endl
    ;
  #endif //_DEBUG_PYTHON_KEYWORDS
    while (group_size <= cnt)
    {
    #ifdef _DEBUG_PYTHON_KEYWORDS
      cout
        << "  group_size = " << group_size << endl
        << "    {";
    #endif //_DEBUG_PYTHON_KEYWORDS
      for (size_t group_num = 0; group_num < cnt - group_size + 1; group_num ++)
      {
      #ifdef _DEBUG_PYTHON_KEYWORDS
        if (group_num)
        {
          cout << "} {";
        }
      #endif //_DEBUG_PYTHON_KEYWORDS
        const string g = m_build_string(k.parts, group_num, group_num + group_size);
      #ifdef _DEBUG_PYTHON_KEYWORDS
        cout << g << " [" << group_num << ", " << (group_num + group_size) << ")";
      #endif //_DEBUG_PYTHON_KEYWORDS
        const idx_item i = {keyword_num, group_num, group_num + group_size, group_size == cnt};
        idx.insert(parts_idx_t::value_type(g, i));
      }
    #ifdef _DEBUG_PYTHON_KEYWORDS
      cout
        << "}" << endl
      ;
    #endif //_DEBUG_PYTHON_KEYWORDS
      group_size ++;
    }
    keyword_num ++;
  }

#ifdef _DEBUG_PYTHON_KEYWORDS
  cout
    << endl
  ;
#endif //_DEBUG_PYTHON_KEYWORDS
}

const string Keywords::m_build_string(const parts_t &parts, const size_t begin, const size_t end) const
{
  string retval;
  const part *part_prev = NULL;

  for (size_t pos = begin; pos < end; pos ++)
  {
    if (part_prev && (part_prev->nums_end || !parts[pos].nums_start))
    {
      retval += '_';
    }
    retval += parts[pos].value;
    part_prev = &parts[pos];
  }
  return retval;
}

#ifdef _DEBUG_PYTHON_KEYWORDS
void Keywords::__report(std::ostream &os) const
{
  if (m_keywords.size())
  {
    os
      << "m_keywords (" << m_keywords.size() << " items):" << endl
    ;
    for (keywords_t::const_iterator iter = m_keywords.begin(); iter != m_keywords.end(); iter ++)
    {
      os
        << setw(3) << (iter - m_keywords.begin()) << endl
      ;
      const keyword &k = *iter;
      os << setw(5) << ' ';
      for (parts_t::const_iterator iter_parts = k.parts.begin(); iter_parts != k.parts.end(); iter_parts ++)
      {
        if (iter_parts - k.parts.begin())
        {
          os << ' ';
        }
        if (iter_parts->nums_start)
        {
          os << '#';
        }
        os << iter_parts->value;
        if (iter_parts->nums_end)
        {
          os << '#';
        }
      }
      os
        << endl
      ;
      if (k.variants.size())
      {
        os << setw(5) << ' ';
        for (variants_t::const_iterator iter_v = k.variants.begin(); iter_v != k.variants.end(); iter_v ++)
        {
          if (iter_v != k.variants.begin())
          {
            os << ' ';
          }
          os << iter_v->first << '@' << iter_v->second;
        }
        os
          << endl
        ;
        os << setw(5) << ' ';
        for (variants_t::const_iterator iter_v = k.variants.begin(); iter_v != k.variants.end(); iter_v ++)
        {
          if (iter_v != k.variants.begin())
          {
            os << ' ';
          }
          for (size_t pos = iter_v->second; pos != iter_v->second + iter_v->first; pos ++)
          {
            if (pos != iter_v->second)
            {
              os << '_';
            }
            os << k.parts[pos].value;
          }
        }
        os
          << endl
        ;
      }
    }
  }
}
#endif //_DEBUG_PYTHON_KEYWORDS

Keywords::ChoiceError::ChoiceError(const failure_idx_t &non_unique, const failure_idx_t &illegal)
  : logic_error(m_describe(non_unique, illegal))
{
}

const string Keywords::ChoiceError::m_describe(const failure_idx_t &non_unique, const failure_idx_t &illegal)
{
  ostringstream ostr;

  if (non_unique.size())
  {
    ostr << "Following arguments: ";
    for (failure_idx_t::const_iterator iter = non_unique.begin(), iter_end = non_unique.end(); iter != iter_end; iter ++)
    {
      if (iter != non_unique.begin())
      {
        ostr << ", ";
      }
      ostr << *iter;
    }
    ostr << " has no unique name parts";
  }
  if (illegal.size())
  {
    ostr << (non_unique.size() ? "; following" : "Following") << " arguments: ";
    for (failure_idx_t::const_iterator iter = illegal.begin(), iter_end = illegal.end(); iter != iter_end; iter ++)
    {
      if (iter != illegal.begin())
      {
        ostr << ", ";
      }
      ostr << *iter;
    }
    ostr << " has no valid name parts";
  }
  return ostr.str();
}

void Keywords::build(ArraySquare &res)
{
  parts_idx_t idx;

  m_build_idx(idx);
  m_assign_variants(idx);

  ChoiceError::failure_idx_t non_unique, illegal;
  size_t cnt = 0;

  for (keywords_t::const_iterator iter = m_keywords.begin(); iter != m_keywords.end(); iter ++)
  {
    const keyword &k = *iter;
    bool
        has_variant = false
      , has_non_numeric = false
    ;
    for(variants_t::const_iterator iter_v = k.variants.begin(), iter_v_end = k.variants.end(); iter_v != iter_v_end; iter_v ++)
    {
      has_variant = true;
      if (!k.parts[iter_v->second].nums_start)
      {
        has_non_numeric = true;
        res.append(m_build_string(k.parts, iter_v->second, iter_v->second + iter_v->first));
        break;
      }
    }

    if (!has_variant)
    {
      non_unique.push_back(cnt);
      //TODO Keywords non unique. Throw exception with detailed info
    }
    else if (!has_non_numeric)
    {
      //TODO Takes the first variant, but if it contains numbers only?
      illegal.push_back(cnt);
    }
    cnt ++;
  }

  if (non_unique.size() || illegal.size())
  {
    throw ChoiceError(non_unique, illegal);
  }
}

void Keywords::add(const string &name)
{
  m_add_keyword(name.c_str(), name.c_str() + name.length());
}

void Keywords::add(const char *begin, const char *end)
{
  m_add_keyword(begin, end);
}

Keywords::Keywords()
{
}

} //namespace pyhrol
