# ---------------------------------
# Finds Python
# Adds library to target
# Adds include path
# ---------------------------------

IF(WIN32)
FIND_PATH(PATH_PYTHON include/Python.h PATHS 
		"c:\python27" $ENV{OpenViBE_dependencies}
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.7\\InstallPath]
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.6\\InstallPath]
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.5\\InstallPath]
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.4\\InstallPath]
		[HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.3\\InstallPath])
	IF(PATH_PYTHON)
		MESSAGE(STATUS "  Found Python...")
		INCLUDE_DIRECTORIES(${PATH_PYTHON}/include)
		
		FILE(GLOB_RECURSE lib_files ${PATH_PYTHON}/libs/*.lib)
		
		FOREACH(PYTHON_LIB ${lib_files})
			MESSAGE(STATUS "    [  OK  ] Third party lib ${PYTHON_LIB}")
			TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic debug ${PYTHON_LIB})
			TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic optimized ${PYTHON_LIB})
		ENDFOREACH(PYTHON_LIB)
		
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic Delayimp )
		SET_TARGET_PROPERTIES(${PROJECT_NAME}-dynamic PROPERTIES LINK_FLAGS "/DELAYLOAD:python27.dll")

		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyPython)
	ELSE(PATH_PYTHON)
		MESSAGE(STATUS "  FAILED to find Python")
	ENDIF(PATH_PYTHON)
ENDIF(WIN32)

IF(UNIX)
	INCLUDE("FindPythonLibs")
	IF(PYTHONLIBS_FOUND)
		MESSAGE(STATUS "  Found Python...")
		INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

		FOREACH(PYTHON_LIB ${PYTHON_LIBRARIES})
			SET(PYTHON_LIB1 "PYTHON_LIB1-NOTFOUND")
			FIND_LIBRARY(PYTHON_LIB1 NAMES ${PYTHON_LIB} PATHS NO_DEFAULT_PATH)
			FIND_LIBRARY(PYTHON_LIB1 NAMES ${PYTHON_LIB} PATHS "/")
			IF(PYTHON_LIB1)
				MESSAGE(STATUS "    [  OK  ] Third party lib ${PYTHON_LIB1}")
				TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${PYTHON_LIB1})
			ELSE(PYTHON_LIB1)
				MESSAGE(STATUS "    [FAILED] Third party lib ${PYTHON_LIB}")
			ENDIF(PYTHON_LIB1)
		ENDFOREACH(PYTHON_LIB)

		ADD_DEFINITIONS(-DTARGET_HAS_ThirdPartyPython)
	ELSE(PYTHONLIBS_FOUND)
		MESSAGE(STATUS "  FAILED to find Python")	
	ENDIF(PYTHONLIBS_FOUND)
ENDIF(UNIX)
