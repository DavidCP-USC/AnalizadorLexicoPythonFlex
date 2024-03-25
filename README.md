# Información

Este proyecto es una primera aproximación de un compilador de Python en el que nos centramos principalmente en el análisis léxico del lenguaje.
Se ha realizado teniendo en cuenta factores como el Sistema de Entrada, la Tabla de Símbolos y la gestión de la memoria.


# Requisitos

* GNU GCC
* GNU Make

# Versiones probadas
* GNU GCC 13.2.1 20230801
* GNU Make 4.4.1

Todas las pruebas se han hecho bajo Ubuntu 22.04.4

# Compilación

Para compilar el proyecto, ejecute el siguiente comando en la terminal dentro de la carpeta obtenida tras descomprimir el archivo:

```
make
```
Se generará un archivo ejecutable con nombre ```ejecutable``` que, de no tener permisos de ejecución, se le deben otorgar con el comando
```chmod +x ./ejecutable```

# Ejecución
Finalmente, una vez el archivo tenga permisos de ejecución se le debe de pasar como argumento el nombre del archivo que se quiere analizar
```
./ejecutable Archivo_A_Analizar.py
```
