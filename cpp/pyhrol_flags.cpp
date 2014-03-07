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

// $Date: 2013-11-29 03:26:15 +0400 (Fri, 29 Nov 2013) $
// $Revision: 847 $

#include <unistd.h>
#include <stdio.h>
#include <pyport.h>
#include <object.h>
#include <methodobject.h>

#include "pyhrol_flags.h"

SmartFlagInt::ftree_t method_flags_struct::m_ftree;

method_flags_struct::method_flags_struct()
  : SmartFlagInt(init())
{
}

const SmartFlagInt::ftree_t *method_flags_struct::init()
{
  if (!m_ftree.filled())
  {
    m_ftree.add(npAnd, METH_OLDARGS, "METH_OLDARGS");
    m_ftree.add(npAnd, METH_VARARGS, "METH_VARARGS", METH_OLDARGS);
    m_ftree.add(npAnd, METH_KEYWORDS, "METH_KEYWORDS", METH_VARARGS);
    m_ftree.add(npAnd, METH_NOARGS, "METH_NOARGS", METH_KEYWORDS);
    m_ftree.add(npAnd, METH_O, "METH_O", METH_NOARGS);
    m_ftree.add(npAnd, METH_CLASS, "METH_CLASS", METH_O);
    m_ftree.add(npAnd, METH_STATIC, "METH_STATIC", METH_CLASS);
    m_ftree.add(npAnd, METH_COEXIST, "METH_COEXIST", METH_STATIC);
  }
  return &m_ftree;
}
