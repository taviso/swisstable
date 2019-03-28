# Required as the first line
cmake_minimum_required(VERSION 3.5)

# Define the current project
project(swisstable VERSION 0.1)

# Tell Abseil which C++ standard to use
set(CMAKE_CXX_STANDARD 17)

# Process abseil with CMake
add_subdirectory(abseil-cpp)

find_package(Threads REQUIRED)

set(DEPENDENT_LIBS
   "abseil-cpp/absl/hash/libabsl_absl_hash.a"
   "abseil-cpp/absl/types/libabsl_absl_bad_variant_access.a"
   "abseil-cpp/absl/hash/libabsl_absl_city.a"
   "abseil-cpp/absl/container/libabsl_absl_raw_hash_set.a"
   "abseil-cpp/absl/types/libabsl_absl_bad_optional_access.a"
   "abseil-cpp/absl/container/libabsl_absl_hashtablez_sampler.a"
   "abseil-cpp/absl/synchronization/libabsl_absl_synchronization.a"
   "abseil-cpp/absl/synchronization/libabsl_absl_graphcycles_internal.a"
   "abseil-cpp/absl/debugging/libabsl_absl_stacktrace.a"
   "abseil-cpp/absl/debugging/libabsl_absl_symbolize.a"
   "abseil-cpp/absl/base/libabsl_absl_malloc_internal.a"
   "abseil-cpp/absl/debugging/libabsl_absl_debugging_internal.a"
   "abseil-cpp/absl/debugging/libabsl_absl_demangle_internal.a"
   "abseil-cpp/absl/time/libabsl_absl_time.a"
   "abseil-cpp/absl/strings/libabsl_absl_strings.a"
   "abseil-cpp/absl/strings/libabsl_absl_strings_internal.a"
   "abseil-cpp/absl/base/libabsl_absl_throw_delegate.a"
   "abseil-cpp/absl/base/libabsl_absl_base.a"
   "abseil-cpp/absl/base/libabsl_absl_dynamic_annotations.a"
   "abseil-cpp/absl/base/libabsl_absl_spinlock_wait.a"
   "abseil-cpp/absl/numeric/libabsl_absl_int128.a"
   "abseil-cpp/absl/time/libabsl_absl_civil_time.a"
   "abseil-cpp/absl/time/libabsl_absl_time_zone.a"
)

# I'm an old fashioned GNU Make guy, I'm trying my best, but as far as I can
# tell there is literally no way to get a list of objects from an INTERFACE
# library target. What a joke.
#
# I'm just hacking it like this with a hardcoded list. Obviously this is going
# to break at some point, but what am I supposed to do?
add_custom_command(OUTPUT libswisstable.a
    COMMAND
        rm -f libswisstable.mri\;
        printf "create libswisstable.a\\\\n"                    >> libswisstable.mri\;
        printf "addlib %s\\\\n" $<TARGET_FILE:cwrappers>        >> libswisstable.mri\;
        printf "addlib %s\\\\n" $<TARGET_FILE:absl::strings>    >> libswisstable.mri\;
        printf "addlib %s\\\\n" ${DEPENDENT_LIBS}               >> libswisstable.mri\;
        printf "save\\\\nend\\\\n"                              >> libswisstable.mri\;
        ar -M < libswisstable.mri\;
        rm -f libswisstable.mri\;
    COMMENT "Creating libswisstable.a"
)


add_library(cwrappers STATIC swisstable.cc)
add_custom_target(swisstable ALL DEPENDS cwrappers libswisstable.a)
target_link_libraries(cwrappers absl::hash absl::strings absl::flat_hash_map Threads::Threads)
configure_file(swisstable.h swisstable.h COPYONLY)
