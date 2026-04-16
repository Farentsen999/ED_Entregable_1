/* Compilación: g++ -g -o img test.cpp
 * Ejecución: ./img
 *
 * Luego de la ejecución se generarán 3 imagines: imagen.png,
 * imagen_desplazada_500.png e imagen_desplazada_600.png
 */

#include "moving_image.h"
#include<unistd.h> // para sleep (linux). Usar  #include<windows.h> para Windows

int main() {
  moving_image im;

  im.draw("imagen.png");
  im.move_right(200);
  sleep(2);
  im.draw("imagen_desplazada_r.png");
  im.move_up(200);
  sleep(2);
  im.draw("imagen_desplazada_u.png");
  im.move_left(200);
  sleep(2);
  im.draw("imagen_desplazada_l.png");
  im.move_down(400);
  sleep(2);
  im.draw("imagen_desplazada_d.png");
  sleep(2);
  im.rotate();
  sleep(2);
  im.draw("imagen_rotada_90.png");



  /* NOTA 1: Si usan el mismo nombre para las imágenes, entonces cada llamada al
  método draw() sobreescribirá a la imagen */

  /* NOTA 2: Si usan un ciclo while(1) y usan el mismo nombre de imagen,
  entonces se puede generar un efecto de desplazamiento continuo (algo así como
  un video =D ) */
  return 0;
}
