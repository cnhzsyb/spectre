# Distributed under the MIT License.
# See LICENSE.txt for details.

set(LIBRARY "Test_FiniteDifference")

set(LIBRARY_SOURCES
  Test_AoWeno53.cpp
  Test_Minmod.cpp
  Test_MonotisedCentral.cpp
  Test_Unlimited.cpp
  )

add_test_library(
  ${LIBRARY}
  "NumericalAlgorithms/FiniteDifference/"
  "${LIBRARY_SOURCES}"
  ""
  )

target_link_libraries(
  ${LIBRARY}
  PRIVATE
  CoordinateMaps
  DataStructures
  Domain
  FiniteDifference
  ErrorHandling
  Utilities
  )
