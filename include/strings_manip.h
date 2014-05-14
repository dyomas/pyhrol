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

// $Date: 2013-08-29 23:50:28 +0400 (Thu, 29 Aug 2013) $
// $Revision: 759 $

#ifndef __strings_manip_h__
#define __strings_manip_h__

//Наиболее популярные манипуляции со строками, не вошедшие в STL

#include <stdint.h>
#include <string>

struct space_splitter
{
  static inline bool field_predicate (char arg)
  {
    return !isspace(arg);
  }

  static inline bool delimiter_predicate (char arg)
  {
    return isspace(arg) != 0;
  }
};

template<char D> struct char_splitter
{
  static inline bool field_predicate (char arg)
  {
    return arg != D;
  }

  static inline bool delimiter_predicate (char arg)
  {
    return arg == D;
  }
};

/**
Деление строки на фрагменты. Контейнер T должен иметь методы insert и end -- вставка фрагмента строки производится в конец контейнера. S должен иметь пару предикатов field_predicate и delimiter_predicate, обнаруживающих начало нового фрагмента строки и начало разделителя соответственно
*/
template <typename T, typename S> void split_string(T &, const std::string &);
template <typename T, typename S> void split_string_unique(T &, const std::string &);

/**
Деление строки на фрагменты по одному фрагменту за итерацию. Память для хранения результата, как в split_string не требуется
@param end конец текущего фрагмента; NOTE начальное значение обязательно NULL
*/
bool parse_string(const char *&/*begin*/, const char *&/*end*/, const char */*src*/, const char /*delimiter*/);
bool parse_string(const char *&/*begin*/, const char *&/*end*/, const char */*src*/, const char */*src_end*/, const char /*delimiter*/);
/**
Деление строки на фрагменты с объединением последовательных разделителей.
Разделителями являются непечатаемые символы (пробел, табуляция и т. д.).
Экранирование разделителей не учитывается
*/
bool parse_string(const char *&/*begin*/, const char *&/*end*/, const char */*src*/);
bool parse_string(const char *&/*begin*/, const char *&/*end*/, const char */*src*/, const char */*src_end*/);

void hex_to_array(uint8_t *, size_t &, const char *);
void array_to_hex(char *, const uint8_t *, const size_t, const bool /*capitals*/ = false);

const std::string cut_spaces(const char *);

void replace_chars(std::string &, const char /*from*/, const char /*to*/);
bool is_number(const char *);
bool is_number(const std::string &);

//TODO Необходимо контроллировать переполнение при преобразовании к целому, а также гарантировать непреодолимость правой границы буфера (функции strtol[l] могут выйти за эту границу при отстутствии нулевого байта)
int8_t c_string_to_int8(const char *);
uint8_t c_string_to_uint8(const char *);

int16_t c_string_to_int16(const char *);
uint16_t c_string_to_uint16(const char *);

int32_t c_string_to_int(const char *);
int32_t string_to_int(const char */*begin*/, const char */*end*/);
int32_t string_to_int(const std::string &);
uint32_t c_string_to_uint(const char *);

int64_t c_string_to_int64(const char *);
int64_t string_to_int64(const char */*begin*/, const char */*end*/);
int64_t string_to_int64(const std::string &);
uint64_t c_string_to_uint64(const char *);

float c_string_to_float(const char *);
double c_string_to_double(const char *);

//Одноименные перегруженные функции для шаблонных классов
void string_to_number(int8_t */*pres*/, const std::string &);
void string_to_number(uint8_t */*pres*/, const std::string &);
void string_to_number(int16_t */*pres*/, const std::string &);
void string_to_number(uint16_t */*pres*/, const std::string &);
void string_to_number(int32_t */*pres*/, const std::string &);
void string_to_number(uint32_t */*pres*/, const std::string &);
void string_to_number(int64_t */*pres*/, const std::string &);
void string_to_number(uint64_t */*pres*/, const std::string &);
void string_to_number(float */*pres*/, const std::string &);
void string_to_number(double */*pres*/, const std::string &);
void string_to_number(int32_t */*pres*/, const char */*begin*/, const char */*end*/);
void string_to_number(int64_t */*pres*/, const char */*begin*/, const char */*end*/);
void string_to_number(float */*pres*/, const char */*begin*/, const char */*end*/);
void string_to_number(double */*pres*/, const char */*begin*/, const char */*end*/);

//TODO Приведение регистра требуется с учетом кодировки
void to_lower_case(std::string &);
void to_lower_case(std::string &, const char *);
void to_upper_case(std::string &);
void to_upper_case(std::string &, const char *);

#endif //__strings_manip_h__
