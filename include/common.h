#ifndef common_h__
#define common_h__

#include <stdbool.h>

#define CELL_HEIGHT 1.4

bool strings_are_equal(char *str1, char *str2);
bool string_contains_substring(char *destination, char *source);
void copy_string_with_length(char *destination, char *source, char length);
double half_perimeter();
void calculate_area (double *width, double *height, double utilization);
void init_pins(double w, double h);
void refresh_coordinates();

#endif
