#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <QObject>
#include <QSettings>
#include <QVariant>
#include <QDir>
#include <memory>

/**
 * @brief Sistema de configuración persistente
 * 
 * Maneja la configuración de la aplicación, guardando y cargando
 * preferencias del usuario de forma persistente.
 */
class Configuracion : public QObject {
    Q_OBJECT

public:
    static Configuracion& instance();
    
    // Getters
    QString getTema() const;
    int getVolumen() const;
    QString getUltimoDirectorio() const;
    QStringList getDirectoriosRecientes() const;
    bool getRepetirReproduccion() const;
    bool getReproduccionAleatoria() const;
    
    // Setters
    void setTema(const QString& tema);
    void setVolumen(int volumen);
    void setUltimoDirectorio(const QString& directorio);
    void agregarDirectorioReciente(const QString& directorio);
    void setRepetirReproduccion(bool repetir);
    void setReproduccionAleatoria(bool aleatorio);
    
    // Gestión de configuración
    void cargarConfiguracion();
    void guardarConfiguracion();
    void restaurarValoresPorDefecto();

signals:
    void temaModificado(const QString& nuevoTema);
    void volumenModificado(int nuevoVolumen);
    void configuracionModificada();

protected:
    friend struct std::default_delete<Configuracion>;
    ~Configuracion() = default;

private:
    explicit Configuracion(QObject *parent = nullptr);
    
    std::unique_ptr<QSettings> settings;
    static std::unique_ptr<Configuracion> instance_;
    
    // Valores por defecto
    static const QString TEMA_DEFAULT;
    static const int VOLUMEN_DEFAULT;
    static const int MAX_DIRECTORIOS_RECIENTES;
};

#endif // CONFIGURACION_H
