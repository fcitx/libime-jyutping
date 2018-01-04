set(JYUTPING_SOURCES
    engine.cpp
)
add_library(jyutping MODULE ${JYUTPING_SOURCES})
target_link_libraries(jyutping Fcitx5::Core Fcitx5::Config Fcitx5::Module::Fullwidth Fcitx5::Module::Spell Fcitx5::Module::Punctuation Fcitx5::Module::QuickPhrase LibIME::Jyutping Boost::iostreams)
set_target_properties(jyutping PROPERTIES PREFIX "")
target_compile_definitions(jyutping
  PRIVATE LIBIME_INSTALL_PKGDATADIR=\"${LIBIME_INSTALL_PKGDATADIR}\"
          FCITX_GETTEXT_DOMAIN=\"fcitx5-jyutping\"
)
install(TARGETS jyutping DESTINATION "${CMAKE_INSTALL_LIBDIR}/fcitx5")
fcitx5_translate_desktop_file(jyutping.conf.in jyutping.conf)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/jyutping.conf" DESTINATION "${CMAKE_INSTALL_DATADIR}/fcitx5/inputmethod")
fcitx5_translate_desktop_file(jyutping-addon.conf.in jyutping-addon.conf)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/jyutping-addon.conf" RENAME jyutping.conf DESTINATION "${FCITX_INSTALL_PKGDATADIR}/addon")

