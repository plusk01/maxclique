set(PYPKG_DIR "${CMAKE_CURRENT_BINARY_DIR}/maxclique")

pybind11_add_module(py_maxclique py_maxclique.cpp)
target_link_libraries(py_maxclique PUBLIC maxclique)
set_target_properties(py_maxclique PROPERTIES OUTPUT_NAME "maxclique")
set_target_properties(py_maxclique PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PYPKG_DIR}")

# copy setup.py file binary dir for install with: pip install .
configure_file(setup.py.in ${CMAKE_CURRENT_BINARY_DIR}/setup.py)

# Create the Python package -- Note that "." is used to conform to PEP 328
file(WRITE "${PYPKG_DIR}/__init__.py"
    "from .maxclique import *\n"
    "from .maxclique import __version__\n"
    "from .maxclique import __doc__")

set(DIST "none")
if(UNIX AND NOT APPLE)
    execute_process(COMMAND bash -c "lsb_release -cs" OUTPUT_VARIABLE UBUNTU_DIST)
    string(STRIP "${UBUNTU_DIST}" UBUNTU_DIST)
    set(DIST "${UBUNTU_DIST}")
elseif(APPLE)
    # TODO: not very specific...
    set(DIST "macos")
elseif(WIN32)
    # TODO: not very specific...
    set(DIST "win10")
endif()

set(PKGSTR maxclique-py3-${DIST}-${PROJECT_VERSION})
add_custom_target(pypkg
    DEPENDS py_maxclique
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PKGSTR}
    COMMAND ${CMAKE_COMMAND} -E copy setup.py ${PKGSTR}/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PYPKG_DIR} ${PKGSTR}/maxclique
    COMMAND ${CMAKE_COMMAND} -E tar zcvf ${PKGSTR}.tar.gz ${PKGSTR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(pip-install
    DEPENDS pypkg
    COMMAND ${PYTHON_EXECUTABLE} -m pip install .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PKGSTR}
)