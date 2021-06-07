#include <stdio.h>
#include <string.h>
#include <png.h>
#include <stdlib.h>
#include <gsl/gsl_matrix.h>

/**
 * Function to read a png image
 * filepath: image path
 * return: gsl_matrix with the image
**/
gsl_matrix *read_image(char *filepath);