#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include "datareader.h"
#include <QDebug>
#include <map>
#include <QFile>
#include <QMessageBox>
#include <QDate>
#include <QFileDialog>
#include "query.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (!(contactdata.empty())) {
        QMessageBox::StandardButton reply=QMessageBox::warning(this,"Table not empty","All your data will be lost if you continue. Do you want to save them?",QMessageBox::Yes | QMessageBox::Discard);
        if (reply == QMessageBox::Yes){
            on_actionSave_as_triggered();
        }
    }
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    for (int i=0;i<ui->lineEdit->text().count(";")+1;i++) {
        on_pushButton_clicked_valami(ui->lineEdit->text().section(";",i,i));
    }
    ui->lineEdit->setText("");
}

void MainWindow::on_pushButton_clicked_valami(QString text)
{
    alma=true;
    a++;
    ui->tableWidget->setRowCount(a);
    ui->tableWidget->setColumnCount(a);
    ui->tableWidget->setItem(a-1,a-1,new QTableWidgetItem(QDate::currentDate().toString("yyyy.MM.dd")));
    alma=true;
    ui->tableWidget->setItem(0,a-1,new QTableWidgetItem(text));
    alma=true;
    ui->tableWidget->setItem(a-1,0,new QTableWidgetItem(text));
    for (int i=1; i<a-1;i++) {
        alma=true;
        ui->tableWidget->setItem(i,a-1,new QTableWidgetItem("0"));
        alma=true;
        ui->tableWidget->setItem(a-1,i,new QTableWidgetItem("0"));
    }
    std::map<unsigned long long,QDate> z;
    std::pair<std::pair<QString,bool>,std::map<unsigned long long,QDate>> z2;
    b++;
    z.insert({b,QDate::currentDate()});
    z2.first.first=text;
    z2.first.second=false;
    z2.second=z;
    contactdata.insert({b,z2});
}

void MainWindow::on_actionHuman_Readable_format_triggered()
{
    DataReader *olvaso=new DataReader(contactdata);
    olvaso->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    std::vector<std::pair<std::pair<unsigned long long,unsigned long long>,QString>> dates;
    std::pair<unsigned long long,unsigned long long> temp;
    for (int i=0; i<ui->textEdit->toPlainText().count(";")+1; i++) {
        if (ui->textEdit->toPlainText().section(";",i,i).count(",")==2) {
            temp.first=ui->textEdit->toPlainText().section(";",i,i).section(",",0,0).toInt();
            temp.second=ui->textEdit->toPlainText().section(";",i,i).section(",",1,1).toInt();
            dates.push_back({temp,ui->textEdit->toPlainText().section(";",i,i).section(",",2,2)});
        } else if (ui->textEdit->toPlainText().section(";",i,i).count("[")==2 || ui->textEdit->toPlainText().section(";",i,i).count("]")==2) {
            for (int k=ui->textEdit->toPlainText().section(";",i,i).section("[",1,1).section(",",0,0).toInt();
                 k<ui->textEdit->toPlainText().section(";",i,i).section("[",2,2).section(",",0,0).toInt();
                 k++) {
                for (int j=ui->textEdit->toPlainText().section(";",i,i).section("[",1,1).section(",",1,1).section("]",0,0).toInt();
                     j<ui->textEdit->toPlainText().section(";",i,i).section("[",2,2).section(",",1,1).section("]",0,0).toInt();
                     j++) {
                    temp.first=k;
                    temp.second=j;
                    dates.push_back({temp,ui->textEdit->toPlainText().section(";",i,i).section("]",2,2).section(",",1,1)});
                }
            }
        } else {
            QMessageBox::warning(this,"Data Manipulation failed","Not Enough or too many arguments to proceed. (You need the coordinates of the cell and the date of encounter as 'row,column,yyyy.mm.dd' or a set of ceils as the format '[row1,column1]*[row2,column2],Date' and it will set all of the cells date of encounter to the Date you have given.)");
            return;
        }
    }
    for (auto it2=dates.begin();it2!=dates.end();it2++) {
        ret=contactdata.find(it2->first.first)->second.second.insert({it2->first.second,QDate::fromString(it2->second,"yyyy.MM.dd")});
        if (QDate::fromString(it2->second,"yyyy.MM.dd")<oldest) {
            oldest=QDate::fromString(it2->second,"yyyy.MM.dd");
        }
        if (!(ret.second)) {
            ret.first->second=QDate::fromString(it2->second,"yyyy.MM.dd");
        }
        ret=contactdata.find(it2->first.second)->second.second.insert({it2->first.first,QDate::fromString(it2->second,"yyyy.MM.dd")});
        if (!(ret.second)) {
            ret.first->second=QDate::fromString(it2->second,"yyyy.MM.dd");
        }
        alma=true;
        ui->tableWidget->item(it2->first.first,it2->first.second)->setText(it2->second);
        alma=true;
        ui->tableWidget->item(it2->first.second,it2->first.first)->setText(it2->second);
    }
    edit.clear();
    dates.clear();
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    auto col=contactdata.find(item->column());
    auto row=contactdata.find(item->row());
    if (item->text()!="0" && !(alma)) {
        alma=true;
        QTableWidgetItem *a=ui->tableWidget->item(item->column(),item->row());
        a->setText(item->text());
        ret=col->second.second.insert({row->first,QDate::fromString(item->text(),"yyyy.MM.dd")});
        if (QDate::fromString(item->text(),"yyyy.MM.dd")<oldest) {
            oldest=QDate::fromString(item->text(),"yyyy.MM.dd");
        }
        if (!(ret.second)) {
            ret.first->second=QDate::fromString(item->text(),"yyyy.MM.dd");
        }
        ret=row->second.second.insert({col->first,QDate::fromString(item->text(),"yyyy.MM.dd")});
        if (!(ret.second)) {
            ret.first->second=QDate::fromString(item->text(),"yyyy.MM.dd");
        }
    } else if (item->text()=="0" && !(alma)) {
        alma=true;
        QTableWidgetItem *a=ui->tableWidget->item(item->column(),item->row());
        a->setText("0");
        col->second.second.erase(row->first);
        row->second.second.erase(col->first);
    } else if (alma) {
        alma=false;
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Open file",
                                                    ".");
    QFile alma(filename);
    if (!alma.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
    QString str1=" ";
    //int whole=ui->tableWidget->rowCount()*ui->tableWidget->columnCount();
    for (int k=0;k<ui->tableWidget->rowCount();k++) {
        for (int i=0;i<ui->tableWidget->columnCount();i++) {
            if (k==0 && i==0) {

            } else if (i==0){
                if (contactdata.find(k)->second.first.second) {
                    str1+=ui->tableWidget->item(k,i)->text()+",true";
                } else {
                    str1+=ui->tableWidget->item(k,i)->text();
                }
            } else if (k==0){
                if (contactdata.find(i)->second.first.second) {
                    str1+=";"+ui->tableWidget->item(k,i)->text()+",true";
                } else {
                    str1+=";"+ui->tableWidget->item(k,i)->text();
                }
            } else {
                str1+=";"+ui->tableWidget->item(k,i)->text();
            }
        }
        str1+='\n';
    }
    QTextStream out(&alma);
    out<<str1;
}

void MainWindow::on_actionNew_triggered()
{
    if (!(contactdata.empty())) {
        QMessageBox::StandardButton reply=QMessageBox::warning(this,"Table not empty","All your data will be lost if you continue. Do you want to save them?",QMessageBox::Yes | QMessageBox::Discard | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes){
            on_actionSave_as_triggered();
        } else if (reply==QMessageBox::Cancel){
            return;
        }
    }
    contactdata.clear();
    b=0;
    a=1;
    edit.clear();
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);
    ui->textEdit->clear();
}

void MainWindow::on_actionRead_triggered()
{
    if (!(contactdata.empty())) {
        QMessageBox::StandardButton reply=QMessageBox::warning(this,"Table not empty","All your data will be lost if you continue. Do you want to save them?",QMessageBox::Yes | QMessageBox::Discard | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes){
            on_actionSave_as_triggered();
        } else if (reply==QMessageBox::Cancel){
            return;
        }
    }
    ui->tableWidget->clear();
    contactdata.clear();
    a=1;
    b=0;
    std::vector<std::pair<QString,bool>> names;
    std::map<std::pair<unsigned long long,unsigned long long>,QDate> temp2;
    std::pair<unsigned long long,unsigned long long> subsubtemp2;
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Open file",
                                                    ".");
    QFile alma(filename);
    if (!alma.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
    QString str1="";
    QTextStream in(&alma);
    int k=0;
    while (!in.atEnd()) {
        str1=in.readLine();
        for (int i=1;i<str1.count(";")+1;i++) {
            if (k==0) {
                if (str1.section(";",i,i).count(",")==1) {
                    names.push_back({str1.section(";",i,i).section(",",0,0),true});
                    //qDebug()<<"true";
                } else {
                    names.push_back({str1.section(";",i,i),false});
                }
            } else {
                if (str1.section(";",i,i)!="0") {
                    subsubtemp2.first=i;
                    subsubtemp2.second=k;
                    temp2.insert({subsubtemp2,QDate::fromString(str1.section(";",i,i),"yyyy.MM.dd")});
                }
            }
        }
        k++;
    }
    for (unsigned long long i=0;i<names.size();i++) {
        ui->lineEdit->setText(names[i].first);
        on_pushButton_clicked();
        if (names[i].second) {
            on_tableWidget_cellClicked(0,i+1);
        }
    }
    for (auto it=temp2.begin();it!=temp2.end();it++) {
        ui->tableWidget->item(it->first.first,it->first.second)->setText(it->second.toString("yyyy.MM.dd"));
    }
}

void MainWindow::on_actionQuery_triggered()
{
    Query *a=new Query(contactdata,oldest);
    a->show();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    auto col=contactdata.find(column);
    auto rou=contactdata.find(row);
    if (row==0 && column==0) {
        return;
    } else if (row==0) {
        if (col->second.first.second==true) {
            col->second.first.second=false;
            alma=true;
            ui->tableWidget->item(row,column)->setToolTip("Currently this person is not infected.\n"
                             "Click on the person's name to set them as infected");
            alma=true;
            ui->tableWidget->item(row,column)->setBackground(QColor(255,255,255));
            alma=true;
            ui->tableWidget->item(column,row)->setToolTip("Currently this person is not infected.\n"
                             "Click on the person's name to set them as infected");
            alma=true;
            ui->tableWidget->item(column,row)->setBackground(QColor(255,255,255));
        } else {
            col->second.first.second=true;
            alma=true;
            ui->tableWidget->item(row,column)->setToolTip("Currently this person is infected.\n"
                             "Click on the person's name to set them as not infected");
            alma=true;
            ui->tableWidget->item(row,column)->setBackground(QColor(250,150,0));
            alma=true;
            ui->tableWidget->item(column,row)->setToolTip("Currently this person is infected.\n"
                             "Click on the person's name to set them as not infected");
            alma=true;
            ui->tableWidget->item(column,row)->setBackground(QColor(250,150,0));
        }
    } else if (column==0) {
        if (rou->second.first.second==true) {
            rou->second.first.second=false;
            alma=true;
            ui->tableWidget->item(row,column)->setToolTip("Currently this person set to be not infected.\n"
                             "Click on the person's name to set them as infected");
            alma=true;
            ui->tableWidget->item(row,column)->setBackground(QColor(255,255,255));
            alma=true;
            ui->tableWidget->item(column,row)->setToolTip("Currently this person set to be not infected.\n"
                             "Click on the person's name to set them as infected");
            alma=true;
            ui->tableWidget->item(column,row)->setBackground(QColor(255,255,255));
        } else  {
            rou->second.first.second=true;
            alma=true;
            ui->tableWidget->item(row,column)->setToolTip("Currently this person is infected.\n"
                             "Click on the person's name to set them as not infected");
            alma=true;
            ui->tableWidget->item(row,column)->setBackground(QColor(250,150,0));
            alma=true;
            ui->tableWidget->item(column,row)->setToolTip("Currently this person is infected.\n"
                             "Click on the person's name to set them as not infected");
            alma=true;
            ui->tableWidget->item(column,row)->setBackground(QColor(250,150,0));
        }
    } else {
        alma=false;
        ui->tableWidget->item(row,column)->setText(QDate::currentDate().toString("yyyy.MM.dd"));
    }
}
