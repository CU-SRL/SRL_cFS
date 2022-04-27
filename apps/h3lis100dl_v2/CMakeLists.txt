cmake_minimum_required(VERSION 2.6.4)
project(CFE_H3LIS100DL_V2 C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module

# line commented our here for testing aimu_lsm6ds33_v2
# add_cfe_app(CFE_H3LIS100DL_V2 ${APP_SRC_FILES})
