#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "metadatamanager.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QTime>
#include <QDir>
#include <QDirIterator>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , reproductor(new QMediaPlayer(this))
    , audioOutput(new QAudioOutput(this))
    , statusLabel(nullptr)
    , bitrateLabel(nullptr)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    configurarReproductor();
    configurarInterfaz();
    configurarConexiones();
}

MainWindow::~MainWindow()
{
    guardarConfiguracionColumnas();
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
    const QMimeData* mimeData = event->mimeData();
    
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        
        bool primeraCancion = true;
        for (const QUrl& url : urlList) {
            QString ruta = url.toLocalFile();
            
            QFileInfo fileInfo(ruta);
            if (fileInfo.isDir()) {
                procesarDirectorio(ruta);
            } else if (esArchivoMusica(ruta)) {
                agregarCancion(ruta);
                if (primeraCancion && reproductor->playbackState() != QMediaPlayer::PlayingState) {
                    reproducir();
                    primeraCancion = false;
                }
            }
        }
        event->acceptProposedAction();
    }
}

void MainWindow::procesarDirectorio(const QString& directorio)
{
    QDirIterator it(directorio, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    bool primeraCancion = ui->playlistWidget->topLevelItemCount() == 0;
    
    while (it.hasNext()) {
        QString ruta = it.next();
        if (esArchivoMusica(ruta)) {
            agregarCancion(ruta);
        }
    }
    
    // Si no había canciones antes y ahora hay, reproducir la primera
    if (primeraCancion && ui->playlistWidget->topLevelItemCount() > 0) {
        ui->playlistWidget->setCurrentItem(ui->playlistWidget->topLevelItem(0));
        reproducir();
    }
}

bool MainWindow::esArchivoMusica(const QString& ruta) const
{
    QString extension = QFileInfo(ruta).suffix().toLower();
    QStringList formatosSoportados = {
        "mp3", "wav", "ogg", "m4a", "flac", "aac", "aiff", "wma", "opus"
    };
    return formatosSoportados.contains(extension);
}

void MainWindow::configurarReproductor()
{
    reproductor->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    ui->volumeSlider->setValue(50);
}

void MainWindow::configurarInterfaz()
{
    // Configurar la lista de reproducción
    ui->playlistWidget->setColumnCount(7);
    ui->playlistWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->playlistWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playlistWidget->setAlternatingRowColors(true);
    ui->playlistWidget->setUniformRowHeights(true);
    ui->playlistWidget->setAllColumnsShowFocus(true);
    
    // Configurar el header
    QHeaderView* header = ui->playlistWidget->header();
    header->setSectionsMovable(true);
    header->setStretchLastSection(false);
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Establecer nombres de columnas iniciales
    QStringList headers;
    headers << "Título" << "Artista" << "Álbum" << "Género" << "Fecha" << "Duración" << "Tonalidad";
    ui->playlistWidget->setHeaderLabels(headers);
    
    // Establecer anchos y alineación de columnas
    ui->playlistWidget->setColumnWidth(Title, 250);      // Título
    ui->playlistWidget->setColumnWidth(Artist, 150);     // Artista
    ui->playlistWidget->setColumnWidth(Album, 150);      // Álbum
    ui->playlistWidget->setColumnWidth(Genre, 100);      // Género
    ui->playlistWidget->setColumnWidth(Date, 70);        // Fecha
    ui->playlistWidget->setColumnWidth(Duration, 70);    // Duración
    ui->playlistWidget->setColumnWidth(InitialKey, 80);  // Tonalidad

    // Configurar alineación de las columnas
    for (int i = 0; i < ui->playlistWidget->columnCount(); ++i) {
        QTreeWidgetItem* headerItem = ui->playlistWidget->headerItem();
        if (i == Date || i == Duration || i == InitialKey) {
            headerItem->setTextAlignment(i, Qt::AlignCenter);
        } else {
            headerItem->setTextAlignment(i, Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
    
    // Configurar botones de control
    QString estiloBoton = R"(
        QPushButton {
            min-width: 50px;
            min-height: 50px;
            padding: 8px;
            border: none;
            border-radius: 25px;
            background-color: transparent;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 0.2);
        }
    )";
    
    ui->previousButton->setStyleSheet(estiloBoton);
    ui->fastRewindButton->setStyleSheet(estiloBoton);
    ui->playButton->setStyleSheet(estiloBoton);
    ui->fastForwardButton->setStyleSheet(estiloBoton);
    ui->nextButton->setStyleSheet(estiloBoton);
    
    // Establecer iconos más grandes
    int iconSize = 32;  // Tamaño del icono en píxeles
    ui->previousButton->setIconSize(QSize(iconSize, iconSize));
    ui->fastRewindButton->setIconSize(QSize(iconSize, iconSize));
    ui->playButton->setIconSize(QSize(iconSize, iconSize));
    ui->fastForwardButton->setIconSize(QSize(iconSize, iconSize));
    ui->nextButton->setIconSize(QSize(iconSize, iconSize));

    // Configurar barra de estado
    QStatusBar* estado = statusBar();
    
    // Label principal para el estado
    statusLabel = new QLabel(this);
    estado->addWidget(statusLabel);
    
    // Label para el bitrate (permanente, a la derecha)
    bitrateLabel = new QLabel(this);
    estado->addPermanentWidget(bitrateLabel);
    
    actualizarBarraEstado("Listo");
    actualizarBitrate(0);
    
    // Cargar configuración de columnas guardada
    cargarConfiguracionColumnas();
}

void MainWindow::configurarConexiones()
{
    connect(reproductor, &QMediaPlayer::positionChanged, this, &MainWindow::actualizarProgreso);
    connect(reproductor, &QMediaPlayer::durationChanged, this, &MainWindow::actualizarDuracion);
    connect(reproductor, &QMediaPlayer::playbackStateChanged, this, &MainWindow::actualizarEstadoReproduccion);
    connect(reproductor, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia) {
            MetadataManager metadata;
            QString ruta = reproductor->source().toString();
            if (metadata.cargarArchivo(ruta)) {
                actualizarBitrate(metadata.obtenerBitrate());
            }
        }
    });
}

void MainWindow::actualizarBarraEstado(const QString& mensaje)
{
    if (!mensaje.isEmpty()) {
        QString nombreCancion;
        if (ui->playlistWidget->currentItem()) {
            nombreCancion = ui->playlistWidget->currentItem()->text(Title);
        }
        
        if (!nombreCancion.isEmpty()) {
            QString textoEstado = mensaje + " - " + nombreCancion;
            statusLabel->setText(textoEstado);
        } else {
            statusLabel->setText(mensaje);
        }
    }
}

void MainWindow::actualizarBitrate(qint64 bitrate)
{
    if (bitrate > 0) {
        bitrateLabel->setText(QString("%1 kbps").arg(bitrate));
    } else {
        bitrateLabel->setText("");
    }
}

void MainWindow::actualizarEstadoReproduccion(QMediaPlayer::PlaybackState state)
{
    switch (state) {
        case QMediaPlayer::PlayingState:
            actualizarBarraEstado("Reproduciendo");
            ui->playButton->setText("⏸");
            break;
        case QMediaPlayer::PausedState:
            actualizarBarraEstado("Pausado");
            ui->playButton->setText("▶");
            break;
        case QMediaPlayer::StoppedState:
            actualizarBarraEstado("Detenido");
            ui->playButton->setText("▶");
            break;
    }
}

void MainWindow::agregarCancion(const QString& ruta)
{
    QFileInfo fileInfo(ruta);
    if (!fileInfo.exists()) return;

    MetadataManager metadata;
    if (!metadata.cargarArchivo(ruta)) {
        qDebug() << "Error al cargar metadatos:" << ruta;
        return;
    }

    QTreeWidgetItem* item = new QTreeWidgetItem(ui->playlistWidget);
    
    // Obtener y limpiar el título de la canción
    QString titulo = metadata.obtenerTitulo();
    if (titulo.isEmpty()) {
        titulo = fileInfo.baseName();
    }
    titulo = limpiarNombreCancion(titulo);
    
    // Obtener y normalizar la tonalidad
    QString tonalidad = metadata.obtenerTonalidad();
    tonalidad = metadata.normalizarTonalidad(tonalidad);
    
    // Establecer los textos para todas las columnas
    item->setText(Title, titulo);
    item->setText(Artist, metadata.obtenerArtista().isEmpty() ? "Artista desconocido" : metadata.obtenerArtista());
    item->setText(Album, metadata.obtenerAlbum().isEmpty() ? "Sin álbum" : metadata.obtenerAlbum());
    item->setText(Genre, metadata.obtenerGenero());
    item->setText(Date, metadata.obtenerFecha());
    item->setText(Duration, metadata.obtenerDuracion());
    item->setText(InitialKey, tonalidad);
    
    // Guardar la ruta del archivo para uso interno
    item->setData(0, Qt::UserRole, ruta);
    
    // Alinear el texto de las columnas
    item->setTextAlignment(Date, Qt::AlignCenter);
    item->setTextAlignment(Duration, Qt::AlignCenter);
    item->setTextAlignment(InitialKey, Qt::AlignCenter);

    // Si es el primer item, seleccionarlo y reproducir
    if (ui->playlistWidget->topLevelItemCount() == 1) {
        ui->playlistWidget->setCurrentItem(item);
        reproducir();
    }
}

QString MainWindow::limpiarNombreCancion(const QString& nombre) const
{
    QString limpio = nombre;
    
    // Eliminar caracteres especiales y números al inicio
    limpio.remove(QRegularExpression("^[\\d\\W]+"));
    
    // Reemplazar guiones y guiones bajos por espacios
    limpio.replace(QRegularExpression("[-_]+"), " ");
    
    // Eliminar extensiones comunes de archivo
    limpio.remove(QRegularExpression("\\.(mp3|wav|flac|m4a|aac)$", QRegularExpression::CaseInsensitiveOption));
    
    // Capitalizar palabras
    QStringList palabras = limpio.split(' ', Qt::SkipEmptyParts);
    for (QString& palabra : palabras) {
        if (!palabra.isEmpty()) {
            palabra[0] = palabra[0].toUpper();
        }
    }
    
    return palabras.join(" ");
}

QString MainWindow::obtenerNombreCancion(const QString& ruta) const
{
    QFileInfo fileInfo(ruta);
    QString nombre = fileInfo.baseName();
    return limpiarNombreCancion(nombre);
}

void MainWindow::on_actionColumnas_triggered()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Configurar Columnas");
    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QList<QCheckBox*> checkboxes;
    for (auto it = columnConfig.begin(); it != columnConfig.end(); ++it) {
        QCheckBox* checkbox = new QCheckBox(it.value().name, &dialog);
        checkbox->setChecked(it.value().visible);
        checkboxes.append(checkbox);
        layout->addWidget(checkbox);
    }

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int i = 0;
        for (auto it = columnConfig.begin(); it != columnConfig.end(); ++it, ++i) {
            it.value().visible = checkboxes[i]->isChecked();
        }
        guardarConfiguracionColumnas();
        actualizarColumnasVisibles();
    }
}

void MainWindow::guardarConfiguracionColumnas()
{
    QSettings settings;
    settings.beginGroup("Columns");
    for (auto it = columnConfig.begin(); it != columnConfig.end(); ++it) {
        settings.setValue(QString::number(it.key()) + "/visible", it.value().visible);
        settings.setValue(QString::number(it.key()) + "/width", it.value().width);
    }
    settings.endGroup();
}

void MainWindow::cargarConfiguracionColumnas()
{
    QSettings settings;
    settings.beginGroup("Columns");
    for (auto it = columnConfig.begin(); it != columnConfig.end(); ++it) {
        it.value().visible = settings.value(QString::number(it.key()) + "/visible", true).toBool();
        it.value().width = settings.value(QString::number(it.key()) + "/width", it.value().width).toInt();
    }
    settings.endGroup();
    actualizarColumnasVisibles();
}

void MainWindow::actualizarColumnasVisibles()
{
    QStringList headers;
    QList<ColumnType> columnasVisibles;
    
    // Crear la lista de encabezados
    for (auto it = columnConfig.begin(); it != columnConfig.end(); ++it) {
        if (it.value().visible) {
            headers << it.value().name;
            columnasVisibles << it.key();
        }
    }
    
    // Asegurarnos de que tenemos exactamente 7 columnas
    while (headers.size() < 7) {
        headers << "";
    }
    
    // Actualizar los encabezados
    ui->playlistWidget->setHeaderLabels(headers);
    
    // Actualizar los anchos de columna
    int col = 0;
    for (auto it = columnConfig.begin(); it != columnConfig.end(); ++it) {
        if (it.value().visible) {
            ui->playlistWidget->setColumnWidth(col++, it.value().width);
        }
    }
    
    // Ocultar columnas restantes
    while (col < 7) {
        ui->playlistWidget->setColumnWidth(col++, 0);
    }
    
    // Actualizar todas las filas
    for (int i = 0; i < ui->playlistWidget->topLevelItemCount(); ++i) {
        actualizarItemColumnas(ui->playlistWidget->topLevelItem(i));
    }
}

void MainWindow::actualizarItemColumnas(QTreeWidgetItem* item)
{
    if (!item) return;

    // Obtener los datos guardados
    QString titulo = item->data(0, Qt::UserRole + Title).toString();
    QString artista = item->data(0, Qt::UserRole + Artist).toString();
    QString album = item->data(0, Qt::UserRole + Album).toString();
    QString genero = item->data(0, Qt::UserRole + Genre).toString();
    QString fecha = item->data(0, Qt::UserRole + Date).toString();
    QString duracion = item->data(0, Qt::UserRole + Duration).toString();
    QString tonalidad = item->data(0, Qt::UserRole + InitialKey).toString();

    // Establecer los textos en las columnas correspondientes
    item->setText(Title, titulo);
    item->setText(Artist, artista);
    item->setText(Album, album);
    item->setText(Genre, genero);
    item->setText(Date, fecha);
    item->setText(Duration, duracion);
    item->setText(InitialKey, tonalidad);
}

void MainWindow::on_actionAbrirArchivo_triggered()
{
    QStringList archivos = QFileDialog::getOpenFileNames(
        this,
        "Seleccionar archivos de música",
        QString(),
        "Archivos de música (*.mp3 *.m4a *.wav *.flac);;Todos los archivos (*.*)"
    );

    for (const QString& archivo : archivos) {
        agregarCancion(archivo);
    }
}

void MainWindow::on_playButton_clicked()
{
    if (reproductor->playbackState() == QMediaPlayer::PlayingState) {
        pausar();
    } else {
        reproducir();
    }
}

void MainWindow::reproducir()
{
    if (ui->playlistWidget->currentItem()) {
        QString ruta = ui->playlistWidget->currentItem()->data(0, Qt::UserRole).toString();
        reproductor->setSource(QUrl::fromLocalFile(ruta));
        reproductor->play();
        ui->playButton->setText("⏸");
    }
}

void MainWindow::pausar()
{
    reproductor->pause();
    ui->playButton->setText("▶");
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    audioOutput->setVolume(value / 100.0);
    ui->volumePercentLabel->setText(QString("%1%").arg(value));
}

void MainWindow::actualizarProgreso(qint64 posicion)
{
    if (!ui->progressSlider->isSliderDown()) {
        ui->progressSlider->setValue(posicion);
    }
    
    // Actualizar etiquetas de tiempo
    int segundos = (posicion / 1000) % 60;
    int minutos = (posicion / 60000) % 60;
    int horas = (posicion / 3600000);
    
    QString formato = horas > 0 ? "%1:%2:%3" : "%2:%3";
    QString tiempo = horas > 0 
        ? formato.arg(horas).arg(minutos, 2, 10, QChar('0')).arg(segundos, 2, 10, QChar('0'))
        : formato.arg(minutos).arg(segundos, 2, 10, QChar('0'));
        
    ui->timeLabel->setText(tiempo);
}

void MainWindow::actualizarDuracion(qint64 duracion)
{
    ui->progressSlider->setMaximum(duracion);
    
    int segundos = (duracion / 1000) % 60;
    int minutos = (duracion / 60000) % 60;
    int horas = (duracion / 3600000);
    
    QString formato = horas > 0 ? "%1:%2:%3" : "%2:%3";
    QString tiempo = horas > 0 
        ? formato.arg(horas).arg(minutos, 2, 10, QChar('0')).arg(segundos, 2, 10, QChar('0'))
        : formato.arg(minutos).arg(segundos, 2, 10, QChar('0'));
        
    ui->totalTimeLabel->setText(tiempo);
}

void MainWindow::on_progressSlider_sliderMoved(int position)
{
    reproductor->setPosition(position);
}

void MainWindow::on_playlistWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item) {
        reproducir();
    }
}

void MainWindow::on_nextButton_clicked()
{
    int currentRow = ui->playlistWidget->currentIndex().row();
    if (currentRow < ui->playlistWidget->topLevelItemCount() - 1) {
        ui->playlistWidget->setCurrentItem(ui->playlistWidget->topLevelItem(currentRow + 1));
        reproducir();
    }
}

void MainWindow::on_previousButton_clicked()
{
    int currentRow = ui->playlistWidget->currentIndex().row();
    if (currentRow > 0) {
        ui->playlistWidget->setCurrentItem(ui->playlistWidget->topLevelItem(currentRow - 1));
        reproducir();
    }
}
