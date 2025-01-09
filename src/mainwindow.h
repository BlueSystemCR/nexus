#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QProgressDialog>
#include <QElapsedTimer>
#include <QTreeWidgetItem>
#include <QShortcut>
#include <QTimer>
#include <QMessageBox>
#include "playlist.h"
#include "metadatamanager.h"
#include "audioconverter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_actionAbrirArchivo_triggered();
    void on_actionAbrirDirectorio_triggered();
    void on_actionSalir_triggered();
    void on_playButton_clicked();
    void on_nextButton_clicked();
    void on_previousButton_clicked();
    void on_volumeSlider_valueChanged(int value);
    void on_progressSlider_sliderMoved(int position);
    void on_playlistWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void manejarError(QMediaPlayer::Error error, const QString& errorString);

private:
    void configurarReproductor();
    void configurarConexiones();
    void configurarInterfaz();
    void configurarShortcuts();
    void reproducirPausa();
    void reproducir();
    void reproducirSiguiente();
    void reproducirAnterior();
    void reproducirRapido();
    void agregarCancion(const QString& ruta);
    void procesarDirectorio(const QString& directorio);
    void actualizarEstadoReproduccion(bool reproduciendo);
    void actualizarProgreso(qint64 posicion);
    void actualizarDuracion(qint64 duracion);
    void actualizarInterfaz();
    void mostrarAnalisis();
    bool esArchivoMusica(const QString& ruta) const;
    void limpiarArchivoTemporal();
    void reproducirArchivo(const QString& archivo);
    void mostrarProgresoConversion(const QString& archivo);
    void ocultarProgresoConversion();
    void optimizarMemoria();

    Ui::MainWindow *ui;
    QMediaPlayer *reproductor;
    QAudioOutput *audioOutput;
    PlayList playlist;
    MetadataManager metadataManager;
    AudioConverter *audioConverter;
    QProgressDialog *progressDialog;
    QElapsedTimer conversionTimer;
    QTimer *timer;
    QShortcut *nextTrackShortcut;
    QShortcut *prevTrackShortcut;
    QShortcut *playPauseShortcut;
    QShortcut *quickPlayNextShortcut;
    bool reproduciendo;
    qint64 duracionActual;
    bool autoPlayEnabled;
    bool sliderPressed;
    QString archivoTemporal;
    qint64 tamanoArchivoActual;
};

#endif // MAINWINDOW_H
