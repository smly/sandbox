/********************************************************************************
** Form generated from reading UI file 'button.ui'
**
** Created: Mon Feb 22 23:33:51 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *northButton;
    QPushButton *eastButton;
    QPushButton *southbutton;
    QPushButton *westButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(127, 119);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 0, 121, 71));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        northButton = new QPushButton(gridLayoutWidget);
        northButton->setObjectName(QString::fromUtf8("northButton"));
        northButton->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(northButton->sizePolicy().hasHeightForWidth());
        northButton->setSizePolicy(sizePolicy);
        northButton->setAutoRepeatDelay(300);
        northButton->setAutoRepeatInterval(100);

        gridLayout->addWidget(northButton, 0, 2, 1, 1);

        eastButton = new QPushButton(gridLayoutWidget);
        eastButton->setObjectName(QString::fromUtf8("eastButton"));

        gridLayout->addWidget(eastButton, 1, 3, 1, 1);

        southbutton = new QPushButton(gridLayoutWidget);
        southbutton->setObjectName(QString::fromUtf8("southbutton"));

        gridLayout->addWidget(southbutton, 2, 2, 1, 1);

        westButton = new QPushButton(gridLayoutWidget);
        westButton->setObjectName(QString::fromUtf8("westButton"));

        gridLayout->addWidget(westButton, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 127, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        northButton->setText(QApplication::translate("MainWindow", "N", 0, QApplication::UnicodeUTF8));
        eastButton->setText(QApplication::translate("MainWindow", "E", 0, QApplication::UnicodeUTF8));
        southbutton->setText(QApplication::translate("MainWindow", "S", 0, QApplication::UnicodeUTF8));
        westButton->setText(QApplication::translate("MainWindow", "W", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BUTTON_H
