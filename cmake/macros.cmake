# Find subdirectories
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# add sources from each subdirectory
MACRO (add_sources)
    FILE (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    FOREACH (_src ${ARGN})
        IF (_relPath)
            LIST (APPEND SRCS "${_relPath}/${_src}")
        ELSE()
            LIST (APPEND SRCS "${_src}")
        ENDIF()
    ENDFOREACH()
    IF (_relPath)
        SET (SRCS ${SRCS} PARENT_SCOPE) # propagate SRCS to parent directory
    ENDIF()
ENDMACRO()

# add headers from each subdirectory
MACRO (add_includes)
    FILE (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    FOREACH (_hdr ${ARGN})
        IF (_relPath)
            LIST (APPEND HDRS "${_relPath}/${_hdr}")
        ELSE()
            LIST (APPEND HDRS "${_hdr}")
        ENDIF()
    ENDFOREACH()
    IF (_relPath)
        SET (HDRS ${HDRS} PARENT_SCOPE) # propagate SRCS to parent directory
    ENDIF()
ENDMACRO()

