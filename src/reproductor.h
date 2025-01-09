#ifndef REPRODUCTOR_H
#define REPRODUCTOR_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

/**
 * @brief Clase que maneja la reproducción de archivos de audio
 * 
 * Esta clase encapsula la funcionalidad de reproducción de audio,
 * incluyendo manejo de errores y validación de formatos soportados.
 */
class Reproductor : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Objeto padre para la gestión de memoria
     * @throws std::runtime_error si hay error al inicializar componentes
     */
    explicit Reproductor(QObject *parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~Reproductor();

    /**
     * @brief Carga un archivo de audio para reproducción
     * @param ruta Ruta al archivo de audio
     */
    void cargarArchivo(const QString &ruta);
    
    /**
     * @brief Inicia la reproducción del archivo cargado
     */
    void reproducir();
    
    /**
     * @brief Pausa la reproducción actual
     */
    void pausar();
    
    /**
     * @brief Obtiene el reproductor multimedia
     * @return Puntero al reproductor multimedia
     */
    QMediaPlayer* obtenerMediaPlayer() { return reproductor; }
    
    /**
     * @brief Obtiene la salida de audio
     * @return Puntero a la salida de audio
     */
    QAudioOutput* obtenerAudioOutput() { return salidaAudio; }

signals:
    /**
     * @brief Señal emitida cuando ocurre un error
     * @param mensaje Descripción del error
     */
    void errorReproductor(const QString &mensaje);

private slots:
    /**
     * @brief Maneja los errores del reproductor multimedia
     * @param error Tipo de error ocurrido
     */
    void manejarError(QMediaPlayer::Error error);

private:
    QMediaPlayer *reproductor;
    QAudioOutput *salidaAudio;
};

#endif // REPRODUCTOR_H
