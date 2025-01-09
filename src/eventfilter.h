#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QDateTime>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMap>
#include <QList>

struct UserInteraction {
    QDateTime timestamp;
    QString objectName;
    QString actionType;
    QString extraInfo;

    QJsonObject toJson() const {
        QJsonObject json;
        json["timestamp"] = timestamp.toString(Qt::ISODate);
        json["objectName"] = objectName;
        json["actionType"] = actionType;
        json["extraInfo"] = extraInfo;
        return json;
    }
};

struct InteractionAnalysis {
    QMap<QString, int> actionCounts;
    QMap<QString, double> averageIntervals;
    QList<QString> frequentPatterns;
    QDateTime startTime;
    QDateTime endTime;
    int totalInteractions;
};

struct ValidationResult {
    bool isValid;
    QString message;
    QList<QString> warnings;
    QMap<QString, int> statistics;
};

class EventFilter : public QObject
{
    Q_OBJECT

public:
    explicit EventFilter(QObject *parent = nullptr);
    ~EventFilter();
    
    // Métodos públicos para consultar análisis
    InteractionAnalysis getHistoricalAnalysis() const;
    QString generateAnalysisReport() const;
    void clearInteractionHistory();
    ValidationResult validateInteractions() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void loadInteractions();
    void logInteraction(const UserInteraction &interaction);
    void saveInteractions();
    void analyzeInteractions();
    
    QList<UserInteraction> interactions;
    static const int MAX_INTERACTIONS_BEFORE_ANALYSIS = 50;
    QString logFilePath;

signals:
    void suggestionGenerated(const QString &suggestion);
    void analysisUpdated(const InteractionAnalysis &analysis);
    void validationComplete(const ValidationResult &result);
};

#endif // EVENTFILTER_H
