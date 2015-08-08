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

// $Date: 2010-10-01 13:44:37 +0400 (Пт., 01 окт. 2010) $
// $Revision: 473 $

#ifndef __binary_operations_h__
#define __binary_operations_h__


#include <sys/types.h>

/**
@brief "Разворачивание" бинарной маски

@param res результирующий массив
@param size размер результирующего массива
@param mask бинарная маска
@retval полное количество вариантов значений

Перебор всех возможных значений целого числа с заданной бинарной маской. Пример:
  для маски 0x0A (установлены 1-й и 3-й биты) возможны 4 значения:
    0   0 (0x0)
    1   2 (0x2)
    2   8 (0x8)
    3  10 (0xa)
Варианты значений записываются в массив res до тех пор пока позволяет его размер size
*/
template <typename T> const T expand_mask(T */*res*/, const size_t /*size*/, T /*mask*/, const bool /*zero_enabled*/ = true);

/**
Возвращает true, если число равно степени 2
*/
template <typename T> bool is_one_bit_number(T /*num*/);

#endif //__binary_operations_h__

