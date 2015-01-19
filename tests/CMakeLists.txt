include_directories (${SAFE_EVENT_INC})

link_directories (${SAFE_EVENT_LIB})

project (event_tests CXX)

file (GLOB headers "*.h")
file (GLOB sources "*.cpp")

set (DIRS "src")

foreach (dir ${DIRS})
   file (GLOB h_${dir} "${dir}/*.h")
   file (GLOB s_${dir} "${dir}/*.cpp")
   source_group (${dir} FILES ${s_${dir}} ${h_${dir}})
   set (sources ${sources} ${s_${dir}})
   set (header ${headers} ${h_${dir}})
endforeach ()

add_executable (event_tests ${sources})

target_link_libraries (event_tests safe_event)
