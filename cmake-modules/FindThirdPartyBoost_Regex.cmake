# ---------------------------------
# Finds third party boost
# Adds library to target
# Adds include path
# ---------------------------------


IF(UNIX)
	FIND_LIBRARY(LIB_Boost_Regex NAMES "boost_regex-mt" PATHS $ENV{OpenViBE_dependencies}/lib NO_DEFAULT_PATH)
	FIND_LIBRARY(LIB_Boost_Regex NAMES "boost_regex-mt" PATHS $ENV{OpenViBE_dependencies}/lib)
	IF(LIB_Boost_Regex)
		MESSAGE(STATUS "    [  OK  ] lib ${LIB_Boost_Regex}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dynamic ${LIB_Boost_Regex} )
	ELSE(LIB_Boost_Regex)
		MESSAGE(STATUS "    [FAILED] lib boost_regex-mt")
	ENDIF(LIB_Boost_Regex)
ENDIF(UNIX)

