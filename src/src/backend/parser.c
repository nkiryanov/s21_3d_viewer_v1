#include "backend/parser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backend/object_t.h"

static int count_vertices_and_polygonc(FILE *file, object_t *Data,
                                       int *len_max_str);
static int calloc_vertices_and_polygonc(object_t *object_data);
static int set_struct(FILE *file, object_t *object_data, int len_max_str);
static int is_empty_str(char *str_f);
static int is_valid_sting(char *str_f);
static int is_valid_face_sting(char *str_f, object_t *object_data, int *rowP,
                               int *err);
static void set_vertices(char *str_f, object_t *object_data, int *rowV);
static void set_polygons(char *str_f, object_t *object_data, int *rowP);

int parser(char *file_model, object_t *object_data) {
  int err = 0;

  // откроем файл
  FILE *file = NULL;
  file = fopen(file_model, "r");

  if (file == NULL) {
    err = 2;

  } else {
    // обнулим структуру
    clean_object_data(object_data);

    // посчитаем кол-во строк начинающихся с 'v' и 'f'
    int len_max_str = 0;
    err = count_vertices_and_polygonc(file, object_data, &len_max_str);
    len_max_str++;
    rewind(file);

    // выделим память для vertex и polygon, тк мы узнали кол-во строк
    if (err == 0) {
      err = calloc_vertices_and_polygonc(object_data);
    }

    // заполняем структуру vertex и polygon
    if (err == 0) {
      err = set_struct(file, object_data, len_max_str);
    }

    // если у нас выделилась память, но дальше была ошибка, освободим ее
    if (err != 0) {
      free_vertices_and_polygonc(object_data);
      clean_object_data(object_data);
    }

    fclose(file);
  }

  return err;
}

void clean_object_data(object_t *object_data) {
  // зануляем структуру
  object_data->vertices = NULL;
  object_data->polygons = NULL;
  object_data->vertices_amount = 0;
  object_data->polygons_amount = 0;
}

static int count_vertices_and_polygonc(FILE *file, object_t *Data,
                                       int *len_max_str) {
  int err = 0;
  char ch = '\0';
  int len_tmp_str = 0;

  // парсим посимвольно и считаем строки V и P
  while ((ch = fgetc(file))) {
    len_tmp_str++;
    if (ch == EOF) {
      if (len_tmp_str > *len_max_str) {
        *len_max_str = len_tmp_str;
      }
      len_tmp_str = 0;
      break;
    }

    if (ch == '\n') {
      if (len_tmp_str > *len_max_str) {
        *len_max_str = len_tmp_str;
      }
      len_tmp_str = 0;
    }

    // если строка начинается с 'v '
    if (ch == 'v' && len_tmp_str == 1) {
      if ((ch = fgetc(file)) && ch == ' ') {
        Data->vertices_amount++;
      }
      len_tmp_str++;

    } else if (ch == 'f' && len_tmp_str == 1) {
      if ((ch = fgetc(file)) && ch == ' ') {
        Data->polygons_amount++;
      }
      len_tmp_str++;
    }
  }
  // printf("len = %d\n",*len_max_str);
  // printf("%d %d\n",Data->vertices_amount, Data->polygons_amount);

  // если нет ни одной строки с v или f
  if (Data->vertices_amount == 0 || Data->polygons_amount == 0) {
    err = 1;
  }

  return err;
}

static int calloc_vertices_and_polygonc(object_t *object_data) {
  // выделим память на vertex и polygon
  int err = 0;

  object_data->vertices =
      (vector_t *)calloc(object_data->vertices_amount, sizeof(vector_t));
  object_data->polygons =
      (polygon_t *)calloc(object_data->polygons_amount, sizeof(polygon_t));

  if (!object_data->vertices || !object_data->polygons) {
    err = 1;
  }

  return err;
}

void free_vertices_and_polygonc(object_t *object_data) {
  // освободим память на vertex и polygon

  if (object_data->vertices) {
    free(object_data->vertices);
  }

  if (object_data->polygons) {
    for (uint32_t i = 0; i < object_data->polygons_amount; i++) {
      if (object_data->polygons[i].vertex_indexes) {
        free(object_data->polygons[i].vertex_indexes);
      }
    }
    free(object_data->polygons);
  }
}

static int set_struct(FILE *file, object_t *object_data, int len_max_str) {
  int err = 0;
  int rowV = 0;  // проверянных строк v
  int rowP = 0;  // проверянных строк f

  // парсим построково
  char str_f[len_max_str];
  while (fgets(str_f, sizeof str_f, file) != NULL) {
    // printf("строка: %s\n", str_f);
    // printf("len = %d\n", strlen(str_f));
    if (strlen(str_f) < 7) {
      continue;
    }
    // найдем 'v ', проверим на символы и запарсим
    if (str_f[0] == 'v' && str_f[1] == ' ') {
      // если нет лишних символов, то парсим строку
      if (is_valid_sting(str_f) && !is_empty_str(str_f)) {
        set_vertices(str_f, object_data, &rowV);
      }

    } else if (str_f[0] == 'f' && str_f[1] == ' ') {
      if (is_valid_sting(str_f) && !is_empty_str(str_f)) {
        if (is_valid_face_sting(str_f, object_data, &rowP, &err) && !err) {
          set_polygons(str_f, object_data, &rowP);
        } else if (err) {
          break;
        }
      }
    }
  }
  object_data->vertices_amount = rowV;
  object_data->polygons_amount = rowP;

  if (object_data->vertices_amount == 0 || object_data->polygons_amount == 0) {
    err = 1;
  }

  return err;
}

static int is_empty_str(char *str_f) {
  int res = 1;
  int i = 0;
  char ch;

  while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')) {
    if ((strchr("1234567890", ch))) {
      res = 0;
      break;
    }
    i++;
  }
  return res;
}

static int is_valid_sting(char *str_f) {
  // проверим разрешенные символы
  int res = 1;
  int i = 1;
  char ch;

  // начинаем проверять строку без 'v/f' вначале
  if (str_f[0] == 'v') {
    while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')) {
      if (!(strchr("+-1234567890. ", ch))) {
        res = 0;
        break;
      }
      i++;
    }

  } else if (str_f[0] == 'f') {
    while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')) {
      if (!(strchr("1234567890 /", ch))) {
        res = 0;
        break;
      }
      i++;
    }
  }

  return res;
}

static int is_valid_face_sting(char *str_f, object_t *object_data, int *rowP,
                               int *err) {
  // проверим разрешенные символы
  int res = 1;
  int i = 2;  // начинаем проверять строку без 'v/f ' вначале
  char ch;

  // сколько чисел в строке f 1 2 3 4 5 6 и запишем в amount
  while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')) {
    if ((strchr("1234567890", ch)) && (strchr(" ", str_f[i - 1]))) {
      object_data->polygons[*rowP].amount++;

      // printf("ch = %c ", ch);
      // printf("*rowP = %d ", *rowP);
      // printf("object_data->polygons[*rowP].amount = %d\n",
      // object_data->polygons[*rowP].amount);
    }
    i++;
  }

  // выделим память под числа, зная их кол-во, если их больше 3х
  // printf("num before = %d\n", object_data->polygons[*rowP].amount);
  if (object_data->polygons[*rowP].amount < 3) {
    res = 0;
    object_data->polygons[*rowP].amount = 0;
  } else {
    object_data->polygons[*rowP].vertex_indexes = (uint32_t *)calloc(
        object_data->polygons[*rowP].amount, sizeof(uint32_t));
    if (!object_data->polygons[*rowP].vertex_indexes) {
      *err = 1;
    }
  }
  // printf("num after = %d\n", object_data->polygons[*rowP].amount);
  // printf("valid = %d\n", res);
  // printf("err = %d\n\n", *err);
  return res;
}

static void set_vertices(char *str_f, object_t *object_data, int *rowV) {
  // парс строки типа 'v 1 2 3'
  // printf("№ %d строка: %s", *rowV, str_f);

  // записываем координаты в структуру
  int success = 0;
  char ch_space = ' ';
  char first_separator = '0';
  char second_separator = '0';
  double x, y, z;

  success = sscanf(str_f, "v %lf%c%lf%c%lf", &x, &first_separator, &y,
                   &second_separator, &z);

  // printf("success = %d\n", success);
  // printf("x = %lf   y = %lf   z = %lf\n", x, y, z);
  // printf("first_separator = '%c'  first_separator = '%c'\n", first_separator,
  // second_separator);

  if (success == 5 &&
      (first_separator == ch_space && second_separator == ch_space)) {
    object_data->vertices[*rowV].x = x;
    object_data->vertices[*rowV].y = y;
    object_data->vertices[*rowV].z = z;
    // printf("%lf %lf %lf\n",  object_data->vertices[*rowV][0],
    // object_data->vertices[*rowV][1],  object_data->vertices[*rowV][2]);
    (*rowV)++;
  }
}

static void set_polygons(char *str_f, object_t *object_data, int *rowP) {
  // парс строки типа 'f 1 2 3'
  // printf("№ %d строка: %s", *rowP, str_f);

  uint32_t success = 0;
  int i = 2;  // начинаем проверять строку без 'f ' вначале
  int j = 0;  // столбец в polygons[*rowP]
  char ch;

  if (object_data->polygons[*rowP].amount >= 3) {
    // сколько чисел в строке f 1 2 3 4 5 6 и запишем в vertices_amount_in_P
    while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')) {
      if ((strchr("1234567890", ch)) && (strchr(" ", str_f[i - 1]))) {
        object_data->polygons[*rowP].vertex_indexes[j] = atoi(str_f + i);
        // printf("i = %d: %d \n",  i,
        // object_data->polygons[*rowP].vertex_indexes[j]);
        j++;
        success++;
      }
      i++;
    }
    // printf("\n");
    // если успешно записали
    if (success == object_data->polygons[*rowP].amount) {
      (*rowP)++;
    } else {
      object_data->polygons[*rowP].amount = 0;
    }
  }
}

void printf_object_data(object_t *object_data) {
  printf("vertices = %d   polygons = %d\n", object_data->vertices_amount,
         object_data->polygons_amount);

  // печать структуры
  for (uint32_t i = 0; i < object_data->vertices_amount; i++) {
    printf("%lf %lf %lf\n", object_data->vertices[i].x,
           object_data->vertices[i].y, object_data->vertices[i].z);
    // printf("%d", i);
  }

  for (uint32_t i = 0; i < object_data->polygons_amount; i++) {
    // printf("object_data->polygons[i].amount = %d\n",
    // object_data->polygons[i].amount);
    for (uint32_t j = 0; j < object_data->polygons[i].amount; j++) {
      printf("%d  ", object_data->polygons[i].vertex_indexes[j]);
    }
    printf("\n");
  }
}
