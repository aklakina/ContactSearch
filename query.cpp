#include "query.h"
#include "ui_query.h"
#include <set>
#include <QMessageBox>

struct comp {
    template <typename T>
    bool operator()(const T& l,
                    const T& r) const
    {
        if (l.second != r.second) {
            return l.second > r.second;
        }
        return l.first > r.first;
    }
};

QStringList sort(std::map<QString, QDate>& M)
{
    std::set<std::pair<QString, QDate>, comp> S(M.begin(),
                                   M.end());
    QStringList sorted;
    for (auto& it : S) {
        sorted.push_back(it.first);
    }
    return sorted;
}

Query::Query(std::map<unsigned long long,std::pair<std::pair<QString,bool>,std::map<unsigned long long,QDate>>> dates,QDate oldest_contact, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Query)
{
    ui->setupUi(this);
    contactdata=dates;
    ui->label->setText(contactdata.find(1)->second.first.first);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(oldest_contact);
    oldest=oldest_contact;
    ui->checkBox->setChecked(true);
    on_pushButton_clicked();
}

Query::~Query()
{
    delete ui;
}

void Query::contactsearch(unsigned long long id) {
    std::pair<std::set<unsigned long long>::iterator,bool> ret;
    auto a=contactdata.find(id)->second.second;
    for (auto it2=a.begin();it2!=a.end();it2++) {
        if (it2->second<ui->dateEdit->date() && it2->second>=ui->dateEdit_2->date()) {
            if ((ui->checkBox->isChecked())) {
                korte.insert(it2->first);
            } else {
                ret=korte.insert(it2->first);
                if (ret.second) {
                    contactsearch(it2->first);
                }
            }
        }
    }
}

void Query::on_pushButton_clicked()
{
    ui->listWidget->clear();
    alma.clear();
    korte.clear();
    for (auto it=contactdata.begin();it!=contactdata.end();it++) {
        if (ui->checkBox_2->isChecked()) {
            if (it->second.first.second) {
                contactsearch(it->first);
                alma.insert({it->first,korte});
                korte.clear();
            }
        } else {
            contactsearch(it->first);
            alma.insert({it->first,korte});
            korte.clear();
        }
    }
    ui->spinBox->setValue(1);
    if (alma.empty()) {
        QMessageBox::warning(this,"Zero ID found","There is no ID in the current database that has any contact matching the current filters.");
        return;
    }
    on_pushButton_7_clicked();
}

void Query::on_pushButton_3_clicked()
{
    pos++;
    std::vector<QString> alm;
    auto a=alma.find(pos);
    if (a==alma.end()) {
        pos--;
        return;
    }
    ui->listWidget->clear();
    ui->label->setText(contactdata.find(pos)->second.first.first);
    for (auto it=a->second.begin();it!=a->second.end();it++) {
        alm.push_back(contactdata.find(*it)->second.first.first);
    }
    sort(alm.begin(), alm.end());
    for (unsigned long long i=0; i<alm.size();i++) {
        ui->listWidget->addItem(alm[i]);
    }
    ui->spinBox->setValue(pos);
}

void Query::on_pushButton_2_clicked()
{
    pos--;
    if (pos==0) {
        pos++;
        return;
    }
    std::vector<QString> alm;
    ui->listWidget->clear();
    auto a=alma.find(pos);
    ui->label->setText(contactdata.find(pos)->second.first.first);
    for (auto it=a->second.begin();it!=a->second.end();it++) {
        alm.push_back(contactdata.find(*it)->second.first.first);
    }
    sort(alm.begin(), alm.end());
    for (unsigned long long i=0; i<alm.size();i++) {
        ui->listWidget->addItem(alm[i]);
    }
    ui->spinBox->setValue(pos);
}

void Query::on_pushButton_4_clicked()
{
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(oldest);
    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(false);
    ui->spinBox->setValue(1);
    on_pushButton_clicked();
}

void Query::on_pushButton_5_clicked()
{
    std::vector<QString> alpaka;
    int names = ui->listWidget->count();
    for(int i = 0; i < names; i++)
    {
        alpaka.push_back(ui->listWidget->item(i)->text());
    }
    sort(alpaka.begin(),alpaka.end());
    ui->listWidget->clear();
    for(auto it=alpaka.begin();it!=alpaka.end();it++) {
        ui->listWidget->addItem(*it);
    }
}

void Query::on_pushButton_6_clicked()
{
    std::map<QString,QDate> alpaka;
    auto a=alma.find(pos);
    for (auto it=a->second.begin();it!=a->second.end();it++) {
        auto b=contactdata.find(*it);
        alpaka.insert({b->second.first.first,b->second.second.find(pos)->second});
    }
    ui->listWidget->clear();
    ui->listWidget->addItems(sort(alpaka));
}


void Query::on_pushButton_7_clicked()
{
    std::vector<QString> alm;
    if (alma.find(ui->spinBox->value())==alma.end()) {
        ui->label->setText(contactdata.find(pos)->second.first.first);
        QMessageBox::warning(this,"No such ID with the current filters","I didn't find any matching ID in the given database with the current filters!");
        ui->spinBox->setValue(pos);
        return;
    }
        pos=ui->spinBox->value();
    ui->listWidget->clear();
    for (auto it=alma.find(pos)->second.begin();it!=alma.find(pos)->second.end();it++) {
        alm.push_back(contactdata.find(*it)->second.first.first);
    }
    sort(alm.begin(), alm.end());
    for (unsigned long long i=0; i<alm.size();i++) {
        ui->listWidget->addItem(alm[i]);
    }
    ui->label->setText(contactdata.find(pos)->second.first.first);
}
