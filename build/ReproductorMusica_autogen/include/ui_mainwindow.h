/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbrirArchivo;
    QAction *actionAbrirDirectorio;
    QAction *actionSalir;
    QAction *actionTemaClaro;
    QAction *actionTemaOscuro;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTreeWidget *playlistWidget;
    QHBoxLayout *controlsLayout;
    QPushButton *previousButton;
    QPushButton *playButton;
    QPushButton *nextButton;
    QHBoxLayout *horizontalLayout;
    QLabel *volumeLabel;
    QSlider *volumeSlider;
    QSlider *progressSlider;
    QMenuBar *menubar;
    QMenu *menuArchivo;
    QMenu *menuTemas;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionAbrirArchivo = new QAction(MainWindow);
        actionAbrirArchivo->setObjectName("actionAbrirArchivo");
        actionAbrirDirectorio = new QAction(MainWindow);
        actionAbrirDirectorio->setObjectName("actionAbrirDirectorio");
        actionSalir = new QAction(MainWindow);
        actionSalir->setObjectName("actionSalir");
        actionTemaClaro = new QAction(MainWindow);
        actionTemaClaro->setObjectName("actionTemaClaro");
        actionTemaOscuro = new QAction(MainWindow);
        actionTemaOscuro->setObjectName("actionTemaOscuro");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        playlistWidget = new QTreeWidget(centralwidget);
        playlistWidget->setObjectName("playlistWidget");
        playlistWidget->setAcceptDrops(true);
        playlistWidget->setDragDropMode(QAbstractItemView::DropOnly);
        playlistWidget->setDefaultDropAction(Qt::CopyAction);

        verticalLayout->addWidget(playlistWidget);

        controlsLayout = new QHBoxLayout();
        controlsLayout->setObjectName("controlsLayout");
        previousButton = new QPushButton(centralwidget);
        previousButton->setObjectName("previousButton");

        controlsLayout->addWidget(previousButton);

        playButton = new QPushButton(centralwidget);
        playButton->setObjectName("playButton");

        controlsLayout->addWidget(playButton);

        nextButton = new QPushButton(centralwidget);
        nextButton->setObjectName("nextButton");

        controlsLayout->addWidget(nextButton);


        verticalLayout->addLayout(controlsLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        volumeLabel = new QLabel(centralwidget);
        volumeLabel->setObjectName("volumeLabel");

        horizontalLayout->addWidget(volumeLabel);

        volumeSlider = new QSlider(centralwidget);
        volumeSlider->setObjectName("volumeSlider");
        volumeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(volumeSlider);


        verticalLayout->addLayout(horizontalLayout);

        progressSlider = new QSlider(centralwidget);
        progressSlider->setObjectName("progressSlider");
        progressSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(progressSlider);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menuArchivo = new QMenu(menubar);
        menuArchivo->setObjectName("menuArchivo");
        menuTemas = new QMenu(menubar);
        menuTemas->setObjectName("menuTemas");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuArchivo->menuAction());
        menubar->addAction(menuTemas->menuAction());
        menuArchivo->addAction(actionAbrirArchivo);
        menuArchivo->addAction(actionAbrirDirectorio);
        menuArchivo->addSeparator();
        menuArchivo->addAction(actionSalir);
        menuTemas->addAction(actionTemaClaro);
        menuTemas->addAction(actionTemaOscuro);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Reproductor de M\303\272sica", nullptr));
        actionAbrirArchivo->setText(QCoreApplication::translate("MainWindow", "Abrir Archivo", nullptr));
        actionAbrirDirectorio->setText(QCoreApplication::translate("MainWindow", "Abrir Directorio", nullptr));
        actionSalir->setText(QCoreApplication::translate("MainWindow", "Salir", nullptr));
        actionTemaClaro->setText(QCoreApplication::translate("MainWindow", "Tema Claro", nullptr));
        actionTemaOscuro->setText(QCoreApplication::translate("MainWindow", "Tema Oscuro", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = playlistWidget->headerItem();
        ___qtreewidgetitem->setText(4, QCoreApplication::translate("MainWindow", "Duraci\303\263n", nullptr));
        ___qtreewidgetitem->setText(3, QCoreApplication::translate("MainWindow", "Comentarios", nullptr));
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("MainWindow", "\303\201lbum", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("MainWindow", "Artista", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "T\303\255tulo", nullptr));
        previousButton->setText(QCoreApplication::translate("MainWindow", "Anterior", nullptr));
        playButton->setText(QCoreApplication::translate("MainWindow", "Reproducir", nullptr));
        nextButton->setText(QCoreApplication::translate("MainWindow", "Siguiente", nullptr));
        volumeLabel->setText(QCoreApplication::translate("MainWindow", "Volumen:", nullptr));
        menuArchivo->setTitle(QCoreApplication::translate("MainWindow", "Archivo", nullptr));
        menuTemas->setTitle(QCoreApplication::translate("MainWindow", "Temas", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
