#!/bin/sh
#  Copyright (c) 2013 Pyhrol, pyhrol@rambler.ru
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

set -e

readonly LIST_DIR=`dirname $0`
readonly BUILD_DIR=`pwd`
readonly TARGETS="${1}"
readonly LIBRARIES="${2:-${1}}"
readonly EXAMPLE="${3:-${1}}"

RETVAL=0


echo "TARGETS = \"$TARGETS\"
LIBRARIES = \"$LIBRARIES\"
EXAMPLE = \"$EXAMPLE\"
LIST_DIR = \"$LIST_DIR\"
BUILD_DIR = \"$BUILD_DIR\"" >&2

if [ -n "$TARGETS" ];
then
  make $TARGETS
fi

cd $LIST_DIR

if [ -n "$LIBRARIES" ];
then
  for LIBRARY in $LIBRARIES;
  do
    if ! [ -e "$LIBRARY.so" ];
    then
      ln -sf $BUILD_DIR/$LIBRARY.so .
    fi
  done
fi


unset __PY_SHOW_CALLS
unset __PY_HACK_SIGNATURES
unset PYHROL_TRACE
unset PYHROL_TUPLE_HEADER_FORMAT
unset PYHROL_TUPLE_BODY_FORMAT
unset PYHROL_ERROR_HEADER_FORMAT
unset PYHROL_ERROR_BODY_FORMAT


readonly TMP_FILE=`mktemp /tmp/__phe_XXXX`
python -SsE $EXAMPLE.py > $TMP_FILE 2>&1 && {
  diff --label="<$EXAMPLE.txt>" --label="(obtained)" -U0 $EXAMPLE.txt $TMP_FILE || {
    RETVAL=$?
  }
} || {
  RETVAL=$?
  cat $TMP_FILE >&2
}

rm -f $TMP_FILE
exit $RETVAL
