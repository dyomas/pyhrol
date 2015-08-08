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

// $Date: 2013-05-21 13:23:01 +0400 (Вт., 21 мая 2013) $
// $Revision: 718 $

#include "printf_parser.h"
#include <strings.h>
#include <stdlib.h>

template <typename F, typename M, typename S> void PrintfParser<F, M, S>::m_add_index_item(const char idx, fieldPartTypes type)
{
  typename index_t::iterator iter = m_index.find(idx);

  if (iter != m_index.end())
  {
    if (iter->second == fptModifier && type == fptModifierPre || iter->second == fptModifierPre && type == fptModifier)
    {//h, h... || hh
    #ifdef _DEBUG
      std::cerr
        << "Changed " << field_part_types_flag_t(iter->second) << " -> " << field_part_types_flag_t(static_cast<fieldPartTypes>(fptModifier | fptModifierPre)) << std::endl
      ;
    #endif //_DEBUG
      iter->second = fieldPartTypes(fptModifier | fptModifierPre);
    }
    else if (
         ((iter->second != fptModifierPre) || (type != fptModifierPre))
      && ((iter->second != fieldPartTypes(fptModifier | fptModifierPre)) || (type != fptModifierPre))
      && ((iter->second != fieldPartTypes(fptModifier | fptModifierPre)) || (type != fptModifier))
    )
    {//iter->second == fptModifier && type == fptModifier (h, h) и все остальные случаи
      std::ostringstream ostr;
      ostr << field_part_types_flag_t(iter->second) << " \'" << idx << "\' exists, while " << field_part_types_flag_t(type) << " with the same name attempted to add";
      throw std::logic_error(ostr.str());
    }
  #ifdef _DEBUG
    else
    {
      std::cerr
        << "Ignored " << field_part_types_flag_t(iter->second) << " -> " << field_part_types_flag_t(type) << std::endl
      ;
    }
  #endif //_DEBUG
  }
  else
  {
    m_index[idx] = type;
  }
}

#ifdef _DEBUG
template <typename F, typename M, typename S> void PrintfParser<F, M, S>::__m_test(const fieldPartTypes exists, const fieldPartTypes adding)
{
  m_index.clear();
  m_index['h'] = exists;
  try
  {
    m_add_index_item('h', adding);
  }
  catch (const std::logic_error &err)
  {
    std::cerr
      << "*** logic_error: " << err.what() << std::endl
    ;
  }
}

template <typename F, typename M, typename S> void PrintfParser<F, M, S>::__test()
{
  __m_test(fptFlag, fptFlag);
  __m_test(fptModifierPre, fptFlag);
  __m_test(fptModifier, fptFlag);
  __m_test(fieldPartTypes(fptModifier | fptModifierPre), fptFlag);
  __m_test(fptSpecifier, fptFlag);

  __m_test(fptFlag, fptModifierPre);
  __m_test(fptModifierPre, fptModifierPre);
  __m_test(fptModifier, fptModifierPre);
  __m_test(fieldPartTypes(fptModifier | fptModifierPre), fptModifierPre);
  __m_test(fptSpecifier, fptModifierPre);

  __m_test(fptFlag, fptModifier);
  __m_test(fptModifierPre, fptModifier);
  __m_test(fptModifier, fptModifier);
  __m_test(fieldPartTypes(fptModifier | fptModifierPre), fptModifier);
  __m_test(fptSpecifier, fptModifier);

  __m_test(fptFlag, fptSpecifier);
  __m_test(fptModifierPre, fptSpecifier);
  __m_test(fptModifier, fptSpecifier);
  __m_test(fieldPartTypes(fptModifier | fptModifierPre), fptSpecifier);
  __m_test(fptSpecifier, fptSpecifier);
}

template <typename F, typename M, typename S> void PrintfParser<F, M, S>::__report(std::ostream &os) const
{
  if (m_flags.size())
  {
    os
      << "flags:" << std::endl
    ;
    for (typename flags_t::const_iterator iter = m_flags.begin(); iter != m_flags.end(); iter ++)
    {
      os
        << "  " << iter->first << " = " << iter->second << " (" << uint32_t(iter->second) << ")" << std::endl
      ;
    }
  }
  if (m_modifiers.size())
  {
    os
      << "modifiers:" << std::endl
    ;
    for (typename modifiers_t::const_iterator iter = m_modifiers.begin(); iter != m_modifiers.end(); iter ++)
    {
      os
        << "  " << iter->first << " = " << iter->second << " (" << uint32_t(iter->second) << ")" << std::endl
      ;
    }
  }
  if (m_specifiers.size())
  {
    os
      << "specifiers:" << std::endl
    ;
    for (typename specifiers_t::const_iterator iter = m_specifiers.begin(); iter != m_specifiers.end(); iter ++)
    {
      os
        << "  " << iter->first << " = " << iter->second << " (" << uint32_t(iter->second) << ")" << std::endl
      ;
    }
  }
  if (m_index.size())
  {
    os
      << "index:" << std::endl
    ;
    for (typename index_t::const_iterator iter = m_index.begin(); iter != m_index.end(); iter ++)
    {
      os
        << "  " << iter->first << " = " << field_part_types_flag_t(iter->second) << " (" << uint32_t(iter->second) << ")" << std::endl
      ;
    }
  }
}
#endif //_DEBUG

template <typename F, typename M, typename S> const size_t PrintfParser<F, M, S>::parse(field *pfields, const size_t fields_cnt, const char *format) const
{
  enum _states
  {
      _st_initial
    , _st_flag_expected
    , _st_width_expected
    , _st_precision_expected
    , _st_precision_required
    , _st_modifier_expected
    , _st_modifier_started
    , _st_specifier_expected
    , _st_finished
  } state = _st_initial;
  size_t field_pos = 0U;
  const char
      *pch = format
    , *pfield_begin = NULL
    , *pmodifier_begin = NULL
  ;
  field *pfield = NULL;
  index_t::const_iterator iter;

  while (*pch)
  {
    if (state == _st_finished)
    {
      if (pfield)
      {
        pfield->after = pch - format;
      }
      field_pos ++;
      state = _st_initial;
    }
    if (state == _st_initial && *pch == '%')
    {
      if (field_pos < fields_cnt)
      {
        pfield = &pfields[field_pos];
        bzero(static_cast<void *>(pfield), sizeof(field));
        pfield->before = pch - format;
        pfield->width = -1;
        pfield->precision = -1;
      }
      else
      {
        pfield = NULL;
      }
      pfield_begin = pch;
      state = _st_flag_expected;
    }
    else if (state != _st_initial)
    {
      if (state == _st_flag_expected)
      {
        if (*pch == '.')
        {//%.1d
          state = _st_precision_required;
          pch ++;
          continue;
        }
        else
        {
          index_t::const_iterator iter = m_index.find(*pch);
          if (iter == m_index.end())
          {//%4d
            state = _st_width_expected;
          }
          else
          {
            const fieldPartTypes type = iter->second;
            if (type == fptFlag)
            {//%+d
              if (pfield)
              {
                pfield->flags = static_cast<F>(pfield->flags | m_flags.at(*pch));
              }
            }
            else if (type == fptModifierPre || type == fieldPartTypes(fptModifier | fptModifierPre))
            {//%hd || %hhd
              pmodifier_begin = pch;
              state = _st_modifier_started;
            }
            else if (type == fptModifier)
            {//%hd
              if (pfield)
              {
                pfield->modifier = m_modifiers.at(std::string(pch, 1));
              }
              state = _st_specifier_expected;
            }
            else if (type == fptSpecifier)
            {//%d
              if (*pch == '%' && pch - pfield_begin > 1)
              {
                state = _st_initial;
                continue;
              }
              else
              {
                if (pfield)
                {
                  pfield->specifier = m_specifiers.at(*pch);
                }
                state = _st_finished;
              }
            }
            pch ++;
            continue;
          }
        }
      }
      //_st_flag_expected -> _st_width_expected
      //  continue -> _st_precision_required, _st_width_expected, _st_modifier_started, _st_specifier_expected, _st_finished
      if (state == _st_precision_expected)
      {
        if (*pch == '.')
        {
          state = _st_precision_required;
          pch ++;
          continue;
        }
        else
        {
          state = _st_modifier_expected;
        }
      }
      //_st_precision_expected -> _st_modifier_expected
      //  continue -> _st_precision_required
      if (state == _st_width_expected || state == _st_precision_required)
      {
        char *endptr;
        const int32_t res = strtol(pch, &endptr, 10);
        if (endptr != pch)
        {
          if (state == _st_width_expected)
          {
            if (pfield)
            {
              pfield->width = (res < 0 ? 0 : (res > 127 ? 127 : res));
            }
            state = _st_precision_expected;
          }
          else if (state == _st_precision_required)
          {
            if (pfield)
            {
              pfield->precision = (res < 0 ? 0 : (res > 127 ? 127 : res));
            }
            state = _st_modifier_expected;
          }
          pch = endptr;
          continue;
        }
        else
        {
          state = _st_modifier_expected;
        }
      }
      //_st_width_expected | _st_precision_required -> _st_modifier_expected
      //  continue -> _st_precision_expected, _st_modifier_expected
      if (state == _st_modifier_expected || state == _st_modifier_started || state == _st_specifier_expected)
      {
        index_t::const_iterator iter = m_index.find(*pch);
        if (iter == m_index.end())
        {
          state = _st_initial;
        }
        else
        {
          const fieldPartTypes type = iter->second;
          if (state == _st_specifier_expected)
          {
            if (type == fptSpecifier)
            {
              if (*pch == '%' && pch - pfield_begin > 1)
              {
                state = _st_initial;
                continue;
              }
              else
              {
                if (pfield)
                {
                  pfield->specifier = m_specifiers.at(*pch);
                }
                state = _st_finished;
              }
            }
            else
            {
              state = _st_initial;
            }
          }
          else if (state == _st_modifier_expected)
          {
            if (type == fptModifier)
            {
              if (pfield)
              {
                pfield->modifier = m_modifiers.at(std::string(pch, 1));
              }
              state = _st_specifier_expected;
            }
            else if (type == fptSpecifier)
            {
              if (*pch == '%' && pch - pfield_begin > 1)
              {
                state = _st_initial;
                continue;
              }
              else
              {
                if (pfield)
                {
                  pfield->specifier = m_specifiers.at(*pch);
                }
                state = _st_finished;
              }
            }
            else if (type == fptModifierPre || type == fieldPartTypes(fptModifierPre | fptModifier))
            {
              pmodifier_begin = pch;
              state = _st_modifier_started;
            }
            else
            {
              state = _st_initial;
            }
          }
          else
          {//_st_modifier_started
            if (type == fptModifier)
            {
              typename modifiers_t::const_iterator iter_mod = m_modifiers.find(std::string(pmodifier_begin, pch + 1));
              if (iter_mod == m_modifiers.end())
              {
                state = _st_initial;
              }
              else
              {
                if (pfield)
                {
                  pfield->modifier = iter_mod->second;
                }
                state = _st_specifier_expected;
              }
            }
            else if (type == fptSpecifier)
            {
              typename modifiers_t::const_iterator iter_mod = m_modifiers.find(std::string(pmodifier_begin, pch));
              if (iter_mod == m_modifiers.end())
              {
                state = _st_initial;
              }
              else
              {
                if (*pch == '%' && pch - pfield_begin > 1)
                {
                  state = _st_initial;
                  continue;
                }
                else
                {
                  if (pfield)
                  {
                    pfield->modifier = iter_mod->second;
                    pfield->specifier = m_specifiers.at(*pch);
                  }
                  state = _st_finished;
                }
              }
            }
            else if (type != fptModifierPre && type != fieldPartTypes(fptModifierPre | fptModifier))
            {
              state = _st_initial;
            }
          }
        }
      }
      //_st_specifier_expected -> _st_finished
      //_st_modifier_expected -> _st_specifier_expected, _st_modifier_started, _st_finished
      //_st_modifier_started -> _st_specifier_expected, _st_finished
      //  error _st_initial
    }
    pch ++;
  }

  if (state == _st_finished)
  {
    if (pfield)
    {
      pfield->after = pch - format;
    }
    field_pos ++;
  }
  return field_pos;
}

template <typename F, typename M, typename S> void PrintfParser<F, M, S>::addFlag(const char flag, const F value)
{
  if (m_flags.count(flag))
  {
    std::ostringstream ostr;
    ostr << "Flag \'" << flag << "\' already exists";
    throw std::logic_error(ostr.str());
  }
  m_add_index_item(flag, fptFlag);
  m_flags.insert(typename flags_t::value_type(flag, value));
}

template <typename F, typename M, typename S> void PrintfParser<F, M, S>::addModifier(const char *modifier, const M value)
{
  const std::string __m(modifier);
  do
  {
    const char ch = * (modifier ++);
    const fieldPartTypes type = *modifier ? fptModifierPre : fptModifier;
    m_add_index_item(ch, type);
  }
  while (*modifier);

  if (m_modifiers.count(__m))
  {
    std::ostringstream ostr;
    ostr << "Modifier \'" << __m << "\' already exists";
    throw std::logic_error(ostr.str());
  }
  m_modifiers.insert(typename modifiers_t::value_type(__m, value));
}

template <typename F, typename M, typename S> void PrintfParser<F, M, S>::addSpecifier(const char specifier, const S value)
{
  if (m_specifiers.count(specifier))
  {
    std::ostringstream ostr;
    ostr << "Specifier \'" << specifier << "\' already exists";
    throw std::logic_error(ostr.str());
  }
  m_add_index_item(specifier, fptSpecifier);
  m_specifiers.insert(typename specifiers_t::value_type(specifier, value));
}
