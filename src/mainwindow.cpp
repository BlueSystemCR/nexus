#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QDirIterator>
#include <QInputDialog>
#include <QDebug>
#include <QThread>
#include "audioconverter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , reproductor(nullptr)
    , audioOutput(nullptr)
    , audioConverter(nullptr)
    , progressDialog(nullptr)
    , timer(new QTimer(this))  // Inicializar timer
    , reproduciendo(false)
    , duracionActual(0)
    , autoPlayEnabled(true)
    , sliderPressed(false)
    , tamanoArchivoActual(0)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    qDebug() << "AcceptDrops:" << acceptDrops();
    
    audioConverter = new AudioConverter(this);
    progressDialog = new QProgressDialog("Convirtiendo archivo...", "Cancelar", 0, 100, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setAutoClose(true);
    progressDialog->setAutoReset(true);
    progressDialog->setMinimumDuration(500);
    progressDialog->hide(); // Asegurarnos que el diálogo está oculto al inicio
    
    configurarReproductor();
    configurarInterfaz();
    configurarConexiones();
    configurarShortcuts();
    
    // Conectar señales del convertidor de audio
    connect(audioConverter, &AudioConverter::conversionProgress,
            this, [this](int progreso) {
                if (progressDialog && progressDialog->isVisible()) {
                    progressDialog->setValue(progreso);
                    
                    // Estimar tiempo restante
                    if (conversionTimer.isValid()) {
                        qint64 tiempoTranscurrido = conversionTimer.elapsed();
                        if (progreso > 0) {
                            qint64 tiempoEstimadoTotal = (tiempoTranscurrido * 100) / progreso;
                            qint64 tiempoRestante = tiempoEstimadoTotal - tiempoTranscurrido;
                            QString mensaje = QString("Convirtiendo archivo... %1% (Restante: %2s)")
                                .arg(progreso)
                                .arg(tiempoRestante / 1000);
                            progressDialog->setLabelText(mensaje);
                        }
                    }
                }
            });
            
    connect(audioConverter, &AudioConverter::conversionFinished,
            this, [this](const QString& outputFile) {
                ocultarProgresoConversion();
                reproducirArchivo(outputFile);
                optimizarMemoria();
            });
            
    connect(audioConverter, &AudioConverter::conversionError,
            this, [this](const QString& error) {
                ocultarProgresoConversion();
                QMessageBox::warning(this, "Error de conversión", error);
            });
    
    // Establecer el mensaje inicial en la barra de estado
    ui->statusbar->showMessage("Listo para reproducir música");
    
    qDebug() << "MainWindow inicializado";
}

void MainWindow::configurarReproductor()
{
    reproductor = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    reproductor->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    ui->volumeSlider->setValue(50);

    // Conectar señales del reproductor
    connect(reproductor, &QMediaPlayer::positionChanged, this, &MainWindow::actualizarProgreso);
    connect(reproductor, &QMediaPlayer::durationChanged, this, &MainWindow::actualizarDuracion);
    connect(reproductor, &QMediaPlayer::playbackStateChanged, [this](QMediaPlayer::PlaybackState state) {
        actualizarEstadoReproduccion(state == QMediaPlayer::PlayingState);
    });
    connect(reproductor, &QMediaPlayer::errorOccurred, this, &MainWindow::manejarError);
    connect(reproductor, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status) {
        qDebug() << "Estado del medio:" << status;
    });

    qDebug() << "Reproductor configurado con soporte para formatos:";
    QStringList formatos = {"mp3", "wav", "ogg", "m4a", "flac", "aac"};
    for (const QString& formato : formatos) {
        qDebug() << " -" << formato;
    }
}

void MainWindow::configurarConexiones() {
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarInterfaz);
    timer->start(1000);  // Actualizar cada segundo
    
    connect(&playlist, &PlayList::listaModificada, this, &MainWindow::actualizarInterfaz);
}

void MainWindow::configurarInterfaz()
{
    // Configurar la lista de reproducción
    ui->playlistWidget->setColumnCount(5);
    QStringList headers;
    headers << "Título" << "Artista" << "Álbum" << "Comentarios" << "Duración";
    ui->playlistWidget->setHeaderLabels(headers);
    ui->playlistWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->playlistWidget->setAlternatingRowColors(true);
    
    // Configurar el diálogo de progreso
    if (progressDialog) {
        progressDialog->close();
        progressDialog->hide();
        progressDialog->reset();
    }
    
    // Configurar la barra de estado
    ui->statusbar->showMessage("Listo para reproducir música");
    
    // Configurar los botones
    ui->playButton->setText("▶");
    ui->nextButton->setText("⏭");
    ui->previousButton->setText("⏮");
    
    // Configurar el slider de volumen
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);
    
    // Configurar el slider de progreso
    ui->progressSlider->setRange(0, 0);
    
    // Configurar el ancho de las columnas
    ui->playlistWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);  // Título
    ui->playlistWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);  // Artista
    ui->playlistWidget->header()->setSectionResizeMode(2, QHeaderView::Stretch);  // Álbum
    ui->playlistWidget->header()->setSectionResizeMode(3, QHeaderView::Stretch);  // Comentarios
    ui->playlistWidget->header()->setSectionResizeMode(4, QHeaderView::Fixed);    // Duración
    ui->playlistWidget->header()->resizeSection(4, 100);  // Ancho fijo para duración
}

void MainWindow::configurarShortcuts() {
    // Crear los shortcuts
    nextTrackShortcut = new QShortcut(QKeySequence(Qt::Key_Right), this);
    prevTrackShortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    playPauseShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    quickPlayNextShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right), this);
    
    // Conectar los shortcuts
    connect(nextTrackShortcut, &QShortcut::activated, this, &MainWindow::reproducirSiguiente);
    connect(prevTrackShortcut, &QShortcut::activated, this, &MainWindow::reproducirAnterior);
    connect(playPauseShortcut, &QShortcut::activated, this, &MainWindow::reproducirPausa);
    connect(quickPlayNextShortcut, &QShortcut::activated, this, &MainWindow::reproducirRapido);
    
    // Debug de shortcuts
    qDebug() << "Estado de los shortcuts:";
    qDebug() << "Next Track:" << nextTrackShortcut->isEnabled();
    qDebug() << "Prev Track:" << prevTrackShortcut->isEnabled();
    qDebug() << "Play/Pause:" << playPauseShortcut->isEnabled();
    qDebug() << "Quick Play:" << quickPlayNextShortcut->isEnabled();
}

void MainWindow::on_actionAbrirArchivo_triggered() {
    QStringList archivos = QFileDialog::getOpenFileNames(
        this,
        "Seleccionar archivos de música",
        QDir::homePath(),
        "Archivos de música (*.mp3 *.wav *.ogg *.m4a *.flac);;Todos los archivos (*.*)"
    );
    
    for (const QString& archivo : archivos) {
        agregarCancion(archivo);
    }
}

void MainWindow::on_actionAbrirDirectorio_triggered() {
    QString directorio = QFileDialog::getExistingDirectory(
        this,
        "Seleccionar directorio",
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!directorio.isEmpty()) {
        procesarDirectorio(directorio);
    }
}

void MainWindow::on_actionSalir_triggered() {
    close();
}

void MainWindow::on_nextButton_clicked() {
    reproducirSiguiente();
}

void MainWindow::on_previousButton_clicked() {
    reproducirAnterior();
}

void MainWindow::reproducirPausa() {
    if (reproductor->playbackState() == QMediaPlayer::PlayingState) {
        reproductor->pause();
    } else {
        reproducir();
    }
}

void MainWindow::reproducir()
{
    QString cancionActual = playlist.obtenerCancionActual();
    if (!cancionActual.isEmpty()) {
        QFileInfo fileInfo(cancionActual);
        QString extension = fileInfo.suffix().toLower();
        
        // Actualizar la interfaz antes de iniciar la reproducción
        actualizarInterfaz();
        ui->statusbar->showMessage("Preparando archivo para reproducción...");
        
        if (extension == "flac") {
            qDebug() << "Detectado archivo FLAC, iniciando conversión...";
            ui->statusbar->showMessage("Convirtiendo archivo FLAC...");
            limpiarArchivoTemporal();
            mostrarProgresoConversion(cancionActual);
            archivoTemporal = audioConverter->convertFlacToWav(cancionActual);
        } else {
            reproducirArchivo(cancionActual);
        }
    }
}

void MainWindow::reproducirArchivo(const QString& archivo)
{
    qDebug() << "Reproduciendo:" << archivo;
    ui->statusbar->showMessage("Cargando archivo...");
    
    // Verificar que el archivo existe
    QFileInfo fileInfo(archivo);
    if (!fileInfo.exists()) {
        qDebug() << "Error: El archivo no existe";
        QMessageBox::warning(this, "Error", "El archivo no existe:\n" + archivo);
        ui->statusbar->showMessage("Error: Archivo no encontrado");
        return;
    }
    
    // Verificar que el archivo es legible
    QFile file(archivo);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error: No se puede leer el archivo";
        QMessageBox::warning(this, "Error", "No se puede leer el archivo:\n" + archivo);
        ui->statusbar->showMessage("Error: No se puede leer el archivo");
        return;
    }
    file.close();
    
    QUrl url = QUrl::fromLocalFile(archivo);
    qDebug() << "URL del archivo:" << url.toString();
    qDebug() << "Tamaño del archivo:" << fileInfo.size() << "bytes";
    qDebug() << "Extensión del archivo:" << fileInfo.suffix().toLower();
    
    reproductor->setSource(url);
    reproductor->play();
    reproduciendo = true;
    ui->playButton->setText("⏸");
    actualizarInterfaz();
}

void MainWindow::reproducirSiguiente()
{
    if (playlist.avanzar()) {
        reproducir();
        actualizarInterfaz();
    }
}

void MainWindow::reproducirAnterior()
{
    if (playlist.retroceder()) {
        reproducir();
        actualizarInterfaz();
    }
}

void MainWindow::reproducirRapido() {
    reproducirSiguiente();
    reproducir();
}

void MainWindow::agregarCancion(const QString& ruta)
{
    QFileInfo fileInfo(ruta);
    if (fileInfo.exists() && esArchivoMusica(ruta)) {
        qDebug() << "Agregando canción:" << ruta;
        AudioMetadata metadata = metadataManager.obtenerMetadata(ruta);
        
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->playlistWidget);
        item->setText(0, metadata.titulo);
        item->setText(1, metadata.artista);
        item->setText(2, metadata.album);
        item->setText(3, metadata.comentario);
        item->setText(4, metadata.duracion);
        item->setData(0, Qt::UserRole, ruta);
        
        playlist.agregarCancion(ruta);
    } else {
        qDebug() << "Archivo no válido o no es un archivo de música:" << ruta;
    }
}

void MainWindow::on_playlistWidget_itemDoubleClicked(QTreeWidgetItem *item, int column) {
    if (!item) {
        qDebug() << "Item nulo";
    } else {
        int indice = ui->playlistWidget->indexOfTopLevelItem(item);
        if (indice != -1) {
            playlist.establecerIndiceActual(indice);
            reproducir();
        }
    }
}

void MainWindow::actualizarEstadoReproduccion(bool reproduciendo) {
    this->reproduciendo = reproduciendo;
    ui->playButton->setIcon(style()->standardIcon(
        reproduciendo ? QStyle::SP_MediaPause : QStyle::SP_MediaPlay
    ));
}

void MainWindow::actualizarProgreso(qint64 posicion) {
    if (!ui->progressSlider->isSliderDown()) {
        ui->progressSlider->setValue(posicion);
    }
}

void MainWindow::actualizarDuracion(qint64 duracion) {
    ui->progressSlider->setRange(0, duracion);
    duracionActual = duracion;
}

void MainWindow::on_volumeSlider_valueChanged(int value) {
    audioOutput->setVolume(value / 100.0);
}

void MainWindow::on_progressSlider_sliderMoved(int position) {
    reproductor->setPosition(position);
}

void MainWindow::actualizarInterfaz()
{
    // Actualizar lista de reproducción
    QTreeWidgetItem* currentItem = ui->playlistWidget->currentItem();
    int currentIndex = playlist.obtenerIndiceActual();
    
    // Si el índice actual es válido pero no coincide con la selección
    if (currentIndex >= 0 && (!currentItem || ui->playlistWidget->indexOfTopLevelItem(currentItem) != currentIndex)) {
        QTreeWidgetItem* newItem = ui->playlistWidget->topLevelItem(currentIndex);
        if (newItem) {
            ui->playlistWidget->setCurrentItem(newItem);
            ui->playlistWidget->scrollToItem(newItem);
        }
    }
    
    // Actualizar barra de estado
    QString mensaje;
    if (reproduciendo) {
        QString cancionActual = playlist.obtenerCancionActual();
        QFileInfo fileInfo(cancionActual);
        mensaje = QString("Reproduciendo: %1").arg(fileInfo.fileName());
        
        // Agregar información del formato
        QString formato = fileInfo.suffix().toLower();
        mensaje += QString(" [%1]").arg(formato.toUpper());
        
        // Agregar duración si está disponible
        if (duracionActual > 0) {
            int minutos = (duracionActual / 60000);
            int segundos = (duracionActual % 60000) / 1000;
            mensaje += QString(" - %1:%2").arg(minutos).arg(segundos, 2, 10, QChar('0'));
        }
    } else {
        mensaje = "Listo";
    }
    ui->statusbar->showMessage(mensaje);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    qDebug() << "dragEnterEvent recibido";
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
    qDebug() << "dragMoveEvent recibido";
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    qDebug() << "dropEvent recibido";
    const QMimeData* mimeData = event->mimeData();
    
    if (mimeData->hasUrls()) {
        qDebug() << "El evento tiene URLs";
        QList<QUrl> urlList = mimeData->urls();
        
        for (const QUrl& url : urlList) {
            QString ruta = url.toLocalFile();
            qDebug() << "Procesando URL:" << url;
            
            QFileInfo fileInfo(ruta);
            if (fileInfo.isDir()) {
                procesarDirectorio(ruta);
            } else if (esArchivoMusica(ruta)) {
                agregarCancion(ruta);
            }
        }
    }
}

void MainWindow::procesarDirectorio(const QString& directorio)
{
    QDir dir(directorio);
    if (!dir.exists()) {
        qDebug() << "El directorio no existe:" << directorio;
        return;
    }

    int contadorArchivos = 0;
    QDirIterator it(directorio, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    
    while (it.hasNext()) {
        QString ruta = it.next();
        if (esArchivoMusica(ruta)) {
            agregarCancion(ruta);
            contadorArchivos++;
        }
    }
    
    qDebug() << "Se agregaron" << contadorArchivos << "archivos";
}

bool MainWindow::esArchivoMusica(const QString& ruta) const
{
    QString extension = QFileInfo(ruta).suffix().toLower();
    QStringList formatosSoportados = {
        "mp3", "wav", "ogg", "m4a", "flac", "aac", "aiff", "wma", "opus"
    };
    
    bool esFormato = formatosSoportados.contains(extension);
    if (!esFormato) {
        qDebug() << "Formato no soportado:" << extension << "para archivo:" << ruta;
    }
    return esFormato;
}

void MainWindow::manejarError(QMediaPlayer::Error error, const QString& errorString)
{
    QString mensaje;
    switch (error) {
        case QMediaPlayer::NoError:
            return;
        case QMediaPlayer::ResourceError:
            mensaje = "Error al acceder al archivo de audio";
            break;
        case QMediaPlayer::FormatError:
            mensaje = "Formato de audio no soportado o archivo corrupto";
            break;
        case QMediaPlayer::NetworkError:
            mensaje = "Error de red al reproducir el archivo";
            break;
        case QMediaPlayer::AccessDeniedError:
            mensaje = "Acceso denegado al archivo de audio";
            break;
        default:
            mensaje = "Error desconocido";
    }

    qDebug() << "Error en el reproductor:" << error << "-" << errorString;
    qDebug() << "Archivo actual:" << playlist.obtenerCancionActual();
    
    // Obtener información adicional del archivo
    QFileInfo fileInfo(playlist.obtenerCancionActual());
    qDebug() << "Información del archivo:";
    qDebug() << " - Tamaño:" << fileInfo.size() << "bytes";
    qDebug() << " - Permisos:" << fileInfo.permissions();
    qDebug() << " - Última modificación:" << fileInfo.lastModified().toString();
    
    QMessageBox::warning(this, "Error de reproducción", 
                        mensaje + "\n\nDetalles: " + errorString + 
                        "\n\nArchivo: " + fileInfo.fileName());

    // Intentar reproducir la siguiente canción si hay error
    if (autoPlayEnabled) {
        reproducirSiguiente();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::limpiarArchivoTemporal()
{
    if (!archivoTemporal.isEmpty()) {
        QFile::remove(archivoTemporal);
        archivoTemporal.clear();
    }
    audioConverter->cleanupTempFiles();
}

void MainWindow::mostrarProgresoConversion(const QString& nombreArchivo)
{
    if (!progressDialog) {
        return;
    }
    
    QFileInfo fileInfo(nombreArchivo);
    QString mensaje = QString("Convirtiendo %1...").arg(fileInfo.fileName());
    progressDialog->setLabelText(mensaje);
    progressDialog->setValue(0);
    progressDialog->reset();
    
    // Solo mostrar si no está visible
    if (!progressDialog->isVisible()) {
        progressDialog->show();
        conversionTimer.start();
    }
}

void MainWindow::ocultarProgresoConversion()
{
    if (!progressDialog) {
        return;
    }
    
    progressDialog->reset();
    progressDialog->close();
    progressDialog->hide();
    conversionTimer.invalidate();
    
    // Actualizar la barra de estado
    if (!reproduciendo) {
        ui->statusbar->showMessage("Listo para reproducir música");
    }
}

void MainWindow::optimizarMemoria()
{
    // Liberar memoria no utilizada
    if (tamanoArchivoActual > 100 * 1024 * 1024) { // Si el archivo es mayor a 100MB
        qDebug() << "Optimizando uso de memoria...";
        QApplication::processEvents();
        
        // En macOS, usamos una estrategia diferente para liberar memoria
        QThread::msleep(100); // Dar tiempo al sistema para liberar recursos
        QApplication::processEvents();
    }
}

void MainWindow::on_playButton_clicked()
{
    if (reproductor->playbackState() == QMediaPlayer::PlayingState) {
        reproductor->pause();
        reproduciendo = false;
        ui->playButton->setText("▶");
        ui->statusbar->showMessage("Reproducción pausada");
    } else {
        if (reproductor->playbackState() == QMediaPlayer::StoppedState) {
            reproducir();
        } else {
            reproductor->play();
            reproduciendo = true;
            ui->playButton->setText("⏸");
        }
    }
    actualizarInterfaz();
}

MainWindow::~MainWindow()
{
    limpiarArchivoTemporal();
    delete ui;
    delete reproductor;
    delete audioOutput;
    delete timer;
    delete nextTrackShortcut;
    delete prevTrackShortcut;
    delete playPauseShortcut;
    delete quickPlayNextShortcut;
}
