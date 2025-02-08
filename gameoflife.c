/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include "imageloader.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Determines what color the cell at the given row/col should be. This function
// allocates space for a new Color. Note that you will need to read the eight
// neighbors of the cell in question. The grid "wraps", so we treat the top row
// as adjacent to the bottom row and the left column as adjacent to the right
// column.
// Helper function to check if a cell is alive
static inline int isCellAlive(Color cell) {
  return cell.R == 255 && cell.G == 255 && cell.B == 255;
}

// Constant direction arrays
static const int MOVE_X[8] = {1, 1, 1, -1, -1, -1, 0, 0};
static const int MOVE_Y[8] = {1, 0, -1, 1, 0, -1, 1, -1};

Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
  if (!image || row < 0 || col < 0 || row >= image->rows ||
      col >= image->cols) {
    return NULL;
  }

  if (isCellAlive(image->image[row][col])) {
    rule >>= 9;
  } else {
    rule &= 0b111111111;
  }
  int liveCells = 0;
  for (int step = 0; step < 8; step++) {
    int new_row = ((MOVE_X[step] + row) + image->rows) % image->rows;
    int new_col = ((MOVE_Y[step] + col) + image->cols) % image->cols;
    if (isCellAlive(image->image[new_row][new_col])) {
      liveCells++;
    }
  }
  Color *c = (Color *)malloc(sizeof(Color));
  if ((rule >> liveCells) & 1) {
    // live
    c->R = c->G = c->B = 255;
  } else {
    // dead
    c->R = c->G = c->B = 0;
  }
  return c;
}

// The main body of Life; given an image and a rule, computes one iteration of
// the Game of Life. You should be able to copy most of this from
// steganography.c
Image *life(Image *image, uint32_t rule) {
  // YOUR CODE HERE
  Image *img = (Image *)malloc(sizeof(Image));
  img->rows = image->rows;
  img->cols = image->cols;
  img->image = (Color **)malloc(sizeof(Color *) * img->rows);
  for (int i = 0; i < img->rows; i++) {
    img->image[i] = (Color *)malloc(sizeof(Color) * img->cols);
  }
  for (int i = 0; i < image->rows; i++) {
    for (int j = 0; j < image->cols; j++) {
      Color *new_color = evaluateOneCell(image, i, j, rule);
      img->image[i][j] = *new_color;
      free(new_color);
    }
  }
  return img;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then
prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this
will be a string. You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you
should exit with code -1. Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv) {
  // YOUR CODE HERE
  if (argc != 3) {
    printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file "
           "(type P3) with maximum value 255.\nrule is a hex number beginning "
           "with 0x; Life is 0x1808.");
    exit(-1);
  }
  char *filename = argv[1];
  Image *origin = readData(filename);
  if (!origin) {
    exit(-1);
  }
  char *charRule = argv[2];
  uint32_t rule = strtoul(charRule, NULL, 16);
  Image *new = life(origin, rule);
  writeData(new);
  // Free the image data
  for (int i = 0; i < origin->rows; i++) {
    free(origin->image[i]);
  }
  free(origin->image);
  free(origin);

  for (int i = 0; i < new->rows; i++) {
    free(new->image[i]);
  }
  free(new->image);
  free(new);
  return 0;
}
