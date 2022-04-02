cmake_minimum_required(VERSION 2.6.4)
project(CFS_DS C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)
include_directories(${cfs_lib_MISSION_DIR}/fsw/public_inc)
include_directories(${aimu_lis3mdl_MISSION_DIR}/fsw/platform_inc)
include_directories(${aimu_lps25h_v2_MISSION_DIR}/fsw/platform_inc)
include_directories(${aimu_lsm6ds33_MISSION_DIR}/fsw/platform_inc)
include_directories(${max7502_MISSION_DIR}/fsw/platform_inc)
include_directories(${ms5611_MISSION_DIR}/fsw/platform_inc)
include_directories(${h3lis100dl_MISSION_DIR}/fsw/platform_inc)
include_directories(fsw/src)

aux_source_directory(fsw/src APP_SRC_FILES)
aux_source_directory(fsw/tables APP_TABLE_FILES)

# Create the app module
add_cfe_app(ds ${APP_SRC_FILES})
add_cfe_tables(ds ${APP_TABLE_FILES})
