#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = nullptr);
    ~ProgressDialog();

    void setMaximum(int value);
public slots:
    void setProgress(int value);
    void setInsertion(bool value);

private slots:
    void on_progressBar_valueChanged(int value);

private:
    Ui::ProgressDialog *ui;
    bool m_Insertion;
};

#endif // PROGRESSDIALOG_H
