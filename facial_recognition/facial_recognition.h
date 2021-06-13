#include <stdio.h>
#include <string.h>
#include <png.h>
#include <stdlib.h>

#include <dirent.h>  // Show directory files

#include <gsl/gsl_matrix.h> // Matrix operations
#include <gsl/gsl_vector.h> // Vector operations
#include <gsl/gsl_linalg.h> // SVD algorithm

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


/**
 * Substracts the average_face to the traning_set, and stores the result
 * in the traning_set matrix
**/ 
gsl_matrix* sub_average_face(gsl_matrix* training_set, gsl_vector* average_face);


/**
 * Transpose matrix
**/
gsl_matrix* transpose(gsl_matrix* matrix);


gsl_vector* mul_matrix_vector(gsl_matrix* matrix, gsl_vector* vector);