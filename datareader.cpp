#include "datareader.h"
#include "ui_datareader.h"
#include <QTreeWidgetItem>
#include <QString>
#include <QTreeWidget>
#include <QDebug>
#include <QDate>

DataReader::DataReader(std::map<unsigned long long,std::pair<std::pair<QString,bool>,std::map<unsigned long long,QDate>>> dates, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataReader)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(4);
    contactdata=dates;
    for (std::map<unsigned long long,std::pair<std::pair<QString,bool>,std::map<unsigned long long,QDate>>>::iterator it=contactdata.begin();it!=contactdata.end();it++)
    {
        addTreeRoot(it->second.first.first,QDate::currentDate().toString(),it->first,it->second.first.second);
    }
}

DataReader::~DataReader()
{
    delete ui;
    delete this;
}

void DataReader::addTreeRoot(QString name, QString description,unsigned long long i,bool ferto)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);
    treeItem->setText(0, name);
    treeItem->setText(1, description);
    treeItem->setText(2,"0");
    treeItem->setText(3,QString::number(i));
    int danger=QDate::currentDate().dayOfYear()-QDate::fromString(description).dayOfYear();
    if (danger<0) {
        danger+=365;
    }
    if (danger<=14) {
        treeItem->setBackground(1,QColor((1-(danger/14))*255,danger/14*150,0));
    } else if (danger<=100) {
        treeItem->setBackground(1,QColor(0,200+danger/100*55,0));
    } else {
        treeItem->setBackground(1,QColor(0,255,0));
    }
    if (ferto) {
        treeItem->setBackground(0,QColor(250,150,0));
    }
}

void DataReader::addTreeChild(QTreeWidgetItem *parent,
                  QString name, QString description,unsigned long long i,bool ferto)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, name);
    treeItem->setText(1, description);
    treeItem->setText(2,"0");
    treeItem->setText(3,QString::number(i));
    int danger=QDate::currentDate().dayOfYear()-QDate::fromString(description).dayOfYear();
    if (danger<0) {
        danger+=365;
    }
    if (danger<=14) {
        treeItem->setBackground(1,QColor((1-(danger/14))*255,danger/14*150,0));
    } else if (danger<=100) {
        treeItem->setBackground(1,QColor(0,200+danger/100*55,0));
    } else {
        treeItem->setBackground(1,QColor(0,255,0));
    }
    if (ferto) {
        treeItem->setBackground(0,QColor(250,150,0));
    }
    parent->addChild(treeItem);
}

void DataReader::on_treeWidget_itemClicked(QTreeWidgetItem *item)
{
    if (item->text(2)=="0") {
        auto a=contactdata.find(item->text(3).toInt())->second.second;
        for (std::map<unsigned long long,QDate>::iterator it=a.begin();it!=a.end();it++) {
            auto b=contactdata.find(it->first)->second.first;
            addTreeChild(item,b.first,it->second.toString(),it->first,b.second);
        }
    }
    item->setText(2,"1");
}
