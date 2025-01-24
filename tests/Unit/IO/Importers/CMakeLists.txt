# Distributed under the MIT License.
# See LICENSE.txt for details.

set(LIBRARY "Test_DataImporter")

set(LIBRARY_SOURCES
  Test_Tags.cpp
  Test_VolumeDataReaderActions.cpp
  )

add_test_library(
  ${LIBRARY}
  "IO/Importers"
  "${LIBRARY_SOURCES}"
  "Domain;IO;Importers;Options"
  )

add_dependencies(
  ${LIBRARY}
  module_GlobalCache
  )

function(add_algorithm_test TEST_NAME DIM)
  set(TEST_BASE ${TEST_NAME}${DIM}D)
  set(HPP_NAME Test_${TEST_NAME})
  set(EXECUTABLE_NAME ${HPP_NAME}${DIM}D)
  set(TEST_IDENTIFIER Integration.Importers.${TEST_BASE})

  add_spectre_parallel_executable(
    ${EXECUTABLE_NAME}
    ${HPP_NAME}
    IO/Importers
    Metavariables<${DIM}>
    "DataStructures;Domain;ErrorHandling;Informer;IO;Options;Parallel;SystemUtilities"
    )

  add_standalone_test(
    "${TEST_IDENTIFIER}"
    INPUT_FILE "Test_${TEST_BASE}.yaml")
endfunction()

add_algorithm_test("VolumeDataReaderAlgorithm" 1)
add_algorithm_test("VolumeDataReaderAlgorithm" 2)
add_algorithm_test("VolumeDataReaderAlgorithm" 3)
