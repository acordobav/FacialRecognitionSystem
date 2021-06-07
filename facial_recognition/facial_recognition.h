#include <stdio.h>
#include <string.h>
#include <png.h>
#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/**
 * Function to read a png image
 * filepath: image path
 * return: gsl_matrix with the image
**/
gsl_matrix *read_image(char *filepath);

/**
 * Vectorize a matrix, creates a single matrix with
 * one column and M x N rows, each column of the original
 * matrix is stacked into one column in the result
**/
gsl_matrix *vectorize_matrix(gsl_matrix *m);