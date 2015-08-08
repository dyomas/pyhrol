#!/bin/sh
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

# $Date: 2014-02-27 22:19:34 +0400 (Чт., 27 февр. 2014) $
# $Revision: 896 $

#Адаптация конфигурационных файлов doxygen (Doxyfile) для CMake

set -e

readonly TRUNK_DIR=`dirname $0 | awk '{print $0 "/../../"}' | xargs realpath`
readonly MYNAME=`basename $0`
readonly MODE_DEFAULT="generate"
readonly MODES_DESCRIPTIONS="${MODE_DEFAULT}.Generate \`-D\` and \`-T\` files|fix.Replace boolean values (ON -> YES, OFF -> NO)"
readonly VALUES_FILE_DEFAULT=Doxyfile.vars
readonly TEMPLATE_FILE_DEFAULT=Doxyfile.template

MODE=$MODE_DEFAULT
EXIT=0
USAGE=0
VERBOSE=0
TMP_USED=0
VALUES_FILE=$VALUES_FILE_DEFAULT
TEMPLATE_FILE=$TEMPLATE_FILE_DEFAULT

usage () {
  echo "Usage: $MYNAME [options] DOXYFILE
Generates files for integration between Doxygen and CMake
Options are:
  -D - file with default values, consists of CMake's \`set\` commands, default \`$VALUES_FILE_DEFAULT\`
  -T - template file, special format for CMake's \`configure_file\` command, default \`$TEMPLATE_FILE_DEFAULT\`
  -m - mode, one of the following, default \`${MODE_DEFAULT}\`" >&2
IFS='|'
for MODE_CUR in $MODES_DESCRIPTIONS;
do
  echo "    ${MODE_CUR%%.*} -- ${MODE_CUR##*.}" >&2
done
echo "  -v - show diagnostic messages
  -vv - show diagnostic messages and set -x
  -h - show help message and exit
DOXYFILE - source file with default values, generated if ommited or final file to be fixed"
}

while getopts D:T:m:vh Option; do
  case $Option in
    D) VALUES_FILE=$OPTARG
    ;;
    T) TEMPLATE_FILE=$OPTARG
    ;;
    m) MODE=$OPTARG
    ;;
    v) VERBOSE=$((VERBOSE + 1))
    ;;
    h) USAGE=1
    ;;
    *)
      USAGE=1
      EXIT=1
    ;;
  esac
done

if [ $USAGE -ne 0 ];
then
  usage
  exit $EXIT
fi

shift $((OPTIND - 1))
DOXYFILE=${*}

if [ "$MODE" != "$MODE_DEFAULT" ];
then
  echo $MODES_DESCRIPTIONS | grep -q -E '(^|[\|])'$MODE'([\.\|]|$)' || {
    echo "*** Mode \"$MODE\" invalid" >&2
    EXIT=1
  }
fi

which doxygen 2>&1 > /dev/null || {
  echo "*** Doxygen executable not found" >&2
  EXIT=1
}

if [ $EXIT -eq 0 ] && [ "$MODE" = "$MODE_DEFAULT" ];
then
  if [ -z "$DOXYFILE" ];
  then
    DOXYFILE=`mktemp $TRUNK_DIR/tmp/__dcg_XXXX`
    doxygen -g $DOXYFILE > /dev/null
    if [ -e "$DOXYFILE.bak" ];
    then
      rm -f $DOXYFILE.bak
    fi
    TMP_USED=1
  elif ! [ -e "$DOXYFILE" ] || ! [ -f "$DOXYFILE" ] || ! [ -r "$DOXYFILE" ];
  then
    echo "*** Doxygen file path \"$DOXYFILE\" invalid" >&2
    EXIT=1
  fi
fi


if [ $EXIT -ne 0 ];
then
  exit $EXIT
fi

if [ $VERBOSE -ne 0 ];
then
  echo "MODE = \"$MODE\"
VALUES_FILE = \"$VALUES_FILE\"
TEMPLATE_FILE = \"$TEMPLATE_FILE\"
DOXYFILE = \"$DOXYFILE\""
fi

if [ $VERBOSE -gt 1 ];
then
  set -x
fi

if [ "$MODE" = "$MODE_DEFAULT" ];
then
  sed -n "s/^\([^#][^[:space:]=]\{1,\}\)[[:space:]]*=[[:space:]]*\([^[:space:]].*\)\$/set(DOXY_\1 \2)/p" $DOXYFILE > $VALUES_FILE
  sed "s/^\([^#][^[:space:]=]\{1,\}\)[[:space:]]*=.*\$/\1 = \"@DOXY_\1@\"/" $DOXYFILE > $TEMPLATE_FILE
elif [ "$MODE" = "fix" ];
then
  sed -i.back "s/^\([^#][^[:space:]=]\{1,\}\)[[:space:]]*=[[:space:]]*\"ON\"[[:space:]]*\$/\1 = \"YES\"/; s/^\([^#][^[:space:]=]\{1,\}\)[[:space:]]*=[[:space:]]*\"TRUE\"[[:space:]]*\$/\1 = \"YES\"/; s/^\([^#][^[:space:]=]\{1,\}\)[[:space:]]*=[[:space:]]*\"OFF\"[[:space:]]*\$/\1 = \"NO\"/; s/^\([^#][^[:space:]=]\{1,\}\)[[:space:]]*=[[:space:]]*\"FALSE\"[[:space:]]*\$/\1 = \"NO\"/;" $DOXYFILE
  if [ $VERBOSE -ne 0 ];
  then
    diff $DOXYFILE.back $DOXYFILE || {
    .
    }
  fi
  rm -f $DOXYFILE.back
fi

if [ $TMP_USED -ne 0 ];
then
  rm -f $DOXYFILE
fi
