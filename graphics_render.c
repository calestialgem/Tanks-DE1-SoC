#include  "Game.h"
#include  "Graphics.h"



void graphics_build(short pixel_map[STANDARD_Y][STANDARD_X], struct game const* game_data){
  int x,y;


  for (y = 0; y <= STANDARD_Y; y++)
      for (x = 0; x <= STANDARD_X; ++x) {
          //if


      }


}





void graphics_render(struct game const* game_data) { //game_data'yı doğru mu alıyorum?

    short pixel_map[STANDARD_Y][STANDARD_X];
    graphics_build(pixel_map,game_data);   //should return 320x240 pixel map.
    int pixel_buf_ptr = *(int *)PIXEL_BUF_CTRL_BASE;
    int pixel_ptr, x, y;

    /* assume that the box coordinates are valid */
    for (y = 0; y <= STANDARD_Y; y++)
        for (x = 0; x <= STANDARD_X; ++x) {

            pixel_ptr = pixel_buf_ptr + (y << 10) + (x << 1); //Change to correct pixel's address
            *(short *)pixel_ptr = pixel_map[y][x];            //Set pixel color

        }

}
