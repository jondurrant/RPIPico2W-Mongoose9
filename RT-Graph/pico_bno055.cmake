if (DEFINED PICO_BNO055_DIR)
	message("Using Given PICO_BNO055_DIR '${PICO_BNO055_DIR}')")
else ()
	set(PICO_BNO055_DIR "${CMAKE_CURRENT_LIST_DIR}lib/pico-bno055/")
    message("Using local PICO_BNO055_DIR '${PICO_BNO055_DIR}')")
endif ()

add_library(pico_bno055 STATIC)
target_sources(pico_bno055 PUBLIC
  ${PICO_BNO055_DIR}/lib/BNO055_driver/bno055_support.c
  ${PICO_BNO055_DIR}/lib/BNO055_driver/bno055.c
)

# Add include directory
target_include_directories(pico_bno055 PUBLIC 
    ${PICO_BNO055_DIR}/lib/BNO055_driver
)


target_link_libraries(pico_bno055 PUBLIC 
    pico_stdlib 
    hardware_i2c 
    )
    
target_compile_definitions(pico_bno055 PUBLIC
    RASPBERRYPI_PICO=1
    )