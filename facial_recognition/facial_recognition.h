#include <stdio.h>
#include <string.h>
#include <png.h>
#include <stdlib.h>
#include <dirent.h> // Show directory files
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
gsl_vector *get_average_face(gsl_matrix *training_set);

/**
 * Reads images stored in folder_path and generates a training set
 * folder_path: folder path with the images to use
 * return: gsl_matrix with the training set
**/
gsl_matrix *gen_training_set(char *folder_path, int rows, int cols);

/**
 * Substracts the average_face to the traning_set, and stores the result
 * in the traning_set matrix
**/
gsl_matrix *sub_average_face(gsl_matrix *training_set, gsl_vector *average_face);

/**
 * Transpose matrix
**/
gsl_matrix *transpose(gsl_matrix *matrix);

/**
 * Multiply matrix and vector
**/
gsl_vector *mul_matrix_vector(gsl_matrix *matrix, gsl_vector *vector);

/**
 * Generates the coordinates of each face in the face subspace
**/
gsl_matrix *get_coordinate_matrix(gsl_matrix *U_t, gsl_matrix *A);

/**
 * Writes an integer in a text file
**/
void write_int(int i, char *filename);

/**
 * Writes a gsl_matrix into a file
**/
void write_matrix(gsl_matrix *matrix, char *filename);

/**
 * Writes a gsl_vector into a file
**/
void write_vector(gsl_vector *vector, char *filename);

/**
 * Reads a gsl_matrix from file
**/
void read_matrix(gsl_matrix *matrix, char *filename);

/**
 * Reads a gsl_vector from file
**/
void read_vector(gsl_vector *vector, char *filename);

/**
 * Apply Facial Recognition Algorithm with Singular Value
 * Decomposition to obtain a face subspace to analyze 
 * if an input image is a known face or not
**/
void analyze_database(char *folderpath, int rows, int cols);

/**
 * Compares an input image against the face space to determine if
 * it is a known face or not
 * filepath: filepath of the image to be analyzed
 * rows: number of rows in the image
 * cols: number of cols in the image
 * e1: minimum value to categorize an image as a face
 * e0: minimum value to categorize an image as a known face
 * return: 0 -> known face
 *        -1 -> not a face
 *        -2 -> unknown face
**/
int compare(char *filepath, int rows, int cols, int e1, int e0, int max_mismatch);

/**
 * Rebuild the founded coincidence
**/
void rebuild_coincidence(gsl_matrix *training_set, int index, int rows, int cols);
