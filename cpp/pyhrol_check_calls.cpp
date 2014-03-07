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

#include <sstream>
#include "pyhrol_check_calls.h"
#include "nullable.hpp"
#include "smart_flag.hpp"

using namespace std;

namespace pyhrol
{

const string CheckCalls::ForbiddenCall::m_describe(const calls c, const callModifiers cv)
{
  ostringstream ostr;
  ostr << "Call of method \"" << calls_flag_t(c) << "\" forbidden because of \"" << call_modifiers_flag_t(cv) << "\" modifier";
  return ostr.str();
}

const string CheckCalls::ForbiddenCall::m_describe(const calls c, const TupleFillState::modes mode)
{
  ostringstream ostr;
  ostr << "Call of method \"" << calls_flag_t(c) << "\" forbidden because tuple in \"" << modes_flag_t(mode) << "\" mode";
  return ostr.str();
}

const string CheckCalls::ForbiddenCall::m_describe(const calls c, const calls c_prev, const Nullable<bool> &answer_prev)
{
  ostringstream ostr;
  ostr << "Call of method \"" << calls_flag_t(c) << "\" after \"" << calls_flag_t(c_prev) << "\" forbidden";
  if (!answer_prev.isNull())
  {
    ostr << ", previous answer was \"" << (answer_prev.getValue() ? "true" : "false") << "\"";
  }
  return ostr.str();
}

CheckCalls::ForbiddenCall::ForbiddenCall(const calls c, const callModifiers cv)
  : AttributeException(m_describe(c, cv))
{
}

CheckCalls::ForbiddenCall::ForbiddenCall(const calls c, const TupleFillState::modes mode)
  : AttributeException(m_describe(c, mode))
{
}


CheckCalls::ForbiddenCall::ForbiddenCall(const calls c, const calls c_prev, const Nullable<bool> &answer_prev)
  : AttributeException(m_describe(c, c_prev, answer_prev))
{
}

void CheckCalls::set_answer(bool answer) const
{
  m_previous_answer = answer;
}

void CheckCalls::check(const calls c, const TupleFillState::modes mode) const
{
  bool valid = false;

  if (
       c == c_prepare_parse
    || c == c_add_input
    || c == c_is_parse_enabled
    || c == c_parse
  )
  {
    if (cv & f_noarg)
    {
      throw ForbiddenCall(c, f_noarg);
    }
    else if (mode == TupleFillState::modGetter)
    {
      throw ForbiddenCall(c, mode);
    }
  }
  else if (
       c == c_prepare_build
    || c == c_add_output
    || c == c_is_build_enabled
    || c == c_build
  )
  {
    if (cv & f_void)
    {
      throw ForbiddenCall(c, f_void);
    }
    else if (mode == TupleFillState::modSetter)
    {
      throw ForbiddenCall(c, mode);
    }
  }

  switch(m_previous_call)
  {
    case c_no:
      if (c == c_set_options || c == c_prepare_parse || c == c_before_build)
      {
        valid = true;
      }
      break;
    case c_set_options:
      if (c == c_prepare_parse || c == c_prepare_build || c == c_after_build)
      {
        valid = true;
      }
      break;
    case c_prepare_parse:
      if (!m_previous_answer.isNull() && (
           m_previous_answer.getValue() && c == c_add_input
        || !m_previous_answer.getValue() && c == c_is_parse_enabled
      ))
      {
        valid = true;
      }
      break;
    case c_add_input:
      if (c == c_add_input || c == c_is_parse_enabled)
      {
        valid = true;
      }
      break;
    case c_is_parse_enabled:
      if (!m_previous_answer.isNull() && (
           m_previous_answer.getValue() && c == c_parse
        || !m_previous_answer.getValue() && (c == c_set_options || c == c_prepare_parse || c == c_before_build)
      ))
      {
        valid = true;
      }
      break;
    case c_parse:
      if (c == c_set_options || c == c_prepare_parse || c == c_before_build)
      {
        valid = true;
      }
      break;
    case c_before_build:
      if (!m_previous_answer.isNull() && (
           m_previous_answer.getValue() && (c == c_set_options || c == c_prepare_build || c == c_after_build)
        || !m_previous_answer.getValue() && (c == c_parsed_variant || c == c_choose_build_variant)
        ) || m_previous_answer.isNull() && c == c_just_filled
      )
      {
        valid = true;
      }
      break;
    case c_prepare_build:
      if (!m_previous_answer.isNull() && (
           m_previous_answer.getValue() && c == c_add_output
        || !m_previous_answer.getValue() && c == c_is_build_enabled
      ))
      {
        valid = true;
      }
      break;
    case c_add_output:
      if (c == c_add_output || c == c_is_build_enabled)
      {
        valid = true;
      }
      break;
    case c_is_build_enabled:
      if (m_previous_answer.assigned() && (c == c_build && m_previous_answer.getValue()) || (c == c_after_build || c == c_set_options || c == c_prepare_build) && !m_previous_answer.getValue())
      {
        valid = true;
      }
      break;
    case c_build:
      if (c == c_after_build || c == c_set_options || c == c_prepare_build)
      {
        valid = true;
      }
      break;
    case c_after_build:
      if (!m_previous_answer.isNull() && (
           m_previous_answer.getValue() && c == c_just_filled
        || !m_previous_answer.getValue() && (c == c_parsed_variant || c == c_choose_build_variant)
        ) || m_previous_answer.isNull() && c == c_just_filled
      )
      {
        valid = true;
      }
      break;
    case c_parsed_variant:
      if (c == c_choose_build_variant || c == c_parsed_variant)
      {
        valid = true;
      }
      break;
    case c_choose_build_variant:
      if (c == c_before_build)
      {
        valid = true;
      }
    case c_just_filled:
      if (c == c_just_filled)
      {
        valid = true;
      }
      break;
  };

  if (!valid)
  {
    throw ForbiddenCall(c, m_previous_call, m_previous_answer);
  }

  m_previous_answer.clear();
  m_previous_call = c;
}

void CheckCalls::clear()
{
  m_previous_answer.clear();
  m_previous_call = c_no;
}

CheckCalls::CheckCalls(const callVariants _cv)
  : cv(_cv)
  , m_previous_call(c_no)
{
}

CheckCalls::CheckCalls(const CheckCalls &cp)
  : cv(cp.cv)
  , m_previous_answer(cp.m_previous_answer)
  , m_previous_call(cp.m_previous_call)
{
}

}; //namespace pyhrol

SMART_FLAG_INSTANTIATION_BEGIN(pyhrol::CheckCalls::calls, t)
{
  t.add(npOr, pyhrol::CheckCalls::c_no, "<no>");
  t.add(npOr, pyhrol::CheckCalls::c_set_options, "set_options", pyhrol::CheckCalls::c_no);
  t.add(npOr, pyhrol::CheckCalls::c_prepare_parse, "prepare_parse", pyhrol::CheckCalls::c_set_options);
  t.add(npOr, pyhrol::CheckCalls::c_add_input, "add_input", pyhrol::CheckCalls::c_prepare_parse);
  t.add(npOr, pyhrol::CheckCalls::c_is_parse_enabled, "is_parse_enabled", pyhrol::CheckCalls::c_add_input);
  t.add(npOr, pyhrol::CheckCalls::c_parse, "parse", pyhrol::CheckCalls::c_is_parse_enabled);
  t.add(npOr, pyhrol::CheckCalls::c_before_build, "before_build", pyhrol::CheckCalls::c_parse);
  t.add(npOr, pyhrol::CheckCalls::c_prepare_build, "prepare_build", pyhrol::CheckCalls::c_before_build);
  t.add(npOr, pyhrol::CheckCalls::c_add_output, "add_output", pyhrol::CheckCalls::c_prepare_build);
  t.add(npOr, pyhrol::CheckCalls::c_is_build_enabled, "is_build_enabled", pyhrol::CheckCalls::c_add_output);
  t.add(npOr, pyhrol::CheckCalls::c_build, "build", pyhrol::CheckCalls::c_is_build_enabled);
  t.add(npOr, pyhrol::CheckCalls::c_after_build, "after_build", pyhrol::CheckCalls::c_build);
  t.add(npOr, pyhrol::CheckCalls::c_parsed_variant, "parsed_variant", pyhrol::CheckCalls::c_after_build);
  t.add(npOr, pyhrol::CheckCalls::c_choose_build_variant, "choose_build_variant", pyhrol::CheckCalls::c_parsed_variant);
  t.add(npOr, pyhrol::CheckCalls::c_just_filled, "just_filled", pyhrol::CheckCalls::c_choose_build_variant);
}
SMART_FLAG_INSTANTIATION_END(t);
