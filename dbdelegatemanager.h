/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  file         dbdelegatemanager.h
  date         Oct 2015
  author

  brief        Contains the definition for DbDelegateManager class.

*//*

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef DBDELEGATEMANAGER_H
#define DBDELEGATEMANAGER_H
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <dbdelegate/dbdelegate-config.h>

#include <QIcon>
#include <QColor>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

//! .
class DBDELEGATE_EXPORT DbDelegateManager {
    //
    //
    //
    //
    /*  DEFINITIONS    ----------------------------------------------------- */


    /*  DEFINITIONS    ===================================================== */
    //
    //
    //
    //
    /*  DATA    ------------------------------------------------------------ */

private:

    QIcon crt_icon_marker_; /**< Icon used to indicate current items */
    QColor crt_color_marker_; /**< Background used to indicate current items */
    static DbDelegateManager * uniq_; /**< The one and only instance */

    /*  DATA    ============================================================ */
    //
    //
    //
    //
    /*  FUNCTIONS    ------------------------------------------------------- */

public:

    //! Initialize the manager
    bool
    init ();

    //! Terminate the manager.
    void
    end ();

    //! Retreive icon marker.
    static const QIcon &
    getIcon () const {
        return uniq_->crt_icon_marker_;
    }

    //! Set icon marker.
    static void
    setIcon (const QIcon & value) {
        uniq_->crt_icon_marker_ = value;
    }

    //! Retreive color marker.
    static const QColor &
    getColor () const {
        return uniq_->crt_color_marker_;
    }

    //! Set color marker.
    static void
    setColor (const QColor & value) {
        uniq_->crt_color_marker_ = value;
    }


protected:

    //! Constructor.
    DbDelegateManager();

    //! destructor
    virtual ~DbDelegateManager();

private:

    /*  FUNCTIONS    ======================================================= */
    //
    //
    //
    //
}; /* class DbDelegateManager */

/*  CLASS    =============================================================== */
//
//
//
//



#endif // DBDELEGATEMANAGER_H
/* ------------------------------------------------------------------------- */
/* ========================================================================= */


