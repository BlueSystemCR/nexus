#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QList>
#include <QString>

/**
 * @brief Gestiona la lista de reproducción
 * 
 * Implementa funcionalidad para manejar listas de reproducción,
 * incluyendo soporte para reproducción aleatoria y repetición.
 */
class PlayList : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase PlayList
     * 
     * Inicializa la lista de reproducción y el índice actual.
     * 
     * @param parent Objeto padre de la clase PlayList
     */
    explicit PlayList(QObject *parent = nullptr);
    
    /**
     * @brief Destructor de la clase PlayList
     */
    ~PlayList();
    
    /**
     * @brief Agrega una canción a la lista de reproducción
     * 
     * @param ruta Ruta de la canción a agregar
     */
    void agregarCancion(const QString& ruta);
    
    /**
     * @brief Remueve una canción de la lista de reproducción
     * 
     * @param indice Índice de la canción a remover
     */
    void removerCancion(int indice);
    
    /**
     * @brief Limpia la lista de reproducción
     */
    void limpiarLista();
    
    /**
     * @brief Obtiene el índice actual de la lista de reproducción
     * 
     * @return Índice actual
     */
    int obtenerIndiceActual() const;
    
    /**
     * @brief Establece el índice actual de la lista de reproducción
     * 
     * @param indice Nuevo índice actual
     */
    void establecerIndiceActual(int indice);
    
    /**
     * @brief Obtiene el índice de la siguiente canción en la lista de reproducción
     * 
     * @return Índice de la siguiente canción
     */
    int siguienteIndice() const;
    
    /**
     * @brief Obtiene el índice de la canción anterior en la lista de reproducción
     * 
     * @return Índice de la canción anterior
     */
    int indiceAnterior() const;
    
    /**
     * @brief Obtiene la lista de reproducción
     * 
     * @return Lista de reproducción
     */
    const QList<QString>& obtenerLista() const;
    
    /**
     * @brief Obtiene la canción actual en la lista de reproducción
     * 
     * @return Canción actual
     */
    QString obtenerCancionActual() const;
    
    /**
     * @brief Avanza a la siguiente canción en la lista de reproducción
     * 
     * @return True si se pudo avanzar, false en caso contrario
     */
    bool avanzar();
    
    /**
     * @brief Retrocede a la canción anterior en la lista de reproducción
     * 
     * @return True si se pudo retroceder, false en caso contrario
     */
    bool retroceder();
    
    /**
     * @brief Verifica si la lista de reproducción está vacía
     * 
     * @return True si la lista está vacía, false en caso contrario
     */
    bool estaVacia() const;
    
    /**
     * @brief Obtiene el tamaño de la lista de reproducción
     * 
     * @return Tamaño de la lista
     */
    int tamano() const;

signals:
    /**
     * @brief Señal emitida cuando la lista de reproducción es modificada
     */
    void listaModificada();

private:
    /**
     * @brief Lista de reproducción
     */
    QList<QString> lista;
    
    /**
     * @brief Índice actual de la lista de reproducción
     */
    int indiceActual;
};

#endif // PLAYLIST_H
