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

// $Date: 2015-04-18 15:32:17 +0300 (Сб., 18 апр. 2015) $
// $Revision: 1031 $

#ifndef __pyhrol_trace_h__
#define __pyhrol_trace_h__

#include "smart_flag.h"

namespace pyhrol
{
  enum traceParts
  {
      tpNo = 0
    , tpInternal = 2
    , tpMediator = 256
    , tpUser = 0x8000
    , tpAll = 0xFFFF
    , tpHideAddress = 0x10000
    , tpShowNum = 0x20000
    , tpOptionsMask = tpHideAddress | tpShowNum
  };

  traceParts tracer_state();
  void reset_tracer(const traceParts);
  void trace(const traceParts, const void *address, const char *signature);
  bool user_trace_enbled();
  std::ostream &user_trace(const void *address, const char *signature);
} //namespace pyhrol

typedef SmartFlag<pyhrol::traceParts> trace_parts_flag_t;

#ifdef USE_TRACER

#define PYHROL_TRACE(part, address, signature)\
pyhrol::trace(part, address, signature);

#define PYHROL_USER_TRACE(address, message)\
do\
{\
  if (pyhrol::user_trace_enbled())\
  {\
    pyhrol::user_trace(address, __PRETTY_FUNCTION__) << message << std::endl;\
  }\
}\
while (false)

#else
#define PYHROL_TRACE(part, address, signature)
#define PYHROL_USER_TRACE(address, message)
#endif //USE_TRACER

#endif //__pyhrol_trace_h__
