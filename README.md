# Instrucciones de compilación.

1. La compilación se debe realizar en un contenedor de Docker con la imagen multiplataforma de este cuatrimestre (`agodio/itba-so-multi-platform:3.0`). Se corre usando el siguiente comando desde la carpeta principal del proyecto:

    ```sh
    docker run -v "${PWD}:/root" --privileged -ti agodio/itba-so-multi-platform:3.0
    ```

2. Una vez dentro del contenedor, se debe ejecutar:

    ```sh
    cd /root
    ```

3. En la carpeta principal, se debe ejecutar:

    ```sh
    make all
    ```

    para compilar el proyecto.

4. Luego, donde se tenga instalado QEMU, se debe ejecutar:
    ```sh
    ./run.sh
    ```