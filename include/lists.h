#ifndef lib_h__
#define lib_h__

#include <stdbool.h>
#define MAX_LINE_CHARS 1024

struct cell {
    char *name;
    char *type;
    double coordinates_center_x;
    double coordinates_center_y;
    double coordinates_x;
    double coordinates_y;
    double height;
    double width;
	bool fixed;
    struct cell *next;
};

struct input {
    char *name;
    char *type;
    double coordinates_x;
    double coordinates_y;
    struct input *next;
};

struct output {
    char *name;
    char *type;
    double coordinates_x;
    double coordinates_y;
    struct output *next;
};

struct cell_list {
    struct cell *point_cell;
    struct cell_list *next;
};

struct net {
    char *name;
	int net_degree;
    struct input *net_input;
    struct output *net_output;
    struct cell_list *cells;	
    struct net *next;
};

typedef struct sorted
{
	struct cell *next_cell;
	struct sorted *next;
}sort;

typedef struct rows
{
	double row_max;
	double filled;
}row;

struct cell *root_cell, *curr_cell, *tmp_cell, *next_cell, *previous_cell;
struct input *root_input, *curr_input, *tmp_input, *next_input, *previous_input;
struct output *root_output, *curr_output, *tmp_output, *next_output, *previous_output;
struct net *root_net, *curr_net, *tmp_net;
struct cell_list *root_net_cell, *curr_net_cell, *tmp_net_cell; 
sort *curr_sort, *pre_sort, *temp_sort,*curr2_sort, *pre2_sort, *root_sort;

void insert_to_cell_list();
void insert_to_input_list();
void insert_to_output_list();
void insert_to_net_list();

/*for nets*/
void allocate_and_initialize_new_net(char *net_name);
bool store_node_info(char *node_name);

void print_lists();
void print_all_sorted_cells_list();
void init_lists();
void free_lists();

#endif
