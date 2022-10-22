#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_generateButton_clicked();

    void on_saveAsButton_clicked();

private:
    Ui::MainWindow *ui;

    std::map<QString,int> typeSizeMap {
        {"BOOLEAN", 8},
        {"CHAR", 8},
        {"INT8", 8},
        {"INT16", 16},
        {"INT32", 32},
        {"INT64", 64},
        {"UINT8", 8},
        {"UINT16", 16},
        {"UINT32", 32},
        {"UINT64", 64},
        {"FLOAT", 32},
        {"DOUBLE", 64},
    };

    std::map<QString, QString> protoTranslation {
        {"BOOLEAN", "bool"},
        {"CHAR", "char"},
        {"INT8", "int8"},
        {"INT16", "int16"},
        {"INT32", "int32"},
        {"INT64", "int64"},
        {"UINT8", "uint8"},
        {"UINT16", "uint16"},
        {"UINT32", "uint32"},
        {"UINT64", "uint64"},
        {"DOUBLE", "double"},
        {"FLOAT", "float"}
    };

    enum COLUMN_NAMES {
        ITEM = 0,
        TYPE = 1,
        ENDIANNESS = 2,
        BASE = 3,
        LENGTH = 4,
        INDEX = 5,
    };

    void errorCondition(QString error);

    QString header = "-- this code is autogenerated using shark-dgen.\r\n"
                     "-- save this file to C:\\Users\\[user]\\AppData\\Roaming\\Wireshark\\Plugins or wherever your Wireshark installation is\r\n\r\n";
    QString protocol = "%1_protocol = Proto(\"%2\", \"%1\")\r\n\r\n";
    QString protoField = "%1 = ProtoField.%2(\"%3.%1\", \"%1\", base.%4)\r\n";
    QString fieldsList = "\r\n%1_protocol.fields = {";
    QString func = "\r\nfunction %1_protocol.dissector(buffer, pinfo, tree)\r\n";
    QString lengthCheck = "   length = buffer:len()\r\n   if length == 0 then return end\r\n\r\n";
    QString protoSet = "   pinfo.cols.protocol = %1_protocol.name\r\n\r\n";

    QString subtree = "   local subtree = tree:add(%1_protocol, buffer(), \"%2\")\r\n\r\n";
    QString subtreeAdd = "   subtree:add(%1, buffer(%2, %3))\r\n";
    QString functionEnd = "\r\nend\r\n\r\n";
    QString portBind = "udp_table = DissectorTable.get(\"udp.port\"):add(%1, %2_protocol)\r\n";

    QString emptyNameErr = "ERROR: please enter a name";
    QString acronymLengthErr = "ERROR: please enter an acronym between 2-3 letters";
    QString portErr = "ERROR: please enter a valid port between 1 and 65535";

};
#endif // MAINWINDOW_H