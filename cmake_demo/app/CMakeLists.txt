# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable(webcap_capture ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(webcap_capture LINK_PRIVATE hal)

# Copy executable to final location (change `webcap_capture` to project name as needed)
add_custom_command(TARGET webcap_capture POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:webcap_capture>"
     "~/cmpt433/public/myApps/project/webcap_capture" 
  COMMENT "Copying ARM executable to public NFS directory")
