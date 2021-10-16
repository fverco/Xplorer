#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

/*!
 * \brief The about dialog
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private slots:
    void on_btnClose_clicked();

private:
    Ui::AboutDialog *ui;    ///< The object containing all the UI elements.
};

#endif // ABOUTDIALOG_H
