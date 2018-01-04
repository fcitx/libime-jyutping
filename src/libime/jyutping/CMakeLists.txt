set(LIBIME_JYUTPING_HDRS
    jyutpingdictionary.h
    jyutpingencoder.h
    jyutpingdata.h
    jyutpingcontext.h
    jyutpingdecoder.h
    jyutpingime.h
    jyutpingmatchstate.h
    ${CMAKE_CURRENT_BINARY_DIR}/libimejyutping_export.h
    )

set(LIBIME_JYUTPING_SRCS
    jyutpingencoder.cpp
    jyutpingdata.cpp
    jyutpingdictionary.cpp
    jyutpingcontext.cpp
    jyutpingdecoder.cpp
    jyutpingime.cpp
    jyutpingmatchstate.cpp
    )

add_library(IMEJyutping SHARED ${LIBIME_JYUTPING_SRCS})
set_target_properties(IMEJyutping
                      PROPERTIES VERSION 0.1
                      SOVERSION 0
                      COMPILE_FLAGS "-fvisibility=hidden"
                      LINK_FLAGS "-Wl,--no-undefined"
                      EXPORT_NAME Jyutping
)
target_include_directories(IMEJyutping PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}/LibIME>)

target_link_libraries(IMEJyutping PUBLIC Fcitx5::Utils Boost::boost LibIME::Core)

install(TARGETS IMEJyutping EXPORT LibIMEJyutpingTargets LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(FILES ${LIBIME_JYUTPING_HDRS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/LibIME/libime/jyutping")

add_library(LibIME::Jyutping ALIAS IMEJyutping)

ecm_setup_version(PROJECT
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LibIMEJyutpingConfigVersion.cmake"
                  SOVERSION 0)

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/LibIMEJyutpingConfig.cmake.in"
                              "${CMAKE_CURRENT_BINARY_DIR}/LibIMEJyutpingConfig.cmake"
                              INSTALL_DESTINATION  "${CMAKE_INSTALL_LIBDIR}/cmake/LibIMEJyutping"
)

generate_export_header(IMEJyutping BASE_NAME LibIMEJyutping)

install(EXPORT LibIMEJyutpingTargets DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/LibIMEJyutping" FILE LibIMEJyutpingTargets.cmake NAMESPACE LibIME:: )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/LibIMEJyutpingConfig.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/LibIMEJyutpingConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/LibIMEJyutping"
        COMPONENT Devel )

