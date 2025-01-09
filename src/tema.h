#ifndef TEMA_H
#define TEMA_H

#include <QString>
#include <QColor>
#include <QFont>

class Tema {
public:
    enum TipoTema {
        AppleMusic,
        Spotify,
        Winamp
    };

    struct EstiloTema {
        // Colores principales
        QColor colorFondo;
        QColor colorPrimario;
        QColor colorSecundario;
        QColor colorTexto;
        QColor colorAcento;
        
        // Fuentes
        QFont fuenteTitulo;
        QFont fuenteNormal;
        
        // Radios de bordes
        int radioBordes;
        
        // Estilos específicos
        QString estiloBotonPrimario;
        QString estiloBotonSecundario;
        QString estiloSlider;
        QString estiloLista;
    };

    static EstiloTema obtenerTema(TipoTema tipo);
    static QString obtenerHojaEstilo(TipoTema tipo);

private:
    static EstiloTema crearTemaAppleMusic();
    static EstiloTema crearTemaSpotify();
    static EstiloTema crearTemaWinamp();
};

#endif // TEMA_H
