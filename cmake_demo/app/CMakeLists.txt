# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable(webcam_capture ${MY_SOURCES})

# Make use of the HAL library
# target_link_libraries(webcam_capture LINK_PRIVATE hal)

# PTHREAD support
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
target_link_libraries(webcam_capture LINK_PRIVATE Threads::Threads)

# Copy executable to final location (change `webcam_capture` to project name as needed)
add_custom_command(TARGET webcam_capture POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:webcam_capture>"
     "~/cmpt433/public/myApps/project/webcam_capture" 
  COMMENT "Copying ARM executable to public NFS directory")

add_custom_command(TARGET webcam_capture POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_SOURCE_DIR}/server"
     "$(HOME)/cmpt433/public/myApps/project/server-files/" 
  COMMENT "Copying server to public NFS directory")

add_custom_command(TARGET webcam_capture POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_SOURCE_DIR}/server2"
     "$(HOME)/cmpt433/public/myApps/project/server-files2/" 
  COMMENT "Copying server to public NFS directory")

  add_custom_command(TARGET webcam_capture POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_SOURCE_DIR}/server3"
     "$(HOME)/cmpt433/public/myApps/project/server-files3/" 
  COMMENT "Copying server to public NFS directory")