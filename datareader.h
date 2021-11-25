#ifndef DATAREADER_H
#define DATAREADER_H

#include <QDialog>
#include <QTableWidget>
#include <QTreeWidgetItem>
#include <QDate>

namespace Ui {
class DataReader;
}

class DataReader : public QDialog
{
    Q_OBJECT

public:
    explicit DataReader(std::map<unsigned long long,std::pair<std::pair<QString,bool>,std::map<unsigned long long,QDate>>> dates, QWidget *parent = nullptr);
    ~DataReader();
private slots:

    void on_treeWidget_itemClicked(QTreeWidgetItem *item);

private:
    Ui::DataReader *ui;
    void addTreeRoot(QString name, QString description,unsigned long long i,bool ferto);
    void addTreeChild(QTreeWidgetItem *parent,
                      QString name, QString description,unsigned long long i,bool ferto);
    std::map<unsigned long long,std::pair<std::pair<QString,bool>,std::map<unsigned long long,QDate>>> contactdata;
    int size;
};

#endif // DATAREADER_H
