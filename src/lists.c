#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/common.h"
#include "../include/lists.h"

void init_lists()
{
	root_cell = NULL;
	root_input = NULL;
	root_output = NULL;
    root_net = NULL;
}

void insert_to_cell_list() 
{
	if (root_cell == NULL) {
		tmp_cell->next = NULL;
		root_cell=tmp_cell;
	}
	else {
		curr_cell = root_cell;
		for( ;curr_cell->next; curr_cell = curr_cell->next);
		curr_cell->next = tmp_cell;
		tmp_cell->next = NULL;
	}
}

void insert_to_input_list()
{
    if (root_input == NULL) {
		tmp_input->next = NULL;
		root_input=tmp_input;
	}
	else {
		curr_input = root_input;
		for( ;curr_input->next; curr_input = curr_input->next);
		curr_input->next = tmp_input;
		tmp_input->next = NULL;
	}
}

void insert_to_output_list()
{
    if (root_output == NULL) {
		tmp_output->next = NULL;
		root_output=tmp_output;
    }
    else {
		curr_output = root_output;
		for( ;curr_output->next; curr_output = curr_output->next);
		curr_output->next = tmp_output;
		tmp_output->next = NULL;
    }
}

void insert_to_net_list()
{
    if (root_net == NULL) {
        tmp_net->next = NULL;
        root_net = tmp_net;
    }
    else {
        curr_net = root_net;
        for( ;curr_net->next; curr_net = curr_net->next);
        curr_net->next = tmp_net;
        tmp_net->next = NULL;
    }
} 


void allocate_and_initialize_new_net(char *net_name)
{
    tmp_net = (struct net *) malloc (sizeof(struct net));
	tmp_net->name = (char *) malloc (sizeof(strlen(net_name)));
	strcpy(tmp_net->name, net_name);
	tmp_net->net_input = NULL;
    tmp_net->net_output = NULL;
    tmp_net->cells = NULL;
}

static void store_cell_in_net_cell_list()
{
    tmp_net_cell = (struct cell_list *) malloc (sizeof(struct cell_list));
    tmp_net_cell->point_cell = curr_cell;
                    
    root_net_cell = tmp_net->cells;
    if (root_net_cell == NULL) {
        tmp_net_cell->next = NULL;
        root_net_cell = tmp_net_cell;
        tmp_net->cells = root_net_cell;
    }
    else {
        curr_net_cell = root_net_cell;
        for( ; curr_net_cell->next; curr_net_cell = curr_net_cell->next);
            tmp_net_cell->next = NULL;
            curr_net_cell->next = tmp_net_cell;
    }
}

static void node_is_cell(char* node_name) 
{
    curr_cell = root_cell;
    for ( ;curr_cell; curr_cell = curr_cell->next) {
        if (strings_are_equal(curr_cell->name, node_name)) {
            store_cell_in_net_cell_list();
            break;
        }
    }
}

static bool node_is_input(char *node_name)
{
    curr_input = root_input;
    for( ; curr_input; curr_input=curr_input->next) {
        if (strings_are_equal(curr_input->name, node_name)) {
            return true;
        }
    }
    return false;
}

static bool node_is_output(char* node_name) 
{
    curr_output = root_output;
    for( ; curr_output; curr_output=curr_output->next) {
        if (strings_are_equal(curr_output->name, node_name)) {
            return true;
        }
    }
    return false;
}

bool store_node_info(char *node_name)
{
    if(strings_are_equal(node_name, tmp_net->name)) {    //this is an input/output! check there!
        if (node_is_input(node_name)) { 
            tmp_net->net_input = curr_input;
        }
        else if (node_is_output(node_name)) {
            tmp_net->net_output = curr_output;
        }
        else {
            printf("PROBLEM WITH NODE %s NET %s! SHOULD NOT GET HERE!", node_name, tmp_net->name);
            return false;
        }
    }
    else {
        node_is_cell(node_name);
    }
    return true;
}


static void print_cell_list () 
{
    curr_cell = root_cell;
	for ( ; curr_cell; curr_cell = curr_cell->next) {
		//printf("%s\t%s\t%lf\t%lf\t%lf\t%lf\n", curr_cell->type, curr_cell->name, curr_cell->coordinates_x, curr_cell->coordinates_y, curr_cell->width, curr_cell->height);
		//printf("%s\t%s\t%lf\t%lf\n", curr_cell->type, curr_cell->name, curr_cell->coordinates_x, curr_cell->coordinates_y);
		printf("%s\t%lf\t%lf\n", curr_cell->name, curr_cell->width, curr_cell->height);
	}
}

static void print_output_list () 
{
    curr_output = root_output;
	for ( ; curr_output; curr_output = curr_output->next) {
		//printf("%s\t%s\t%lf\t%lf\n", curr_output->type, curr_output->name, curr_output->coordinates_x, curr_output->coordinates_y);
		printf("%s\t0\t0\tterminal\n", curr_output->name);
	}
}

static void print_input_list ()
{
    curr_input = root_input;
	for ( ; curr_input; curr_input = curr_input->next) {
		//printf("%s\t%s\t%lf\t%lf\n", curr_input->type, curr_input->name, curr_input->coordinates_x, curr_input->coordinates_y);
		printf("%s\t0\t0\tterminal\n", curr_input->name);	
	}
}

static void print_net_list () 
{
    curr_net = root_net;
    for ( ; curr_net; curr_net = curr_net->next) {
        printf("%s\t",curr_net->name);
        if(curr_net->net_input != NULL)
            printf("%s\t", curr_net->net_input->name);
        if(curr_net->net_output != NULL)
            printf("%s\t", curr_net->net_output->name);
        curr_net_cell = curr_net->cells;
        for( ; curr_net_cell; curr_net_cell = curr_net_cell->next) {
            printf("%s\t", curr_net_cell->point_cell->name); 
        }
        printf("\n");
    }
}

void print_all_sorted_cells_list()
{
	for(curr_sort = root_sort; curr_sort; curr_sort = curr_sort->next) {
		 printf("%s\n", curr_sort->next_cell->name); 
	}
}

void print_lists() 
{
	print_cell_list();
	print_output_list();
    print_input_list();
    print_net_list();
}

static void free_input_list() 
{
    while(root_input) {
		curr_input = root_input;
        root_input = curr_input->next;
        free(curr_input->name);
        free(curr_input->type);
        free(curr_input);
    }
}

static void free_output_list() 
{
    while(root_output) {
		curr_output = root_output;
        root_output = curr_output->next;
        free(curr_output->name);
        free(curr_output->type);
        free(curr_output);
    }
}

static void free_cell_list() 
{
    while(root_cell) {
		curr_cell = root_cell;
        root_cell = root_cell->next;
        free(curr_cell->name);
        free(curr_cell->type);
        free(curr_cell);
    }
}

static void free_net_list()
{
	while(root_net) {
		curr_net = root_net;
		root_net = root_net->next;
		if (curr_net->cells)
		{
			root_net_cell = curr_net->cells;
			while(root_net_cell) {
				curr_net_cell = root_net_cell;
				root_net_cell = curr_net_cell->next;
				free(curr_net_cell);
			}
		}
		free(curr_net);
	}
}

void free_lists()
{
    free_cell_list();
    free_input_list();
    free_output_list();
    free_net_list();    
}
