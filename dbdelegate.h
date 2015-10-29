/**
 * @file dbdelegate.h
 * @brief Declarations for DbDelegate class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_DBDELEGATE_H_INCLUDE
#define GUARD_DBDELEGATE_H_INCLUDE

#include <dbdelegate/dbdelegate-config.h>

#include <QStyledItemDelegate>

//! Provides display and editing facilities for data items from a model
class DBDELEGATE_EXPORT DbDelegate : public QStyledItemDelegate {

public:

    //! Default constructor.
    DbDelegate ();

    //! Destructor.
    virtual ~DbDelegate();

protected:

private:

};

#endif // GUARD_DBDELEGATE_H_INCLUDE
