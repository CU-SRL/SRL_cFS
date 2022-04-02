cmake_minimum_required(VERSION 2.6.4)
project(CFS_SCH_LAB C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)
include_directories(${ci_lab_MISSION_DIR}/fsw/platform_inc)
include_directories(${to_lab_MISSION_DIR}/fsw/platform_inc)
include_directories(${hs_MISSION_DIR}/fsw/platform_inc)
include_directories(${fm_MISSION_DIR}/fsw/platform_inc)
include_directories(${sc_MISSION_DIR}/fsw/platform_inc)
include_directories(${ds_MISSION_DIR}/fsw/platform_inc)
include_directories(${cs_MISSION_DIR}/fsw/platform_inc)
include_directories(${hk_MISSION_DIR}/fsw/platform_inc)
include_directories(${aimu_lis3mdl_MISSION_DIR}/fsw/platform_inc)
include_directories(${aimu_lps25h_v2_MISSION_DIR}/fsw/platform_inc)
include_directories(${aimu_lsm6ds33_MISSION_DIR}/fsw/platform_inc)
include_directories(${max7502_MISSION_DIR}/fsw/platform_inc)
include_directories(${ms5611_MISSION_DIR}/fsw/platform_inc)
include_directories(${h3lis100dl_MISSION_DIR}/fsw/platform_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module
add_cfe_app(sch_lab ${APP_SRC_FILES})
