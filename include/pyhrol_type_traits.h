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

// $Date: 2013-09-18 02:14:07 +0400 (Ср., 18 сент. 2013) $
// $Revision: 785 $

#ifndef __pyhrol_type_traits_h__
#define __pyhrol_type_traits_h__


//#include <>
//#include ""

//#define

namespace pyhrol
{

class TypeTraits
{
  static int m_get_type_id(const char *);
  static int m_get_type_id(char **);
  static int m_get_type_id(const char **);
  static int m_get_type_id(const int8_t *);
  static int m_get_type_id(const uint8_t *);
  static int m_get_type_id(const bool *);
  static int m_get_type_id(const int16_t *);
  static int m_get_type_id(const uint16_t *);
  static int m_get_type_id(const int32_t *);
  static int m_get_type_id(const uint32_t *);
  static int m_get_type_id(const long int *);
  static int m_get_type_id(const unsigned long int *);
  static int m_get_type_id(const long long int *);
  static int m_get_type_id(const unsigned long long int *);
  static int m_get_type_id(const float *);
  static int m_get_type_id(const double *);
  static int m_get_type_id(PyObject **);
  static int m_get_type_id(const PyObject **);

public:
  template <typename T> static int get_type_id(T *arg, bool &is_readonly);
  template <typename T> static int get_type_id(const T *arg, bool &is_readonly);
  template <typename T> static int get_type_id(T **arg, bool &is_readonly);
  template <typename T> static int get_type_id(const T **arg, bool &is_readonly);
  template <int E> static int get_type_id(char (*arg)[E], bool &is_readonly);
  template <int E> static int get_type_id(const char (*arg)[E], bool &is_readonly);

  static const char *to_string(const int);
};

} //namespace pyhrol

#endif //__pyhrol_type_traits_h__
