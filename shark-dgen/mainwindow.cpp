#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    if(!(ui->fieldNameBox->toPlainText().isEmpty())) {
        bool alreadyInTable = false;
        for (int i = 0; i < ui->fieldTable->rowCount(); i++) {
            if ( ui->fieldTable->item(i, COLUMN_NAMES::ITEM)->text() == ui->fieldNameBox->toPlainText()) {
                alreadyInTable = true;
            }
        }
        if(!alreadyInTable) {
            int row = ui->fieldTable->rowCount();
            ui->fieldTable->insertRow(row);
            ui->fieldTable->setItem(row, COLUMN_NAMES::ITEM, new QTableWidgetItem(ui->fieldNameBox->toPlainText()));
            ui->fieldTable->setItem(row, COLUMN_NAMES::TYPE, new QTableWidgetItem(ui->typeBox->currentText()));
            if(ui->endianCheckbox->isChecked()) {
                ui->fieldTable->setItem(row, COLUMN_NAMES::ENDIANNESS, new QTableWidgetItem(tr("LE")));
            }
            else {
                ui->fieldTable->setItem(row, COLUMN_NAMES::ENDIANNESS, new QTableWidgetItem(tr("BE")));
            }
            ui->fieldTable->setItem(row, COLUMN_NAMES::BASE, new QTableWidgetItem(ui->baseBox->currentText()));
            auto it = typeSizeMap.find(ui->typeBox->currentText());
            ui->fieldTable->setItem(row, COLUMN_NAMES::LENGTH, new QTableWidgetItem(QString::number((it->second)/8)));
            if(row == 0) {
                ui->fieldTable->setItem(row, COLUMN_NAMES::INDEX, new QTableWidgetItem(QString::number(0)));
            }
            else {
                int newIndex = ui->fieldTable->item(row-1, COLUMN_NAMES::INDEX)->text().toInt() + ui->fieldTable->item(row-1, 4)->text().toInt();
                ui->fieldTable->setItem(row, COLUMN_NAMES::INDEX, new QTableWidgetItem(QString::number(newIndex)));

            }

        }
    }
}


void MainWindow::on_deleteButton_clicked()
{
    QItemSelectionModel *select = ui->fieldTable->selectionModel();
    if(select->hasSelection()) {
        int row = ui->fieldTable->currentRow();
        ui->fieldTable->removeRow(row);
    }
}

void MainWindow::errorCondition(QString error) {
    ui->dissectorEdit->clear();
    ui->dissectorEdit->insertPlainText(error);
}


void MainWindow::on_generateButton_clicked()
{
    ui->dissectorEdit->clear();
    if(ui->nameEdit->toPlainText().isEmpty()) {
        errorCondition(emptyNameErr);
        return;
    }
    if((ui->acronymEdit->toPlainText().size() < 2) || (ui->acronymEdit->toPlainText().size() > 3)) {
        errorCondition(acronymLengthErr);
        return;
    }
    int portNumber = ui->portEdit->toPlainText().toInt();
    if((portNumber < 1) || (portNumber > 65535)) {
        errorCondition(portErr);
        return;
    }

    if(ui->fieldTable->rowCount() > 0) {
        ui->dissectorEdit->insertPlainText(header);
        ui->dissectorEdit->insertPlainText(protocol.arg(ui->nameEdit->toPlainText(), ui->acronymEdit->toPlainText()));

        for (int i = 0; i < ui->fieldTable->rowCount(); i++) {
            ui->dissectorEdit->insertPlainText(protoField.arg(ui->fieldTable->item(i, COLUMN_NAMES::ITEM)->text(), ui->fieldTable->item(i, COLUMN_NAMES::TYPE)->text().toLower(), ui->nameEdit->toPlainText(), ui->fieldTable->item(i, COLUMN_NAMES::BASE)->text()));

        }

        ui->dissectorEdit->insertPlainText(fieldsList.arg(ui->nameEdit->toPlainText()));
        for (int i = 0; i < ui->fieldTable->rowCount(); i++) {
            ui->dissectorEdit->insertPlainText(ui->fieldTable->item(i, COLUMN_NAMES::ITEM)->text());
            if(i != (ui->fieldTable->rowCount()-1)) {
                ui->dissectorEdit->insertPlainText(tr(", "));
            }
            else {
                ui->dissectorEdit->insertPlainText(tr("}\r\n"));
            }
        }
        ui->dissectorEdit->insertPlainText(func.arg(ui->nameEdit->toPlainText()));
        ui->dissectorEdit->insertPlainText(lengthCheck);

                ui->dissectorEdit->insertPlainText(protoSet.arg(ui->nameEdit->toPlainText()));

        ui->dissectorEdit->insertPlainText(subtree.arg(ui->nameEdit->toPlainText(), ui->nameEdit->toPlainText()));



        for (int i = 0; i < ui->fieldTable->rowCount(); i++) {
            ui->dissectorEdit->insertPlainText(subtreeAdd.arg(ui->fieldTable->item(i, COLUMN_NAMES::ITEM)->text(), ui->fieldTable->item(i, COLUMN_NAMES::INDEX)->text(), ui->fieldTable->item(i, COLUMN_NAMES::LENGTH)->text()));
        }

        ui->dissectorEdit->insertPlainText(functionEnd);
        ui->dissectorEdit->insertPlainText(portBind.arg(ui->portEdit->toPlainText(), ui->nameEdit->toPlainText()));
    }
}


void MainWindow::on_saveAsButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Dissector"), "",
            tr("dissector (*.lua);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }

     else {
         QFile file(fileName);
         if (!file.open(QIODevice::WriteOnly)) {
             QMessageBox::information(this, tr("Unable to open file"),
                 file.errorString());
             return;
         }

         QTextStream out(&file);
         out << ui->dissectorEdit->toPlainText();

    }
    return;
}

