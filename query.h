#ifndef QUERY_H
#define QUERY_H

#include <QDialog>
#include <set>
#include <QDate>

namespace Ui {
class Query;
}

class Query : public QDialog
{
    Q_OBJECT

public:
    explicit Query(std::map<unsigned long long,std::pair<std::pair<QString,bool>,std::map<unsigned long long,QDate>>> dates,QDate oldest_contact, QWidget *parent = nullptr);
    ~Query();

private slots:

    //unsigned long long find_key(QString a);

    void contactsearch(unsigned long long id);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Query *ui;
    std::map<unsigned long long,std::pair<std::pair<QString,bool>,std::map<unsigned long long,QDate>>> contactdata;
    std::map<unsigned long long,std::set<unsigned long long>> alma;
    unsigned long long pos;
    std::set<unsigned long long> korte;
    QDate oldest;
};

#endif // QUERY_H
