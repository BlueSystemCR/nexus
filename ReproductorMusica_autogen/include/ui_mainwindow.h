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
    QAction *actionAbrirDirectorio;
    QAction *actionSalir;
    QAction *actionTemaClaro;
    QAction *actionTemaOscuro;
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
        controlsLayout->setObjectName("controlsLayout");
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
        MainWindow->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"    /* Estilo general de la ventana */\n"
"    QMainWindow {\n"
"        background-color: #1E1E1E;\n"
"        color: #FFFFFF;\n"
"    }\n"
"\n"
"    /* Lista de reproducci\303\263n */\n"
"    QTreeWidget {\n"
"        background-color: #2D2D2D;\n"
"        border: none;\n"
"        color: #FFFFFF;\n"
"    }\n"
"    QTreeWidget::item {\n"
"        height: 25px;\n"
"        padding: 4px;\n"
"    }\n"
"    QTreeWidget::item:selected {\n"
"        background-color: #0078D4;\n"
"    }\n"
"    QTreeWidget::item:hover {\n"
"        background-color: #3D3D3D;\n"
"    }\n"
"    QHeaderView::section {\n"
"        background-color: #2D2D2D;\n"
"        color: #FFFFFF;\n"
"        padding: 4px;\n"
"        border: none;\n"
"    }\n"
"\n"
"    /* Controles de reproducci\303\263n */\n"
"    QPushButton {\n"
"        background-color: transparent;\n"
"        color: #FFFFFF;\n"
"        border: none;\n"
"        font-size: 16px;\n"
"        padding: 8px;\n"
"        min-width: 32px;\n"
"    }\n"
"    QPushButton:hover"
                        " {\n"
"        background-color: #3D3D3D;\n"
"        border-radius: 4px;\n"
"    }\n"
"    QPushButton:pressed {\n"
"        background-color: #0078D4;\n"
"    }\n"
"\n"
"    /* Sliders */\n"
"    QSlider::groove:horizontal {\n"
"        border: none;\n"
"        height: 4px;\n"
"        background: #3D3D3D;\n"
"        margin: 2px 0;\n"
"    }\n"
"    QSlider::handle:horizontal {\n"
"        background: #0078D4;\n"
"        width: 12px;\n"
"        height: 12px;\n"
"        margin: -4px 0;\n"
"        border-radius: 6px;\n"
"    }\n"
"    QSlider::sub-page:horizontal {\n"
"        background: #0078D4;\n"
"    }\n"
"    QSlider::handle:horizontal:hover {\n"
"        background: #2B9FFF;\n"
"    }\n"
"\n"
"    /* Etiquetas de tiempo */\n"
"    QLabel {\n"
"        color: #FFFFFF;\n"
"    }\n"
"\n"
"    /* ComboBox de velocidad */\n"
"    QComboBox {\n"
"        background-color: #3D3D3D;\n"
"        color: #FFFFFF;\n"
"        border: none;\n"
"        padding: 4px;\n"
"        border-radius: 4px;\n"
"    }\n"
""
                        "    QComboBox::drop-down {\n"
"        border: none;\n"
"        width: 20px;\n"
"    }\n"
"    QComboBox::down-arrow {\n"
"        image: url(:/icons/down-arrow.png);\n"
"    }\n"
"    QComboBox:hover {\n"
"        background-color: #4D4D4D;\n"
"    }\n"
"    QComboBox QAbstractItemView {\n"
"        background-color: #2D2D2D;\n"
"        color: #FFFFFF;\n"
"        selection-background-color: #0078D4;\n"
"        selection-color: #FFFFFF;\n"
"        border: none;\n"
"    }\n"
"\n"
"    /* Barra de estado */\n"
"    QStatusBar {\n"
"        background-color: #1E1E1E;\n"
"        color: #FFFFFF;\n"
"    }\n"
"    QStatusBar::item {\n"
"        border: none;\n"
"    }\n"
"\n"
"    /* Scrollbars */\n"
"    QScrollBar:vertical {\n"
"        background: #2D2D2D;\n"
"        width: 10px;\n"
"        margin: 0;\n"
"    }\n"
"    QScrollBar::handle:vertical {\n"
"        background: #3D3D3D;\n"
"        min-height: 20px;\n"
"        border-radius: 5px;\n"
"    }\n"
"    QScrollBar::add-line:vertical,\n"
"    QScroll"
                        "Bar::sub-line:vertical {\n"
"        height: 0;\n"
"    }\n"
"    QScrollBar::up-arrow:vertical,\n"
"    QScrollBar::down-arrow:vertical {\n"
"        height: 0;\n"
"    }\n"
"    QScrollBar::add-page:vertical,\n"
"    QScrollBar::sub-page:vertical {\n"
"        background: none;\n"
"    }\n"
"   ", nullptr));
        actionAbrirArchivo->setText(QCoreApplication::translate("MainWindow", "Abrir Archivo", nullptr));
        actionAbrirDirectorio->setText(QCoreApplication::translate("MainWindow", "Abrir Directorio", nullptr));
        actionSalir->setText(QCoreApplication::translate("MainWindow", "Salir", nullptr));
        actionTemaClaro->setText(QCoreApplication::translate("MainWindow", "Tema Claro", nullptr));
        actionTemaOscuro->setText(QCoreApplication::translate("MainWindow", "Tema Oscuro", nullptr));
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
        playButton->setText(QCoreApplication::translate("MainWindow", "\342\217\270", nullptr));
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
        menuArchivo->setTitle(QCoreApplication::translate("MainWindow", "Archivo", nullptr));
        menuTemas->setTitle(QCoreApplication::translate("MainWindow", "Temas", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
