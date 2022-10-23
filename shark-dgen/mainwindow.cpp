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
                int newIndex = ui->fieldTable->item(row-1, COLUMN_NAMES::INDEX)->text().toInt() + ui->fieldTable->item(row-1, COLUMN_NAMES::LENGTH)->text().toInt();
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
    ui->logEdit->clear();
    ui->logEdit->setText(error);
}


QString MainWindow::getOperator(QString str) {
    if(str.contains("=")) {
        return "=";
    }
    if(str.contains(">")) {
        return ">";
    }
    if(str.contains(">=")) {
        return ">=";
    }
    if(str.contains("<")) {
        return "<";
    }
    if(str.contains("<=")) {
        return "<=";
    }
    if(str.contains("~=")) {
        return "~=";
    }
    return "=";
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


        // ENUM FUNCTION SECTION
        for (const auto &enums : enumMap) {
            ui->dissectorEdit->insertPlainText(enumFunctionHeader.arg(enums.first));
            for (const auto& qstr : enums.second) {
                QString splitOp = getOperator(qstr);
                ui->dissectorEdit->insertPlainText(checkEnumValue.arg(qstr.split(splitOp)[1], qstr.split(splitOp)[0]));
            }
            ui->dissectorEdit->insertPlainText(enumNotFoundReturn);
            ui->dissectorEdit->insertPlainText(functionEnd);
            //ui->dissectorEdit->insertPlainText(checkEnumValue.arg(enums.split(" = ")[0], enums.split(" = ")[1]));
        }

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
            bool isAnEnum = false;
            for (const auto &enums : enumMap) {

                if (enums.first == ui->fieldTable->item(i, COLUMN_NAMES::ITEM)->text()) {

                    auto it = protoTranslation.find(ui->fieldTable->item(i, COLUMN_NAMES::TYPE)->text());
                    if(it != protoTranslation.end()) {
                        ui->dissectorEdit->insertPlainText(subtreeAddAppend.arg(ui->fieldTable->item(i, COLUMN_NAMES::ITEM)->text(),
                            ui->fieldTable->item(i, COLUMN_NAMES::INDEX)->text(), ui->fieldTable->item(i, COLUMN_NAMES::LENGTH)->text(),
                               it->second));
                        isAnEnum = true;
                    }

                }
            }
            if(!isAnEnum) {
                ui->dissectorEdit->insertPlainText(subtreeAdd.arg(ui->fieldTable->item(i, COLUMN_NAMES::ITEM)->text(), ui->fieldTable->item(i, COLUMN_NAMES::INDEX)->text(), ui->fieldTable->item(i, COLUMN_NAMES::LENGTH)->text()));
            }

        }

        ui->dissectorEdit->insertPlainText(functionEnd);
        //ui->dissectorEdit->insertPlainText(portBind.arg(ui->portEdit->toPlainText(), ui->nameEdit->toPlainText()));

        for ( const auto& qstr : portList) {
            ui->dissectorEdit->insertPlainText(portBind.arg(qstr, ui->nameEdit->toPlainText()));
        }

        if(!ui->saveAsButton->isEnabled()) ui->saveAsButton->setEnabled(true);
    }
}


void MainWindow::on_saveAsButton_clicked()
{
    if(!ui->dissectorEdit->document()->isEmpty()) {
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
    }
    return;
}


void MainWindow::on_fieldTable_itemSelectionChanged()
{
    QItemSelectionModel *select = ui->fieldTable->selectionModel();
    if(select->hasSelection()) {
        if(!ui->enumStringLine->isEnabled()) ui->enumStringLine->setEnabled(true);
        if(!ui->enumValueLine->isEnabled()) ui->enumValueLine->setEnabled(true);
        if(!ui->enumOperatorBox->isEnabled()) ui->enumOperatorBox->setEnabled(true);
        if(!ui->enumAddButton->isEnabled()) ui->enumAddButton->setEnabled(true);

        ui->enumBox->clear();
        int row = ui->fieldTable->currentRow();
        auto it = enumMap.find(ui->fieldTable->item(row, COLUMN_NAMES::ITEM)->text());
        if (it == enumMap.end()) {
            QStringList newQSL;
            enumMap.try_emplace(ui->fieldTable->item(row, COLUMN_NAMES::ITEM)->text(), newQSL);
        }
        else {
            ui->enumBox->addItems(it->second);
        }
    }

    return;
}


void MainWindow::on_enumAddButton_clicked()
{
    QItemSelectionModel *select = ui->fieldTable->selectionModel();
    if(select->hasSelection()) {
        int row = ui->fieldTable->currentRow();
        auto it = enumMap.find(ui->fieldTable->item(row, COLUMN_NAMES::ITEM)->text());
        if (it != enumMap.end()) {
            QString enumStr = ui->enumStringLine->toPlainText() + ui->enumOperatorBox->currentText() + ui->enumValueLine->toPlainText();
            bool duplicate = false;
            for (const auto& qstr : it->second) {
                if(enumStr.split(ui->enumOperatorBox->currentText())[0] == qstr.split(ui->enumOperatorBox->currentText())[0]) {
                    duplicate = true;
                }
            }
            if (!duplicate) {
                it->second << enumStr;
                ui->enumBox->clear();
                ui->enumBox->addItems(it->second);
            }
        }
        if(ui->enumBox->count() > 0) {
            ui->enumBox->setEnabled(true);
            ui->enumDeleteButton->setEnabled(true);
        }
    }
}


void MainWindow::on_enumDeleteButton_clicked()
{
    QItemSelectionModel *select = ui->fieldTable->selectionModel();

    if (select->hasSelection()) {
        int row = ui->fieldTable->currentRow();
        auto it = enumMap.find(ui->fieldTable->item(row, COLUMN_NAMES::ITEM)->text());
        if (it != enumMap.end()) {
            it->second.removeAll(ui->enumBox->currentText());
            ui->enumBox->clear();
            ui->enumBox->addItems(it->second);
            if (ui->enumBox->count() == 0) {
                ui->enumBox->setDisabled(true);
                ui->enumDeleteButton->setDisabled(true);
            }
        }
    }
}


void MainWindow::on_portAddButton_clicked()
{
    if(!ui->portEdit->toPlainText().isEmpty()) {
        if(!portList.contains(ui->portEdit->toPlainText())) {
            portList.append(ui->portEdit->toPlainText());
            ui->portComboBox->clear();
            ui->portComboBox->addItems(portList);
        }
        if(ui->portComboBox->count() > 0) {
            ui->portComboBox->setEnabled(true);
            ui->portDeleteButton->setEnabled(true);
        }
    }
}


void MainWindow::on_portDeleteButton_clicked()
{
    if(ui->portComboBox->count() > 0) {
        portList.removeAll(ui->portComboBox->currentText());
        ui->portComboBox->clear();
        if(portList.count() > 0) {
            ui->portComboBox->addItems(portList);
        }
        else {
            ui->portComboBox->setEnabled(false);
            ui->portDeleteButton->setEnabled(false);
        }
    }
}

