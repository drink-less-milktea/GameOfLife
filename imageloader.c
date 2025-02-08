/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include "imageloader.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Opens a .ppm P3 image file, and constructs an Image object.
// You may find the function fscanf useful.
// Make sure that you close the file with fclose before returning.
Image *readData(char *filename) {
  // YOUR CODE HERE
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    return NULL;
  }
  Image *img = (Image *)malloc(sizeof(Image));
  char *buf = (char *)malloc(sizeof(char) * 20);
  int row, col, max;
  fscanf(fp, "%s %d %d %d", buf, &col, &row, &max);
  Color **colors = (Color **)malloc(sizeof(Color *) * row);
  for (int i = 0; i < row; i++) {
    colors[i] = (Color *)malloc(sizeof(Color) * col);
  }
  int r, g, b;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      fscanf(fp, "%d %d %d", &r, &g, &b);
      colors[i][j].R = (unsigned char)r;
      colors[i][j].G = (unsigned char)g;
      colors[i][j].B = (unsigned char)b;
    }
  }
  img->cols = col;
  img->rows = row;
  img->image = colors;
  free(buf);
  fclose(fp);
  return img;
}

// Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the
// image's data.
void writeData(Image *image) {
  // YOUR CODE HERE
  printf("P3\n%d %d\n255\n", image->cols, image->rows);
  for (int i = 0; i < image->rows; i++) {
    /* printf(" "); */
    for (int j = 0; j < image->cols - 1; j++) {
      printf("%3d %3d %3d   ", image->image[i][j].R, image->image[i][j].G,
             image->image[i][j].B);
    }
    printf("%3d %3d %3d", image->image[i][image->cols - 1].R,
           image->image[i][image->cols - 1].G,
           image->image[i][image->cols - 1].B);
    printf("\n");
  }
}

// Frees an image
void freeImage(Image *image) {
  // YOUR CODE HERE
  for (int i = 0; i < image->rows; i++) {
    free(image->image[i]);
  }
  free(image->image);
  free(image);
}

/* int main() { */
/*   Image *img = readData("testInputs/JohnConway.ppm"); */
/*   writeData(img); */
/*   return 0; */
/* } */
