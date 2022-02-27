cmake_minimum_required(VERSION 3.16)
project(massspectro VERSION 0.1.0) 

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
        
# Print function
function(print)
  foreach(var ${ARGN})
    message(STATUS "${var} = ${${var}}")
  endforeach()
endfunction()

# Print ENV function
function(print_env)
  foreach(var ${ARGN})
    message(STATUS "${var} = $ENV{${var}}")
  endforeach()
endfunction()

# Output directory for Nodeset Compiler
set(GENERATE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/src_generated/")
print(GENERATE_OUTPUT_DIR)

# open62541 must be installed.
# If in custom path, then use -DCMAKE_PREFIX_PATH=/home/user/install

if($ENV{ARM_COMPILATION})
        message(STATUS "ARM_COMPIL")

        #Include folder for ARM 
        set(mInclude $ENV{HOME}/install_arm/include)
        set(mLib $ENV{HOME}/install_arm/lib)
        set(mLibArm $ENV{HOME}/rpi/rootfs/usr/lib/arm-linux-gnueabihf)
        set(mIncludeJson $ENV{HOME}/rpi/rootfs/usr/local/include)
        set(mLibJson $ENV{HOME}/rpi/rootfs/usr/local/lib)

        #directories where header are located (resolve location error in VSCode)
        include_directories(${mInclude} ${mIncludeJson})

        print(CMAKE_LIBRARY_PATH)
        print(CMAKE_FIND_ROOT_PATH)
        print(CMAKE_PREFIX_PATH)
        print(CMAKE_MODULE_PATH)

        #Define libraries for linking
        find_library(LIBS_OPC NAMES open62541)
        find_library(LIBS_MBEDTLS NAMES mbedtls)
        find_library(LIBS_MBEDx NAMES mbedx509)
        find_library(LIBS_MBEDCRYPTO NAMES mbedcrypto)
        find_library(LIBS_PTHREAD NAMES pthread)
        find_library(LIBS_JSON NAMES json-c)

        #[[
        #OPCUA precompiled library
        set(LIBOPCUA ${mLib}/libopen62541.a)
        #OPC UA namespace
        set(UA_NAMESPACE_ZERO Full)
        ]]
        
        #json precompiled library
        set(LIBjson ${mLibJson}/libjson-c.a)

        #ARM precompiled library shared ones
        add_library(LIBmbedTls SHARED IMPORTED)
        set_target_properties(LIBmbedTls PROPERTIES IMPORTED_LOCATION ${mLibArm}/libmbedtls.so)
        add_library(LIBmbedx509 SHARED IMPORTED)
        set_target_properties(LIBmbedx509 PROPERTIES IMPORTED_LOCATION ${mLibArm}/libmbedx509.so)
        add_library(LIBmbedcrypto SHARED IMPORTED)
        set_target_properties(LIBmbedcrypto PROPERTIES IMPORTED_LOCATION ${mLibArm}/libmbedxcrypto.so)
        #add_library(LIBjson SHARED IMPORTED)
        #set_target_properties(LIBjson PROPERTIES IMPORTED_LOCATION ${mLib}/libmbedxcrypto.so)
        #[[
        add_library(LIBpthread STATIC IMPORTED)
        set_target_properties(LIBpthread PROPERTIES IMPORTED_LOCATION ${mLibArm}/libpthread.so)
        ]]
        #list(APPEND CMAKE_PREFIX_PATH "/home/philippe/install_arm/lib/cmake/open62541")
        find_package(open62541 1.1 REQUIRED COMPONENTS FullNamespace)       
        #generate nodeset and datatype from project NodeSet2.xml file
        ua_generate_nodeset_and_datatypes(
        NAME "massspectro"
        TARGET_PREFIX ${PROJECT_NAME}
        FILE_CSV "${PROJECT_SOURCE_DIR}/model/published/coater/ModelDesign.csv"
        FILE_BSD "${PROJECT_SOURCE_DIR}/model/published/coater/Coater.Types.bsd"
        OUTPUT_DIR ${GENERATE_OUTPUT_DIR}
        # This namespace index must match the order in which you are adding the nodeset in the source code
        NAMESPACE_MAP "1:http://opcfoundation.org/Coater"
        FILE_NS "${PROJECT_SOURCE_DIR}/model/published/coater/Coater.NodeSet2.xml"
        INTERNAL
        )
else()
        message(STATUS "X86_COMPIL")
        list(APPEND CMAKE_PREFIX_PATH "/home/user/install" "/home/philippe/install/lib/cmake/open62541")
        find_package(open62541 1.1 REQUIRED COMPONENTS FullNamespace)       
        #generate nodeset and datatype from project NodeSet2.xml file
        ua_generate_nodeset_and_datatypes(
        NAME "massspectro"
        TARGET_PREFIX ${PROJECT_NAME}
        FILE_CSV "${PROJECT_SOURCE_DIR}/model/published/coater/ModelDesign.csv"
        FILE_BSD "${PROJECT_SOURCE_DIR}/model/published/coater/Coater.Types.bsd"
        OUTPUT_DIR ${GENERATE_OUTPUT_DIR}
        # This namespace index must match the order in which you are adding the nodeset in the source code
        NAMESPACE_MAP "1:http://opcfoundation.org/Coater"
        FILE_NS "${PROJECT_SOURCE_DIR}/model/published/coater/Coater.NodeSet2.xml"
        INTERNAL
        )
endif()

file(MAKE_DIRECTORY ${GENERATE_OUTPUT_DIR})
include_directories(${GENERATE_OUTPUT_DIR})
#include_directories("${GENERATE_OUTPUT_DIR}/open62541")

message(STATUS ${PROJECT_SOURCE_DIR})
message(STATUS ${GENERATE_OUTPUT_DIR})
message(STATUS ${UA_NODESET_MASSSPECTRO_SOURCES})

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
add_compile_options(
  -Wall
  -Wextra
  -Wconversion
  -Wsign-conversion
  -g3
  -Og
)
#add_compile_definitions(DEBUG)

#list header filess
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c)
#list source files
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)

# Previous macro automatically sets some variables which hold the generated source code files using the provided NAME
add_executable(${PROJECT_NAME}
               ${UA_NODESET_MASSSPECTRO_SOURCES}
               ${UA_TYPES_MASSSPECTRO_SOURCES}
               ${SOURCE_FILES}
               ${HEADER_FILES}
               )

# Make sure the nodeset compiler is execute before compiling the main file
add_dependencies(
        massspectro
        #open62541-generator-ns-MassSpectro
        ${PROJECT_NAME}-ns-massspectro
        )

#link libraries to project
if (${CMAKE_ARM_COMPILATION})
        target_link_libraries(
                ${PROJECT_NAME} 
                ${LIBOPCUA} 
                ${LIBmbedTls}
                ${LIBmbedx509}
                ${LIBmbedcrypto}
                ${LIBS_PTHREAD}
                ${LIBjson}
                )
        #directories of libraries
        target_include_directories(${PROJECT_NAME} PRIVATE ${mLib})
        target_include_directories(${PROJECT_NAME} PRIVATE ${mLibArm})
else()
        #directories of libraries
        target_link_libraries(massspectro open62541::open62541 json-c pthread)
endif()


# Compiler options
# target_compile_options(massspectro PUBLIC -fsanitize=address)
