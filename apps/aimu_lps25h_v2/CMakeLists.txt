cmake_minimum_required(VERSION 2.6.4)
project(CFE_AIMU_LPS25H_V2 C)

include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module
add_cfe_app(aimu_lps25h_v2 ${APP_SRC_FILES})
