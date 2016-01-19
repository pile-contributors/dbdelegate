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
#if DBSTRUCT_MAJOR_VERSION >= 1
#include <dbstruct/dbdatatype.h>
#endif

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
        const DbModel * dbmod = dbModelConst (index);
        if (dbmod == NULL)
            break;
        if (!dbmod->isValid())
            break;

        // get data about this column
        const DbModelCol & col_data = dbmod->columnData (index.column());
        if (col_data.original_.readOnly ()) {
            DBDELEGATE_DEBUGM("Read-only column requests editor\n");
            return NULL;
        }

        QVariant value = getData (index, Qt::EditRole);

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

            switch (col_data.original_.columnType ()) {
            case DbDataType::DTY_IMAGE:
            case DbDataType::DTY_ROWVERSION:
            case DbDataType::DTY_HIERARCHYID:
            case DbDataType::DTY_DECIMALSCALE:
            case DbDataType::DTY_NUMERICSCALE:
            case DbDataType::DTY_DATETIMEOFFSET:
            case DbDataType::DTY_VARBINARY:
            case DbDataType::DTY_BINARY: {
                break; }

            case DbDataType::DTY_BIGINT:
            case DbDataType::DTY_UNIQUEIDENTIFIER:
            case DbDataType::DTY_INTEGER:
            case DbDataType::DTY_SMALLINT:
            case DbDataType::DTY_TINYINT: {
                QSpinBox * editor = new QSpinBox (parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbDataType::DTY_TRISTATE:
            case DbDataType::DTY_BIT: {
                QCheckBox * editor = new QCheckBox (
                            /*dbmod->data (index, Qt::DisplayRole).toString(),*/
                            parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbDataType::DTY_TEXT:
            case DbDataType::DTY_SQL:
            case DbDataType::DTY_XML:
            case DbDataType::DTY_VARCHAR:
            case DbDataType::DTY_NCHAR:
            case DbDataType::DTY_NVARCHAR:
            case DbDataType::DTY_NTEXT:
            case DbDataType::DTY_CHAR: {
                QLineEdit * editor = new QLineEdit (parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbDataType::DTY_DATE: {
                QDateEdit * editor = new QDateEdit (parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbDataType::DTY_TIME: {
                QTimeEdit * editor = new QTimeEdit (parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbDataType::DTY_SMALLDATETIME:
            case DbDataType::DTY_DATETIME2:
            case DbDataType::DTY_DATETIME: {
                QDateTimeEdit * editor = new QDateTimeEdit (parent);
                setupControl (col_data, editor, value);
                result = editor;
                break; }

            case DbDataType::DTY_REAL:
            case DbDataType::DTY_NUMERIC:
            case DbDataType::DTY_MONEY:
            case DbDataType::DTY_SMALLMONEY:
            case DbDataType::DTY_DECIMAL:
            case DbDataType::DTY_FLOAT: {
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
        QWidget * /*editor*/, const QModelIndex &/*index*/) const
{
    DBDELEGATE_TRACE_ENTRY;
    //QStyledItemDelegate::setEditorData (editor, index);
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
        DbModel * dbmod = dbModel (index);
        if (dbmod == NULL)
            break;
        if (!dbmod->isValid())
            break;

        QModelIndex mapped = mapIndex (index);
        // get data about this column
        const DbModelCol & col_data = dbmod->columnData (mapped.column());
        if (col_data.original_.readOnly ()) {
            DBDELEGATE_DEBUGM("Read-only column requests editor\n");
            return;
        }

        if (col_data.original_.columnType () == DbDataType::DTY_TRISTATE) {
            if (!col_data.getTristateValue (
                        mapped, dbmod,
                        qobject_cast<QCheckBox*>(editor))) {
                DBDELEGATE_DEBUGM("Failed to save check-box in tristate\n");
                return;
            }

            b_ret = true;
            break;
        }

        if (col_data.isForeign()) {
            // for foreign columns we always present a drop-down
            QComboBox *combo = static_cast<QComboBox *>(editor);
            if (!col_data.getComboValue (mapped, dbmod, combo))
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
            DbDelegate * delg = new DbDelegate(view);
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
        const DbModelCol & col_data, QComboBox *control,
        const QVariant & value, bool b_delegate_enh)
{
    return col_data.setCombo (control, value, b_delegate_enh);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QSpinBox *control, const QVariant & value)
{
    switch (col_data.original_.columnType ()) {
    case DbDataType::DTY_BIGINT: {
        control->setMinimum (LONG_MIN);
        control->setMaximum (LONG_MAX);
        control->setValue (static_cast<int> (value.toLongLong()));
        break; }

    case DbDataType::DTY_UNIQUEIDENTIFIER:
    case DbDataType::DTY_INTEGER: {
        control->setMinimum (INT_MIN);
        control->setMaximum (INT_MAX);
        control->setValue (value.toInt());
        break; }

    case DbDataType::DTY_SMALLINT: {
        control->setMinimum (SHRT_MIN);
        control->setMaximum (SHRT_MAX);
        control->setValue (value.toInt());
        break; }

    case DbDataType::DTY_TINYINT: {
        control->setMinimum (CHAR_MIN);
        control->setMaximum (CHAR_MAX);
        control->setValue (value.toInt());
        break; }

    default:
        control->setValue (value.toInt());
    }

    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QDoubleSpinBox *control,
        const QVariant & value)
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
        const DbModelCol & col_data, QCheckBox *control,
        const QVariant & value)
{
    if (col_data.original_.columnType () == DbDataType::DTY_TRISTATE) {
        col_data.setTristate (control, value, true);
    } else {
        control->setTristate (false);
        control->setChecked (value.toBool());
    }
    control->setAutoFillBackground (true);
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & col_data, QLineEdit *control,
        const QVariant & value)
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
        const DbModelCol & /*col_data*/, QTextEdit *control,
        const QVariant & value)
{
    control->setPlainText (value.toString());
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & /*col_data*/, QDateEdit *control,
        const QVariant & value)
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
        const DbModelCol & /*col_data*/, QTimeEdit *control,
        const QVariant & value)
{
    control->setTime (value.toTime ());
    // see [here](http://doc.qt.io/qt-5/qdatetime.html#toString)
    control->setDisplayFormat (
                QCoreApplication::translate(
                    "UserTime", "h:mm:ss"));
    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool DbDelegate::setupControl (
        const DbModelCol & /*col_data*/, QDateTimeEdit *control,
        const QVariant & value)
{
    control->setDateTime (value.toDateTime());
    control->setCalendarPopup (true);
    // see [here](http://doc.qt.io/qt-5/qdatetime.html#toString)
    control->setDisplayFormat (
                QCoreApplication::translate(
                    "UserTime", "yyyy-MMM-dd h:mm:ss"));

    return true;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
const DbModel * DbDelegate::dbModelConst (const QModelIndex &index) const
{
    return qobject_cast<const DbModel *>(index.model ());
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
DbModel * DbDelegate::dbModel (const QModelIndex &index) const
{
    const DbModel * fakery = qobject_cast<const DbModel *>(index.model ());
    return const_cast<DbModel*>(fakery);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QVariant DbDelegate::getData (const QModelIndex &index, int role) const
{
    const DbModel * dbmod = dbModel (index);
    return dbmod->data (index, role);
}
/* ========================================================================= */

void DbDelegate::anchorVtable() const {}
