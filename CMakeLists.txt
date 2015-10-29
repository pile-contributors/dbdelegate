if (NOT DBDELEGATE_BUILD_MODE)
    set (DBDELEGATE_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (dbdelegate)
dbdelegateInit(${DBDELEGATE_BUILD_MODE})
