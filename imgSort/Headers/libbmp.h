#ifndef _LIBBMP_H_
#define _LIBBMP_H_

typedef struct Pixel_t
{
    unsigned char B, G, R;
    unsigned char Y, Cb, Cr;
    int H;
    float S, V;
} Pixel;

typedef struct Image_t
{
    Pixel* pix;
    int width, height;
    int size;
    unsigned char header[54];
} Image;


int RGBtoHSV(Pixel* p);
int RGBtoYCbCr(Pixel* p);

Image* readImage(char* imgpath);
int writeImage(Image* img, char* imgpath);
int writePixs(Pixel* pixels, Image* img, char* imgpath);

#endif
