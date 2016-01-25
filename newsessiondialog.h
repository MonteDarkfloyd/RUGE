#ifndef NEWSESSIONDIALOG_H
#define NEWSESSIONDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "trafficwindow.h"
#include "xmlwriter.h"
#include "xmlreader.h"

namespace Ui {
class NewSessionDialog;
}

class NewSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewSessionDialog(TrafficWindow *parent = 0);
    ~NewSessionDialog();

    // Used to get amount of xml files found in the predefined folder.
    unsigned int getPredefinedAmount();

public slots:
    //Update the view after changes made in sessionList
    void update();

private slots:
    //After a predefined session is selected
    void on_buttonBox_accepted();

    //After create new session button clicked
    void on_createButton_clicked();

    //After item selected from list , enable OK button
    void on_predefinedList_itemSelectionChanged();

    //Specific cell selected
    void cellSelected(int row, int col);

    //Edit button on a row is clicked
    void editClicked(int nRow);

    //Delete button on a row is clicked
    void deleteClicked(int nRow);

    //Fill the list and table with data from List
    void fill();

    //Clear the list
    void clearList();



private:
    Ui::NewSessionDialog *ui;
    TrafficWindow* parentPointer;
    QList<Session*> sessionList;
    xmlreader Xreader;
    xmlWriter xWriter;
};

#endif // NEWSESSIONDIALOG_H
