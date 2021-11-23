get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${TARGET_NAME} VERSION 21.12)

include(my_boost)
include(my_git)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_executable ( ${PROJECT_NAME} ${SOURCES} ${HEADERS} )
target_link_libraries (${PROJECT_NAME}
			gtcommon
			vx
			gtlan
			ipx
			vxsqlite
			Boost::program_options
)
target_compile_definitions(${PROJECT_NAME} PRIVATE GIT_REVISION="${${PROJECT_NAME}_GIT_REVISION}" )

#install ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install(TARGETS ${PROJECT_NAME} DESTINATION bin)



