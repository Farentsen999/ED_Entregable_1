#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include <stack>
#include <iostream>
#include <vector>
// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image {
private:
  // Struct que guarda el estado de las tres matrices para almacenar cada transformación.
  struct EstadoImagen{
    //usamos vectores para guardar las matrices, así el manejo de la memoria dinámica 
    //al usar operaciones con el stack se le delega a vector.
    std::vector<std::vector<unsigned char>> capa_roja, capa_verde, capa_azul;

    EstadoImagen(unsigned char **r, unsigned char **g, unsigned char **b){
      //indicamos que tenemos un vector de tañamo H_IMG  de vectores de tamaño  W_IMG para cada capa
      capa_roja=  std::vector<std::vector<unsigned char>>(H_IMG, std::vector<unsigned char>(W_IMG));
      capa_verde=  std::vector<std::vector<unsigned char>>(H_IMG, std::vector<unsigned char>(W_IMG));
      capa_azul=  std::vector<std::vector<unsigned char>>(H_IMG, std::vector<unsigned char>(W_IMG));
     
      //Llenamos con los pixeles dados en el constructor
      for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++) {
	      capa_roja[i][j] = r[i][j];
        capa_verde[i][j] = g[i][j];
       	capa_azul[i][j] = b[i][j];
      }
    }
  };
  std::stack<EstadoImagen> stackEstadoImagen1; //para undo()
  std::stack<EstadoImagen> stackEstadoImagen2; //para redo()
  unsigned char **red_layer; // Capa de tonalidades rojas
  unsigned char **green_layer; // Capa de tonalidades verdes
  unsigned char **blue_layer; // Capa de tonalidades azules
  // Enum para clasificar el tipo de movimiento
  enum TipoMovimiento {NINGUNO, IZQUIERDA, DERECHA, ARRIBA, ABAJO, ROTAR};
  TipoMovimiento ultimoTipo = NINGUNO;
  int ultimaDistancia = 0;


public:
  // Constructor de la imagen. Se crea una imagen por defecto
  moving_image() {
    // Reserva de memoria para las 3 matrices RGB
    red_layer = new unsigned char*[H_IMG];
    green_layer = new unsigned char*[H_IMG];
    blue_layer = new unsigned char*[H_IMG];
    
    for(int i=0; i < H_IMG; i++) {
      red_layer[i] = new unsigned char[W_IMG];
      green_layer[i] = new unsigned char[W_IMG];
      blue_layer[i] = new unsigned char[W_IMG];
    }

    // Llenamos la imagen con su color de fondo
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++) {
	red_layer[i][j] = DEFAULT_R;
	green_layer[i][j] = DEFAULT_G;
	blue_layer[i][j] = DEFAULT_B;
      }

    // Dibujamos el objeto en su posición inicial
    for(int i=0; i < 322; i++)
      for(int j=0; j < 256; j++) {
	if(!s_R[i][j] && !s_G[i][j] && !s_B[i][j]) {
	  red_layer[INIT_Y+i][INIT_X+j] = DEFAULT_R;
	  green_layer[INIT_Y+i][INIT_X+j] = DEFAULT_G;
	  blue_layer[INIT_Y+i][INIT_X+j] = DEFAULT_B;
	} else {
	  red_layer[INIT_Y+i][INIT_X+j] = s_R[i][j];
	  green_layer[INIT_Y+i][INIT_X+j] = s_G[i][j];
	  blue_layer[INIT_Y+i][INIT_X+j] = s_B[i][j];
	}
      }   
  }

  // Destructor de la clase
  ~moving_image() {
    for(int i=0; i < H_IMG; i++) {
      delete red_layer[i];
      delete green_layer[i];
      delete blue_layer[i];
    }

    delete red_layer;
    delete green_layer;
    delete blue_layer;
  }

  // Función utilizada para guardar la imagen en formato .png
  void draw(const char* nb) {
    _draw(nb);
  }
  void guardarEstadoImagen(){
    EstadoImagen estadoAnterior= EstadoImagen(red_layer,green_layer,blue_layer);
    stackEstadoImagen1.push(estadoAnterior);
  }

  // Función que similar desplazar la imagen, de manera circular, d pixeles a la izquierda
  void move_left(int d) {
    //guardamos el estado actual de la imagen en el stack
     guardarEstadoImagen();
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
	tmp_layer[i][j] = red_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][j] = red_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	tmp_layer[i][j] = green_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][j] = green_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	tmp_layer[i][j] = blue_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][j] = blue_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	blue_layer[i][j] = tmp_layer[i][j];
    
    // Liberar memoria
    for(int i=0; i < H_IMG; i++) {
        delete[] tmp_layer[i]; // Borra cada fila
    }
    delete[] tmp_layer;
    ultimoTipo = IZQUIERDA; 
    ultimaDistancia = d; 
  }

  void move_right(int d) {
    //guardamos el estado actual de la imagen en el stack
     guardarEstadoImagen();
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
	tmp_layer[i][j+d] = red_layer[i][j];      
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][k] = red_layer[i][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	tmp_layer[i][j+d] = green_layer[i][j];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][k] = green_layer[i][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	tmp_layer[i][j+d] = blue_layer[i][j];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	tmp_layer[i][k] = blue_layer[i][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	blue_layer[i][j] = tmp_layer[i][j];

    // Liberar memoria
    for(int i=0; i < H_IMG; i++) {
        delete[] tmp_layer[i]; // Borra cada fila
    }
    delete[] tmp_layer;
    ultimoTipo = DERECHA; 
    ultimaDistancia = d; 
  }

  void move_up(int d) {
    //guardamos el estado actual de la imagen en el stack
     guardarEstadoImagen();
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
	tmp_layer[i][j] = red_layer[i+d][j];      
    
    for(int i=H_IMG-d, k=0 ; i<H_IMG; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = red_layer[k][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = green_layer[i+d][j];      
    
    for(int i=H_IMG-d, k=0 ; i<H_IMG; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = green_layer[k][j];       

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = blue_layer[i+d][j];      
    
    for(int i=H_IMG-d, k=0 ; i<H_IMG; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i][j] = blue_layer[k][j];        

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	blue_layer[i][j] = tmp_layer[i][j];

    // Liberar memoria
    for(int i=0; i < H_IMG; i++) {
        delete[] tmp_layer[i]; // Borra cada fila
    }
    delete[] tmp_layer;
    ultimoTipo = ARRIBA; 
    ultimaDistancia = d; 
  }

  void move_down(int d) {
    //guardamos el estado actual de la imagen en el stack
     guardarEstadoImagen();
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    // Mover la capa roja
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
	tmp_layer[i+d][j] = red_layer[i][j];      
    
    for(int i=H_IMG-d, k=0 ; i<H_IMG; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[k][j] = red_layer[i][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i+d][j] = green_layer[i][j];      
    
    for(int i=H_IMG-d, k=0 ; i<H_IMG; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[k][j] = green_layer[i][j];       

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for(int i=0; i < H_IMG-d; i++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[i+d][j] = blue_layer[i][j];      
    
    for(int i=H_IMG-d, k=0 ; i<H_IMG; i++, k++)
      for(int j=0; j < W_IMG; j++)
    	tmp_layer[k][j] = blue_layer[i][j];        

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	blue_layer[i][j] = tmp_layer[i][j];

    // Liberar memoria
    for(int i=0; i < H_IMG; i++) {
        delete[] tmp_layer[i]; // Borra cada fila
    }
    delete[] tmp_layer;
    ultimoTipo = ABAJO; 
    ultimaDistancia = d; 
  }

  void rotate() {
    //guardamos el estado actual de la imagen en el stack
     guardarEstadoImagen();
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];

    // Rotar 90 grados antihorario:
    // La nueva posicion (i, j) toma el valor de la original (j, W_IMG - 1 - i)
    
    // Rotar la capa Roja
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        tmp_layer[i][j] = red_layer[j][W_IMG - 1 - i];
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        red_layer[i][j] = tmp_layer[i][j];

    // Rotar la capa Verde
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        tmp_layer[i][j] = green_layer[j][W_IMG - 1 - i];
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        green_layer[i][j] = tmp_layer[i][j];

    // Rotar la capa Azul
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        tmp_layer[i][j] = blue_layer[j][W_IMG - 1 - i];
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        blue_layer[i][j] = tmp_layer[i][j];

    // Liberar memoria
    for(int i=0; i < H_IMG; i++)
      delete[] tmp_layer[i];
    delete[] tmp_layer;
    ultimoTipo = ROTAR;
  }
 //Metodo que devuelve el estado de la imagen anterior al ultimo movimiento.
  void undo(){
     if(stackEstadoImagen1.empty()){
      std::cout << "No hay movimientos anteriores" << std::endl;
      return;
     }
     //Guardamos el estado actual en nuestro segundo stack, para luego ser usado en redo()
     EstadoImagen estadoActual= EstadoImagen(red_layer,green_layer,blue_layer);
     stackEstadoImagen2.push(estadoActual);

     //Miramos el estado anterior y lo sacamos del primer stack.
     EstadoImagen estadoAnterior= stackEstadoImagen1.top();
     stackEstadoImagen1.pop();
     //Reemplazamos las matrices actuales por las del ultimo estado
     for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++) {
	      red_layer[i][j] = estadoAnterior.capa_roja[i][j];
        green_layer[i][j] = estadoAnterior.capa_verde[i][j];
       	blue_layer[i][j] = estadoAnterior.capa_azul[i][j];
      }
     
  }
  void redo(){

     if(stackEstadoImagen2.empty()){
      std::cout << "No hay movimientos posteriores" << std::endl;
      return;
     }
     //Miramos el estado anterior y lo sacamos
     EstadoImagen estadoAnterior= stackEstadoImagen2.top();
     stackEstadoImagen2.pop();
     //Reemplazamos las matrices actuales por las del ultimo estado
     for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++) {
	      red_layer[i][j] = estadoAnterior.capa_roja[i][j];
        green_layer[i][j] = estadoAnterior.capa_verde[i][j];
       	blue_layer[i][j] = estadoAnterior.capa_azul[i][j];
      }    
  }

 void repeat() {
    // En caso de que no se haya realizado ninguna accion
    if (ultimoTipo == NINGUNO) {
        return;
    }
    // Se repite el ultimo movimiento
    switch (ultimoTipo) {
      case IZQUIERDA:
        move_left(ultimaDistancia);
        break;
      case DERECHA:
        move_right(ultimaDistancia);
        break;
      case ARRIBA:
        move_up(ultimaDistancia);
        break;
      case ABAJO:
        move_down(ultimaDistancia);
        break;
      case ROTAR:
        rotate();
        break;
      default:
        break;
    }
  }

private:

  // Función privada que guarda la imagen en formato .png
  void _draw(const char* nb) {
    //    unsigned char rgb[H_IMG * W_IMG * 3], *p = rgb;
    unsigned char *rgb = new unsigned char[H_IMG * W_IMG * 3];
    unsigned char *p = rgb;
    unsigned x, y;

    // La imagen resultante tendrá el nombre dado por la variable 'nb'
    FILE *fp = fopen(nb, "wb");

    // Transformamos las 3 matrices en una arreglo unidimensional
    for (y = 0; y < H_IMG; y++)
        for (x = 0; x < W_IMG; x++) {
            *p++ = red_layer[y][x];    /* R */
            *p++ = green_layer[y][x];    /* G */
            *p++ = blue_layer[y][x];    /* B */
        }
    // La función svpng() transforma las 3 matrices RGB en una imagen PNG 
    svpng(fp, W_IMG, H_IMG, rgb, 0);
    fclose(fp);
}

  
};

#endif
