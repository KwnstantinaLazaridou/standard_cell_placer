#include <stdlib.h>
#include <stdio.h>
#include "../include/lists.h"
#include "../include/common.h"
#include <math.h>

void create_sort_array(sort **sort_array, int rows) 
{
	int i;
	
	for (i = 0; i < rows; i++)
	{
		sort_array[i] = (sort *)malloc(sizeof(sort));
		sort_array[i]->next=NULL;
		sort_array[i]->next_cell=NULL;
	}
}

void legalize_y(double height) 
{
	for(curr_cell = root_cell; curr_cell != NULL; curr_cell = curr_cell->next)
	{
		double current_row = CELL_HEIGHT;
		for (current_row = CELL_HEIGHT; current_row < curr_cell->coordinates_y; current_row += CELL_HEIGHT);
		if((current_row - (curr_cell->coordinates_y)) > (CELL_HEIGHT / 2.0)) {
			curr_cell->coordinates_y = current_row - CELL_HEIGHT;
		}
		else {
			curr_cell->coordinates_y = current_row;
		}
		if (curr_cell->coordinates_y == height) {
			curr_cell->coordinates_y = height - CELL_HEIGHT; 
		}
		
	}
}

bool insert_by_coordinate_x ()
{
	if ((curr_sort->next_cell->coordinates_x) > (curr_cell->coordinates_x)) {
		temp_sort=(sort *)malloc(sizeof(sort));
		temp_sort->next=curr_sort;
		temp_sort->next_cell=curr_cell;
		pre_sort->next=temp_sort;
		pre_sort=temp_sort;
		return true;
	}	
	return false;
}

bool insert_by_larger_width()
{
	if ((curr_sort->next_cell->width) < (curr_cell->width)) {
		temp_sort=(sort *)malloc(sizeof(sort));
		temp_sort->next=curr_sort;
		temp_sort->next_cell=curr_cell;
		pre_sort->next=temp_sort;
		pre_sort=temp_sort;
		return true;
	}	
	return false;
}

bool insert_by_smaller_width()
{
	if ((curr_sort->next_cell->width) > (curr_cell->width)) {
		temp_sort=(sort *)malloc(sizeof(sort));
		temp_sort->next=curr_sort;
		temp_sort->next_cell=curr_cell;
		pre_sort->next=temp_sort;
		pre_sort=temp_sort;
		return true;
	}	
	return false;
}

void insert_cell_in_row_in_sort_array(sort *sort_array_row)
{
	for (pre_sort = sort_array_row, curr_sort = sort_array_row->next;
		 curr_sort != NULL; 
		 pre_sort = curr_sort, curr_sort = curr_sort->next) {
		
		if (curr_sort != NULL) {
			if (insert_by_smaller_width()) { 				
				break;
			}
		}
	}
	if (curr_sort == NULL) {
		curr_sort=(sort *)malloc(sizeof(sort));
		curr_sort->next=NULL;
		pre_sort->next=curr_sort;
		curr_sort->next_cell=curr_cell;	
	}
}

void insert_cells_in_rows(sort **sort_array, int rows)
{
	int i;		
	create_sort_array(sort_array, rows);

	for (curr_cell = root_cell ;curr_cell != NULL; curr_cell = curr_cell->next)
	{
		for (i=0 ; i < rows ; i++)
		{
			double row_coordinate_y = i * CELL_HEIGHT;
			if(row_coordinate_y == curr_cell->coordinates_y || row_coordinate_y - curr_cell->coordinates_y > 0.0001)
			{
				insert_cell_in_row_in_sort_array(sort_array[i]);
				break;	
			}	
		}
	}
}

double calculate_new_width_of_row (sort *sort_array_row)
{
	if (sort_array_row->next == NULL) {
		pre2_sort = sort_array_row;	
		return curr_sort->next_cell->width;	
	}
	else {
		for (pre2_sort = sort_array_row, curr2_sort = sort_array_row->next;
		 	 curr2_sort != NULL;
		 	 pre2_sort = curr2_sort, curr2_sort = curr2_sort->next);		
		return pre2_sort->next_cell->coordinates_x + pre2_sort->next_cell->width + curr_sort->next_cell->width;
	}
}

void insert_cell_to_new_sorted_position()
{
	pre_sort->next = curr_sort->next;
	curr_sort->next = curr2_sort;
	pre2_sort->next = curr_sort;
}

bool cell_fits_in_rows_below(sort **sort_array, int i, double chip_width)
{
	int j = 0;
	
	for(j=i-1; j>=0; j=j-1) {
		double new_width = calculate_new_width_of_row(sort_array[j]);
		
		if (new_width <= chip_width) {	
			insert_cell_to_new_sorted_position();
			if(pre2_sort->next_cell == NULL) { 
				//first element of the array
				curr_sort->next_cell->coordinates_x= 0;
			}
			else {
				curr_sort->next_cell->coordinates_x= (pre2_sort->next_cell->coordinates_x) + (pre2_sort->next_cell->width);
			}
			curr_sort->next_cell->coordinates_y = j * CELL_HEIGHT;
			curr_sort = pre_sort;
			return true;
		}
	}
	return false;
}

bool cell_has_larger_coordinate_x()
{
	if ((curr2_sort->next_cell->coordinates_x) > (curr_sort->next_cell->coordinates_x)) {
		insert_cell_to_new_sorted_position();		
		return true; 	
	}
	return false;
}

bool cell_is_larger()
{
	if ((curr2_sort->next_cell->width) > (curr_sort->next_cell->width)) {
		insert_cell_to_new_sorted_position();		
		return true; 	
	}
	return false;
}

bool cell_is_smaller()
{
	if ((curr2_sort->next_cell->width) < (curr_sort->next_cell->width)) {
		insert_cell_to_new_sorted_position();		
		return true; 	
	}
	return false;
}

void original_row_is_first(sort *sort_array_row)
{
	/* move cell to next line, sort the cell there and 
	** let the algorithm take over on next repeat
	*/
	curr_sort->next_cell->coordinates_y += CELL_HEIGHT;

	for (pre2_sort = sort_array_row, curr2_sort = sort_array_row->next;
		 curr2_sort != NULL;
		 pre2_sort = curr2_sort, curr2_sort = curr2_sort->next) {	

		if(curr2_sort != NULL) {
			if(cell_has_larger_coordinate_x()) {
				//curr_cell has smaller coordinate_x	
				break; 
			}
		}
	}
						
	if (curr2_sort == NULL) {
		insert_cell_to_new_sorted_position();
	}
	curr_sort=pre_sort;
}

void original_row_is_not_first(sort **sort_array, int i, double chip_width, int rows) 
{
	if (cell_fits_in_rows_below(sort_array, i, chip_width) == false) {
		if( i < rows) {				
			original_row_is_first(sort_array[i]);
		}
		else {
			printf("Something wrong happened!\n");
		}
	}	
}

void cell_does_not_fit_in_row(sort **sort_array, int i, double chip_width, int rows) 
{	
	if (i == 0) {		
		original_row_is_first(sort_array[i + 1]);
	}
	else {
		original_row_is_not_first(sort_array, i, chip_width, rows);
	}
}

void legalize_x(double chip_width, double chip_height)
{
	int rows =(int) round(chip_height / CELL_HEIGHT);
	sort *sort_array[rows];

	insert_cells_in_rows(sort_array, rows);

	int i = 0;
/*	for(i=0; i< rows; i++) {
		printf ("row %d\n", i);
		curr_sort = sort_array[i]->next;
		while(curr_sort) {
			printf(" name: %s ", curr_sort->next_cell->name);
			curr_sort = curr_sort->next;
		}
		printf("\n"); 
	}
*/
	

	for (i=0 ; i < rows ; i++)
	{
		double  total_width = 0;
		for (pre_sort = sort_array[i], curr_sort = sort_array[i]->next;
			 curr_sort != NULL; 
			 pre_sort = curr_sort, curr_sort = curr_sort->next) {

			double new_total_width = total_width + curr_sort->next_cell->width;
			if(new_total_width > chip_width)
			{	
				cell_does_not_fit_in_row(sort_array, i, chip_width, rows); 
			}
			else {
				curr_sort->next_cell->coordinates_x = total_width;
				total_width = new_total_width;
			}		
		}
	}

/*	printf("\n\n\n");
	for(i=0; i< rows; i++) {
		printf ("row %d\n", i);
		curr_sort = sort_array[i]->next;
		while(curr_sort) {
			printf(" name: %s %lf %lf", curr_sort->next_cell->name, curr_sort->next_cell->coordinates_x, curr_sort->next_cell->coordinates_x+curr_sort->next_cell->width);
			curr_sort = curr_sort->next;
		}
		printf("\n"); 
	}
	printf("\n\n\n");
*/
}


void domino() 
{
	
}

void sort_all_cells_by_coordinate_x()
{
	tmp_cell = root_cell;
	root_sort = NULL;

	for(;tmp_cell; tmp_cell = tmp_cell->next) {
		if (root_sort == NULL) {
			temp_sort = (sort *)malloc(sizeof(sort));
			temp_sort->next_cell = tmp_cell;
			temp_sort->next = NULL;
			root_sort = temp_sort;		
		}
		else {
			curr_sort = root_sort;
			pre_sort = NULL;
			for (; curr_sort; pre_sort = curr_sort,curr_sort = curr_sort->next) {
				if (tmp_cell->coordinates_x < curr_sort->next_cell->coordinates_x) {
					temp_sort = (sort *)malloc(sizeof(sort));
					temp_sort->next_cell = tmp_cell;
					if (pre_sort == NULL) {
						temp_sort->next = curr_sort;
						root_sort = temp_sort;					
					}
					else {
						pre_sort->next = temp_sort;
						temp_sort->next = curr_sort;					
					}
					break;			
				}		
			}
			if (curr_sort == NULL){
				temp_sort = (sort *)malloc(sizeof(sort));
				temp_sort->next_cell = tmp_cell;
				pre_sort->next = temp_sort;
				temp_sort->next = curr_sort;
			}	 		
		}
	}	
}


void init_map_area(int rows_number, row *map_area, double chip_width)
{
	int i;

	for(i = 0; i<rows_number; i++) {
		map_area[i].filled = 0;
		map_area[i].row_max = chip_width;
	}
}

double calculate_cell_movement_cost(double row_x, double row_y, sort *curr_sort) 
{
    return fabs(curr_sort->next_cell->coordinates_x - row_x) + fabs(curr_sort->next_cell->coordinates_y - row_y);
}

void tetris_assign_cells_to_rows(int rows_number, row *map_area)
{
	int i;
	double new_cost, best_cost = INFINITY;
	int best_row;

	for(curr_sort = root_sort; curr_sort; curr_sort = curr_sort->next) {
		best_cost = INFINITY;
		for (i = 0; i < rows_number; i++) {
			if ((curr_sort->next_cell->width + map_area[i].filled) > map_area[i].row_max) {
				new_cost = INFINITY;
			} 
			else {
				new_cost = calculate_cell_movement_cost (map_area[i].filled, i*CELL_HEIGHT, curr_sort);
			}
			if (best_cost > new_cost) {
				best_cost = new_cost;
				best_row = i;
			}  
		}
		curr_sort->next_cell->coordinates_x = map_area[best_row].filled;
		curr_sort->next_cell->coordinates_y = best_row * CELL_HEIGHT;
		map_area[best_row].filled += curr_sort->next_cell->width;
	}
} 

void tetris(double chip_height, double chip_width)
{
	sort_all_cells_by_coordinate_x();
	//print_all_sorted_cells_list();
	int rows_number =(int) round(chip_height / CELL_HEIGHT);				
	row map_area[rows_number];

	int i = 0;

	init_map_area(rows_number, map_area, chip_width);
	tetris_assign_cells_to_rows(rows_number, map_area);
}

void calculate_row_utilization(int rows_number, sort **sort_array, row *map_area)
{
	int i;

	for (i=0 ; i < rows_number ; i++) {
		curr_sort=sort_array[i]->next;			
		while(curr_sort !=NULL) {
			map_area[i].filled += curr_sort->next_cell->width;
			curr_sort=curr_sort->next;
		}
	}
}

bool cells_fit_in_row(int row_number, row *map_area)
{
	if (map_area[row_number].filled > map_area[row_number].row_max)
		return false;
	else
		return true;
}

bool row_is_first(int row_number)
{
	if (row_number == 0)
		return true;
	else 
		return false;
}

bool row_is_last(int i, int rows_number)
{
	if (i == (rows_number-1))
		return true;
	else
		return false;
}

void update_map_area(int row_under_examination, int new_row, row *map_area) 
{
	map_area[new_row].filled += curr_sort->next_cell->width;
	map_area[row_under_examination].filled -= curr_sort->next_cell->width;
}

void set_cell_to_next_row()
{
	pre_sort->next = curr_sort->next;				
	curr_sort->next = curr2_sort->next;				
	curr2_sort->next = curr_sort;				
	curr_sort = pre_sort->next;	
}

bool place_cell_to_new_row(int row_under_examination, int new_row, row *map_area)
{
	bool change_done = false;

	if(curr2_sort->next == NULL) {
		update_map_area(row_under_examination, new_row, map_area);
		set_cell_to_next_row();
	}
	else {
		while(curr2_sort->next != NULL) {
			if((curr2_sort->next->next_cell->coordinates_x) > (curr_sort->next_cell->coordinates_x)) {
				update_map_area(row_under_examination, new_row, map_area);
				set_cell_to_next_row();
				return true;
			}
			pre2_sort=pre2_sort->next;
			curr2_sort=curr2_sort->next;
		}
		if(!change_done) { 
			update_map_area(row_under_examination, new_row, map_area);
			set_cell_to_next_row();
			return true;
		}
	}
	return false;
}

void move_cell_to_row_up(int row_under_examination, row *map_area, sort **sort_array)
{
	double length = 0;
	pre_sort = sort_array[row_under_examination];
	curr_sort = sort_array[row_under_examination]->next;			

	while(curr_sort != NULL) {
		bool change_done  = false;
		//place cells and find cells that do not fit in row
		if(map_area[row_under_examination].row_max > (length + curr_sort->next_cell->width)) {
			curr_sort->next_cell->coordinates_x = length;
			length += curr_sort->next_cell->width;
		}
		else {
			//move to next row
			curr_sort->next_cell->coordinates_y += CELL_HEIGHT;	
			pre2_sort=sort_array[row_under_examination + 1];
			curr2_sort=sort_array[row_under_examination + 1];
			
			change_done = place_cell_to_new_row(row_under_examination, row_under_examination + 1, map_area);						
		}
		if(!change_done) {
			pre_sort=curr_sort;
			curr_sort=curr_sort->next;
		}
	}
}

void move_cell_to_rows_down(int row_under_examination, int rows_number, row *map_area, sort **sort_array)
{
	double length = 0;
	int j;
	pre_sort = sort_array[row_under_examination];
	curr_sort = sort_array[row_under_examination]->next;
			
	while(curr_sort !=NULL) {
		bool change_done  = false;
		//place cells and find cells that do not fit in row
		if(map_area[row_under_examination].row_max > (length + curr_sort->next_cell->width)) {
			curr_sort->next_cell->coordinates_x = length;
			length += curr_sort->next_cell->width;
		}
		else {
			//move to next row
			for(j = rows_number-2; j >=0 ; j = j-1) {
				curr_sort->next_cell->coordinates_y -= CELL_HEIGHT;	
				pre2_sort=sort_array[j];
				curr2_sort=sort_array[j];

				if(map_area[j].row_max > (map_area[j].filled + curr_sort->next_cell->width) ) {
					change_done = place_cell_to_new_row(row_under_examination, j, map_area);
					break;
				}
			}
		}
		if(!change_done) {
			pre_sort = pre_sort->next;
			curr_sort = curr_sort->next;
		}
	}
}


bool check_if_fits_down(int row_under_examination, int new_row, row *map_area, sort **sort_array)
{									
	pre2_sort = sort_array[row_under_examination-new_row];
	curr2_sort = sort_array[row_under_examination-new_row];

    if (map_area[row_under_examination-new_row].row_max > (map_area[row_under_examination-new_row].filled + curr_sort->next_cell->width) ) {	
		curr_sort->next_cell->coordinates_y= CELL_HEIGHT*(row_under_examination-new_row);	
		if (curr2_sort->next == NULL) {
			update_map_area(row_under_examination, row_under_examination - new_row, map_area);
			set_cell_to_next_row();
		}
		else {
			while(curr2_sort->next!=NULL) {
				if((curr2_sort->next->next_cell->coordinates_x) > (curr_sort->next_cell->coordinates_x)) {
					update_map_area(row_under_examination, row_under_examination - new_row, map_area);
					set_cell_to_next_row();
					return true;
				}
				pre2_sort=pre2_sort->next;
				curr2_sort=curr2_sort->next;
			}
			update_map_area(row_under_examination, row_under_examination - new_row, map_area);
			set_cell_to_next_row();
			return true;
		}
		return true;
	}
	return false;
}					

bool check_if_fits_up(int row_under_examination, int new_row, row *map_area, sort **sort_array)
{
	pre2_sort = sort_array[row_under_examination+new_row];
	curr2_sort = sort_array[row_under_examination+new_row];

	if (map_area[row_under_examination+new_row].row_max > (map_area[row_under_examination+new_row].filled + curr_sort->next_cell->width)) {
		curr_sort->next_cell->coordinates_y = CELL_HEIGHT*(row_under_examination-new_row);
		if (curr2_sort->next == NULL) {
			update_map_area(row_under_examination, row_under_examination + new_row, map_area);
			set_cell_to_next_row();
		}
		else {
			while(curr2_sort->next!=NULL) {
				if((curr2_sort->next->next_cell->coordinates_x) > (curr_sort->next_cell->coordinates_x)) {
					update_map_area(row_under_examination, row_under_examination + new_row, map_area);
					set_cell_to_next_row();
					return true;
				}
				pre2_sort = pre2_sort->next;
				curr2_sort = curr2_sort->next;
			}
			update_map_area(row_under_examination, row_under_examination + new_row, map_area);
			set_cell_to_next_row();
		}
		return true;					
	}
	return false;
}		
	
void move_cell_where_it_fits(int row_under_examination, int rows_number, row *map_area, sort **sort_array)
{
	int new_row = 1;
	double length = 0;
	pre_sort = sort_array[row_under_examination];
	curr_sort = sort_array[row_under_examination]->next;
			
	while(curr_sort != NULL) {	
		bool replaced = false;
		if (map_area[row_under_examination].row_max > (length + curr_sort->next_cell->width)) {
			curr_sort->next_cell->coordinates_x = length;
			length += curr_sort->next_cell->width;
		}
		else {
			new_row = 1;
			while ((row_under_examination-new_row) >=0 || (row_under_examination+new_row) < rows_number) {
				if(row_under_examination-new_row >= 0) {
					replaced = check_if_fits_down(row_under_examination, new_row, map_area, sort_array);							
				}		
				if ((row_under_examination+new_row < rows_number) && !replaced) {
					replaced = check_if_fits_up(row_under_examination, new_row, map_area, sort_array);							
				}
				if(replaced) {
					break;
				}
				new_row++;
			}
		}
		if(!replaced) {
			pre_sort=pre_sort->next;
			curr_sort=curr_sort->next;
		}
	}
}

void move_cells_that_do_not_fit_in_rows(int rows_number, sort **sort_array,row *map_area)
{
	int i;

	for (i=0 ; i < rows_number; i++) {
		
		pre_sort=sort_array[i];
		curr_sort=sort_array[i]->next;
		if (!cells_fit_in_row(i, map_area)) {	
			if (row_is_first(i)) {
				move_cell_to_row_up(i, map_area, sort_array);
			}
			else if(row_is_last(i, rows_number)) {
				move_cell_to_rows_down(i, rows_number, map_area, sort_array);
			}
			else {
				move_cell_where_it_fits(i, rows_number, map_area, sort_array);
			}
		}
	}
}

void legalize_every_row(int rows_number, sort **sort_array, double chip_width)
{
	int i;
	for(i=0; i<rows_number; i++) {
		double over=0;
		int overlap=0;
		curr_sort=sort_array[i]->next;
		do {
			overlap=0;
			if(curr_sort != NULL) {
				while(curr_sort->next != NULL) {
					if(curr_sort->next_cell->fixed == 1) {
						if(curr_sort->next !=NULL) {
							if((curr_sort->next->next_cell->fixed !=1)&&((curr_sort->next_cell->coordinates_x + curr_sort->next_cell->width)>curr_sort->next->next_cell->coordinates_x)) {
								overlap=1;
								if((curr_sort->next_cell->coordinates_x+curr_sort->next_cell->width + 0.00001)>=chip_width) {
									curr_sort->next->next_cell->coordinates_x=chip_width- curr_sort->next->next_cell->width;
									curr_sort->next->next_cell->fixed=0;
									curr_sort->next_cell->fixed=0;
									break;
								}
								else {
									curr_sort->next->next_cell->coordinates_x=curr_sort->next_cell->coordinates_x + curr_sort->next_cell->width;
									curr_sort->next->next_cell->fixed=1;
								}					
							}
							else if(((curr_sort->next->next_cell->fixed ==1)&&((curr_sort->next_cell->coordinates_x + curr_sort->next_cell->width)>curr_sort->next->next_cell->coordinates_x+0.0001))) {
								overlap=1;
								curr_sort->next->next_cell->fixed =0;
							}
						}
					}
					else if((curr_sort->next_cell->coordinates_x + curr_sort->next_cell->width)>curr_sort->next->next_cell->coordinates_x + 0.00001) {	
						over=fabs(((curr_sort->next_cell->coordinates_x + curr_sort->next_cell->width)- curr_sort->next->next_cell->coordinates_x)/2);
						if((curr_sort->next_cell->coordinates_x - over )<0) {
							curr_sort->next_cell->coordinates_x=0;
							curr_sort->next->next_cell->coordinates_x=curr_sort->next_cell->width;
							curr_sort->next_cell->fixed=1;
							curr_sort->next->next_cell->fixed=1;
						}
						else if((curr_sort->next->next_cell->coordinates_x + curr_sort->next->next_cell->width+ over )>chip_width) {
							curr_sort->next->next_cell->coordinates_x=chip_width - curr_sort->next->next_cell->width;
							curr_sort->next_cell->coordinates_x=curr_sort->next->next_cell->coordinates_x - curr_sort->next_cell->width;
							curr_sort->next_cell->fixed=0;
							curr_sort->next->next_cell->fixed=1;
						}
						else {
							if(curr_sort->next->next_cell->fixed==1) {
					  			curr_sort->next_cell->coordinates_x= curr_sort->next->next_cell->coordinates_x- curr_sort->next_cell->width;
								curr_sort->next_cell->fixed=1;
							}	
							else {
								curr_sort->next_cell->coordinates_x-=over;
								curr_sort->next->next_cell->coordinates_x+=over;
							}
						}
						overlap=1;
					}
					curr_sort=curr_sort->next;
				}
			}
			curr_sort=sort_array[i]->next;
		} while(overlap==1);
	}
}

