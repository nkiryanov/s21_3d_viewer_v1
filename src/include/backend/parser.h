#ifndef BACKEND_PARSER_H_
#define BACKEND_PARSER_H_

int parser(char *file_model, object_t *Data);
void clean_object_data(object_t *Data);
static int count_vertices_and_polygonc(FILE *file, object_t *Data,
                                       int *len_max_str);
static int calloc_vertices_and_polygonc(object_t *Data);
void free_vertices_and_polygonc(object_t *Data);
static int set_struct(FILE *file, object_t *object_data, int len_max_str);
static int is_valid_sting(char *str_f);
static int is_valid_face_sting(char *str_f, object_t *object_data, int *rowP,
                               int *err);
static void set_vertices(char *str_f, object_t *Data, int *num);
static void set_polygons(char *str_f, object_t *Data, int *num);
static void printf_object_data(object_t *Data);
static int is_empty_str(char *str_f);

#endif  // BACKEND_PARSER_H_
