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
cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set (PYTHON_REQUIRED ON)

if (CMAKE_VERBOSE_MAKEFILE)
  set (VERBOSE -v)
endif()

include(pyhrol_init)
include_directories (include)

get_filename_component(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} REALPATH)

enable_testing()
add_subdirectory(examples)


find_package (PythonInterp 2.4 REQUIRED)
install (TARGETS pyhrol pyhrolsafe pyself pyselfsafe DESTINATION lib PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
install (FILES
  include/pyhrol_fmt_in_macro.h
  include/pyhrol_fmt_out_macro.h
  cpp/pyhrol_starter.cpp
  include/pyhrol.h
  include/pyhrol_auto_holder.h
  include/pyhrol_common.h
  include/pyhrol_container.h
  include/pyhrol_container.hpp
  include/pyhrol_exception_handler.h
  include/pyhrol_helpers.hpp
  include/pyhrol_macro.h
  include/pyhrol_ptr.h
  include/pyhrol_ptr.hpp
  include/pyhrol_trace.h
  include/pyhrol_tuple_describer.h
  include/pyhrol_tuple_in.h
  include/pyhrol_tuple_out.h
  include/pyhrol_tuples.h
  include/pyhrol_type.h
  include/pyhrol_type.hpp
  include/pyhrol_type_base.h
  include/pyhrol_type_base.hpp
  include/pyhrol_type_iterable.h
  include/pyhrol_type_iterable.hpp
  include/pyhrol_type_map.h
  include/pyhrol_type_map.hpp
  include/pyhrol_type_number.h
  include/pyhrol_type_number.hpp
  include/pyhrol_type_number_any.h
  include/pyhrol_type_number_any.hpp
  include/pyhrol_type_number_common.h
  include/pyhrol_type_number_common.hpp
  include/pyhrol_type_pointer.h
  include/pyhrol_type_pointer.hpp
  include/pyhrol_type_sequence.h
  include/pyhrol_type_sequence.hpp
  include/pyhrol_type_smart.h
  include/pyhrol_type_smart.hpp
  include/pyhrol_type_special.h
  include/pyhrol_type_special.hpp
  include/pyhrol_type_traits.h
  include/pyhrol_type_traits.hpp
  include/self_addressed_cstring.h
  include/smart_flag.h
DESTINATION include/pyhrol PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

foreach (TARGET_NAME pyhrol pyhrolsafe pyself pyselfsafe)
  get_target_property (TMP_NAME ${TARGET_NAME} LOCATION)
  get_filename_component(TMP_NAME ${TMP_NAME} NAME)
  set(LIBRARY_${TARGET_NAME}_DESTINATION lib/${TMP_NAME})
endforeach()
configure_file(cmake/pyhrol-config.cmake.template ${CMAKE_BINARY_DIR}/pyhrol-config.cmake @ONLY)
install (FILES ${CMAKE_BINARY_DIR}/pyhrol-config.cmake DESTINATION lib/pyhrol)



execute_process(COMMAND ${ROOT_DIR}/sh/maintain/doxy_conf.sh ${VERBOSE} WORKING_DIRECTORY ${CMAKE_BINARY_DIR} RESULT_VARIABLE DOXY_CONF_RESULT)
if (DOXY_CONF_RESULT)
  message(WARNING "Documentation generating with doxygen impossible")
else()
  set (HAVE_DOT FALSE CACHE BOOL "")
  include (${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.vars)
  set (DOXY_PROJECT_NAME pyhrol)
  set (DOXY_PROJECT_NUMBER 0.1)
  set (DOXY_PROJECT_BRIEF "Python binding for C++")
  set (DOXY_PROJECT_LOGO ${ROOT_DIR}/jpg/pyhrol.jpg)
  set (DOXY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
  set (DOXY_STRIP_FROM_PATH ${ROOT_DIR})
  set (DOXY_STRIP_FROM_INC_PATH ${ROOT_DIR})
  set (DOXY_TAB_SIZE 2)
  set (DOXY_EXTRACT_ALL TRUE)
  set (DOXY_EXTRACT_PRIVATE TRUE)
  set (DOXY_EXTRACT_STATIC TRUE)
  set (DOXY_SHOW_DIRECTORIES TRUE)
  set (DOXY_QUIET TRUE)
  set (DOXY_WARN_LOGFILE ${CMAKE_BINARY_DIR}/doxy.log)
  set (DOXY_INPUT "${ROOT_DIR}/include\" \\
    \"${ROOT_DIR}/cpp\" \\
    \"${ROOT_DIR}/examples/")
  set (DOXY_FILE_PATTERNS "pyhrol.h\" \\
    \"pyhrol_*.h\" \\
    \"pyhrol_*.hpp\" \\
    \"example*.cpp\" \\
    \"self_addressed_cstring.h\" \\
    \"smart_flag.h\" \\
    \"example*.h\" \\
    \"example*.py\" \\
    \"myclass*.cpp\" \\
    \"myclass*.h")
  set (DOXY_RECURSIVE TRUE)
  set (DOXY_EXAMPLE_PATH ${ROOT_DIR}/examples)
  set (DOXY_SOURCE_BROWSER TRUE)
  set (DOXY_INLINE_SOURCES TRUE)
  set (DOXY_COLS_IN_ALPHA_INDEX 4)
  set (DOXY_SERVER_BASED_SEARCH TRUE)
  set (DOXY_GENERATE_LATEX FALSE)
  set (DOXY_HAVE_DOT ${HAVE_DOT})
  set (DOXY_CALL_GRAPH TRUE)
  set (DOXY_CALLER_GRAPH TRUE)
  configure_file(${CMAKE_BINARY_DIR}/Doxyfile.template ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
    COMMAND ${ROOT_DIR}/sh/maintain/doxy_conf.sh ${VERBOSE} -mfix Doxyfile
    COMMAND doxygen Doxyfile
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
endif()

