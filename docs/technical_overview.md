# Nexus - Reproductor de Música
## Documentación Técnica para Desarrollo

### 1. Arquitectura del Sistema

#### 1.1 Componentes Principales
- **MainWindow**: Interfaz principal y controlador central
- **AudioConverter**: Manejo de conversión de formatos
- **PlayList**: Gestión de lista de reproducción
- **MetadataManager**: Extracción y manejo de metadatos
- **Logger**: Sistema de registro de eventos
- **EventFilter**: Manejo de eventos globales
- **Configuracion**: Gestión de configuraciones
- **Tema**: Sistema de temas y estilos

#### 1.2 Tecnologías Utilizadas
- **Framework**: Qt 6
- **Lenguaje**: C++17
- **Biblioteca de Audio**: Qt Multimedia
- **Conversión**: FFmpeg
- **Build System**: CMake
- **Control de Versiones**: Git

### 2. Funcionalidades Implementadas

#### 2.1 Reproducción de Audio
- Soporte para múltiples formatos:
  - MP3
  - WAV
  - OGG
  - M4A
  - FLAC
  - AAC
- Control de volumen con slider
- Barra de progreso interactiva
- Botones de control (Play/Pause, Siguiente, Anterior)
- Reproducción continua automática

#### 2.2 Conversión de Audio
- Conversión automática de FLAC a WAV
- Indicador de progreso visual
- Estimación de tiempo restante
- Manejo de errores de conversión
- Limpieza automática de archivos temporales

#### 2.3 Gestión de Lista de Reproducción
- Vista en árbol con columnas:
  - Título
  - Artista
  - Álbum
  - Comentarios
  - Duración
- Soporte para arrastrar y soltar
- Selección múltiple
- Ordenamiento por columnas
- Persistencia de la lista

#### 2.4 Interfaz de Usuario
- Diseño moderno y minimalista
- Barra de estado informativa
- Indicadores visuales de estado
- Soporte para temas claros/oscuros
- Atajos de teclado configurables

#### 2.5 Optimización de Recursos
- Gestión eficiente de memoria
- Liberación de recursos no utilizados
- Caché de archivos convertidos
- Procesamiento asíncrono de conversiones

### 3. Áreas de Mejora Potencial

#### 3.1 Funcionalidades
- [ ] Ecualizador de audio
- [ ] Visualizaciones de audio
- [ ] Soporte para listas de reproducción en la nube
- [ ] Sincronización entre dispositivos
- [ ] Soporte para letras de canciones
- [ ] Radio por Internet
- [ ] Modo karaoke
- [ ] Crossfade entre canciones

#### 3.2 Técnicas
- [ ] Implementar tests unitarios
- [ ] Mejorar el manejo de memoria en archivos grandes
- [ ] Optimizar el proceso de conversión
- [ ] Agregar soporte para más formatos
- [ ] Mejorar la detección de metadatos
- [ ] Implementar caché de miniaturas
- [ ] Soporte para plugins

#### 3.3 Interfaz de Usuario
- [ ] Personalización de temas
- [ ] Modo mini-reproductor
- [ ] Vista de álbum con carátulas
- [ ] Gestor de biblioteca musical
- [ ] Búsqueda avanzada
- [ ] Filtros inteligentes
- [ ] Estadísticas de reproducción

### 4. API y Extensibilidad

#### 4.1 Señales Principales
```cpp
// MainWindow
void reproduccionIniciada(const QString& archivo);
void reproduccionPausada();
void reproduccionDetenida();
void progresoActualizado(qint64 posicion);
void volumenCambiado(int volumen);

// AudioConverter
void conversionProgress(int porcentaje);
void conversionFinished(const QString& outputFile);
void conversionError(const QString& error);

// PlayList
void listaModificada();
void indiceActualCambiado(int nuevoIndice);
```

#### 4.2 Slots Públicos
```cpp
// MainWindow
void reproducir();
void pausar();
void detener();
void siguiente();
void anterior();
void setVolumen(int volumen);
void setPosicion(qint64 posicion);

// PlayList
void agregarCancion(const QString& ruta);
void eliminarCancion(int indice);
void limpiar();
void moverCancion(int desde, int hasta);
```

### 5. Configuración del Entorno de Desarrollo

#### 5.1 Requisitos
- Qt 6.7.3 o superior
- CMake 3.31.3 o superior
- Compilador con soporte para C++17
- FFmpeg instalado en el sistema
- Git

#### 5.2 Compilación
```bash
mkdir build
cd build
cmake ..
make
```

#### 5.3 Estructura del Proyecto
```
nexus/
├── src/
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── audioconverter.cpp
│   ├── playlist.cpp
│   └── ...
├── include/
│   ├── mainwindow.h
│   ├── audioconverter.h
│   ├── playlist.h
│   └── ...
├── resources/
│   ├── icons/
│   └── styles/
├── docs/
├── tests/
└── CMakeLists.txt
```

### 6. Guías de Contribución

#### 6.1 Estilo de Código
- Seguir el estilo de Qt
- Usar camelCase para nombres de funciones y variables
- Documentar todas las funciones públicas
- Mantener las funciones pequeñas y enfocadas
- Usar nombres descriptivos y en español

#### 6.2 Proceso de Pull Request
1. Crear una rama para cada feature
2. Seguir el formato de commit convencional
3. Incluir tests cuando sea posible
4. Actualizar la documentación
5. Solicitar revisión de código

### 7. Rendimiento y Seguridad

#### 7.1 Consideraciones de Rendimiento
- Uso de memoria optimizado para archivos grandes
- Conversión asíncrona de archivos
- Caché de archivos convertidos
- Liberación proactiva de recursos

#### 7.2 Seguridad
- Validación de archivos de entrada
- Manejo seguro de archivos temporales
- Protección contra desbordamiento de buffer
- Sanitización de nombres de archivo

### 8. Roadmap Futuro

#### 8.1 Corto Plazo (3 meses)
- Implementar ecualizador
- Agregar visualizaciones de audio
- Mejorar el manejo de metadatos
- Optimizar la conversión de FLAC

#### 8.2 Medio Plazo (6 meses)
- Soporte para streaming
- Sincronización en la nube
- Modo colaborativo
- API para plugins

#### 8.3 Largo Plazo (12 meses)
- Integración con servicios de música
- Aplicación móvil companion
- Sistema de recomendaciones
- Análisis de audio avanzado

---

*Última actualización: 9 de enero de 2025*
