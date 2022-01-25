#include <stdio.h>

// RGB to hex color converter:
// https://www.rapidtables.com/convert/color/rgb-to-hex.html Color format= 5 bit
// red, 6 bit green, 5 bit blue = 16 bits.
/********************************************************************************
 * Resamples 24-bit color to 16-bit or 8-bit color
 *******************************************************************************/
int resample_rgb(int num_bits, int color) {
    if (num_bits == 8) {
        color = (((color >> 16) & 0x000000E0) | ((color >> 11) & 0x0000001C) | ((color >> 6) & 0x00000003));
        color = (color << 8) | color;
    } else if (num_bits == 16) {
        color = (((color >> 8) & 0x0000F800) | ((color >> 5) & 0x000007E0) | ((color >> 3) & 0x0000001F));
    }
    return color;
}


void main(){
  unsigned int num_bits, input_hex;
  printf(" Welcome to Hex (24 bit) to 16 or 8 bir Color Converter \n");
  printf("--------------------------------------------------------\n");
  printf("Do you want to convert to 16 bit colors or 8 bit colors?\n");
  printf("Input 16 or 8: ");
  scanf("%u",&num_bits);

  while(1){
    printf("\nEnter hexadecimal color code without \"0x\": ");
    scanf("%x", &input_hex);
    printf("%d bit Color Code= 0X%X",num_bits, resample_rgb(num_bits,input_hex));
  }

}
