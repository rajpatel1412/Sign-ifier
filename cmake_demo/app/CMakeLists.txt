# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable(webcam_capture ${MY_SOURCES})

# Make use of the HAL library
# target_link_libraries(webcam_capture LINK_PRIVATE hal)

# Copy executable to final location (change `webcam_capture` to project name as needed)
add_custom_command(TARGET webcam_capture POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:webcam_capture>"
     "~/cmpt433/public/myApps/project/webcam_capture" 
  COMMENT "Copying ARM executable to public NFS directory")
