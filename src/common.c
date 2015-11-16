#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/common.h"
#include "../include/lists.h"
#include <math.h>

bool strings_are_equal(char *str1, char *str2)
{
	if (strcmp(str1, str2) == 0)
		return true;
	return false;
}

bool string_contains_substring(char *destination, char *source)
{
    if (strstr(destination, source) != NULL)
        return true;
    return false;
}

void copy_string_with_length(char *destination, char *source, char length)
{
    strncpy(destination, source, length);
}

static void init_bounding_box(double* gminx, double* gminy, double* gmaxx, double* gmaxy, double node_x, double node_y)
{
    *gminy = node_y;
    *gminx = node_x;
    *gmaxy = node_y;						
    *gmaxx = node_x;
}

static void recalculate_bounding_box(double* gminx, double* gminy, double* gmaxx, double* gmaxy, double node_x, double node_y)
{
    if (node_y < *gminy) 
        *gminy = node_y;
    if (node_x < *gminx) 
        *gminx = node_x;      
    if (node_y > *gmaxy) 
        *gmaxy = node_y;
    if (node_x > *gmaxx) 
        *gmaxx = node_x;
}

double half_perimeter()
{
    double twl=0;

    for (curr_net = root_net; curr_net; curr_net = curr_net->next) {
        double gminx=0,gminy=0,gmaxx=0,gmaxy=0;
        int found_pin = 0;
        if (curr_net->net_input) {
            init_bounding_box(&gminx, &gminy, &gmaxx, &gmaxy, curr_net->net_input->coordinates_x, curr_net->net_input->coordinates_y);
            found_pin = 1;
        }
        if ((found_pin == 0) && (curr_net->net_output)) {
            init_bounding_box(&gminx, &gminy, &gmaxx, &gmaxy, curr_net->net_output->coordinates_x, curr_net->net_output->coordinates_y);
            found_pin = 1;
        }

        root_net_cell = curr_net->cells;
        if (root_net_cell != NULL) {
            if (found_pin == 0) {
                init_bounding_box(&gminx, &gminy, &gmaxx, &gmaxy, root_net_cell->point_cell->coordinates_center_x, root_net_cell->point_cell->coordinates_center_y);
            }
            else {
                    recalculate_bounding_box(&gminx, &gminy, &gmaxx, &gmaxy, root_net_cell->point_cell->coordinates_center_x, root_net_cell->point_cell->coordinates_center_y);
            }
            for(curr_net_cell = root_net_cell->next; curr_net_cell; curr_net_cell = curr_net_cell->next) {
                recalculate_bounding_box(&gminx, &gminy, &gmaxx, &gmaxy, curr_net_cell->point_cell->coordinates_center_x, curr_net_cell->point_cell->coordinates_center_y);
            }
        }
        twl+=(gmaxx-gminx)+(gmaxy-gminy);
    }
    return twl;
}


void calculate_area (double *width, double *height, double utilization) {
	double total_area = 0;
	double total_x = 0;	
	double temp, temp2 = 0;
	
	for(curr_cell = root_cell; curr_cell; curr_cell = curr_cell->next) {
		total_x += curr_cell->width;
	}
	total_area = total_x * CELL_HEIGHT * utilization;

	temp = sqrt(total_area);

    while(temp2 < temp)
	{
	    temp2 = temp2 + CELL_HEIGHT;
	}
	*height = temp2;
  	*width = total_area/(*height);
}

void init_pins(double w, double h)
{   
    int criterion=1;
    srand( 100 );
 
    for (curr_input = root_input;curr_input != NULL; curr_input = curr_input->next) {
		if (criterion == 0) {
	    	curr_input->coordinates_x=0.0;  
	    	curr_input->coordinates_y=(rand() / ((double)RAND_MAX + 1.0))* (h - 0.0) + 0.0; 
	    	criterion++;
		}
		else if (criterion == 1) {
	    	curr_input->coordinates_x=(rand() / ((double)RAND_MAX + 1.0))* (w - 0.0) + 0.0; 
	    	curr_input->coordinates_y=h;
	    	criterion++; 
		}
		else if (criterion == 2) {
	    	curr_input->coordinates_x=w; 
	    	curr_input->coordinates_y=(rand() / ((double)RAND_MAX + 1.0))* (h - 0.0) + 0.0;
	    	criterion++; 
		}
		else if (criterion == 3) {
	    	curr_input->coordinates_x=(rand() / ((double)RAND_MAX + 1.0))* (w - 0.0) + 0.0; 
	    	curr_input->coordinates_y=0.0;
	    	criterion=0;
		}
	}

	for (curr_output = root_output;curr_output != NULL; curr_output = curr_output->next) {
		if (criterion == 0) {
	    	curr_output->coordinates_x=0.0; 
	   	 	curr_output->coordinates_y=(rand() / ((double)RAND_MAX + 1.0))* (h - 0.0) + 0.0;
	    	criterion++;   
		}
		else if (criterion == 1) {
	    	curr_output->coordinates_x=(rand() / ((double)RAND_MAX + 1.0))* (w - 0.0) + 0.0; 
	    	curr_output->coordinates_y=h;
	    	criterion++;
		}
		else if (criterion == 2) {
	    	curr_output->coordinates_x=w; 
	    	curr_output->coordinates_y=(rand() / ((double)RAND_MAX + 1.0))* (h - 0.0) + 0.0;
	    	criterion++;
		}
		else if(criterion == 3) {
	    	curr_output->coordinates_x=(rand() / ((double)RAND_MAX + 1.0))* (w - 0.0) + 0.0; 
	    	curr_output->coordinates_y=0.0;
	    	criterion=0;
		}
    }  
}

void refresh_coordinates()
{
	for(curr_cell = root_cell; curr_cell != NULL; curr_cell = curr_cell->next) {
		curr_cell->coordinates_center_x = curr_cell->coordinates_x + (curr_cell->width / 2.0);
		curr_cell->coordinates_center_y = curr_cell->coordinates_y + (curr_cell->height / 2.0); 	
	}
}
