cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(firmware)
pico_sdk_init()
add_executable(firmware main.c mongoose/mongoose.c mongoose/mongoose_fs.c mongoose/mongoose_glue.c mongoose/mongoose_impl.c)
target_include_directories(firmware PUBLIC . mongoose)

target_link_libraries(firmware pico_stdlib hardware_spi pico_rand pico_cyw43_driver pico_cyw43_arch_none hardware_flash pico_bootrom)
pico_add_extra_outputs(firmware)     # create map/bin/hex file etc.

pico_enable_stdio_usb(firmware 1)    # Route stdio

# Prevent extreme optimization from breaking OTA
target_compile_options(firmware PRIVATE -fno-tree-loop-distribute-patterns)
