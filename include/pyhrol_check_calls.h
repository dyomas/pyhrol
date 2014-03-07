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

#ifndef __pyhrol_check_calls_h__
#define __pyhrol_check_calls_h__

#include "smart_flag.h"
#include "nullable.h"
#include "pyhrol_common.h"
#include "pyhrol_tuple_fill_state.h"

namespace pyhrol
{

class CheckCalls
{
  public:
    enum calls
    {
        c_no
      , c_set_options
      , c_prepare_parse
      , c_add_input
      , c_is_parse_enabled
      , c_parse
      , c_before_build
      , c_prepare_build
      , c_add_output
      , c_is_build_enabled
      , c_build
      , c_after_build
      , c_parsed_variant
      , c_choose_build_variant
      , c_just_filled
    };

    const callVariants cv;

    class ForbiddenCall: public AttributeException
    {
      static const std::string m_describe(const calls c, const callModifiers cv);
      static const std::string m_describe(const calls c, const TupleFillState::modes mode);
      static const std::string m_describe(const calls c, const calls c_prev, const Nullable<bool> &answer_prev);
    public:
      ForbiddenCall(const calls c, const callModifiers cv);
      ForbiddenCall(const calls c, const TupleFillState::modes mode);
      ForbiddenCall(const calls c, const calls c_prev, const Nullable<bool> &answer_prev);
    };

    void set_answer(bool) const;
    void check(const calls, const TupleFillState::modes = TupleFillState::modUnspecified) const;
    void clear();

    CheckCalls(const callVariants);
    CheckCalls(const CheckCalls &);

  private:
    mutable Nullable<bool> m_previous_answer;
    mutable calls m_previous_call;
};

}; //namespace pyhrol

typedef SmartFlag<pyhrol::CheckCalls::calls> calls_flag_t;

#endif //__pyhrol_check_calls_h__
