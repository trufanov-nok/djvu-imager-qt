#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    setInsertion(false);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::setMaximum(int value)
{
    ui->progressBar->setMaximum(value);
}

void ProgressDialog::setProgress(int value)
{
    ui->progressBar->setValue(value);
}

void ProgressDialog::setInsertion(bool val) {
    m_Insertion = val;
    ui->lblMsg->setText( val ? tr("Insertion is in progress...") :
                               tr("Processing is in progress..."));
    setWindowTitle(val ? tr("Insertion") : tr("Progress"));
}

void ProgressDialog::on_progressBar_valueChanged(int value)
{
    const int maximum = ui->progressBar->maximum();
    if (value && value >= maximum) {
        ui->buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Ok);
        ui->lblMsg->setText(m_Insertion ? tr("Conversion succesfully ended.") :
                                          tr("Insertion successfully finished."));
    }
    ui->lblProgress->setText(tr("Processed pages: %1 of %2", nullptr, value).arg(value).arg(maximum));
}
