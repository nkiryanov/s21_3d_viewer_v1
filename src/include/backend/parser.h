#ifndef BACKEND_PARSER_H_
#define BACKEND_PARSER_H_


int parser(char *file_model, object_t *Data);
void f_clean_object_data(object_t *Data);
static int f_countvf(FILE *file, object_t *Data, int *len_max_str);
static int f_calloc_vertices(object_t *Data);
void f_free_vertices(object_t *Data);
static int f_getstruct(FILE *file, object_t *object_data, int len_max_str);
static int is_valid_sting(char *str_f);
static int is_valid_face_sting(char *str_f, object_t *Data, int *rowP);
static void f_set_vertices(char *str_f, object_t *Data, int *num);
static void f_set_polygons(char *str_f, object_t *Data, int *num);
static void f_printf(object_t *Data);
static int is_empty_str(char *str_f);


#endif  // BACKEND_PARSER_H_
