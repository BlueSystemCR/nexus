#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDir>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , reproductor(new GStreamerPlayer(this))
    , statusLabel(nullptr)
    , bitrateLabel(nullptr)
{
    ui->setupUi(this);
    configurarReproductor();
    configurarInterfaz();
    configurarConexiones();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        bool primeraCancion = ui->playlistWidget->count() == 0;
        const QList<QUrl> urls = event->mimeData()->urls();
        for (const QUrl &url : urls) {
            QString ruta = url.toLocalFile();
            QFileInfo fileInfo(ruta);
            if (fileInfo.isDir()) {
                procesarDirectorio(ruta);
            } else if (esArchivoMusica(ruta)) {
                agregarCancion(ruta, primeraCancion);
                primeraCancion = false;
            }
        }
        event->acceptProposedAction();
    }
}

void MainWindow::procesarDirectorio(const QString &ruta)
{
    QDir dir(ruta);
    QFileInfoList entradas = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    bool primeraCancion = ui->playlistWidget->count() == 0;
    
    QFileInfoList::const_iterator it;
    for (it = entradas.constBegin(); it != entradas.constEnd(); ++it) {
        if (it->isDir()) {
            procesarDirectorio(it->absoluteFilePath());
        } else if (esArchivoMusica(it->absoluteFilePath())) {
            agregarCancion(it->absoluteFilePath(), primeraCancion);
            primeraCancion = false;
        }
    }
}

void MainWindow::configurarReproductor()
{
    // Conectar señales del reproductor
    connect(reproductor, &GStreamerPlayer::error, this, &MainWindow::manejarErrorReproduccion);
    connect(reproductor, &GStreamerPlayer::stateChanged, this, &MainWindow::actualizarEstadoReproduccion);
    connect(reproductor, &GStreamerPlayer::positionChanged, this, &MainWindow::actualizarProgreso);
    connect(reproductor, &GStreamerPlayer::durationChanged, this, &MainWindow::actualizarDuracion);
}

void MainWindow::configurarInterfaz()
{
    // Configurar el slider de volumen
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);
    
    // Configurar el slider de progreso
    ui->progressSlider->setRange(0, 0);
    
    // Configurar las etiquetas de tiempo
    ui->timeLabel->setText("00:00");
    ui->totalTimeLabel->setText("00:00");
    
    // Configurar la barra de estado
    statusLabel = new QLabel(this);
    bitrateLabel = new QLabel(this);
    ui->statusbar->addWidget(statusLabel);
    ui->statusbar->addPermanentWidget(bitrateLabel);
    
    // Configurar el botón de reproducción
    ui->playButton->setText("▶");
    
    // Configurar el widget de lista de reproducción
    ui->playlistWidget->setAcceptDrops(true);
    ui->playlistWidget->setDragEnabled(true);
}

void MainWindow::configurarConexiones()
{
    // Conexiones para el progreso y duración
    connect(reproductor, &GStreamerPlayer::positionChanged, this, &MainWindow::actualizarProgreso);
    connect(reproductor, &GStreamerPlayer::durationChanged, this, &MainWindow::actualizarDuracion);
    
    // Conexiones para el estado de reproducción
    connect(reproductor, &GStreamerPlayer::stateChanged, this, &MainWindow::actualizarEstadoReproduccion);
    
    // Conexiones para manejo de errores
    connect(reproductor, &GStreamerPlayer::error, this, &MainWindow::manejarErrorReproduccion);
}

void MainWindow::actualizarBarraEstado(const QString &mensaje)
{
    if (statusLabel) {
        statusLabel->setText(mensaje);
    }
}

void MainWindow::agregarCancion(const QString &ruta, bool primeraCancion)
{
    if (!esArchivoMusica(ruta)) {
        return;
    }

    QString nombreCancion = obtenerNombreCancion(ruta);
    QListWidgetItem *item = new QListWidgetItem(nombreCancion);
    item->setData(Qt::UserRole, ruta);
    ui->playlistWidget->addItem(item);

    if (primeraCancion) {
        ui->playlistWidget->setCurrentItem(item);
        reproducir();
    }
}

bool MainWindow::esArchivoMusica(const QString &ruta) const
{
    QFileInfo fileInfo(ruta);
    QString extension = fileInfo.suffix().toLower();
    return extension == "mp3" || extension == "wav" || extension == "ogg" || extension == "flac";
}

QString MainWindow::obtenerNombreCancion(const QString &ruta) const
{
    QFileInfo fileInfo(ruta);
    return fileInfo.baseName();
}

void MainWindow::on_actionAgregar_Archivos_triggered()
{
    QStringList archivos = QFileDialog::getOpenFileNames(
        this,
        "Seleccionar archivos de música",
        QDir::homePath(),
        "Archivos de música (*.mp3 *.wav *.ogg *.flac);;Todos los archivos (*.*)"
    );

    for (const QString& archivo : archivos) {
        agregarCancion(archivo, true);
    }
}

void MainWindow::on_actionAgregar_Carpeta_triggered()
{
    QString carpeta = QFileDialog::getExistingDirectory(
        this,
        "Seleccionar carpeta de música",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (!carpeta.isEmpty()) {
        procesarDirectorio(carpeta);
    }
}

void MainWindow::on_actionSalir_triggered()
{
    close();
}

void MainWindow::on_actionAcerca_de_triggered()
{
    QMessageBox::about(this, "Acerca de Reproductor de Música",
                      "Reproductor de Música v1.0\n\n"
                      "Un reproductor de música simple y eficiente.\n"
                      "Soporta formatos MP3, WAV, OGG y FLAC.");
}

void MainWindow::on_actionLimpiar_Lista_triggered()
{
    ui->playlistWidget->clear();
    reproductor->stop();
    actualizarBarraEstado("Lista de reproducción limpiada");
}

void MainWindow::on_playlistWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item) {
        ui->playlistWidget->setCurrentItem(item);
        QString ruta = item->data(Qt::UserRole).toString();
        if (!ruta.isEmpty()) {
            reproductor->stop();
            reproductor->play(ruta);
        }
    }
}

void MainWindow::reproducir()
{
    if (ui->playlistWidget->currentItem()) {
        QString ruta = ui->playlistWidget->currentItem()->data(Qt::UserRole).toString();
        qDebug() << "Intentando reproducir archivo:" << ruta;
        
        if (reproductor->play(ruta)) {
            ui->playButton->setText("⏸");
            actualizarBarraEstado("Reproduciendo: " + obtenerNombreCancion(ruta));
        }
    }
}

void MainWindow::on_playButton_clicked()
{
    if (ui->playButton->text() == "▶") {
        reproducir();
    } else {
        reproductor->pause();
        ui->playButton->setText("▶");
        actualizarBarraEstado("Pausado");
    }
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    double volumen = value / 100.0;
    reproductor->setVolume(volumen);
    qDebug() << "Volumen ajustado a:" << volumen;
}

void MainWindow::actualizarEstadoReproduccion(const QString &estado)
{
    if (estado == "playing") {
        actualizarBarraEstado("Reproduciendo");
        ui->playButton->setText("⏸");
    } else if (estado == "paused") {
        actualizarBarraEstado("Pausado");
        ui->playButton->setText("▶");
    } else if (estado == "stopped") {
        actualizarBarraEstado("Detenido");
        ui->playButton->setText("▶");
    }
}

void MainWindow::manejarErrorReproduccion(const QString &error)
{
    qDebug() << "Error de reproducción:" << error;
    actualizarBarraEstado("Error: " + error);
    QMessageBox::warning(this, "Error de reproducción", error);
}

void MainWindow::actualizarProgreso(qint64 posicion)
{
    if (ui->progressSlider->maximum() > 0) {
        ui->progressSlider->setValue(posicion);
    }
    
    // Actualizar etiqueta de tiempo
    QTime tiempo(0, 0);
    tiempo = tiempo.addMSecs(posicion);
    ui->timeLabel->setText(tiempo.toString("mm:ss"));
}

void MainWindow::actualizarDuracion(qint64 duracion)
{
    ui->progressSlider->setMaximum(duracion);
    
    // Actualizar etiqueta de duración total
    QTime tiempo(0, 0);
    tiempo = tiempo.addMSecs(duracion);
    ui->totalTimeLabel->setText(tiempo.toString("mm:ss"));
}

void MainWindow::on_addButton_clicked()
{
    QStringList archivos = QFileDialog::getOpenFileNames(
        this,
        tr("Seleccionar archivos de música"),
        QDir::homePath(),
        tr("Archivos de música (*.mp3 *.wav *.ogg *.flac);;Todos los archivos (*.*)")
    );

    if (!archivos.isEmpty()) {
        agregarArchivosALista(archivos);
    }
}

void MainWindow::on_nextButton_clicked()
{
    int currentRow = ui->playlistWidget->currentRow();
    int nextRow = (currentRow + 1) % ui->playlistWidget->count();
    ui->playlistWidget->setCurrentRow(nextRow);
    reproducir();
}

void MainWindow::on_previousButton_clicked()
{
    int currentRow = ui->playlistWidget->currentRow();
    int prevRow = (currentRow - 1 + ui->playlistWidget->count()) % ui->playlistWidget->count();
    ui->playlistWidget->setCurrentRow(prevRow);
    reproducir();
}

void MainWindow::on_actionColumnas_triggered()
{
    actualizarColumnasVisibles();
}
