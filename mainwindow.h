#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDate>

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
    void on_pushButton_clicked();

    void on_actionHuman_Readable_format_triggered();

    void on_pushButton_2_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_actionSave_as_triggered();

    void on_actionNew_triggered();

    void on_actionRead_triggered();

    void on_actionQuery_triggered();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    int a=1;
    unsigned long long b=0;
    std::map<unsigned long long,std::pair<std::pair<QString,bool>,std::map<unsigned long long,QDate>>> contactdata;
    std::map<unsigned long long,unsigned long long> edit;
    bool alma=false;
    QDate oldest=QDate::currentDate();
    std::pair<std::map<unsigned long long,QDate>::iterator,bool> ret;
    void on_pushButton_clicked_valami(QString text);
};
#endif // MAINWINDOW_H
