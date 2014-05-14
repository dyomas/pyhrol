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

import example_0500

l = example_0500.MyClass("Moikka ")
r = example_0500.MyClass("maailma!")

c = l + r
c.say()

print len (c)

l += r
l.say()

l *= 2
l.say()

c = l[0]
c.say()

c = l[7:10]
c.say()

try:
  c = r[12:15]
except IndexError as ex:
  print "***", ex

l[3:6] = r
l.say()

del l[3:11]
l.say()

if r in l:
  print 'Y'
else:
  print 'N'

if l in r:
  print 'Y'
else:
  print 'N'

for i in r:
  i.say()

try:
  c = l * 2
except NotImplementedError as ex:
  print '***', ex

try:
  c += True
except TypeError as ex:
  print '***', ex

