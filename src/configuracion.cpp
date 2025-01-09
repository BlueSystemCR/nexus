#include "configuracion.h"
#include "logger.h"

std::unique_ptr<Configuracion> Configuracion::instance_;
const QString Configuracion::TEMA_DEFAULT = "claro";
const int Configuracion::VOLUMEN_DEFAULT = 50;
const int Configuracion::MAX_DIRECTORIOS_RECIENTES = 10;

Configuracion& Configuracion::instance() {
    if (!instance_) {
        instance_ = std::unique_ptr<Configuracion>(new Configuracion());
    }
    return *instance_;
}

Configuracion::Configuracion(QObject *parent)
    : QObject(parent)
{
    settings = std::make_unique<QSettings>("MiOrganizacion", "ReproductorMusica");
    cargarConfiguracion();
}

void Configuracion::cargarConfiguracion() {
    LOG_INFO("Cargando configuración");
    try {
        if (settings->value("PrimeraEjecucion", true).toBool()) {
            LOG_INFO("Primera ejecución - configurando valores por defecto");
            restaurarValoresPorDefecto();
            settings->setValue("PrimeraEjecucion", false);
        }
    } catch (const std::exception& e) {
        LOG_ERROR(QString("Error al cargar configuración: %1").arg(e.what()));
    }
}

void Configuracion::guardarConfiguracion() {
    LOG_INFO("Guardando configuración");
    settings->sync();
}

void Configuracion::restaurarValoresPorDefecto() {
    LOG_INFO("Restaurando valores por defecto");
    setTema(TEMA_DEFAULT);
    setVolumen(VOLUMEN_DEFAULT);
    setRepetirReproduccion(false);
    setReproduccionAleatoria(false);
    guardarConfiguracion();
}

// Getters
QString Configuracion::getTema() const {
    return settings->value("Tema", TEMA_DEFAULT).toString();
}

int Configuracion::getVolumen() const {
    return settings->value("Volumen", VOLUMEN_DEFAULT).toInt();
}

QString Configuracion::getUltimoDirectorio() const {
    return settings->value("UltimoDirectorio", QDir::homePath()).toString();
}

QStringList Configuracion::getDirectoriosRecientes() const {
    return settings->value("DirectoriosRecientes").toStringList();
}

bool Configuracion::getRepetirReproduccion() const {
    return settings->value("RepetirReproduccion", false).toBool();
}

bool Configuracion::getReproduccionAleatoria() const {
    return settings->value("ReproduccionAleatoria", false).toBool();
}

// Setters
void Configuracion::setTema(const QString& tema) {
    if (tema != getTema()) {
        settings->setValue("Tema", tema);
        emit temaModificado(tema);
        emit configuracionModificada();
    }
}

void Configuracion::setVolumen(int volumen) {
    if (volumen != getVolumen()) {
        settings->setValue("Volumen", volumen);
        emit volumenModificado(volumen);
        emit configuracionModificada();
    }
}

void Configuracion::setUltimoDirectorio(const QString& directorio) {
    settings->setValue("UltimoDirectorio", directorio);
    emit configuracionModificada();
}

void Configuracion::agregarDirectorioReciente(const QString& directorio) {
    QStringList directorios = getDirectoriosRecientes();
    directorios.removeAll(directorio); // Eliminar si ya existe
    directorios.prepend(directorio); // Agregar al principio
    
    // Mantener solo los últimos MAX_DIRECTORIOS_RECIENTES
    while (directorios.size() > MAX_DIRECTORIOS_RECIENTES) {
        directorios.removeLast();
    }
    
    settings->setValue("DirectoriosRecientes", directorios);
    emit configuracionModificada();
}

void Configuracion::setRepetirReproduccion(bool repetir) {
    settings->setValue("RepetirReproduccion", repetir);
    emit configuracionModificada();
}

void Configuracion::setReproduccionAleatoria(bool aleatorio) {
    settings->setValue("ReproduccionAleatoria", aleatorio);
    emit configuracionModificada();
}
