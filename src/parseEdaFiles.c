#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/lists.h"
#include "../include/common.h"

int numNodes = 0;
int numTerminals = 0;
int numNets = 0;
int numPins = 0;

void parse_size_file(char *type, double *width, double *height)
{
    FILE *sizes;
    char buffer[MAX_LINE_CHARS] = {0, };
    char *tmp;
    char delimeter[4] = " \t\n";
    sizes = fopen("NangateLibrarySizes.txt", "r");
    
    if (sizes == NULL) {
        printf("Didn't open Sizes file.\n");
    }
    
    while (fgets(buffer, MAX_LINE_CHARS, sizes) != NULL) {
            tmp = strtok(buffer, delimeter);
            if (tmp == NULL)
                continue;
            if(strings_are_equal(tmp, type)) {
                tmp = strtok(NULL, delimeter);
                *width = (double) atof(tmp);
                tmp = strtok(NULL, delimeter);
                *height = (double) atof(tmp);
            }
	}
}

static void create_cell_list (char *type, char *name, double coordinates_center_x, double coordinates_center_y) 
{
	double height = 0;
	double width = 0;
	
	tmp_cell = (struct cell *) malloc (sizeof(struct cell));
	tmp_cell->type = (char *) malloc (sizeof(strlen(type)));
	tmp_cell->name = (char *) malloc (sizeof(strlen(name)));
    
    parse_size_file(type, &width, &height);

    tmp_cell->width = width;
    tmp_cell->height = height;
	strcpy(tmp_cell->type, type);
    strcpy(tmp_cell->name, name);
	tmp_cell->coordinates_center_x = coordinates_center_x;
	tmp_cell->coordinates_center_y = coordinates_center_y;
	tmp_cell->coordinates_x = tmp_cell->coordinates_center_x - (tmp_cell->width/2);
	tmp_cell->coordinates_y = tmp_cell->coordinates_center_y - (tmp_cell->height/2);
	
    insert_to_cell_list();
	numNodes++;
}

static void create_input_list (char *type, char *name, double coordinates_center_x, double coordinates_center_y)
{
	tmp_input = (struct input *) malloc (sizeof(struct input));
	tmp_input->type = (char *) malloc (sizeof(strlen(type)));
	tmp_input->name = (char *) malloc (sizeof(strlen(name)));

	strcpy(tmp_input->type, type);
    strcpy(tmp_input->name, name);
	tmp_input->coordinates_x = coordinates_center_x;
	tmp_input->coordinates_y = coordinates_center_y;
	
    insert_to_input_list();
	numNodes++;
	numTerminals++;
} 

static void create_output_list (char *type, char *name, double coordinates_center_x, double coordinates_center_y)
{
	tmp_output = (struct output *) malloc (sizeof(struct output));
	tmp_output->type = (char *) malloc (sizeof(strlen(type)));
	tmp_output->name = (char *) malloc (sizeof(strlen(name)));

	strcpy(tmp_output->type, type);
    strcpy(tmp_output->name, name);
	tmp_output->coordinates_x = coordinates_center_x;
	tmp_output->coordinates_y = coordinates_center_y;
	
    insert_to_output_list();
	numNodes++;
	numTerminals++;
}

static bool node_is_last_in_net(char *node_name)
{
    if(string_contains_substring(node_name,";")) {
        node_name[strlen(node_name)-1] = '\0';
        return true;
    }
    return false;
}

static void create_net_list(char *buffer) 
{
	char delimeter[3] = "\t\n";
	char *temp_name;
	int net_degree = 0;
	temp_name = strtok(buffer, delimeter);
	
	allocate_and_initialize_new_net(temp_name);
    numNets++;
	
    while(1) {
		temp_name = strtok(NULL, delimeter);
        int last_node = 0;
		
        if (node_is_last_in_net(temp_name)) {
            last_node = 1 ;
        }
        store_node_info(temp_name);
		numPins++;
		net_degree++;

        if (last_node == 1) {
            break;
        }
    }
    tmp_net->net_degree = net_degree;
    insert_to_net_list();
}

static void check_type_read_and_store_info(char *type, char *name, double coordinates_center_x, double coordinates_center_y)
{
	if (strings_are_equal(type, "input")) {
		create_input_list (type, name, coordinates_center_x, coordinates_center_y);
	}
	else if (strings_are_equal(type, "output")) {
		create_output_list (type, name, coordinates_center_x, coordinates_center_y);
	}
	else {	//it is a cell
		create_cell_list (type, name, coordinates_center_x, coordinates_center_y);
	}
}

static void create_lists_depending_on_type(char *buffer)
{
	char delimeter[3] = "\t\n";
	char *temp_type;
	char *temp_name;
	char *temp_coorx;
	char *temp_coory;
	double coordinates_center_x;
	double coordinates_center_y;

	temp_type = strtok(buffer, delimeter);
	temp_name = strtok(NULL, delimeter);
	temp_coorx = strtok(NULL, delimeter);
	temp_coory = strtok(NULL, delimeter);
	
	coordinates_center_x = (double) atof(temp_coorx);
	coordinates_center_y = (double) atof(temp_coory);
	
	check_type_read_and_store_info(temp_type, temp_name, coordinates_center_x, coordinates_center_y);
}

int read_my_testcase(char *cells_file,char *nets_file, char *area_file, double *chip_width, double *chip_height)
{
    FILE *cells, *area, *nets;
	char buffer[MAX_LINE_CHARS] = {0,};
	double utilization = 0;

	cells = fopen(cells_file, "r");	
	
	if (cells == NULL) {
		printf("Could not open cells file.");
		return 1;
	}
	
	while (fgets(buffer, MAX_LINE_CHARS, cells) != NULL) {
		create_lists_depending_on_type(buffer);
	}
	fclose(cells);

	area = fopen(area_file, "r");
	if(area != NULL) {
		fscanf(area,"chip height = %lf chip width = %lf", chip_height, chip_width);
		fclose(area);		
	}
	else {
		printf("No area file given. Calculating area form circuit.\n");
		scanf("%lf", &utilization);		
		calculate_area(chip_width, chip_height, utilization);
		init_pins(*chip_width, *chip_height);
	}
	
	nets = fopen(nets_file, "r");
	if(nets == NULL) {
		printf("Could not open nets file.\n");
		return 1;
	}
	while (fgets(buffer, MAX_LINE_CHARS, nets) != NULL) {
		create_net_list(buffer);
	}
	fclose(nets);
	
	return 0;
}
