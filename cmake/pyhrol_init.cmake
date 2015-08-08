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
# Find Python library and headers and define targets for extension modules
# Behaviour:
#
#  -- If Python library and headers found, define targets:
#     pygen, pyhrol[safe], pyself[safe] used by any Pyhrol based projects
#
#  -- If Python library or headers not found, and PYTHON_REQUIRED defined
#     fatal error occured
#
#  -- If Python library or headers not found or invalid, and PYTHON_REQUIRED
#     not defined warns and continue;
#     Python related targets must be inactivated by PYTHONLIBS_FOUND switch

# $Date: 2015-06-05 01:14:02 +0300 (Пт., 05 июня 2015) $
# $Revision: 1035 $

find_package (PythonLibs 2.4)


set (TMP_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../tmp/include CACHE PATH "Path to temporary \"include\" directory")
set (TMP_RES_DIR ${CMAKE_CURRENT_LIST_DIR}/../tmp/res CACHE PATH "Path to directory for generated files")
set (PYHROL_MAX_INPUT_ARGS 16 CACHE STRING "Max number of arguments for PyArg_*ParseTuple* function, limited by the number of relevant macros PYHROL_PARSE_TUPLE_{M}")
set (PYHROL_MAX_OUTPUT_ARGS 8 CACHE STRING "Max number of arguments for Py_VaBuildValue, limited by the number of relevant macros PYHROL_BUILD_VALUE_{N}")


if (NOT PYTHONLIBS_FOUND)
  if (PYTHON_REQUIRED)
    message (FATAL_ERROR "Python libraries not found")
  else()
    message (WARNING "Python related targets disabled")
  endif()
endif()

if (CMAKE_VERBOSE_MAKEFILE)
  message (STATUS "PythonLibs
  PYTHONLIBS_FOUND           = \"${PYTHONLIBS_FOUND}\"
  PYTHON_LIBRARIES           = \"${PYTHON_LIBRARIES}\"
  PYTHON_INCLUDE_PATH        = \"${PYTHON_INCLUDE_PATH}\"
  PYTHON_INCLUDE_DIRS        = \"${PYTHON_INCLUDE_DIRS}\"
  PYTHON_DEBUG_LIBRARIES     = \"${PYTHON_DEBUG_LIBRARIES}\"
  PYTHONLIBS_VERSION_STRING  = \"${PYTHONLIBS_VERSION_STRING}\"
")
endif()

if (PYTHONLIBS_FOUND)
  set (PYHROL_SAFE_MODE OFF CACHE BOOL "Use library version with minimal functionality")
  set (PYHROL_USE_STATIC OFF CACHE BOOL "Use static Pyhrol libraries instead of dynamic")

  add_library(pyhrolbase OBJECT
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/array_square.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/binary_operations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/demangling.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_auto_holder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_index.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_keywords.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_trace.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_describer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_in.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_out.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_iterable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_number_any.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_sequence.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_type_traits.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/self_addressed_cstring.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/signature_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/strings_manip.cpp
  )
  set_target_properties (pyhrolbase PROPERTIES
    COMPILE_FLAGS "-fPIC -I${PYTHON_INCLUDE_PATH}"
    COMPILE_DEFINITIONS USE_TRACER
  )

  set (PYHROL_COMMON_DEV
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_container.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_exception_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_exceptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/array_void.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/printf_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_check_calls.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_describer_flexible.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_fill_state.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_fill_state_getsetter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_fill_state_normal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_format.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_in_default_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_out_default_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_state.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_data2_default_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_data_default_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_default_impl.cpp
  )

  add_library(pyhrolstddev OBJECT ${PYHROL_COMMON_DEV})
  set_target_properties (pyhrolstddev PROPERTIES
    COMPILE_FLAGS "-fPIC -I${PYTHON_INCLUDE_PATH}"
    COMPILE_DEFINITIONS USE_TRACER
  )

  add_library(pyhrol SHARED
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_init.cpp
    $<TARGET_OBJECTS:pyhrolstddev>
    $<TARGET_OBJECTS:pyhrolbase>
  )
  add_dependencies(pyhrol pygen)
  set_target_properties (pyhrol PROPERTIES
    COMPILE_FLAGS "-I${PYTHON_INCLUDE_PATH}"
    COMPILE_DEFINITIONS USE_TRACER
  )
  target_link_libraries (pyhrol ${PYTHON_LIBRARIES})

  add_library(pyhrola STATIC
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_init.cpp
    $<TARGET_OBJECTS:pyhrolstddev>
    $<TARGET_OBJECTS:pyhrolbase>
  )
  add_dependencies(pyhrola pygen)
  set_target_properties (pyhrola PROPERTIES
    COMPILE_FLAGS "-fPIC -I${PYTHON_INCLUDE_PATH}"
    COMPILE_DEFINITIONS USE_TRACER
    OUTPUT_NAME pyhrol
  )
  target_link_libraries (pyhrola ${PYTHON_LIBRARIES})

  set (PYHROL_SAFE_DEV
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_container.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_exception_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_exceptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_base_safe.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_describer_safe.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_in_safe_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuple_out_safe_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_data_safe_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_tuples_safe_impl.cpp
  )

  add_library(pyhrolsafedev OBJECT ${PYHROL_SAFE_DEV})
  set_target_properties (pyhrolsafedev PROPERTIES
    COMPILE_FLAGS "-fPIC -I${PYTHON_INCLUDE_PATH}"
    COMPILE_DEFINITIONS "USE_TRACER; PYHROL_SAFE_MODE"
  )

  add_library(pyhrolsafe SHARED
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_init.cpp
    $<TARGET_OBJECTS:pyhrolsafedev>
    $<TARGET_OBJECTS:pyhrolbase>
  )
  add_dependencies(pyhrolsafe pygen)
  set_target_properties (pyhrolsafe PROPERTIES
    COMPILE_FLAGS "-I${PYTHON_INCLUDE_PATH}"
    COMPILE_DEFINITIONS "USE_TRACER; PYHROL_SAFE_MODE"
  )
  target_link_libraries (pyhrolsafe ${PYTHON_LIBRARIES})

  add_library(pyhrolsafea STATIC
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_init.cpp
    $<TARGET_OBJECTS:pyhrolsafedev>
    $<TARGET_OBJECTS:pyhrolbase>
  )
  add_dependencies(pyhrolsafea pygen)
  set_target_properties (pyhrolsafea PROPERTIES
    COMPILE_FLAGS "-fPIC -I${PYTHON_INCLUDE_PATH}"
    COMPILE_DEFINITIONS "USE_TRACER; PYHROL_SAFE_MODE"
    OUTPUT_NAME pyhrolsafe
  )
  target_link_libraries (pyhrolsafea ${PYTHON_LIBRARIES})


  add_executable(pygen
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/strings_manip.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_generator.cpp
  )
  set_target_properties (pygen PROPERTIES
    OUTPUT_NAME __pygen
    COMPILE_FLAGS "-I${CMAKE_CURRENT_LIST_DIR}/../include"
  )
  add_custom_command(
    TARGET pygen POST_BUILD
    COMMAND $<TARGET_FILE:pygen> -L ../LICENSE -C -m1 -N${PYHROL_MAX_INPUT_ARGS} > pyhrol_fmt_in_macro.h
    COMMAND $<TARGET_FILE:pygen> -L ../LICENSE -C -m2 -N${PYHROL_MAX_OUTPUT_ARGS} > pyhrol_fmt_out_macro.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../include
    COMMENT "Generating pyhrol PARSE/BUILD macros"
  )


  string(REPLACE ";" " -I" TMP_INCLUDE "${PYTHON_INCLUDE_DIRS}")

  add_library(pyselfdev STATIC
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_flags.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self_modules.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self_types.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self_methods.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self_functions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/smart_flag_int.cpp
  )
  set_target_properties (pyselfdev PROPERTIES
    COMPILE_DEFINITIONS "USE_TRACER"
    COMPILE_FLAGS "-fPIC \\
      -I${TMP_INCLUDE} \\
      -I${CMAKE_CURRENT_LIST_DIR}/../include"
  )
  add_dependencies(pyselfdev pygen)

  set (PYHROL_STARTER_FILE ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_starter_pyhrol.cpp)
  file(WRITE ${PYHROL_STARTER_FILE} "//Generated by ${CMAKE_CURRENT_LIST_FILE}\n\nvoid pyhrol_init(const char *);\n\nextern \"C\" void initpyhrol()\n{\n  pyhrol_init(\"pyhrol\");\n}\n")
  add_library (starter_pyhrol STATIC ${PYHROL_STARTER_FILE})
  set_target_properties (starter_pyhrol PROPERTIES COMPILE_FLAGS "-fPIC")

  add_library(pyself MODULE
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self.cpp
  )
  set_target_properties (pyself PROPERTIES
    PREFIX ""
    COMPILE_DEFINITIONS "USE_TRACER"
    COMPILE_FLAGS " \\
      -I${TMP_INCLUDE} \\
      -I${CMAKE_CURRENT_LIST_DIR}/../include"
    OUTPUT_NAME pyselfstd
  )
  target_link_libraries (pyself pyhrol pyselfdev -Wl,-whole-archive starter_pyhrol -Wl,-no-whole-archive)


  add_library(pyselfsafe MODULE
    ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_self.cpp
  )
  set_target_properties (pyselfsafe PROPERTIES
    PREFIX ""
    COMPILE_DEFINITIONS "USE_TRACER;PYHROL_SAFE_MODE"
    COMPILE_FLAGS " \\
      -I${TMP_INCLUDE} \\
      -I${CMAKE_CURRENT_LIST_DIR}/../include"
    OUTPUT_NAME pyselfsafe
  )
  target_link_libraries (pyselfsafe pyhrolsafe pyselfdev -Wl,-whole-archive starter_pyhrol -Wl,-no-whole-archive)

  if (PYHROL_SAFE_MODE)
    set (PYHROL_SUFFIX safe)
  endif()
  add_custom_target(link COMMAND sh -c "ln -sf $<TARGET_FILE:pyself${PYHROL_SUFFIX}> pyhrol.so" DEPENDS pyself pyselfsafe)

endif (PYTHONLIBS_FOUND)


function (pyhrol_configure_targets)
  string(REGEX MATCHALL "([^ ]+)" PYHROL_TARGETS "${ARGV}")
  foreach (PYHROL_TARGET ${PYHROL_TARGETS})
    get_target_property (TMP_FLAGS ${PYHROL_TARGET} COMPILE_FLAGS)
    if (NOT TMP_FLAGS)
      unset(TMP_FLAGS)
    endif()

    string(REPLACE ";" " -I" TMP_INCLUDE "${PYTHON_INCLUDE_DIRS}")

    set_target_properties (${PYHROL_TARGET} PROPERTIES
      COMPILE_FLAGS "${TMP_FLAGS} \\
        -I${TMP_INCLUDE} \\
        -I${CMAKE_CURRENT_LIST_DIR}/../include"
    )

    get_target_property (TMP_NAME ${PYHROL_TARGET} PREFIX)
    if (NOT TMP_NAME)
      set_target_properties (${PYHROL_TARGET} PROPERTIES PREFIX "")
    endif()
    get_target_property (TMP_NAME ${PYHROL_TARGET} LOCATION)
    get_filename_component(TMP_NAME ${TMP_NAME} NAME_WE)

    set (PYHROL_STARTER_FILE ${CMAKE_CURRENT_LIST_DIR}/../cpp/pyhrol_starter_${TMP_NAME}.cpp)
    file(WRITE ${PYHROL_STARTER_FILE} "//Generated by ${CMAKE_CURRENT_LIST_FILE}\n\nvoid pyhrol_init(const char *);\n\nextern \"C\" void init${TMP_NAME}()\n{\n  pyhrol_init(\"${TMP_NAME}\");\n}\n")
    add_library (starter_${PYHROL_TARGET} STATIC ${PYHROL_STARTER_FILE})
    set_target_properties (starter_${PYHROL_TARGET} PROPERTIES COMPILE_FLAGS "-fPIC")

    if (PYTHONLIBS_FOUND)
      if (PYHROL_USE_STATIC)
        set (TMP_BEFORE_LIB -Wl,-whole-archive)
        set (TMP_AFTER_LIB -Wl,-no-whole-archive)
      endif ()
      if (PYHROL_SAFE_MODE AND PYHROL_USE_STATIC)
        set (PYHROL_SUFFIX safea)
      elseif (PYHROL_SAFE_MODE)
        set (PYHROL_SUFFIX safe)
      elseif (PYHROL_USE_STATIC)
        set (PYHROL_SUFFIX a)
      endif()
      target_link_libraries (${PYHROL_TARGET} ${TMP_BEFORE_LIB} pyhrol${PYHROL_SUFFIX} ${TMP_AFTER_LIB} ${PYTHON_LIBRARIES} -Wl,-whole-archive starter_${PYHROL_TARGET} -Wl,-no-whole-archive)
      add_dependencies(${PYHROL_TARGET} pygen link)
    endif()
  endforeach()
endfunction (pyhrol_configure_targets)
