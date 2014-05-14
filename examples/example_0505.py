#!/usr/bin/python
# -*- coding: utf-8 -*-
#  Copyright (c) 2013, 2014, Pyhrol, pyhrol@rambler.ru
#  GEO: N55.703431,E37.623324 .. N48.742359,E44.536997
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  4. Neither the name of the Pyhrol nor the names of its contributors
#     may be used to endorse or promote products derived from this software
#     without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
#  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
#  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#  SUCH DAMAGE.
#

import example_0505

c1 = example_0505.MyClass("Bhliain ")
c2 = example_0505.MyClass("2014")
c3 = example_0505.MyClass(" nua Shona!")

c = c1 + '2014' + c3
c.say()

try:
  c = 1 + c2
except TypeError as ex:
  print '***', ex

if 2014 in c:
  print 'Y'
else:
  print 'N'

if '2014' in c:
  print 'Y'
else:
  print 'N'

try:
  if c in '2014':
    print 'Y'
  else:
    print 'N'
except TypeError as ex:
  print '***', ex

c = c1 + c3
c.say()
c[8:8] = 1974
c.say()
c[8:12] = '1999'
c.say()

c = c1
c += 2009
c += c3
c.say()
