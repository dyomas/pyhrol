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
# $Date: 2014-02-27 22:19:34 +0400 (Thu, 27 Feb 2014) $
# $Revision: 896 $

cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

project (pyhrol_examples)

set (PYTHON_REQUIRED ON)

include_directories (/usr/local/include ../include)

string(REGEX REPLACE "[ ]*\([0-9]+\)[ ]*" "example_\\1;" FUNCTION_EXAMPLES "0010 0020 0030 0040 0050 0060 0070 0080 0090 0100 0110 0120")
foreach (PYHROL_TARGET ${FUNCTION_EXAMPLES})
  add_library(${PYHROL_TARGET} MODULE EXCLUDE_FROM_ALL
    ../cpp/pyhrol_starter.cpp
    ${PYHROL_TARGET}.cpp
  )
  set_target_properties(${PYHROL_TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endforeach()


string(REGEX REPLACE "[ ]*\([0-9]+\)[ ]*" "example_\\1;" CLASS_EXAMPLES "0400 0410 0420 0430 0440 0450 0455 0460 0470 0480 0490 0495 0500 0505 0510 0520 0540 0550 0560")
foreach (PYHROL_TARGET ${CLASS_EXAMPLES})
  add_library(${PYHROL_TARGET} MODULE EXCLUDE_FROM_ALL
    ../cpp/pyhrol_starter.cpp
    ${PYHROL_TARGET}.cpp
    myclass.cpp
  )
  set_target_properties(${PYHROL_TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endforeach()


string(REGEX REPLACE "[ ]*\([0-9]+\)[ ]*" "example_\\1;" OTHER_EXAMPLES "0800 0810 0820 0830 0840 0850")
foreach (PYHROL_TARGET ${OTHER_EXAMPLES})
  if (${PYHROL_TARGET} MATCHES "example_08[0123]0")
    #No such target
  else()
    add_library(${PYHROL_TARGET} MODULE EXCLUDE_FROM_ALL
      ../cpp/pyhrol_starter.cpp
      ${PYHROL_TARGET}.cpp
    )
    set_target_properties(${PYHROL_TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  endif()
endforeach()


string(REGEX REPLACE "[ ]*\([0-9]+\)[ ]*" "example_\\1;" ERRORS_EXAMPLES "0900 0910 0920 0925 0926 0930 0940 0950")
foreach (PYHROL_TARGET ${ERRORS_EXAMPLES})
  if (${PYHROL_TARGET} MATCHES "example_0(910|926)")
#     No such target
  else()
    add_library(${PYHROL_TARGET} MODULE EXCLUDE_FROM_ALL
      ../cpp/pyhrol_starter.cpp
      ${PYHROL_TARGET}.cpp
      myclass.cpp
    )
    set_target_properties(${PYHROL_TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  endif()
endforeach()


set (PYHROL_TARGETS ${FUNCTION_EXAMPLES} ${CLASS_EXAMPLES} ${OTHER_EXAMPLES} ${ERRORS_EXAMPLES})


enable_testing()
foreach (PYHROL_TARGET ${PYHROL_TARGETS})
  if (${PYHROL_TARGET} STREQUAL "example_0120")
    #Target without test
  elseif (${PYHROL_TARGET} MATCHES "example_0800")
    if (NOT PYHROL_SAFE_MODE)
      add_test(NAME ${PYHROL_TARGET}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_LIST_DIR}/test.sh "example_0010 example_0495" "pyhrol example_0010 example_0495" ${PYHROL_TARGET})
    endif()
  elseif (${PYHROL_TARGET} MATCHES "example_0810")
    if (NOT PYHROL_SAFE_MODE)
      add_test(NAME ${PYHROL_TARGET}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_LIST_DIR}/test.sh "" "pyhrol" ${PYHROL_TARGET})
    endif()
  elseif (${PYHROL_TARGET} MATCHES "example_0820")
    if (NOT PYHROL_SAFE_MODE)
      add_test(NAME ${PYHROL_TARGET}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_LIST_DIR}/test.sh "example_0120 example_0495" "pyhrol example_0120 example_0495" ${PYHROL_TARGET})
    endif()
  elseif (${PYHROL_TARGET} MATCHES "example_0830")
    if (PYHROL_SAFE_MODE)
      add_test(NAME ${PYHROL_TARGET}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_LIST_DIR}/test.sh "example_0120" "pyhrol example_0120" ${PYHROL_TARGET})
    endif()
  elseif (${PYHROL_TARGET} MATCHES "example_09[0]0")
    #Target without test
  elseif (${PYHROL_TARGET} MATCHES "example_0910")
    add_test(NAME ${PYHROL_TARGET}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/test.sh "example_0500 example_0505" "example_0500 example_0505" ${PYHROL_TARGET})
  elseif (${PYHROL_TARGET} MATCHES "example_092[05]")
    if (NOT PYHROL_SAFE_MODE)
      add_test(NAME ${PYHROL_TARGET}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_LIST_DIR}/test.sh "${PYHROL_TARGET}" "pyhrol ${PYHROL_TARGET}" ${PYHROL_TARGET})
    endif()
  elseif (${PYHROL_TARGET} MATCHES "example_0926")
    add_test(NAME ${PYHROL_TARGET}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/test.sh "example_0925" "example_0925" ${PYHROL_TARGET})
  else()
    add_test(NAME ${PYHROL_TARGET}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/test.sh ${PYHROL_TARGET})
  endif()

  if (${PYHROL_TARGET} MATCHES "example_08[0123]0")
    #No such target
  elseif (${PYHROL_TARGET} MATCHES "example_(0910|0926)")
    #No such target
  else()
    set_target_properties (${PYHROL_TARGET} PROPERTIES
      COMPILE_DEFINITIONS "USE_TRACER"
    )
  endif()
endforeach()


set (PYTHON_PACKAGES ${CMAKE_BINARY_DIR})
string(REGEX REPLACE "example_08[0123]0" "" PYHROL_TARGETS "${PYHROL_TARGETS}")
string(REGEX REPLACE "example_(0910|0926)" "" PYHROL_TARGETS "${PYHROL_TARGETS}")

pyhrol_configure_targets(${PYHROL_TARGETS})

set (PROJECT_FILES " ")
