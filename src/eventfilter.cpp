#include "eventfilter.h"
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QStandardPaths>
#include <QMenuBar>
#include <QDateTime>
#include <QDebug>

EventFilter::EventFilter(QObject *parent) : QObject(parent)
{
    qDebug() << "Iniciando EventFilter...";
    
    // Configurar la ruta del archivo de log
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    logFilePath = dataPath + "/user_interactions.json";
    
    qDebug() << "Directorio de datos:" << dataPath;
    qDebug() << "Archivo de log:" << logFilePath;
    
    // Asegurar que el directorio existe
    if (!QDir(dataPath).exists()) {
        bool created = QDir().mkpath(dataPath);
        qDebug() << "Creando directorio de datos:" << (created ? "éxito" : "error");
    }
    
    // Cargar interacciones previas si existen
    loadInteractions();
    
    qDebug() << "EventFilter iniciado correctamente";
}

void EventFilter::loadInteractions()
{
    QFile file(logFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cargando interacciones previas...";
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray array = doc.array();
        for (const QJsonValue &val : array) {
            QJsonObject obj = val.toObject();
            UserInteraction interaction;
            interaction.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
            interaction.objectName = obj["objectName"].toString();
            interaction.actionType = obj["actionType"].toString();
            interaction.extraInfo = obj["extraInfo"].toString();
            interactions.append(interaction);
        }
        file.close();
        qDebug() << "Interacciones cargadas:" << interactions.size();
    } else {
        qDebug() << "No se encontraron interacciones previas";
    }
}

EventFilter::~EventFilter()
{
    saveInteractions();
}

bool EventFilter::eventFilter(QObject *watched, QEvent *event)
{
    QString objectName;
    QString actionType;
    QString extraInfo;
    bool shouldLog = false;

    // Obtener el nombre del objeto
    if (QWidget *widget = qobject_cast<QWidget*>(watched)) {
        objectName = widget->objectName();
        if (objectName.isEmpty()) {
            objectName = widget->metaObject()->className();
        }
    }

    // Analizar el tipo de evento
    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            actionType = "MousePress";
            extraInfo = QString("Button: %1, Pos: (%2,%3)")
                .arg(mouseEvent->button())
                .arg(mouseEvent->pos().x())
                .arg(mouseEvent->pos().y());
            shouldLog = true;
            qDebug() << "MousePress detectado en:" << objectName;
            break;
        }
        case QEvent::KeyPress: {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            actionType = "KeyPress";
            extraInfo = QString("Key: %1, Text: %2")
                .arg(keyEvent->key())
                .arg(keyEvent->text());
            shouldLog = true;
            qDebug() << "KeyPress detectado:" << extraInfo;
            break;
        }
        case QEvent::MouseButtonDblClick: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            actionType = "DoubleClick";
            extraInfo = QString("Button: %1, Pos: (%2,%3)")
                .arg(mouseEvent->button())
                .arg(mouseEvent->pos().x())
                .arg(mouseEvent->pos().y());
            shouldLog = true;
            qDebug() << "DoubleClick detectado en:" << objectName;
            break;
        }
        default:
            break;
    }

    // Manejar acciones de menú
    if (auto* menu = qobject_cast<QMenu*>(watched)) {
        if (event->type() == QEvent::Show) {
            objectName = menu->objectName();
            actionType = "MenuOpen";
            extraInfo = menu->title();
            shouldLog = true;
            qDebug() << "Menú abierto:" << menu->title();
        }
    }
    else if (auto* action = qobject_cast<QAction*>(watched)) {
        if (event->type() == QEvent::MouseButtonRelease) {
            objectName = action->objectName();
            actionType = "MenuAction";
            extraInfo = action->text();
            shouldLog = true;
            qDebug() << "Acción de menú:" << action->text();
        }
    }

    if (shouldLog && !objectName.isEmpty()) {
        UserInteraction interaction;
        interaction.timestamp = QDateTime::currentDateTime();
        interaction.objectName = objectName;
        interaction.actionType = actionType;
        interaction.extraInfo = extraInfo;
        
        logInteraction(interaction);
    }

    return QObject::eventFilter(watched, event);
}

void EventFilter::logInteraction(const UserInteraction &interaction)
{
    interactions.append(interaction);
    
    // Log detallado de la interacción
    qDebug() << "\n=== Nueva Interacción ===";
    qDebug() << "Tiempo:" << interaction.timestamp.toString(Qt::ISODate);
    qDebug() << "Objeto:" << interaction.objectName;
    qDebug() << "Acción:" << interaction.actionType;
    qDebug() << "Info Extra:" << interaction.extraInfo;
    qDebug() << "Total interacciones:" << interactions.size();
    
    // Guardar después de cada interacción para no perder datos
    saveInteractions();

    // Si alcanzamos el límite, analizamos
    if (interactions.size() >= MAX_INTERACTIONS_BEFORE_ANALYSIS) {
        qDebug() << "Iniciando análisis de patrones...";
        analyzeInteractions();
    }
}

void EventFilter::saveInteractions()
{
    QJsonArray array;
    for (const UserInteraction &interaction : interactions) {
        array.append(interaction.toJson());
    }

    QJsonDocument doc(array);
    QFile file(logFilePath);
    
    qDebug() << "Intentando guardar interacciones en:" << logFilePath;
    qDebug() << "Número de interacciones:" << interactions.size();
    
    if (!QFileInfo(QFileInfo(logFilePath).path()).exists()) {
        QDir().mkpath(QFileInfo(logFilePath).path());
        qDebug() << "Creando directorio:" << QFileInfo(logFilePath).path();
    }
    
    if (file.open(QIODevice::WriteOnly)) {
        qint64 bytesWritten = file.write(doc.toJson());
        file.close();
        qDebug() << "Guardadas" << interactions.size() << "interacciones (" 
                 << bytesWritten << "bytes) en:" << logFilePath;
    } else {
        qDebug() << "Error al guardar interacciones:" << file.errorString();
        qDebug() << "Permisos del archivo:" << file.permissions();
    }
}

void EventFilter::analyzeInteractions()
{
    qDebug() << "\n=== Análisis de Patrones ===";
    
    // Aquí implementaremos el análisis de patrones
    QMap<QString, int> actionCounts;
    QMap<QString, QList<QDateTime>> actionSequences;

    // Contar frecuencia de acciones
    for (const UserInteraction &interaction : interactions) {
        QString key = interaction.objectName + ":" + interaction.actionType;
        actionCounts[key]++;
        actionSequences[key].append(interaction.timestamp);
    }

    // Analizar patrones
    qDebug() << "Analizando frecuencias de acciones:";
    for (auto it = actionCounts.begin(); it != actionCounts.end(); ++it) {
        qDebug() << "  " << it.key() << ":" << it.value() << "veces";
        if (it.value() > 5) {
            QString suggestion = QString("La acción '%1' se ha usado %2 veces. "
                                      "Considere crear un atajo de teclado.")
                                      .arg(it.key())
                                      .arg(it.value());
            qDebug() << "Sugerencia generada:" << suggestion;
            emit suggestionGenerated(suggestion);
        }
    }

    // Analizar secuencias temporales
    qDebug() << "\nAnalizando secuencias temporales:";
    for (auto it = actionSequences.begin(); it != actionSequences.end(); ++it) {
        if (it.value().size() > 3) {
            QList<qint64> intervals;
            for (int i = 1; i < it.value().size(); i++) {
                intervals.append(it.value()[i-1].secsTo(it.value()[i]));
            }
            
            qDebug() << "  Secuencia:" << it.key();
            qDebug() << "    Ocurrencias:" << it.value().size();
            
            // Calcular intervalo promedio y desviación
            qint64 avgInterval = 0;
            for (qint64 interval : intervals) {
                avgInterval += interval;
            }
            avgInterval /= intervals.size();
            
            qDebug() << "    Intervalo promedio:" << avgInterval << "segundos";
            
            if (avgInterval < 60) { // Si las acciones ocurren en menos de 1 minuto
                QString suggestion = QString("Se detectó un patrón repetitivo en '%1'. "
                                          "¿Desea automatizar esta acción?")
                                          .arg(it.key());
                qDebug() << "Sugerencia generada:" << suggestion;
                emit suggestionGenerated(suggestion);
            }
        }
    }
}

InteractionAnalysis EventFilter::getHistoricalAnalysis() const
{
    InteractionAnalysis analysis;
    
    if (interactions.isEmpty()) {
        return analysis;
    }
    
    // Establecer período de análisis
    analysis.startTime = interactions.first().timestamp;
    analysis.endTime = interactions.last().timestamp;
    analysis.totalInteractions = interactions.size();
    
    // Mapas temporales para análisis
    QMap<QString, QList<QDateTime>> actionTimestamps;
    
    // Recolectar datos
    for (const UserInteraction &interaction : interactions) {
        QString key = interaction.objectName + ":" + interaction.actionType;
        analysis.actionCounts[key]++;
        actionTimestamps[key].append(interaction.timestamp);
    }
    
    // Calcular intervalos promedio
    for (auto it = actionTimestamps.begin(); it != actionTimestamps.end(); ++it) {
        if (it.value().size() > 1) {
            double totalInterval = 0;
            for (int i = 1; i < it.value().size(); i++) {
                totalInterval += it.value()[i-1].secsTo(it.value()[i]);
            }
            analysis.averageIntervals[it.key()] = totalInterval / (it.value().size() - 1);
        }
    }
    
    // Identificar patrones frecuentes
    for (auto it = analysis.actionCounts.begin(); it != analysis.actionCounts.end(); ++it) {
        if (it.value() > 5) {
            double avgInterval = analysis.averageIntervals.value(it.key(), 0);
            if (avgInterval > 0 && avgInterval < 60) { // Patrón repetitivo en menos de 1 minuto
                analysis.frequentPatterns.append(it.key());
            }
        }
    }
    
    return analysis;
}

QString EventFilter::generateAnalysisReport() const
{
    InteractionAnalysis analysis = getHistoricalAnalysis();
    
    if (analysis.totalInteractions == 0) {
        return "No hay datos de interacción disponibles.";
    }
    
    QString report;
    QTextStream stream(&report);
    
    // Encabezado
    stream << "=== Reporte de Análisis de Interacciones ===\n\n";
    
    // Período
    stream << "Período de análisis:\n";
    stream << "  Inicio: " << analysis.startTime.toString(Qt::ISODate) << "\n";
    stream << "  Fin: " << analysis.endTime.toString(Qt::ISODate) << "\n";
    stream << "  Total interacciones: " << analysis.totalInteractions << "\n\n";
    
    // Acciones más frecuentes
    stream << "Acciones más frecuentes:\n";
    QList<QPair<QString, int>> sortedActions;
    for (auto it = analysis.actionCounts.begin(); it != analysis.actionCounts.end(); ++it) {
        sortedActions.append(qMakePair(it.key(), it.value()));
    }
    std::sort(sortedActions.begin(), sortedActions.end(),
              [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
                  return a.second > b.second;
              });
    
    for (const auto &pair : sortedActions) {
        if (pair.second > 1) { // Solo mostrar acciones usadas más de una vez
            stream << QString("  %1: %2 veces")
                     .arg(pair.first, -30)
                     .arg(pair.second) << "\n";
            
            // Mostrar intervalo promedio si existe
            if (analysis.averageIntervals.contains(pair.first)) {
                stream << QString("    Intervalo promedio: %1 segundos\n")
                         .arg(analysis.averageIntervals[pair.first], 0, 'f', 1);
            }
        }
    }
    
    // Patrones frecuentes
    if (!analysis.frequentPatterns.isEmpty()) {
        stream << "\nPatrones frecuentes detectados:\n";
        for (const QString &pattern : analysis.frequentPatterns) {
            stream << QString("  - %1 (usado %2 veces, intervalo promedio: %3s)\n")
                     .arg(pattern)
                     .arg(analysis.actionCounts[pattern])
                     .arg(analysis.averageIntervals[pattern], 0, 'f', 1);
        }
    }
    
    // Sugerencias
    stream << "\nSugerencias de optimización:\n";
    for (const QString &pattern : analysis.frequentPatterns) {
        if (pattern.contains("MenuAction")) {
            stream << "  - Considere crear un atajo de teclado para: " << pattern << "\n";
        } else if (analysis.averageIntervals[pattern] < 30) {
            stream << "  - Considere automatizar la secuencia: " << pattern << "\n";
        }
    }
    
    return report;
}

void EventFilter::clearInteractionHistory()
{
    interactions.clear();
    saveInteractions(); // Guardar lista vacía para limpiar el archivo
    qDebug() << "Historial de interacciones limpiado";
}

ValidationResult EventFilter::validateInteractions() const
{
    ValidationResult result;
    result.isValid = true;
    
    // Validar si hay datos
    if (interactions.isEmpty()) {
        result.isValid = false;
        result.message = "No hay interacciones registradas para analizar.";
        return result;
    }
    
    // Estadísticas básicas
    result.statistics["total_interactions"] = interactions.size();
    
    // Validar cronología
    QDateTime lastTime = interactions.first().timestamp;
    int cronologyErrors = 0;
    
    QMap<QString, int> actionTypes;
    QMap<QString, int> objectTypes;
    QSet<QString> uniqueObjects;
    
    for (const UserInteraction &interaction : interactions) {
        // Validar orden cronológico
        if (interaction.timestamp < lastTime) {
            cronologyErrors++;
            result.warnings.append(QString("Error de cronología detectado en: %1")
                                 .arg(interaction.timestamp.toString(Qt::ISODate)));
        }
        lastTime = interaction.timestamp;
        
        // Contar tipos de acciones
        actionTypes[interaction.actionType]++;
        
        // Contar tipos de objetos
        QString objectType = interaction.objectName.split(":").first();
        objectTypes[objectType]++;
        
        // Registrar objetos únicos
        uniqueObjects.insert(interaction.objectName);
    }
    
    // Almacenar estadísticas
    result.statistics["unique_objects"] = uniqueObjects.size();
    result.statistics["action_types"] = actionTypes.size();
    result.statistics["chronology_errors"] = cronologyErrors;
    
    // Validar distribución de acciones
    for (auto it = actionTypes.begin(); it != actionTypes.end(); ++it) {
        double percentage = (it.value() * 100.0) / interactions.size();
        if (percentage > 80) {
            result.warnings.append(QString("La acción '%1' representa el %2% del total de interacciones")
                                 .arg(it.key())
                                 .arg(percentage, 0, 'f', 1));
        }
    }
    
    // Validar intervalos entre acciones
    QList<double> intervals;
    for (int i = 1; i < interactions.size(); i++) {
        double interval = interactions[i-1].timestamp.msecsTo(interactions[i].timestamp) / 1000.0;
        intervals.append(interval);
    }
    
    if (!intervals.isEmpty()) {
        // Calcular estadísticas de intervalos
        double totalInterval = 0;
        double minInterval = intervals.first();
        double maxInterval = intervals.first();
        
        for (double interval : intervals) {
            totalInterval += interval;
            minInterval = qMin(minInterval, interval);
            maxInterval = qMax(maxInterval, interval);
        }
        
        double avgInterval = totalInterval / intervals.size();
        result.statistics["avg_interval_secs"] = static_cast<int>(avgInterval);
        result.statistics["min_interval_secs"] = static_cast<int>(minInterval);
        result.statistics["max_interval_secs"] = static_cast<int>(maxInterval);
        
        // Validar intervalos sospechosos
        if (minInterval < 0.05) { // Menos de 50ms entre acciones
            result.warnings.append("Se detectaron intervalos muy cortos entre acciones (<50ms)");
        }
        if (maxInterval > 3600) { // Más de 1 hora
            result.warnings.append("Se detectaron intervalos muy largos entre acciones (>1h)");
        }
    }
    
    // Validar coherencia de objetos
    QMap<QString, QSet<QString>> objectActions;
    for (const UserInteraction &interaction : interactions) {
        objectActions[interaction.objectName].insert(interaction.actionType);
    }
    
    // Buscar objetos con comportamiento inusual
    for (auto it = objectActions.begin(); it != objectActions.end(); ++it) {
        if (it.value().size() > 5) { // Objeto con más de 5 tipos diferentes de acciones
            result.warnings.append(QString("El objeto '%1' tiene %2 tipos diferentes de acciones")
                                 .arg(it.key())
                                 .arg(it.value().size()));
        }
    }
    
    // Resumen final
    if (result.warnings.isEmpty()) {
        result.message = "Validación completada: No se encontraron problemas significativos.";
    } else {
        result.message = QString("Validación completada: Se encontraron %1 advertencias.")
                        .arg(result.warnings.size());
    }
    
    return result;
}
