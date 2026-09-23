# Include python

IF (WIN32)
find_package(Python3 COMPONENTS Interpreter Development.Module Development.Embed NumPy REQUIRED)
ELSE (WIN32)
    find_package(Python3 COMPONENTS Interpreter Development.Module NumPy REQUIRED)
ENDIF (WIN32)

IF (Python3_NumPy_VERSION LESS 2.2)
    MESSAGE(FATAL_ERROR "Your version of NumPy is ${Python3_NumPy_VERSION}. You need to upgrade to 2.3 at the very least. Note that NumPy 2.3 ends on 08 Jun 2027.")
ENDIF ()

SET (module temp_gvxrPython3)

IF (USE_SYSTEM_GLEW)
    find_package(GLEW REQUIRED)
    IF (GLEW_USE_STATIC_LIBS)
        SET (requiredLibs GLEW::glew_s ${requiredLibs})
    ELSE (GLEW_USE_STATIC_LIBS)
        SET (requiredLibs GLEW::GLEW ${requiredLibs})
    ENDIF (GLEW_USE_STATIC_LIBS)
ELSE (USE_SYSTEM_GLEW)
    SET (requiredLibs ${GLEW_LIBRARY} ${requiredLibs})
ENDIF (USE_SYSTEM_GLEW)

FIND_PACKAGE(Doxygen)


if(Doxygen_FOUND AND BUILD_SimpleGVXR_DOCUMENTATION)

    SET (SimpleGVXR_DOC_I_FILE 		${CMAKE_CURRENT_BINARY_DIR}/SimpleGVXR-doc.i)

    # Rule to create SimpleGVXR-doc.i a.k.a. ${SimpleGVXR_DOC_I_FILE}
    add_custom_command(
        OUTPUT ${SimpleGVXR_DOC_I_FILE}
        COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/gvxrPython3/doxy2swig/doxy2swig.py ${SimpleGVXR_Doxy_XML}/index.xml ${SimpleGVXR_DOC_I_FILE}
        DEPENDS SGVXR_Documentation
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        COMMENT "Create ${SimpleGVXR_DOC_I_FILE}"
        VERBATIM
    )
    add_custom_target(doxy2swig ALL DEPENDS ${SimpleGVXR_DOC_I_FILE})

    message(STATUS "The help about gVXR in Python will be built.")
 endif()




# Create ${module}.i
IF (Doxygen_FOUND AND BUILD_SimpleGVXR_DOCUMENTATION)
    configure_file(../gvxrWrapper-doc.in ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
ELSE ()
    configure_file(../gvxrWrapper-nodoc.in ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)

    IF (NOT Doxygen_FOUND)
        message(WARNING "Doxygen was not found, the help about gVXR in Python will be disabled.")
    ELSE (NOT Doxygen_FOUND)
        IF (NOT BUILD_SimpleGVXR_DOCUMENTATION)
            message(WARNING "Doxygen was found but you disabled build the documentation, the help about gVXR in Python will be disabled.")
        ENDIF (NOT BUILD_SimpleGVXR_DOCUMENTATION)
    ENDIF (NOT Doxygen_FOUND)
ENDIF ()

SET_PROPERTY(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${module}.i PROPERTY CPLUSPLUS ON)

IF (WIN32)
  ADD_DEFINITIONS(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
ENDIF()

# Add swig module
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0")
    SWIG_ADD_MODULE(${module} python ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
ELSE (${CMAKE_VERSION} VERSION_LESS "3.8.0")
    SWIG_ADD_LIBRARY(${module} LANGUAGE python SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${module}.i)
ENDIF (${CMAKE_VERSION} VERSION_LESS "3.8.0")


TARGET_INCLUDE_DIRECTORIES(${module} PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

set_property(
    SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${module}.i
    PROPERTY SWIG_MODULE_NAME ${module})

#find_package(CUDAToolkit)
#IF (CUDAToolkit_FOUND)
#    SET (optionalLibs CUDA::cudart_static CUDA::cudart ${requiredLibs} ${optionalLibs})
#ENDIF (CUDAToolkit_FOUND)

IF (APPLE)
    #TARGET_LINK_LIBRARIES(${module} Python3::Python ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${requiredLibs} ${optionalLibs})
    TARGET_LINK_LIBRARIES(${module} ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${requiredLibs} ${optionalLibs} Python3::NumPy)
ELSE (APPLE)
    IF (WIN32)
        TARGET_LINK_LIBRARIES(${module} Python3::Python ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${requiredLibs} ${optionalLibs} Python3::NumPy)
    ELSE (WIN32)
        TARGET_LINK_LIBRARIES(${module} ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${requiredLibs} ${optionalLibs} Python3::NumPy)
    ENDIF (WIN32)
ENDIF (APPLE)

target_include_directories (${module} PUBLIC ${PYTHON_INCLUDE_DIRS})

IF (USE_SUPERBUILD)

    ADD_DEPENDENCIES( ${module} SimpleGVXR )

    IF (Doxygen_FOUND AND BUILD_SimpleGVXR_DOCUMENTATION)
        ADD_DEPENDENCIES(${module} doxy2swig)
    ENDIF ()

ENDIF ()

configure_file(test.py.in ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/test.py)
FILE(COPY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/test.py #${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
		FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
  			GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

FILE(COPY ../welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

FILE(COPY computeLBuffer.py
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
		FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ 
  			GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

FILE (GLOB TEST_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.py")
FILE (GLOB TWIN_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/twins/*.py")
FILE (GLOB TWIN_MODELS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/twins/models/*.json5")

#FILE(COPY ${TWIN_SRC}
#		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/twins
#		FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ 
#  			GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

#FILE(COPY ${TWIN_MODELS}
#		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/twins/models
#		FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ 
#  			GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

#FILE(COPY ${TEST_SRC}
#		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests
#		FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ 
#  			GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# Files to install with Python
set(PYTHON_INSTALL_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${module}.py
        #${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
        ${CMAKE_CURRENT_SOURCE_DIR}/../welsh-dragon-small.stl
)

IF (WIN32 AND MSVC)

    FILE(COPY ${XRAYLIB_SHARED_LIB_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    set(PYTHON_INSTALL_FILES
        ${PYTHON_INSTALL_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}/Release/_${module}.pyd
        ${XRAYLIB_SHARED_LIB_PATH}
    )

ELSE (WIN32 AND MSVC)
 	set(PYTHON_INSTALL_FILES
		${PYTHON_INSTALL_FILES}
		${CMAKE_CURRENT_BINARY_DIR}/_${module}.so)

ENDIF (WIN32 AND MSVC)




#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/
#    DESTINATION .
#    FILE_PERMISSIONS      OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
#    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
#    COMPONENT Python
#    FILES_MATCHING PATTERN "*.py"
#)



install(FILES
    ${TEST_SRC}
    DESTINATION "tests"
    PERMISSIONS      OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    COMPONENT Python)

install(FILES
    ${TWIN_SRC}
    DESTINATION "twins"
    PERMISSIONS      OWNER_READ OWNER_WRITE  GROUP_READ WORLD_READ 
    COMPONENT Python)

install(FILES
    ${TWIN_MODELS}
    DESTINATION "twins/models"
    PERMISSIONS      OWNER_READ OWNER_WRITE  GROUP_READ WORLD_READ 
    COMPONENT Python)

install(FILES
    ${PYTHON_INSTALL_FILES}
    DESTINATION "_gvxrPython3"
    COMPONENT Python)

add_custom_target(install-python
    ${CMAKE_COMMAND}
    -DCMAKE_INSTALL_COMPONENT=Python
    -DCMAKE_BUILD_TYPE=Release
    -P "${PROJECT_BINARY_DIR}/cmake_install.cmake"
    DEPENDS gVirtualXRay SimpleGVXR ${module}
    )

SET (CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL}  Python PARENT_SCOPE)
