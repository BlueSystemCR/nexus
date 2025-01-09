#include <QApplication>
#include <QDir>
#include <QLibraryInfo>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // Configurar las rutas de plugins antes de crear la aplicación
    qputenv("GST_PLUGIN_PATH", "/opt/homebrew/lib/gstreamer-1.0");
    qputenv("QT_PLUGIN_PATH", "/opt/homebrew/opt/qt6/plugins");
    
    QApplication a(argc, argv);
    
    // Imprimir información de depuración
    qDebug() << "Rutas de bibliotecas:";
    qDebug() << QCoreApplication::libraryPaths();
    qDebug() << "\nRuta de plugins GStreamer:";
    qDebug() << qgetenv("GST_PLUGIN_PATH");
    qDebug() << "\nRuta de plugins Qt:";
    qDebug() << qgetenv("QT_PLUGIN_PATH");
    
    MainWindow w;
    w.show();
    return a.exec();
}
