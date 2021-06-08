#include <stdio.h>
#include <string.h>
#include <png.h>
#include <stdlib.h>

#include <dirent.h>  // Show directory files

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
gsl_vector *vectorize_matrix(gsl_matrix *m);

/**
 * Adds each face in the training set to calculate the average face
**/
gsl_vector *get_average_face(gsl_matrix* training_set);

/**
 * Reads images stored in folder_path and generates a training set
 * folder_path: folder path with the images to use
 * return: gsl_matrix with the training set
**/
gsl_matrix *gen_training_set(char* folder_path, int rows, int cols);