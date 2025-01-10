#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTreeWidgetItem>
#include <QSettings>
#include <QDialog>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QStatusBar>
#include <QLabel>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Enumeración para tipos de columnas
enum ColumnType {
    Title = 0,
    Artist = 1,
    Album = 2,
    Genre = 3,
    Date = 4,
    Duration = 5,
    InitialKey = 6
};

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

private slots:
    void on_actionAbrirArchivo_triggered();
    void on_playButton_clicked();
    void on_volumeSlider_valueChanged(int value);
    void on_progressSlider_sliderMoved(int position);
    void on_playlistWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_nextButton_clicked();
    void on_previousButton_clicked();
    void on_actionColumnas_triggered();
    void actualizarEstadoReproduccion(QMediaPlayer::PlaybackState state);
    void actualizarBitrate(qint64 bitrate);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *reproductor;
    QAudioOutput *audioOutput;
    QLabel *statusLabel;
    QLabel *bitrateLabel;
    
    // Métodos privados
    void configurarReproductor();
    void configurarInterfaz();
    void configurarConexiones();
    void reproducir();
    void pausar();
    void actualizarProgreso(qint64 posicion);
    void actualizarDuracion(qint64 duracion);
    void agregarCancion(const QString& ruta);
    void guardarConfiguracionColumnas();
    void cargarConfiguracionColumnas();
    void actualizarColumnasVisibles();
    void actualizarItemColumnas(QTreeWidgetItem* item);
    void procesarDirectorio(const QString& directorio);
    bool esArchivoMusica(const QString& ruta) const;
    QString obtenerNombreCancion(const QString& ruta) const;
    void actualizarBarraEstado(const QString& mensaje);
    QString limpiarNombreCancion(const QString& nombre) const;
    QString normalizarTonalidad(const QString& tonalidad) const;
    
    // Estructura para la configuración de columnas
    struct ColumnInfo {
        QString name;
        bool visible;
        int width;
    };
    
    // Configuración de columnas con nombres en español
    QMap<ColumnType, ColumnInfo> columnConfig = {
        {Title, {"Título", true, 300}},
        {Artist, {"Artista", true, 200}},
        {Album, {"Álbum", true, 200}},
        {Genre, {"Género", true, 150}},
        {Date, {"Fecha", true, 100}},
        {Duration, {"Duración", true, 100}},
        {InitialKey, {"Tonalidad", true, 100}}
    };
};

#endif // MAINWINDOW_H
