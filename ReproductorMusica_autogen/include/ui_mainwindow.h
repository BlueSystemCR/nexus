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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbrirArchivo;
    QAction *actionAbrirCarpeta;
    QAction *actionSalir;
    QAction *actionTemaClaro;
    QAction *actionTemaOscuro;
    QAction *actionColumnas;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTreeWidget *playlistWidget;
    QHBoxLayout *progressLayout;
    QLabel *timeLabel;
    QSlider *progressSlider;
    QLabel *totalTimeLabel;
    QHBoxLayout *controlsLayout;
    QPushButton *rewindStartButton;
    QPushButton *previousButton;
    QPushButton *playButton;
    QPushButton *nextButton;
    QPushButton *fastForwardButton;
    QSpacerItem *horizontalSpacer;
    QLabel *volumeLabel;
    QSlider *volumeSlider;
    QLabel *volumePercentLabel;
    QComboBox *speedComboBox;
    QMenuBar *menubar;
    QMenu *menuArchivo;
    QMenu *menuVer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionAbrirArchivo = new QAction(MainWindow);
        actionAbrirArchivo->setObjectName("actionAbrirArchivo");
        actionAbrirCarpeta = new QAction(MainWindow);
        actionAbrirCarpeta->setObjectName("actionAbrirCarpeta");
        actionSalir = new QAction(MainWindow);
        actionSalir->setObjectName("actionSalir");
        actionTemaClaro = new QAction(MainWindow);
        actionTemaClaro->setObjectName("actionTemaClaro");
        actionTemaOscuro = new QAction(MainWindow);
        actionTemaOscuro->setObjectName("actionTemaOscuro");
        actionColumnas = new QAction(MainWindow);
        actionColumnas->setObjectName("actionColumnas");
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

        progressLayout = new QHBoxLayout();
        progressLayout->setObjectName("progressLayout");
        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName("timeLabel");
        timeLabel->setMinimumWidth(60);
        timeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        progressLayout->addWidget(timeLabel);

        progressSlider = new QSlider(centralwidget);
        progressSlider->setObjectName("progressSlider");
        progressSlider->setOrientation(Qt::Horizontal);

        progressLayout->addWidget(progressSlider);

        totalTimeLabel = new QLabel(centralwidget);
        totalTimeLabel->setObjectName("totalTimeLabel");
        totalTimeLabel->setMinimumWidth(60);
        totalTimeLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        progressLayout->addWidget(totalTimeLabel);


        verticalLayout->addLayout(progressLayout);

        controlsLayout = new QHBoxLayout();
        controlsLayout->setSpacing(10);
        controlsLayout->setObjectName("controlsLayout");
        controlsLayout->setContentsMargins(20, -1, 20, -1);
        rewindStartButton = new QPushButton(centralwidget);
        rewindStartButton->setObjectName("rewindStartButton");

        controlsLayout->addWidget(rewindStartButton);

        previousButton = new QPushButton(centralwidget);
        previousButton->setObjectName("previousButton");

        controlsLayout->addWidget(previousButton);

        playButton = new QPushButton(centralwidget);
        playButton->setObjectName("playButton");

        controlsLayout->addWidget(playButton);

        nextButton = new QPushButton(centralwidget);
        nextButton->setObjectName("nextButton");

        controlsLayout->addWidget(nextButton);

        fastForwardButton = new QPushButton(centralwidget);
        fastForwardButton->setObjectName("fastForwardButton");

        controlsLayout->addWidget(fastForwardButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlsLayout->addItem(horizontalSpacer);

        volumeLabel = new QLabel(centralwidget);
        volumeLabel->setObjectName("volumeLabel");

        controlsLayout->addWidget(volumeLabel);

        volumeSlider = new QSlider(centralwidget);
        volumeSlider->setObjectName("volumeSlider");
        volumeSlider->setMaximum(100);
        volumeSlider->setValue(50);
        volumeSlider->setOrientation(Qt::Horizontal);

        controlsLayout->addWidget(volumeSlider);

        volumePercentLabel = new QLabel(centralwidget);
        volumePercentLabel->setObjectName("volumePercentLabel");
        volumePercentLabel->setMinimumWidth(40);

        controlsLayout->addWidget(volumePercentLabel);

        speedComboBox = new QComboBox(centralwidget);
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->addItem(QString());
        speedComboBox->setObjectName("speedComboBox");

        controlsLayout->addWidget(speedComboBox);


        verticalLayout->addLayout(controlsLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menuArchivo = new QMenu(menubar);
        menuArchivo->setObjectName("menuArchivo");
        menuVer = new QMenu(menubar);
        menuVer->setObjectName("menuVer");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuArchivo->menuAction());
        menubar->addAction(menuVer->menuAction());
        menuArchivo->addAction(actionAbrirArchivo);
        menuArchivo->addAction(actionAbrirCarpeta);
        menuVer->addAction(actionColumnas);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Reproductor de M\303\272sica", nullptr));
        MainWindow->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"    /* Estilo general de la ventana */\n"
"    QMainWindow {\n"
"        background-color: #121212;\n"
"        color: #FFFFFF;\n"
"    }\n"
"\n"
"    /* Lista de reproducci\303\263n */\n"
"    QTreeWidget {\n"
"        background-color: #121212;\n"
"        border: none;\n"
"        color: #FFFFFF;\n"
"    }\n"
"\n"
"    /* Controles de reproducci\303\263n */\n"
"    QPushButton {\n"
"        background-color: transparent;\n"
"        color: #FFFFFF;\n"
"        border: none;\n"
"        font-size: 18px;\n"
"        padding: 10px;\n"
"        min-width: 40px;\n"
"        font-weight: bold;\n"
"    }\n"
"    QPushButton:hover {\n"
"        color: #2196F3;\n"
"    }\n"
"    QPushButton:pressed {\n"
"        color: #1976D2;\n"
"    }\n"
"\n"
"    /* Sliders */\n"
"    QSlider::groove:horizontal {\n"
"        border: none;\n"
"        height: 4px;\n"
"        background: #333333;\n"
"        margin: 2px 0;\n"
"    }\n"
"    QSlider::handle:horizontal {\n"
"        background: #2196F3;\n"
"        width: 16px"
                        ";\n"
"        height: 16px;\n"
"        margin: -6px 0;\n"
"        border-radius: 8px;\n"
"    }\n"
"    QSlider::sub-page:horizontal {\n"
"        background: #2196F3;\n"
"    }\n"
"\n"
"    /* Etiquetas */\n"
"    QLabel {\n"
"        color: #FFFFFF;\n"
"        font-size: 14px;\n"
"    }\n"
"\n"
"    /* ComboBox de velocidad */\n"
"    QComboBox {\n"
"        background-color: #333333;\n"
"        color: #FFFFFF;\n"
"        border: none;\n"
"        padding: 5px 10px;\n"
"        border-radius: 4px;\n"
"        min-width: 70px;\n"
"        font-size: 14px;\n"
"    }\n"
"    QComboBox:hover {\n"
"        background-color: #404040;\n"
"    }\n"
"    QComboBox::drop-down {\n"
"        border: none;\n"
"        width: 20px;\n"
"    }\n"
"    QComboBox::down-arrow {\n"
"        image: none;\n"
"        width: 0;\n"
"    }\n"
"\n"
"    /* Barra de estado */\n"
"    QStatusBar {\n"
"        background-color: #121212;\n"
"        color: #FFFFFF;\n"
"    }\n"
"\n"
"    /* Layout espec\303\255fico */\n"
"    QWidge"
                        "t#centralwidget {\n"
"        background-color: #121212;\n"
"    }\n"
"    \n"
"    /* Volumen espec\303\255fico */\n"
"    #volumeLabel {\n"
"        font-size: 14px;\n"
"        margin-right: 10px;\n"
"    }\n"
"    #volumePercentLabel {\n"
"        font-size: 14px;\n"
"        margin-left: 10px;\n"
"    }\n"
"    #volumeSlider {\n"
"        min-width: 150px;\n"
"        max-width: 200px;\n"
"    }\n"
"   ", nullptr));
        actionAbrirArchivo->setText(QCoreApplication::translate("MainWindow", "Abrir Archivo", nullptr));
        actionAbrirCarpeta->setText(QCoreApplication::translate("MainWindow", "Abrir Carpeta", nullptr));
        actionSalir->setText(QCoreApplication::translate("MainWindow", "Salir", nullptr));
        actionTemaClaro->setText(QCoreApplication::translate("MainWindow", "Tema Claro", nullptr));
        actionTemaOscuro->setText(QCoreApplication::translate("MainWindow", "Tema Oscuro", nullptr));
        actionColumnas->setText(QCoreApplication::translate("MainWindow", "Columnas", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = playlistWidget->headerItem();
        ___qtreewidgetitem->setText(3, QCoreApplication::translate("MainWindow", "Duraci\303\263n", nullptr));
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("MainWindow", "\303\201lbum", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("MainWindow", "Artista", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "T\303\255tulo", nullptr));
        timeLabel->setText(QCoreApplication::translate("MainWindow", "00:00", nullptr));
        totalTimeLabel->setText(QCoreApplication::translate("MainWindow", "00:00", nullptr));
        rewindStartButton->setText(QCoreApplication::translate("MainWindow", "\342\217\256", nullptr));
#if QT_CONFIG(tooltip)
        rewindStartButton->setToolTip(QCoreApplication::translate("MainWindow", "Ir al inicio", nullptr));
#endif // QT_CONFIG(tooltip)
        previousButton->setText(QCoreApplication::translate("MainWindow", "\342\217\252", nullptr));
#if QT_CONFIG(tooltip)
        previousButton->setToolTip(QCoreApplication::translate("MainWindow", "Anterior", nullptr));
#endif // QT_CONFIG(tooltip)
        playButton->setText(QCoreApplication::translate("MainWindow", "\342\226\266", nullptr));
#if QT_CONFIG(tooltip)
        playButton->setToolTip(QCoreApplication::translate("MainWindow", "Reproducir/Pausar", nullptr));
#endif // QT_CONFIG(tooltip)
        nextButton->setText(QCoreApplication::translate("MainWindow", "\342\217\251", nullptr));
#if QT_CONFIG(tooltip)
        nextButton->setToolTip(QCoreApplication::translate("MainWindow", "Siguiente", nullptr));
#endif // QT_CONFIG(tooltip)
        fastForwardButton->setText(QCoreApplication::translate("MainWindow", "\342\217\255", nullptr));
#if QT_CONFIG(tooltip)
        fastForwardButton->setToolTip(QCoreApplication::translate("MainWindow", "Ir al final", nullptr));
#endif // QT_CONFIG(tooltip)
        volumeLabel->setText(QCoreApplication::translate("MainWindow", "Volumen", nullptr));
        volumePercentLabel->setText(QCoreApplication::translate("MainWindow", "50%", nullptr));
        speedComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "0.5x", nullptr));
        speedComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "0.75x", nullptr));
        speedComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "1.0x", nullptr));
        speedComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "1.25x", nullptr));
        speedComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "1.5x", nullptr));
        speedComboBox->setItemText(5, QCoreApplication::translate("MainWindow", "2.0x", nullptr));

#if QT_CONFIG(tooltip)
        speedComboBox->setToolTip(QCoreApplication::translate("MainWindow", "Velocidad de reproducci\303\263n", nullptr));
#endif // QT_CONFIG(tooltip)
        speedComboBox->setCurrentText(QCoreApplication::translate("MainWindow", "0.5x", nullptr));
        menuArchivo->setTitle(QCoreApplication::translate("MainWindow", "Archivo", nullptr));
        menuVer->setTitle(QCoreApplication::translate("MainWindow", "Ver", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
