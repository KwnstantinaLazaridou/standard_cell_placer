#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/lists.h"
#include "include/common.h"
#include "include/parseEdaFiles.h"
#include "include/legalizer.h"

double chip_height = 0;

int main (int argc, char *argv[]) {

	double chip_width = 0;
	double chip_area = 0;
	double initial_wirelength = 0;
	double final_wirelength = 0;

	init_lists();
	
	if (read_my_testcase(argv[1], argv[2], argv[3], &chip_width, &chip_height) != 0) {
		printf("An error occured during initialize.");
		return 1;
	}
	chip_area = chip_height * chip_width;
	
	//print_lists();
	printf("width = %lf height = %lf\n area = %lf\n", chip_width, chip_height, chip_area);
	initial_wirelength = half_perimeter();
	printf(" %lf \t", initial_wirelength);
	
	//legalize_y(chip_height);
	//legalize_x(chip_width, chip_height);	
	tetris(chip_width, chip_height);
	

	refresh_coordinates();
	final_wirelength = half_perimeter();
	printf(" %lf \t", final_wirelength);

	printf(" %lf \n", ((final_wirelength - initial_wirelength) / initial_wirelength)* 100);
	
//	print_lists();
//	create_bookself_files(argv[1]);

	free_lists();
	return 0;
}
