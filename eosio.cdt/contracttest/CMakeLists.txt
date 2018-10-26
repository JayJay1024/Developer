cmake_minimum_required(VERSION 3.5)
project(contracttest VERSION 1.0.0)

# 这句要加上，要不然下面 add_contract 将出错
find_package(eosio.cdt)

### Only generate the wasm
#add_executable( contracttest.wasm ${CMAKE_CURRENT_SOURCE_DIR}/src/contracttest.cpp )

### Generate the wasm and abi
add_contract(contracttest contracttest ${CMAKE_CURRENT_SOURCE_DIR}/src/contracttest.cpp)
target_include_directories(contracttest.wasm
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(contracttest.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")