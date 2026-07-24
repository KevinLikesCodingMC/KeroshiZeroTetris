file(GLOB_RECURSE TEST_SOURCES "*.cpp")

foreach (TEST_SRC ${TEST_SOURCES})
	get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
	set(TARGET_NAME "test_${TEST_NAME}")
	add_executable(${TARGET_NAME} ${TEST_SRC})
endforeach ()
