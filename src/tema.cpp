#include "tema.h"
#include <QApplication>

Tema::EstiloTema Tema::obtenerTema(TipoTema tipo) {
    switch (tipo) {
        case AppleMusic:
            return crearTemaAppleMusic();
        case Spotify:
            return crearTemaSpotify();
        case Winamp:
            return crearTemaWinamp();
        default:
            return crearTemaSpotify();
    }
}

Tema::EstiloTema Tema::crearTemaAppleMusic() {
    EstiloTema tema;
    // Colores Apple Music
    tema.colorFondo = QColor("#FFFFFF");
    tema.colorPrimario = QColor("#FB233B");
    tema.colorSecundario = QColor("#000000");
    tema.colorTexto = QColor("#000000");
    tema.colorAcento = QColor("#FB233B");

    // Fuentes Apple Music
    tema.fuenteTitulo = QFont("SF Pro Display", 24, QFont::Light);
    tema.fuenteNormal = QFont("SF Pro Display", 12);
    tema.radioBordes = 8;

    // Estilos específicos
    tema.estiloBotonPrimario = R"(
        QPushButton {
            background-color: #FB233B;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 8px 16px;
            font-family: "SF Pro Display";
        }
        QPushButton:hover {
            background-color: #FF4B60;
        }
    )";

    tema.estiloBotonSecundario = R"(
        QPushButton {
            background-color: #F5F5F7;
            color: #000000;
            border: none;
            border-radius: 8px;
            padding: 8px 16px;
        }
        QPushButton:hover {
            background-color: #E5E5E7;
        }
    )";

    tema.estiloSlider = R"(
        QSlider::groove:horizontal {
            border: none;
            height: 4px;
            background: #E5E5E7;
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            background: #FB233B;
            width: 18px;
            height: 18px;
            margin: -7px 0;
            border-radius: 9px;
        }
    )";

    tema.estiloLista = R"(
        QTreeWidget {
            background-color: white;
            border: none;
        }
        QTreeWidget::item {
            height: 40px;
            border-radius: 4px;
        }
        QTreeWidget::item:selected {
            background-color: #F5F5F7;
            color: #FB233B;
        }
    )";

    return tema;
}

Tema::EstiloTema Tema::crearTemaSpotify() {
    EstiloTema tema;
    // Colores Spotify
    tema.colorFondo = QColor("#191414");
    tema.colorPrimario = QColor("#1DB954");
    tema.colorSecundario = QColor("#282828");
    tema.colorTexto = QColor("#FFFFFF");
    tema.colorAcento = QColor("#1DB954");

    // Fuentes Spotify
    tema.fuenteTitulo = QFont("Circular", 24, QFont::Bold);
    tema.fuenteNormal = QFont("Circular", 12);
    tema.radioBordes = 4;

    // Estilos específicos
    tema.estiloBotonPrimario = R"(
        QPushButton {
            background-color: #1DB954;
            color: white;
            border: none;
            border-radius: 24px;
            padding: 8px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #1ED760;
            transform: scale(1.04);
        }
    )";

    tema.estiloBotonSecundario = R"(
        QPushButton {
            background-color: #282828;
            color: white;
            border: none;
            border-radius: 24px;
            padding: 8px 16px;
        }
        QPushButton:hover {
            background-color: #333333;
        }
    )";

    tema.estiloSlider = R"(
        QSlider::groove:horizontal {
            border: none;
            height: 4px;
            background: #535353;
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            background: #1DB954;
            width: 12px;
            height: 12px;
            margin: -4px 0;
            border-radius: 6px;
        }
        QSlider::handle:horizontal:hover {
            background: #1ED760;
        }
    )";

    tema.estiloLista = R"(
        QTreeWidget {
            background-color: #191414;
            border: none;
            color: #FFFFFF;
        }
        QTreeWidget::item {
            height: 56px;
            border-radius: 4px;
            color: #FFFFFF;
        }
        QTreeWidget::item:selected {
            background-color: #282828;
            color: #1DB954;
        }
        QTreeWidget::item:hover {
            background-color: #282828;
        }
        QTreeWidget QHeaderView::section {
            background-color: #191414;
            color: #B3B3B3;
            border: none;
            padding: 4px;
            font-size: 14px;
        }
    )";

    return tema;
}

Tema::EstiloTema Tema::crearTemaWinamp() {
    EstiloTema tema;
    // Colores Winamp
    tema.colorFondo = QColor("#333333");
    tema.colorPrimario = QColor("#0066CC");
    tema.colorSecundario = QColor("#222222");
    tema.colorTexto = QColor("#00FF00");
    tema.colorAcento = QColor("#00FF00");

    // Fuentes Winamp
    tema.fuenteTitulo = QFont("Courier New", 12, QFont::Bold);
    tema.fuenteNormal = QFont("Courier New", 10);
    tema.radioBordes = 2;

    // Estilos específicos
    tema.estiloBotonPrimario = R"(
        QPushButton {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #666666, stop:0.5 #333333, stop:1 #222222);
            color: #00FF00;
            border: 1px solid #0066CC;
            border-radius: 2px;
            padding: 4px 8px;
            font-family: "Courier New";
        }
        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #777777, stop:0.5 #444444, stop:1 #333333);
        }
    )";

    tema.estiloBotonSecundario = R"(
        QPushButton {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #444444, stop:0.5 #333333, stop:1 #222222);
            color: #00FF00;
            border: 1px solid #444444;
            border-radius: 2px;
            padding: 4px 8px;
        }
        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #555555, stop:0.5 #444444, stop:1 #333333);
        }
    )";

    tema.estiloSlider = R"(
        QSlider::groove:horizontal {
            border: 1px solid #0066CC;
            height: 8px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #222222, stop:1 #333333);
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00FF00, stop:1 #00CC00);
            border: 1px solid #0066CC;
            width: 14px;
            height: 14px;
            margin: -4px 0;
            border-radius: 2px;
        }
    )";

    tema.estiloLista = R"(
        QTreeWidget {
            background-color: #222222;
            border: 1px solid #0066CC;
            color: #00FF00;
        }
        QTreeWidget::item {
            height: 20px;
            border: none;
        }
        QTreeWidget::item:selected {
            background-color: #0066CC;
            color: #FFFFFF;
        }
        QTreeWidget::item:hover {
            background-color: #444444;
        }
    )";

    return tema;
}

QString Tema::obtenerHojaEstilo(TipoTema tipo) {
    EstiloTema tema = obtenerTema(tipo);
    QString estilo;

    switch (tipo) {
        case AppleMusic:
            estilo = QString(R"(
                QMainWindow {
                    background-color: %1;
                }
                QLabel {
                    color: %2;
                    font-family: "SF Pro Display";
                }
            )").arg(tema.colorFondo.name(), tema.colorTexto.name());
            break;

        case Spotify:
            estilo = QString(R"(
                QMainWindow {
                    background-color: %1;
                }
                QLabel {
                    color: %2;
                    font-family: "Circular";
                }
            )").arg(tema.colorFondo.name(), tema.colorTexto.name());
            break;

        case Winamp:
            estilo = QString(R"(
                QMainWindow {
                    background-color: %1;
                    border: 1px solid %3;
                }
                QLabel {
                    color: %2;
                    font-family: "Courier New";
                }
            )").arg(tema.colorFondo.name(), tema.colorTexto.name(), tema.colorPrimario.name());
            break;
    }

    return estilo + tema.estiloBotonPrimario + tema.estiloBotonSecundario + 
           tema.estiloSlider + tema.estiloLista;
}
