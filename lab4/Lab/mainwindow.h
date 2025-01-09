#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>
void displayScatter(bool state, QStackedWidget *stackedWidget)
{
    if (state)
    {
        stackedWidget->setCurrentIndex(1);
    }
}
void displaySurface(bool state, QStackedWidget *stackedWidget)
{
    if (state)
    {
        stackedWidget->setCurrentIndex(0);
    }
}
#endif	  // MAINWINDOW_H
