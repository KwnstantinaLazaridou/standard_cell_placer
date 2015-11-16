#ifndef parseEdaFiles_h__
#define parseEdaFiles_h__

void parse_size_file(char *type, double *width, double *height);
int read_my_testcase(char *cells_file,char *area_file, char *nets_file, double *chip_width, double *chip_height);

#endif
