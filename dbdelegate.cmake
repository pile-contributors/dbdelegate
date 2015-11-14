
# enable/disable cmake debug messages related to this pile
set (DBDELEGATE_DEBUG_MSG ON)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (dbdelegateInit
          ref_cnt_use_mode)

    # default name
    if (NOT DBDELEGATE_INIT_NAME)
        set(DBDELEGATE_INIT_NAME "DbDelegate")
    endif ()

    # compose the list of headers and sources
    set(DBDELEGATE_HEADERS
        "dbdelegate.h")
    set(DBDELEGATE_SOURCES
        "dbdelegate.cc")
    set(DBDELEGATE_QT_MODS
        Core Sql)

    pileSetSources(
        "${DBDELEGATE_INIT_NAME}"
        "${DBDELEGATE_HEADERS}"
        "${DBDELEGATE_SOURCES}")

    pileSetCommon(
        "${DBDELEGATE_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${ref_cnt_use_mode}"
        ""
        "category1"
        "tag1;tag2")

endmacro ()
