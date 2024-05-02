if( NOT TARGET Cinder-NDI )
	
	if( NOT DEFINED ENV{NDI_SDK_PATH} )
		message( FATAL_ERROR "The env variable NDI_SDK_PATH is not set!" )
	else()
		set( NDI_PATH "$ENV{NDI_SDK_PATH}" )
		message( "NDI_SDK_PATH : " "${NDI_PATH}" )
	endif()

	get_filename_component( NDI_INCLUDE_PATH "${NDI_PATH}/include" ABSOLUTE )

	get_filename_component( CINDER_NDI_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE )
	get_filename_component( CINDER_NDI_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../include" ABSOLUTE )
	get_filename_component( CINDER_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../.." ABSOLUTE )
	
	add_library( Cinder-NDI "${CINDER_NDI_SOURCE_PATH}/CinderNDIReceiver.cpp"
							"${CINDER_NDI_SOURCE_PATH}/CinderNDISender.cpp"
	)

	target_include_directories( Cinder-NDI PUBLIC "${CINDER_NDI_INCLUDE_PATH}" "${NDI_INCLUDE_PATH}" )
	
	target_compile_options( Cinder-NDI PRIVATE "-std=c++11" )
	
	find_library( NDI_LIBRARY PATHS "${NDI_PATH}/bin/x64" "${NDI_PATH}/lib/x86_64-linux-gnu-5.3" NAMES ndi REQUIRED )
	target_link_libraries( Cinder-NDI PUBLIC "${NDI_LIBRARY}" )

	if( NOT TARGET cinder )
		include( "${CINDER_PATH}/proj/cmake/configure.cmake" )
		find_package( cinder REQUIRED PATHS
			"${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
			"$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}" )
	endif()
	target_link_libraries( Cinder-NDI PRIVATE cinder )
endif()
