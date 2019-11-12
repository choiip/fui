#compile glsl file to spv file
find_program(GLSL_VALIDATOR glslangValidator)
if (NOT GLSL_VALIDATOR)
  message(ERROR "GLSL_VALIDATOR not found")
else()
  message(STATUS "Found GLSL_VALIDATOR (${GLSL_VALIDATOR})")
endif()

function(TARGET_GLSL2SPIRV TARGET OUTPUT_DIR GLSL_SOURCE_FILES)
  foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${OUTPUT_DIR}/${FILE_NAME}.spv")
    add_custom_command(
      TARGET ${TARGET}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
      COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    )
  endforeach(GLSL)
endfunction(TARGET_GLSL2SPIRV)

function(GLSL2SPIRV2UCHAR OUTFILE GLSL_SOURCE_FILE)
    get_filename_component(FILE_NAME ${GLSL_SOURCE_FILE} NAME)
    set(OUTPUT_DIR ${PROJECT_BINARY_DIR}/spv)
    set(SPIRV "${OUTPUT_DIR}/${FILE_NAME}.spv")
    add_custom_command(
      OUTPUT ${SPIRV}
      DEPENDS ${GLSL_SOURCE_FILE}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
      COMMAND ${GLSL_VALIDATOR} -V ${GLSL_SOURCE_FILE} -o ${SPIRV}
    )
    add_custom_command(OUTPUT ${OUTFILE}
      COMMAND ${CMAKE_COMMAND}
      ARGS -DINFILE=${OUTPUT_DIR}/${FILE_NAME}.spv -DOUTFILE=${OUTFILE} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/File2c.cmake
      DEPENDS ${OUTPUT_DIR}/${FILE_NAME}.spv
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Building uchar - ${OUTFILE}"
      PRE_BUILD VERBATIM)

endfunction(GLSL2SPIRV2UCHAR)
