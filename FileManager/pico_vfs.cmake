# Add library cpp files

if (NOT DEFINED PICO_VFS_PATH)
    set(PICO_VFS_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/pico-vfs")
endif()

message("Using Pico VFS from ${PICO_VFS_PATH}")

# Blockdevice header library
add_library(blockdevice INTERFACE)
target_include_directories(blockdevice INTERFACE 
	${PICO_VFS_PATH}/include
)

# SD Card blockdevice library
add_library(blockdevice_sd INTERFACE)
target_sources(blockdevice_sd INTERFACE
  ${PICO_VFS_PATH}/src/blockdevice/sd.c
)
target_link_libraries(blockdevice_sd INTERFACE
  blockdevice
  hardware_spi
  pico_sync
)

# Onboard-Flash blockdevice library
add_library(blockdevice_flash INTERFACE)
target_sources(blockdevice_flash INTERFACE
  ${PICO_VFS_PATH}/src/blockdevice/flash.c
)
target_link_libraries(blockdevice_flash INTERFACE
  blockdevice
  hardware_exception
  hardware_flash
  pico_sync
  pico_flash
)

# Heap memory blockdevice library
add_library(blockdevice_heap INTERFACE)
target_sources(blockdevice_heap INTERFACE
  ${PICO_VFS_PATH}/src/blockdevice/heap.c
)
target_link_libraries(blockdevice_heap INTERFACE
  blockdevice
  pico_sync
)

# Loopback virtual blockdevice library
add_library(blockdevice_loopback INTERFACE)
target_sources(blockdevice_loopback INTERFACE
  ${PICO_VFS_PATH}/src/blockdevice/loopback.c
)
target_link_libraries(blockdevice_loopback INTERFACE
  blockdevice
  pico_sync
)


# Filesystem header library
add_library(filesystem INTERFACE)
target_include_directories(filesystem INTERFACE 
	${PICO_VFS_PATH}/include
)

# FAT filesystem library
add_library(filesystem_fat INTERFACE)
target_sources(filesystem_fat INTERFACE
  ${PICO_VFS_PATH}/src/filesystem/fat.c
  ${PICO_VFS_PATH}/vendor/ff15/source/ff.c
  ${PICO_VFS_PATH}/vendor/ff15/source/ffsystem.c
  ${PICO_VFS_PATH}/vendor/ff15/source/ffunicode.c
)
target_include_directories(filesystem_fat INTERFACE
  ${PICO_VFS_PATH}
  ${PICO_VFS_PATH}/include/filesystem/ChaN
  ${PICO_VFS_PATH}/vendor/ff15/source
)
target_link_libraries(filesystem_fat INTERFACE
  filesystem
  pico_sync
)

# littlefs filesystem library
add_library(filesystem_littlefs INTERFACE)
target_sources(filesystem_littlefs INTERFACE
  ${PICO_VFS_PATH}/src/filesystem/littlefs.c
  ${PICO_VFS_PATH}/vendor/littlefs/lfs.c
  ${PICO_VFS_PATH}/vendor/littlefs/lfs_util.c
)
target_include_directories(filesystem_littlefs INTERFACE 
	${PICO_VFS_PATH}/vendor/littlefs
	)
target_compile_options(filesystem_littlefs INTERFACE -DLFS_NO_DEBUG -Wno-unused-function -Wno-null-dereference)
target_link_libraries(filesystem_littlefs INTERFACE
  filesystem
  pico_sync
)


# VFS interface library
add_library(filesystem_vfs INTERFACE)
target_sources(filesystem_vfs INTERFACE 
	${PICO_VFS_PATH}/src/filesystem/vfs.c
)
target_include_directories(filesystem_vfs INTERFACE
  ${PICO_VFS_PATH}/include
)
target_link_libraries(filesystem_vfs INTERFACE pico_sync)

# Default file system library
add_library(filesystem_default INTERFACE)
target_sources(filesystem_default INTERFACE 
	${PICO_VFS_PATH}/src/filesystem/fs_init.c
)
target_include_directories(filesystem_default INTERFACE
  ${PICO_VFS_PATH}/include
)
target_link_libraries(filesystem_default INTERFACE
  blockdevice_flash
  filesystem_littlefs
  filesystem_vfs
)


