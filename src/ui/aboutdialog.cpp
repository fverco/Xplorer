#include "aboutdialog.h"
#include "ui_aboutdialog.h"

/*!
 * \brief The constructor.
 * \param parent = The QWidget to which this dialog is bound.
 */
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

/*!
 * \brief The destructor.
 */
AboutDialog::~AboutDialog()
{
    delete ui;
}

/*!
 * \brief Closes the dialog.
 */
void AboutDialog::on_btnClose_clicked()
{
    this->close();
}

