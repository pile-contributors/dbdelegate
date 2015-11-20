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
#include <dbstruct/dbcolumn.h>

#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QTableView;
class QAbstractItemModel;
QT_END_NAMESPACE

class DbTaew;

//! Provides display and editing facilities for data items from a model.
class DBDELEGATE_EXPORT DbDelegate : public QStyledItemDelegate {
    Q_OBJECT

    DbColumn column_;
public:

    //! Sets all delegates for a table.
    static bool
    setAllDelegates (
            QTableView * view);

    //! Sets all delegates for a table.
    static bool
    setAllDelegates (
            DbTaew * table,
            QTableView * view);

    //! Resets all delegates for a table.
    static bool
    resetAllDelegates (
            QTableView * view);


public:

    //! Default constructor.
    DbDelegate (
            QObject *parent = NULL);

    //! Destructor.
    virtual ~DbDelegate();

    //! Painting an item.
    virtual void
    paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;

    //! The size needed by the delegate to display the item.
    virtual QSize
    sizeHint(
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;

    //! Create a widget for editing a gived value.
    virtual QWidget *
    createEditor (
            QWidget *parent,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;

    //! Sets the data to be displayed and edited from the model.
    virtual void
    setEditorData (
            QWidget *editor,
            const QModelIndex &index) const;

    //! Gets data from the editor and stores it in the model.
    virtual void
    setModelData (
            QWidget *editor,
            QAbstractItemModel *model,
            const QModelIndex &index) const;

    //! Updates the editor for the item according to the style.
    virtual void
    updateEditorGeometry (
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;

    //! Last chance to change displayed text before actual paint.
    virtual QString
    displayText (
            const QVariant & value,
            const QLocale &locale) const;

protected:

    //! Initialize style options for an item.
    virtual void
    initStyleOption (
            QStyleOptionViewItem *option,
            const QModelIndex &index) const;

    //! Filter out some events.
    virtual bool
    eventFilter (
            QObject *object,
            QEvent *event);

    //! Start of editing and mouse events.
    virtual bool
    editorEvent (
            QEvent *event,
            QAbstractItemModel *model,
            const QStyleOptionViewItem &option,
            const QModelIndex &index);

private:


};

#endif // GUARD_DBDELEGATE_H_INCLUDE
