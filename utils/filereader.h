void read_int_array(char* filename, int len, int* arr);
void read_csv_ints(char* filename, int len, int* arr, char separator);
void read_lines(char* filename, char* arr, int max_line_length, int max_lines);
int count_lines(char* filename);
int count_fields(char* filename, char separator);
