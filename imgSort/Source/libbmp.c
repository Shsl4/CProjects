#include "../Headers/libbmp.h"
#include "../Headers/Utilities.h"

#ifdef _WIN32

// Disables the fopen_s / fwrite_s... deprecation warnings on windows with the visual studio compiler
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4996)

#endif

int RGBtoHSV(Pixel* p)
{
    const float r = p->R / 256.f, g = p->G / 256.f, b = p->B / 256.f;
    float max = r, min = r;

    p->H = 0;
    p->S = 0;
    p->V = 0;

    if (b > max)
        max = b;
    if (g > max)
        max = g;

    if (b < min)
        min = b;
    if (g < min)
        min = g;

    // Hue
    if (max == min)
        p->H = 0;
    if (max == r)
        p->H = (int)(60 * (float)(g - b) / (max - min) + 360) % 360;
    if (max == g)
        p->H = (int)(60 * (float)(b - r) / (max - min) + 120);
    if (max == b)
        p->H = (int)(60 * (float)(r - g) / (max - min) + 240);

    // Saturation
    p->S = (max == 0 ? 0.f : 1.f - ((float)min / max));

    // Value
    p->V = (float)max;

    return 0;
}

int RGBtoYCbCr(Pixel* p)
{
    const unsigned char R = p->R, G = p->G, B = p->B;
    p->Y = (int)(0.299 * R + 0.587 * G + 0.114 * B);
    p->Cb = (int)(-0.1687 * R - 0.3313 * G + 0.5 * B + 128);
    p->Cr = (int)(0.5 * R - 0.4187 * G - 0.0813 * B + 128);
    return 0;
}

Image* readImage(char* imgpath)
{
    int buf = 0, i, pos = 0;

    Image* img = (Image*)calloc(sizeof(Image), 1);

    printf("\nReading %s...", imgpath);
    FILE* pfile = fopen(imgpath, "rb");
    if (!pfile)
    {
        printf("\nUnable to open %s\n", imgpath);
        return 0;
    }

    fread(img->header, 54, 1, pfile);
    fseek(pfile, 18, SEEK_SET);
    fread(&img->width, 4, 1, pfile);

    printf(" Size: %dx", img->width);

    const int pad = (4 - (img->width * 3 % 4)) % 4;

    fread(&img->height, 4, 1, pfile);
    printf("%d\n\n", img->height);

    img->size = img->height * img->width;
    img->pix = (Pixel*)calloc(sizeof(Pixel), img->size);

    fseek(pfile, 54, SEEK_SET);
    for (i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            fread(&(img->pix[pos].B), 1, 1, pfile);
            fread(&(img->pix[pos].G), 1, 1, pfile);
            fread(&(img->pix[pos].R), 1, 1, pfile);
            pos++;
        }
        fread(&buf, pad, 1, pfile);
    }

    for (i = 0; i < img->size; i++)
    {
        RGBtoHSV(&(img->pix[i]));
        RGBtoYCbCr(&(img->pix[i]));
    }

    fclose(pfile);

    return img;
}


int writeImage(Image* img, char* imgpath)
{
    int pos = 0;
    int buf = 0;

    const int pad = (4 - (img->width * 3 % 4)) % 4;

    if(!minimal)
    {
        printf("Writing image %s...\n", imgpath);
    }
    
    FILE* output = fopen(imgpath, "wb");
    if (!output)
    {
        printf("Unable to open %s for writing.\n", imgpath);
        return -1;
    }
    fwrite(img->header, 54, 1, output);
    for (int i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            fwrite(&(img->pix[pos].B), 1, 1, output);
            fwrite(&(img->pix[pos].G), 1, 1, output);
            fwrite(&(img->pix[pos].R), 1, 1, output);
            pos++;
        }
        fwrite(&buf, pad, 1, output);
    }

    fclose(output);

    if(!minimal)
    {
        printf("Wrote image %s\n\n", imgpath);
    }

    return 0;
}

int writePixs(Pixel* pixels, Image* img, char* imgpath)
{
    int pos = 0;
    int buf = 0;

    const int pad = (4 - (img->width * 3 % 4)) % 4;
    if(!minimal)
    {
        printf("Writing image %s...\n", imgpath);
    }
    
    FILE* output = fopen(imgpath, "wb");
    if (!output)
    {
        printf("Unable to open %s for writing.\n", imgpath);
        return -1;
    }

    fwrite(img->header, 54, 1, output);

    for (int i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            fwrite(&(pixels[pos].B), 1, 1, output);
            fwrite(&(pixels[pos].G), 1, 1, output);
            fwrite(&(pixels[pos].R), 1, 1, output);
            pos++;
        }
        fwrite(&buf, pad, 1, output);
    }

    fclose(output);

    if(!minimal)
    {
        printf("Wrote image %s\n\n", imgpath);
    }
    
    return 0;
}
