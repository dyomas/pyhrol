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

// $Date: 2013-09-18 02:14:07 +0400 (Wed, 18 Sep 2013) $
// $Revision: 785 $

#include <stdio.h>
#include <pyport.h>
#include <object.h>
#include <structmember.h>

#include "pyhrol_type_traits.h"


namespace pyhrol
{

int TypeTraits::m_get_type_id(const char *)
{
  return T_CHAR;
}

int TypeTraits::m_get_type_id(char **)
{
  return T_STRING;
}

int TypeTraits::m_get_type_id(const char **)
{
  return T_STRING;
}

int TypeTraits::m_get_type_id(const int8_t *)
{
  return T_BYTE;
}

int TypeTraits::m_get_type_id(const uint8_t *)
{
  return T_UBYTE;
}

int TypeTraits::m_get_type_id(const bool *)
{
  return T_BOOL;
}

int TypeTraits::m_get_type_id(const int16_t *)
{
  return T_SHORT;
}

int TypeTraits::m_get_type_id(const uint16_t *)
{
  return T_USHORT;
}

int TypeTraits::m_get_type_id(const int32_t *)
{
  return T_INT;
}

int TypeTraits::m_get_type_id(const uint32_t *)
{
  return T_UINT;
}

int TypeTraits::m_get_type_id(const long int *)
{
  return T_LONG;
}

int TypeTraits::m_get_type_id(const unsigned long int *)
{
  return T_ULONG;
}

int TypeTraits::m_get_type_id(const long long int *)
{
  return T_LONGLONG;
}

int TypeTraits::m_get_type_id(const unsigned long long int *)
{
  return T_ULONGLONG;
}

int TypeTraits::m_get_type_id(const float *)
{
  return T_FLOAT;
}

int TypeTraits::m_get_type_id(const double *)
{
  return T_DOUBLE;
}

int TypeTraits::m_get_type_id(PyObject **)
{
  //TODO Think about switch between T_OBJECT and T_OBJECT_EX
  return T_OBJECT_EX;
}

int TypeTraits::m_get_type_id(const PyObject **)
{
  return T_OBJECT_EX;
}

const char *TypeTraits::to_string(const int type)
{
  static const char *names[] =
  {
      "T_SHORT" //0
    , "T_INT" //1
    , "T_LONG" //2
    , "T_FLOAT" //3
    , "T_DOUBLE" //4
    , "T_STRING" //5
    , "T_OBJECT" //6
    , "T_CHAR" //7
    , "T_BYTE" //8
    , "T_UBYTE" //9
    , "T_USHORT" //10
    , "T_UINT" //11
    , "T_ULONG" //12
    , "T_STRING_INPLACE" //13
    , "T_BOOL" //14
    , NULL
    , "T_OBJECT_EX" //16
    , "T_LONGLONG" //17
    , "T_ULONGLONG" //18
    , "T_PYSSIZET" //19
  };

  return names[type];
}

}; //namespace pyhrol


