#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();
    void search();
    void saveResults();

private:
    Ui::MainWindow* ui;
    QString currentFile;
    QString fileContent;
    QStringList searchResults;
    int searchHits;

    void displayContent();
    void displaySearchResults();
    void displaySearchStatistics();
};

#endif // MAINWINDOW_H
