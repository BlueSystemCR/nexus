#include "playlist.h"

PlayList::PlayList(QObject *parent)
    : QObject(parent)
    , indiceActual(-1)
{
}

PlayList::~PlayList()
{
}

void PlayList::agregarCancion(const QString& ruta)
{
    lista.append(ruta);
    if (indiceActual == -1) {
        indiceActual = 0;
    }
    emit listaModificada();
}

void PlayList::removerCancion(int indice)
{
    if (indice >= 0 && indice < lista.size()) {
        lista.removeAt(indice);
        if (lista.isEmpty()) {
            indiceActual = -1;
        } else if (indice <= indiceActual) {
            indiceActual = qMax(0, indiceActual - 1);
        }
        emit listaModificada();
    }
}

void PlayList::limpiarLista()
{
    lista.clear();
    indiceActual = -1;
    emit listaModificada();
}

int PlayList::obtenerIndiceActual() const
{
    return indiceActual;
}

void PlayList::establecerIndiceActual(int indice)
{
    if (indice >= -1 && indice < lista.size()) {
        indiceActual = indice;
        emit listaModificada();
    }
}

int PlayList::siguienteIndice() const
{
    if (lista.isEmpty()) return -1;
    return (indiceActual + 1) % lista.size();
}

int PlayList::indiceAnterior() const
{
    if (lista.isEmpty()) return -1;
    return (indiceActual - 1 + lista.size()) % lista.size();
}

const QList<QString>& PlayList::obtenerLista() const
{
    return lista;
}

QString PlayList::obtenerCancionActual() const
{
    if (indiceActual >= 0 && indiceActual < lista.size()) {
        return lista[indiceActual];
    }
    return QString();
}

bool PlayList::estaVacia() const
{
    return lista.isEmpty();
}

int PlayList::tamano() const
{
    return lista.size();
}

bool PlayList::avanzar()
{
    if (indiceActual < lista.size() - 1) {
        indiceActual++;
        return true;
    }
    return false;
}

bool PlayList::retroceder()
{
    if (indiceActual > 0) {
        indiceActual--;
        return true;
    }
    return false;
}
