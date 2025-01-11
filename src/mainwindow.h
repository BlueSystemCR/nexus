#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Includes del sistema
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QLabel>
#include "gstreamerplayer.h"
#include <QListWidget>
#include <QMessageBox>
#include <QTime>

// Includes de Qt
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
#include <QFileInfo>
#include <QListWidgetItem>
#include <QStringList>

// Includes del proyecto
#include "metadatamanager.h"

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
    void on_addButton_clicked();
    void on_playButton_clicked();
    void on_volumeSlider_valueChanged(int value);
    void on_actionAgregar_Archivos_triggered();
    void on_actionAgregar_Carpeta_triggered();
    void on_actionSalir_triggered();
    void on_actionAcerca_de_triggered();
    void on_actionLimpiar_Lista_triggered();
    void on_playlistWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_nextButton_clicked();
    void on_previousButton_clicked();
    void on_actionColumnas_triggered();
    
    // Slots para manejo de medios
    void actualizarProgreso(qint64 posicion);
    void actualizarDuracion(qint64 duracion);
    void actualizarEstadoReproduccion(const QString &estado);
    void manejarErrorReproduccion(const QString &error);

private:
    Ui::MainWindow *ui;
    GStreamerPlayer *reproductor;
    QAudioOutput *audioOutput;
    QLabel *statusLabel;
    QLabel *bitrateLabel;
    
    // Métodos privados
    void configurarReproductor();
    void configurarInterfaz();
    void configurarConexiones();
    void reproducir();
    void pausar();
    void agregarCancion(const QString& ruta, bool primeraCancion = false);
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
        {Title, {"Título", true, 250}},
        {Artist, {"Artista", true, 150}},
        {Album, {"Álbum", true, 150}},
        {Genre, {"Género", true, 100}},
        {Date, {"Fecha", true, 70}},
        {Duration, {"Duración", true, 70}},
        {InitialKey, {"Tonalidad", true, 80}}
    };
};

#endif // MAINWINDOW_H
