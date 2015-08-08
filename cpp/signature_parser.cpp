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

// $Date: 2013-11-28 03:36:46 +0400 (Чт., 28 нояб. 2013) $
// $Revision: 844 $

#include <stddef.h>
#include <string.h>

#include "signature_parser.h"

ParserState::ParserState()
  : part(pInitial)
{
}

const char *ParserState::parse(ParserState &state, const char *&src)
{
  if (!*src || state.part == ParserState::pTemplateValue && *src == ']')
  {
    return NULL;
  }

  const char *retval = NULL;

  if (state.part == ParserState::pInitial)
  {
    const char
        *pch = src
      , *pwith = NULL
    ;
    while (*pch != '\0')
    {
      if (*pch == ' ')
      {
        if (pwith)
        {
          if (pch - pwith == 6 && !strncmp(pwith, " [with", 6))
          {
            state.part = ParserState::pSignature;
            retval = src;
            src = pwith;
            break;
          }
          else
          {
            pwith = pch;
          }
        }
        else
        {
          pwith = pch;
        }
      }
      pch ++;
    }
    if (state.part != ParserState::pSignature)
    {
      state.part = ParserState::pSignature;
      retval = src;
      src = pch;
    }
  }
  else if (state.part == ParserState::pSignature)
  {
    const char
        *pch = src + 6
      , *pequal = NULL
    ;

    while (*pch != '\0')
    {
      if (*pch == ' ')
      {
        if (pequal)
        {
          if (pch - pequal == 2 && !strncmp(pequal, " =", 2))
          {
            state.part = ParserState::pTemplateName;
            retval = src + 7;
            src = pequal;
            break;
          }
          else
          {
            pequal = pch;
          }
        }
        else
        {
          pequal = pch;
        }
      }
      pch ++;
    }
    if (state.part != ParserState::pTemplateName)
    {
      state.part = ParserState::pTemplates;
      retval = src + 7;
      src = pch;
    }
  }
  else if (state.part == ParserState::pTemplateName)
  {
    const char *pch = src + 3;
    uint16_t braces = 0, angle_braces = 0, square_braces = 0;

    while (*pch != '\0')
    {
      switch (*pch)
      {
        case ',':
        case ';':
        case ']':
          if (!braces && !angle_braces && !square_braces)
          {
            state.part = ParserState::pTemplateValue;
            retval = src + 3;
            src = pch;
          }
          else if (*pch == ']')
          {
            square_braces --;
          }
          break;
        case '(':
          braces ++;
          break;
        case ')':
          braces --;
          break;
        case '<':
          angle_braces ++;
          break;
        case '>':
          angle_braces --;
          break;
        case '[':
          square_braces ++;
          break;
      }
      if (state.part == ParserState::pTemplateValue)
      {
        break;
      }
      pch ++;
    }
    if (state.part != ParserState::pTemplateValue)
    {
      state.part = ParserState::pTemplates;
      retval = src + 3;
      src = pch;
    }
  }
  else if (state.part == ParserState::pTemplateValue)
  {
    const char
        *pch = src + 2
      , *pequal = NULL
    ;

    while (*pch != '\0')
    {
      if (*pch == ' ')
      {
        if (pequal)
        {
          if (pch - pequal == 2 && !strncmp(pequal, " =", 2))
          {
            state.part = ParserState::pTemplateName;
            retval = src + 2;
            src = pequal;
            break;
          }
          else
          {
            pequal = pch;
          }
        }
        else
        {
          pequal = pch;
        }
      }
      pch ++;
    }
    if (state.part != ParserState::pTemplateName)
    {
      state.part = ParserState::pTemplates;
      retval = src + 2;
      src = pch;
    }
  }
  return retval;
}
