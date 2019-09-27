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
