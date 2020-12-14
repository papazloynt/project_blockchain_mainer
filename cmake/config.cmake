macro(make_pic lib)
	hunter_config(
		${lib}
		VERSION ${HUNTER_${lib}_VERSION}
		CMAKE_ARGS CMAKE_POSITION_INDEPENDENT_CODE=TRUE	
	)
endmacro()

foreach(lib ZLIB re2 abseil c-ares)
	make_pic(${lib})
endforeach()