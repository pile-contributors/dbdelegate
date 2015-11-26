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
#include <QCoreApplication>

#include <QSpinBox>
#include <QCheckBox>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QTextEdit>

/**
 * @class DbDelegate
 *
 * .
 */

static int min_combo_list_w = -1;

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
            DBDELEGATE_DEBUGM("Read-only column requests editor\n");
            return NULL;
        }

        QVariant value = dbmod->data (index, Qt::EditRole);

        if (col_data.isForeign()) {
            // for foreign columns we always present a drop-down
            if (!col_data.table_->isValid())
                return NULL;
            QComboBox *combo = new QComboBox (parent);
            setupControl (col_data, combo, value);
            if (min_combo_list_w == -1) {
                QFontMetrics fm (combo->font());
                min_combo_list_w = fm.width("ComboBox Here");
            }
            combo->view()->setMinimumWidth (min_combo_list_w);
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

            case DbColumn::DTY_BIGINT:
            case DbColumn::DTY_UNIQUEIDENTIFIER:
            case DbColumn::DTY_INTEGER:
            case DbColumn::DTY_SMALLINT:
            case DbColumn::DTY_TINYINT: {
                QSpinBox * editor = new QSpinBox (parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbColumn::DTY_BIT: {
                QCheckBox * editor = new QCheckBox (
                            /*dbmod->data (index, Qt::DisplayRole).toString(),*/
                            parent);
                setupControl (col_data, editor, value);
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
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbColumn::DTY_DATE: {
                QDateEdit * editor = new QDateEdit (parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbColumn::DTY_TIME: {
                QTimeEdit * editor = new QTimeEdit (parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbColumn::DTY_SMALLDATETIME:
            case DbColumn::DTY_DATETIME2:
            case DbColumn::DTY_DATETIME: {
                QDateTimeEdit * editor = new QDateTimeEdit (parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbColumn::DTY_REAL:
            case DbColumn::DTY_NUMERIC:
            case DbColumn::DTY_MONEY:
            case DbColumn::DTY_SMALLMONEY:
            case DbColumn::DTY_DECIMAL:
            case DbColumn::DTY_FLOAT: {
                QDoubleSpinBox * editor = new QDoubleSpinBox (parent);
                setupControl (col_data, editor, value);
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
            DBDELEGATE_DEBUGM("Read-only column requests editor\n");
            return;
        }

        if (col_data.isForeign()) {
            // for foreign columns we always present a drop-down
            QComboBox *combo = static_cast<QComboBox *>(editor);
            if (!col_data.getComboValue (index, dbmod, combo))
                return;

            b_ret = true;
        }

        // default processing
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
            DBDELEGATE_DEBUGM("Instamce is not a DbModel; is a %s\n",
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
            DBDELEGATE_DEBUGM("A table must be provided\n");
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
            DBDELEGATE_DEBUGM("Instamce is not a DbModel; is a %s\n",
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

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QComboBox *control, const QVariant & value)
{
    return col_data.setCombo (control, value);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QSpinBox *control, const QVariant & value)
{
    switch (col_data.original_.datatype_) {
    case DbColumn::DTY_BIGINT: {
        control->setMinimum (LONG_MIN);
        control->setMaximum (LONG_MAX);
        control->setValue (value.toLongLong());
        break; }

    case DbColumn::DTY_UNIQUEIDENTIFIER:
    case DbColumn::DTY_INTEGER: {
        control->setMinimum (INT_MIN);
        control->setMaximum (INT_MAX);
        control->setValue (value.toInt());
        break; }

    case DbColumn::DTY_SMALLINT: {
        control->setMinimum (SHRT_MIN);
        control->setMaximum (SHRT_MAX);
        control->setValue (value.toInt());
        break; }
    }

    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QDoubleSpinBox *control, const QVariant & value)
{
    control->setValue (value.toReal());
    control->setMinimum (-DBL_MAX);
    control->setMaximum (DBL_MAX);
    if (!col_data.original_.original_format_.isEmpty()) {
        control->setDecimals (col_data.original_.precision_);
    }
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QCheckBox *control, const QVariant & value)
{
    control->setChecked (value.toBool());
    control->setAutoFillBackground (true);
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QLineEdit *control, const QVariant & value)
{
    control->setText (value.toString());
    if (col_data.original_.length_ > 0) {
        control->setMaxLength (col_data.original_.length_);
    }
    if (!col_data.original_.original_format_.isEmpty()) {
        control->setInputMask (col_data.original_.original_format_);
    }
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QTextEdit *control, const QVariant & value)
{
    control->setPlainText (value.toString());
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QDateEdit *control, const QVariant & value)
{
    control->setDate (value.toDate());
    control->setCalendarPopup (true);
    // see [here](http://doc.qt.io/qt-5/qdatetime.html#toString)
    control->setDisplayFormat (
                QCoreApplication::translate(
                    "UserTime", "yyyy-MMM-dd"));

    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QTimeEdit *control, const QVariant & value)
{
    control->setTime (value.toTime ());
    // see [here](http://doc.qt.io/qt-5/qdatetime.html#toString)
    control->setDisplayFormat (
                QCoreApplication::translate(
                    "UserTime", "h:m:s"));
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QDateTimeEdit *control, const QVariant & value)
{
    control->setDateTime (value.toDateTime());
    control->setCalendarPopup (true);
    // see [here](http://doc.qt.io/qt-5/qdatetime.html#toString)
    control->setDisplayFormat (
                QCoreApplication::translate(
                    "UserTime", "yyyy-MMM-dd h:m:s"));

    return true;
}
/* ========================================================================= */

