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

// $Date: 2014-04-18 01:08:27 +0400 (Fri, 18 Apr 2014) $
// $Revision: 909 $

#include <errno.h>
// Enables series of macros U?INT(8|16|32|64)_(MIN|MAX) used below
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <set>
#include <stdexcept>
#include <iomanip>
#include "strings_manip.h"

using namespace std;


#ifndef __BSD_VISIBLE
static int64_t strtonum(const char *numstr, int64_t minval, int64_t maxval, const char **errstrp)
{
  int64_t ret_val = 0;
  errno = 0;

  if (errstrp)
  {
    *errstrp = NULL;
  }

  if (minval > maxval)
  {
    errno = EINVAL;
  }
  else
  {
    char *ep;
    ret_val = strtoll(numstr, &ep, 10);
    if (numstr == ep || *ep)
    {
      errno = EINVAL;
    }
    else if (errstrp && (ret_val == LLONG_MIN && errno == ERANGE || ret_val < minval))
    {
      *errstrp = "too small";
    }
    else if (errstrp && (ret_val == LLONG_MAX && errno == ERANGE || ret_val > maxval))
    {
      *errstrp = "too large";
    }
  }

  if (errstrp && errno == EINVAL)
  {
    *errstrp = "invalid";
  }
  return ret_val;
}
#endif //__BSD_VISIBLE

bool parse_string(const char *&begin, const char *&end, const char *src, const char delimiter)
{
  if (!end)
  {
    end = src;
  }
  else
  {
    if (!*end)
    {
      return false;
    }
    else if (*end == delimiter)
    {
      end ++;
    }
  }

  begin = end;

  while (*end && *end != delimiter)
  {
    end ++;
  }
  return true;
}

bool parse_string(const char *&begin, const char *&end, const char *src, const char *src_end, const char delimiter)
{
  if (!end)
  {
    end = src;
  }
  else
  {
    if (end == src_end)
    {
      return false;
    }
    else if (*end == delimiter)
    {
      end ++;
    }
  }

  begin = end;

  while (end != src_end && *end != delimiter)
  {
    end ++;
  }
  return true;
}

bool parse_string(const char *&begin, const char *&end, const char *src)
{
  if (!end)
  {
    end = src;
  }
  while (*end && isspace(*end))
  {
    end ++;
  }
  if (!*end)
  {
    return false;
  }

  begin = end;

  while (*end && !isspace(*end))
  {
    end ++;
  }
  return true;
}

bool parse_string(const char *&begin, const char *&end, const char *src, const char *src_end)
{
  if (!end)
  {
    end = src;
  }
  while (end != src_end && isspace(*end))
  {
    end ++;
  }
  if (end == src_end)
  {
    return false;
  }

  begin = end;

  while (end != src_end && !isspace(*end))
  {
    end ++;
  }
  return true;
}

void hex_to_array(uint8_t *res, size_t &res_length, const char *src)
{
  ostringstream ostr;
  bool
      braced = false
    , odd = false
  ;
  uint8_t quartet;
  res_length = 0;

  if (src[0] == '0' && src[1] == 'x')
  {
    src += 2;
  }

  if (*src == '{')
  {
    braced = true;
    src ++;
  }

  while (*src)
  {
    if (*src >= '0' && *src <= '9')
    {
      quartet = *src - '0';
    }
    else if (*src >= 'a' && *src <= 'f')
    {
      quartet = *src - 'a' + '\x0A';
    }
    else if (*src >= 'A' && *src <= 'F')
    {
      quartet = *src - 'A' + '\x0A';
    }
    else if (braced && *src == '}')
    {
      braced = false;
      src ++;
      break;
    }
    else
    {
      ostr << "Non hex symbol \'" << *src << "\'";
      throw runtime_error(ostr.str());
    }
    if (odd)
    {
      *res += quartet;
      res ++;
      res_length ++;
    }
    else
    {
      *res = quartet << 4;
    }
    odd = !odd;
    src ++;
  }

  if (odd)
  {
    ostr << "Unpaired hexadecimal symbol";
    throw runtime_error(ostr.str());
  }
  else if (braced)
  {
    ostr << "Unpaired brace";
    throw runtime_error(ostr.str());
  }
  else if (*src)
  {
    ostr << "Unexpected line end (\'" << *src << "\')";
    throw runtime_error(ostr.str());
  }
}

void array_to_hex(char *res, const uint8_t *src, const size_t length, const bool capitals)
{
  size_t pos = length;
  uint8_t ch;

  while (pos)
  {
    ch = *src >> 4;
    ch += ch > 0x9 ? (capitals ? '\x37' : '\x57') : '\x30';
    *res ++ = ch;
    ch = *src & '\x0F';
    ch += ch > 0x9 ? (capitals ? '\x37' : '\x57') : '\x30';
    *res ++ = ch;
    pos --;
    src ++;
  }
}

const string cut_spaces(const char *src)
{
  const char
      *pbegin = NULL
    , *pend = NULL
  ;
  string ret_val;

  do
  {
    const bool space = !*src || isspace(*src);
    if (!pbegin)
    {
      if (!space)
      {
        pbegin = src;
      }
    }
    else
    {
      if (!pend && space)
      {
        pend = src;
      }
      else if (pend && !space)
      {
        pend = NULL;
      }
    }
  }
  while (*src ++);

  if (pbegin && pend)
  {
    ret_val.assign(pbegin, pend);
  }
  return ret_val;
}

void replace_chars(string &str, const char from, const char to)
{
  string::size_type pos = 0U;

  while ((pos = str.find(from, pos)) != string::npos)
  {
    str[pos] = to;
  }
}

bool is_number(const char *str)
{
  while (*str)
  {
    if (*str < '0' || *str > '9')
    {
      return false;
    }
    str ++;
  }
  return true;
}

bool is_number(const string &str)
{
  for (string::const_iterator iter = str.begin(); iter != str.end(); iter ++)
  {
    if (*iter < '0' || *iter > '9')
    {
      return false;
    }
  }
  return true;
}

int8_t c_string_to_int8(const char *src)
{
  const char *errstr;
  int8_t res = strtonum(src, -128, 127, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int8_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
  return res;
}

uint8_t c_string_to_uint8(const char *src)
{
  const char *errstr;
  uint8_t res = strtonum(src, 0, UINT8_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to uint8_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
  return res;
}

int16_t c_string_to_int16(const char *src)
{
  const char *errstr;
  int16_t res = strtonum(src, INT16_MIN, INT16_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int16_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
  return res;
}

uint16_t c_string_to_uint16(const char *src)
{
  const char *errstr;
  uint16_t res = strtonum(src, 0, UINT16_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to uint16_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
  return res;
}

int32_t c_string_to_int(const char *src)
{
  char *endptr;
  int32_t res = strtol(src, &endptr, 10);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
  return res;
}

int32_t string_to_int(const char *begin, const char *end)
{
  char *endptr;
  int32_t res = strtol(begin, &endptr, 10);
  int errno_copy = errno;

  if (endptr != end)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << string(begin, end) << "\" to int";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
  return res;
}

int32_t string_to_int(const string &src)
{
  char *endptr;
  int32_t res = strtol(src.c_str(), &endptr, 10);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
  return res;
}

uint32_t c_string_to_uint(const char *src)
{
  const char *errstr;
  uint32_t res = strtonum(src, 0, UINT32_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to uint32_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
  return res;
}

int64_t c_string_to_int64(const char *src)
{
  char *endptr;
  int64_t res = strtoll(src, &endptr, 10);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int64_t";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
  return res;
}

int64_t string_to_int64(const char *begin, const char *end)
{
  char *endptr;
  int64_t res = strtoll(begin, &endptr, 10);
  int errno_copy = errno;

  if (endptr != end)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << string(begin, end) << "\" to int64_t";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
  return res;
}

int64_t string_to_int64(const string &src)
{
  char *endptr;
  int64_t res = strtoll(src.c_str(), &endptr, 10);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int64_t";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
  return res;
}

uint64_t c_string_to_uint64(const char *src)
{
  const char *errstr;
  uint64_t res = strtonum(src, 0, UINT64_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to uint64_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
  return res;
}

float c_string_to_float(const char *src)
{
  char *endptr;
  float res = strtof(src, &endptr);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to float";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
  return res;
}

double c_string_to_double(const char *src)
{
  char *endptr;
  double res = strtod(src, &endptr);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to double";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
  return res;
}

void string_to_number(int8_t *pres, const string &src)
{
  const char *errstr;
  *pres = strtonum(src.c_str(), -128, 127, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int8_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
}

void string_to_number(uint8_t *pres, const string &src)
{
  const char *errstr;
  *pres = strtonum(src.c_str(), 0, UINT8_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to uint8_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
}

void string_to_number(int16_t *pres, const string &src)
{
  const char *errstr;
  *pres = strtonum(src.c_str(), INT16_MIN, INT16_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int16_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
}

void string_to_number(uint16_t *pres, const string &src)
{
  const char *errstr;
  *pres = strtonum(src.c_str(), 0, UINT16_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to uint16_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
}

void string_to_number(int32_t *pres, const string &src)
{
  char *endptr;
  *pres = strtol(src.c_str(), &endptr, 10);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
}

void string_to_number(uint32_t *pres, const string &src)
{
  const char *errstr;
  *pres = strtonum(src.c_str(), 0, UINT32_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to uint32_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
}

void string_to_number(int64_t *pres, const string &src)
{
  char *endptr;
  *pres = strtoll(src.c_str(), &endptr, 10);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to int64_t";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
}

void string_to_number(uint64_t *pres, const string &src)
{
  const char *errstr;
  *pres = strtonum(src.c_str(), 0, UINT64_MAX, &errstr);
  int errno_copy = errno;

  if (errstr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to uint64_t (";
    if (errno_copy)
    {
      ostr << errno_copy << ", ";
    }
    ostr << errstr << ")";
    throw runtime_error(ostr.str());
  }
}

void string_to_number(float *pres, const string &src)
{
  char *endptr;
  *pres = strtof(src.c_str(), &endptr);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to float";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
}

void string_to_number(double *pres, const string &src)
{
  char *endptr;
  *pres = strtod(src.c_str(), &endptr);
  int errno_copy = errno;

  if (*endptr)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << src << "\" to double";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
}

void string_to_number(int32_t *pres, const char *begin, const char *end)
{
  char *endptr;
  *pres = strtol(begin, &endptr, 10);
  int errno_copy = errno;

  if (endptr != end || errno_copy == ERANGE)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << string(begin, end) << "\" to int32_t";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
}

void string_to_number(int64_t *pres, const char *begin, const char *end)
{
  char *endptr;
  int errno_copy;

  *pres = strtoll(begin, &endptr, 10);
  errno_copy = errno;

  if (endptr != end || errno_copy == ERANGE)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << string(begin, end) << "\" to int64_t";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
}

void string_to_number(float *pres, const char *begin, const char *end)
{
  char *endptr;
  int errno_copy;

  *pres = strtod(begin, &endptr);
  errno_copy = errno;

  if (endptr != end || errno_copy == ERANGE)
  {
    ostringstream ostr;
    ostr << "Conversion failed from string \"" << string(begin, end) << "\" to float";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
}

void string_to_number(double *pres, const char *begin, const char *end)
{
  ostringstream ostr;
  char *endptr;
  int errno_copy;

  *pres = strtod(begin, &endptr);
  errno_copy = errno;

  if (endptr != end || errno_copy == ERANGE)
  {
    ostr << "Conversion failed from string \"" << string(begin, end) << "\" to double";
    if (errno_copy)
    {
      ostr << " (" << errno_copy << ", " << strerror(errno_copy) << ")";
    }
    throw runtime_error(ostr.str());
  }
}

void to_lower_case(string &data)
{
  for (string::iterator iter = data.begin(); iter != data.end(); iter ++)
  {
    *iter = tolower(*iter);
  }
}

void to_lower_case(string &res, const char *src)
{
  while (*src)
  {
    res.push_back(tolower(*src ++));
  }
}

void to_upper_case(string &data)
{
  for (string::iterator iter = data.begin(); iter != data.end(); iter ++)
  {
    *iter = toupper(*iter);
  }
}

void to_upper_case(string &res, const char *src)
{
  while (*src)
  {
    res.push_back(toupper(*src ++));
  }
}
