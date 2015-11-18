/**
 * @file dbdelegate.cc
 * @brief Definitions for DbDelegate class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "dbdelegate.h"
#include "dbdelegate-private.h"
#include <dbmodel/dbmodel.h>
#include <dbstruct/dbcolumn.h>

#include <QMetaObject>
#include <QTableView>
#include <QAbstractItemModel>

/**
 * @class DbDelegate
 *
 * .
 */

/* ------------------------------------------------------------------------- */
DbDelegate::DbDelegate() :
    QStyledItemDelegate ()
{
    DBDELEGATE_TRACE_ENTRY;

    DBDELEGATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
DbDelegate::~DbDelegate()
{
    DBDELEGATE_TRACE_ENTRY;

    DBDELEGATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void DbDelegate::paint (
        QPainter *painter, const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    DBDELEGATE_TRACE_ENTRY;
    QStyledItemDelegate::paint (painter, option, index);
    DBDELEGATE_TRACE_EXIT;}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QSize DbDelegate::sizeHint (
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    DBDELEGATE_TRACE_ENTRY;
    QSize result = QStyledItemDelegate::sizeHint (
                option, index);

    DBDELEGATE_TRACE_EXIT;
    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QWidget *DbDelegate::createEditor (
        QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    DBDELEGATE_TRACE_ENTRY;
    QWidget * result = QStyledItemDelegate::createEditor (
                parent, option, index);
    DBDELEGATE_TRACE_EXIT;
    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void DbDelegate::setEditorData (
        QWidget *editor, const QModelIndex &index) const
{
    DBDELEGATE_TRACE_ENTRY;
    QStyledItemDelegate::setEditorData (editor, index);
    DBDELEGATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void DbDelegate::setModelData (
        QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const
{
    DBDELEGATE_TRACE_ENTRY;
    QStyledItemDelegate::setModelData (editor, model, index);
    DBDELEGATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void DbDelegate::updateEditorGeometry (
        QWidget *editor, const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    DBDELEGATE_TRACE_ENTRY;
    QStyledItemDelegate::updateEditorGeometry (editor, option, index);
    DBDELEGATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString DbDelegate::displayText (
        const QVariant &value, const QLocale &locale) const
{
    DBDELEGATE_TRACE_ENTRY;

    DBDELEGATE_TRACE_EXIT;
    return QStyledItemDelegate::displayText (value, locale);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void DbDelegate::initStyleOption (
        QStyleOptionViewItem *option, const QModelIndex &index) const
{
    DBDELEGATE_TRACE_ENTRY;
    QStyledItemDelegate::initStyleOption (option, index);
    DBDELEGATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::eventFilter (
        QObject *object, QEvent *event)
{
    DBDELEGATE_TRACE_ENTRY;
    bool b_ret = QStyledItemDelegate::eventFilter (object, event);

    DBDELEGATE_TRACE_EXIT;
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::editorEvent (
        QEvent *event, QAbstractItemModel *model,
        const QStyleOptionViewItem &option, const QModelIndex &index)
{
    DBDELEGATE_TRACE_ENTRY;
    bool b_ret = QStyledItemDelegate::editorEvent (
                event, model, option, index);

    DBDELEGATE_TRACE_EXIT;
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * This overload simply extracts the table or view definition and passes that
 * to base implementation.
 *
 * @param view The target view.
 * @return true if the model is a DbModel and has a valid meta data object.
 */
bool DbDelegate::setAllDelegates (QTableView * view)
{
    DBDELEGATE_TRACE_ENTRY;
    bool b_ret = false;
    for (;;) {
        DbModel * dbmod = qobject_cast<DbModel *>(view->model ());
        if (dbmod == NULL) {
            DBDELEGATE_DEBUGM("Instamce is not a DbModel; is a %s",
                              view->model ()->staticMetaObject.className());
            break;
        }
        b_ret = setAllDelegates (dbmod->metaTaew (), view);
        break;
    }
    DBDELEGATE_TRACE_EXIT;
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setAllDelegates (DbTaew * table, QTableView * view)
{
    DBDELEGATE_TRACE_ENTRY;
    bool b_ret = false;
    for (;;) {
        if (table == NULL) {
            DBDELEGATE_DEBUGM("A table must be provided");
            break;
        }

        int i_max = table->columnCount();
        for (int i = 0; i < i_max; ++i) {
            DbColumn col = table->columnCtor (i);
            if (col.isForeignKey()) {

            } else {

            }




            /** @todo */








        }

        b_ret = true;
        break;
    }
    DBDELEGATE_TRACE_EXIT;
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * The method iterates all columns, gets the delegate and checks if it is
 * a DbDelegate. If so it removes it and deletes the instance.
 *
 * @param view the view where delegates were installed
 * @return true if the model is a DbModel.
 */
bool DbDelegate::resetAllDelegates (QTableView * view)
{
    DBDELEGATE_TRACE_ENTRY;
    bool b_ret = false;
    for (;;) {
        DbModel * dbmod = qobject_cast<DbModel *>(view->model());
        if (dbmod == NULL) {
            DBDELEGATE_DEBUGM("Instamce is not a DbModel; is a %s",
                              view->model()->staticMetaObject.className());
            break;
        }

        int i_max = dbmod->columnCount();
        for (int i = 0; i < i_max; ++i) {
            DbDelegate * delegate = qobject_cast<DbDelegate *>(
                        view->itemDelegateForColumn (i));
            if (delegate != NULL) {
                view->setItemDelegateForColumn (i, NULL);
                delegate->deleteLater ();
            }
        }

        b_ret = true;
        break;
    }
    DBDELEGATE_TRACE_EXIT;
    return b_ret;
}
/* ========================================================================= */
