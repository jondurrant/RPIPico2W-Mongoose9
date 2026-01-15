# Add library cpp files

if (NOT DEFINED MONGOOSE_PATH)
    set(MONGOOSE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/mongoose")
endif()
if (NOT DEFINED MONGOOSE_PORT)
    set(MONGOOSE_PORT "${CMAKE_CURRENT_LIST_DIR}/port/mongoose")
endif()

message("Using MONGOOSE from ${MONGOOSE_PATH} with Port folder ${MONGOOSE_PORT}")

file(GLOB_RECURSE SOURCES ${MONGOOSE_PATH}/*.c)

add_library(mongoose STATIC)

target_sources(mongoose PUBLIC
	${SOURCES}
)


# Add include directory
target_include_directories(mongoose PUBLIC 
    ${MONGOOSE_PATH}
    ${MONGOOSE_PORT}
)

target_link_libraries(mongoose PUBLIC 
	pico_stdlib
	hardware_pio 
	hardware_dma 
	pico_rand 
	pico_cyw43_driver 
	pico_cyw43_arch_none
	)

target_compile_definitions(mongoose PRIVATE
    WIZARD_WIFI_NAME=\"$ENV{WIFI_SSID}\"
    WIZARD_WIFI_PASS=\"$ENV{WIFI_PASSWORD}\"
)

