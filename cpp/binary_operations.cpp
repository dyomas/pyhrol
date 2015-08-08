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

// $Date: 2012-10-21 01:22:26 +0400 (Вс., 21 окт. 2012) $
// $Revision: 662 $

#include <stdint.h>
#include "binary_operations.h"

template <typename T> const T expand_mask(T *res, const size_t size, T mask, const bool zero_enabled)
{
  size_t size_cur = 0;
  T
      ret_val = 1
    , mask_cur = 1
  ;

  if (zero_enabled && size_cur < size)
  {
    res[size_cur ++] = 0;
  }

  while (mask)
  {
    if (mask & mask_cur)
    {
      const size_t
          size_prev = size_cur + (zero_enabled ? 0 : 1)
        , size_max = size_cur * 2 + (zero_enabled ? 0 : 1)
      ;
      if (size_cur < size)
      {
        res[size_cur ++] = mask_cur;
      }
      while (size_cur < size && size_cur < size_max)
      {
        res[size_cur] = mask_cur | res[size_cur - size_prev];
        size_cur ++;
      }

      ret_val *= 2;
    }

    mask = mask &~ mask_cur;
    mask_cur *= 2;
  }

  return zero_enabled ? ret_val : ret_val - 1;
}

template <typename T> bool is_one_bit_number(T num)
{
  while (num)
  {
    if (num & 1)
    {
      num >>= 1;
      break;
    }
    num >>= 1;
  }

  return !num;
}

template const uint8_t expand_mask<uint8_t>(uint8_t *, const size_t, uint8_t, const bool);
template const uint16_t expand_mask<uint16_t>(uint16_t *, const size_t, uint16_t, const bool);
template const uint32_t expand_mask<uint32_t>(uint32_t *, const size_t, uint32_t, const bool);
template const uint64_t expand_mask<uint64_t>(uint64_t *, const size_t, uint64_t, const bool);

template bool is_one_bit_number<uint8_t>(uint8_t);
template bool is_one_bit_number<uint16_t>(uint16_t);
template bool is_one_bit_number<uint32_t>(uint32_t);
template bool is_one_bit_number<uint64_t>(uint64_t);
