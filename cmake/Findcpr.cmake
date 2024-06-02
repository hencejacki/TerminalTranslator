# Findcpr.cmake
#
# This module finds the installed CPR library.
# It sets the following variables:
#  CPR_FOUND - Whether the CPR library was found
#  CPR_INCLUDE_DIRS - The directory containing the CPR headers
#  CPR_LIBRARIES - The CPR library

find_path(CPR_INCLUDE_DIRS "cpr/cpr.h")
find_library(CPR_LIBRARIES NAMES cpr)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cpr DEFAULT_MSG CPR_LIBRARIES CPR_INCLUDE_DIRS)