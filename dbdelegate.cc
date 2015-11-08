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
