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
#include <QSqlTableModel>
#include <QModelIndex>
#include <QComboBox>
#include <QSqlRecord>

/**
 * @class DbDelegate
 *
 * .
 */

/* ------------------------------------------------------------------------- */
DbDelegate::DbDelegate(QObject *parent) :
    QStyledItemDelegate (parent)
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
#include <QSpinBox>
#include <QCheckBox>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QLineEdit>

/* ------------------------------------------------------------------------- */
QWidget *DbDelegate::createEditor (
        QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    DBDELEGATE_TRACE_ENTRY;
    QWidget * result = NULL;
    for (;;) {
        // validate input
        const DbModel * dbmod = qobject_cast<const DbModel *>(index.model ());
        if (dbmod == NULL)
            break;
        if (!dbmod->isValid())
            break;

        // get data about this column
        const DbModelCol & col_data = dbmod->columnData (index.column());
        if (col_data.original_.read_only_) {
            DBDELEGATE_DEBUGM("Read-only column requests editor");
            return NULL;
        }

        if (col_data.isForeign()) {
            // for foreign columns we always present a drop-down
            if (!col_data.table_->isValid())
                return NULL;
            QComboBox *combo = new QComboBox (parent);
            combo->setModel (col_data.table_->model);
            combo->setModelColumn (col_data.t_display_);
            result = combo;
        } else {
            switch (col_data.original_.datatype_) {
            case DbColumn::DTY_IMAGE:
            case DbColumn::DTY_ROWVERSION:
            case DbColumn::DTY_HIERARCHYID:
            case DbColumn::DTY_DECIMALSCALE:
            case DbColumn::DTY_NUMERICSCALE:
            case DbColumn::DTY_DATETIMEOFFSET:
            case DbColumn::DTY_VARBINARY:
            case DbColumn::DTY_BINARY: {
                break; }

            case DbColumn::DTY_BIGINT: {
                QSpinBox * editor = new QSpinBox (parent);
                editor->setMinimum (LONG_MIN);
                editor->setMaximum (LONG_MAX);
                editor->setValue (dbmod->data (index, Qt::EditRole).toLongLong());
                result = editor;
                break; }

            case DbColumn::DTY_UNIQUEIDENTIFIER:
            case DbColumn::DTY_INTEGER: {
                QSpinBox * editor = new QSpinBox (parent);
                editor->setMinimum (INT_MIN);
                editor->setMaximum (INT_MAX);
                editor->setValue (dbmod->data (index, Qt::EditRole).toInt());
                result = editor;
                break; }

            case DbColumn::DTY_SMALLINT: {
                QSpinBox * editor = new QSpinBox (parent);
                editor->setMinimum (SHRT_MIN);
                editor->setMaximum (SHRT_MAX);
                editor->setValue (dbmod->data (index, Qt::EditRole).toInt());
                result = editor;
                break; }

            case DbColumn::DTY_TINYINT: {
                QSpinBox * editor = new QSpinBox (parent);
                editor->setMinimum (SCHAR_MIN);
                editor->setMaximum (SCHAR_MAX);
                editor->setValue (dbmod->data (index, Qt::EditRole).toInt());
                result = editor;
                break; }

            case DbColumn::DTY_BIT: {
                QCheckBox * editor = new QCheckBox (parent);
                editor->setChecked (dbmod->data (index, Qt::EditRole).toBool());
                result = editor;
                break; }

            case DbColumn::DTY_TEXT:
            case DbColumn::DTY_SQL:
            case DbColumn::DTY_XML:
            case DbColumn::DTY_VARCHAR:
            case DbColumn::DTY_NCHAR:
            case DbColumn::DTY_NVARCHAR:
            case DbColumn::DTY_NTEXT:
            case DbColumn::DTY_CHAR: {
                QLineEdit * editor = new QLineEdit (parent);
                editor->setText (dbmod->data (index, Qt::EditRole).toString());
                if (col_data.original_.length_ > 0) {
                    editor->setMaxLength (col_data.original_.length_);
                }
                result = editor;
                break; }

            case DbColumn::DTY_DATE: {
                QDateEdit * editor = new QDateEdit (parent);
                editor->setDate (dbmod->data (index, Qt::EditRole).toDate());
                editor->setCalendarPopup (true);
                // editor->setDisplayFormat ();
                result = editor;
                break; }

            case DbColumn::DTY_TIME: {
                QTimeEdit * editor = new QTimeEdit (parent);
                editor->setTime (dbmod->data (index, Qt::EditRole).toTime ());
                // editor->setDisplayFormat ();
                result = editor;
                break; }

            case DbColumn::DTY_SMALLDATETIME:
            case DbColumn::DTY_DATETIME2:
            case DbColumn::DTY_DATETIME: {
                QDateTimeEdit * editor = new QDateTimeEdit (parent);
                editor->setDateTime (dbmod->data (index, Qt::EditRole).toDateTime());
                editor->setCalendarPopup (true);
                // editor->setDisplayFormat ();
                result = editor;
                break; }

            case DbColumn::DTY_REAL:
            case DbColumn::DTY_NUMERIC:
            case DbColumn::DTY_MONEY:
            case DbColumn::DTY_SMALLMONEY:
            case DbColumn::DTY_DECIMAL:
            case DbColumn::DTY_FLOAT: {
                QDoubleSpinBox * editor = new QDoubleSpinBox (parent);
                editor->setValue (dbmod->data (index, Qt::EditRole).toReal());
                result = editor;
                break; }
            }
        }

        break;
    }

    if (result == NULL) {
        result = QStyledItemDelegate::createEditor (
                    parent, option, index);
    }
    result->installEventFilter (const_cast<DbDelegate *>(this));
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
    bool b_ret = false;
    for (;;) {

        // validate input
        DbModel * dbmod = qobject_cast<DbModel *>(model);
        if (dbmod == NULL)
            break;
        if (!dbmod->isValid())
            break;

        // get data about this column
        const DbModelCol & col_data = dbmod->columnData (index.column());
        if (col_data.original_.read_only_) {
            DBDELEGATE_DEBUGM("Read-only column requests editor");
            return;
        }

        if (col_data.isForeign()) {
            // for foreign columns we always present a drop-down
            if (!col_data.table_->isValid())
                return;
            QComboBox *combo = static_cast<QComboBox *>(editor);
            QSqlRecord rec = col_data.table_->model->record (
                        combo->currentIndex());
//            dbmod->setData (
//                        index, rec.value (col_data.t_display_), Qt::DisplayRole);
            dbmod->setData (
                        index, rec.value (col_data.t_primary_), Qt::EditRole);

            b_ret = true;
        } else {

        }

        // get new key from the related model


        break;
    }

    if (!b_ret) {
        QStyledItemDelegate::setModelData (editor, model, index);
    }

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
            DbDelegate * delg = new DbDelegate();
            /** @todo */

            view->setItemDelegateForColumn (i, delg);
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
