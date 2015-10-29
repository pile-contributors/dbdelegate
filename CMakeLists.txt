if (NOT DBDELEGATE_BUILD_MODE)
    set (DBDELEGATE_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (DbDelegate)
DbDelegateInit(${DBDELEGATE_BUILD_MODE})
