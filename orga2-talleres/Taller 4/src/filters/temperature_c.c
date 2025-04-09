
#include <math.h>
#include "../simd.h"


bool between(unsigned int val, unsigned int a, unsigned int b)
{
	return a <= val && val < b;//modifique between para que quede acorde a la definicion de temperatura
}


void temperature_c    (
	unsigned char *src,
	unsigned char *dst,
	int width,
	int height,
	int src_row_size,
	int dst_row_size)
{
	unsigned char (*src_matrix)[src_row_size] = (unsigned char (*)[src_row_size]) src;
	unsigned char (*dst_matrix)[dst_row_size] = (unsigned char (*)[dst_row_size]) dst;


	for (int i_d = 0, i_s = 0; i_d < height; i_d++, i_s++) {
		for (int j_d = 0, j_s = 0; j_d < width; j_d++, j_s++) {
			bgra_t *p_d = (bgra_t*)&dst_matrix[i_d][j_d*4];
			bgra_t *p_s = (bgra_t*)&src_matrix[i_s][j_s*4];
			int sum = p_s->b+p_s->g+p_s->r;
			int temp;
           		 temp = (int) ((float)(sum)/3);
            if(temp < 32){
                p_d->r = 0;
                p_d->g = 0;
                p_d->b = temp*4+128;

            }
            else if(between(temp,32,96)){
                p_d->r = 0;
                p_d->g = (temp-32)*4;
                p_d->b = 255;
            }
            else if(between(temp,96,160)){
                p_d->r = (temp-96)*4;
                p_d->g = 255;
                p_d->b = 255-(temp-96)*4;

            }
            else if(between(temp,160,224)){
                p_d->r = 255;
                p_d->g = 255-(temp-160)*4;
                p_d->b = 0;
            }
            else{
                p_d->r = 255-(temp-224)*4;
                p_d->g = 0;
                p_d->b = 0;
            }
            p_d->a = 255;
			
		}
	}

}
