#include "../../include/backend/object_t.h"


// int main(){
//     //printf("here");
//     object_t Data;

    
//     int err = f_parser("src/backend/1.obj", &Data);

//     printf("err = %d\n", err);
//     if (err == 0){ 


//         f_printf(&Data);
//         f_free(&Data);
//     }
   
    
//     return 0;
// }



int f_parser(char *file_model, object_t *Data){
    int err = 0;

    //откроем файл 
    FILE *file = NULL;
    file = fopen(file_model, "r");

    if (file == NULL){
        err = 1; 
        
    } else {
        
        // обнулим структуру
        f_clean(Data);

        //посчитаем кол-во строк начинающихся с 'v' и 'f'
        err = f_countvf(file, Data);
        rewind(file);
        
        // выделим память для vertex и polygon, тк мы узнали кол-во строк
        if (err == 0) { err = f_calloc(Data);}

        //заполняем структуру vertex и polygon
        if (err == 0) { err = f_getstruct(file, Data);}

        // находим мин макс точки
        if (err == 0) (f_minmax(Data));

        // если у нас выделилась память, но дальше была ошибка, освободим ее
        if (err != 0) { f_free(Data);f_clean(Data);}

        fclose(file);
    }
    
    return err;
}

void f_clean(object_t *Data){
    // зануляем структуру
    Data->V = NULL;
    Data->P = NULL;
    Data->vertices_amount = 0;
    Data->polygons_amount = 0;
    Data->x_max = 0;
    Data->y_max = 0;
    Data->z_max = 0;
    Data->x_min = 0;
    Data->y_min = 0;
    Data->z_min = 0;
}

int isEmpty(char *str_f){
    int res = 1;
    int i = 0;
    char ch;
    
    while( (ch = str_f[i]) && (ch != '\0' && ch != '\n') ) {
        if ( (strchr("1234567890", ch))){
            res = 0;
            break;
        }
        i++;
    }
    return res;
}

int f_countvf(FILE *file, object_t *Data){
    
    int err = 0;

    // парсим построково и считаем строки V и P
    char str_f[256] = {'\0'};
    while (fgets(str_f, sizeof str_f, file) != NULL) {
        // если в строке меньше 7 символов, парсим дальше
        // тк мин нормальная строка = 7 символов 'v 1 2 3'
        if (strlen(str_f) < 7){
            continue;
        }
        // если строка начинается с 'v ' будем и она не пуста    
        if (str_f[0] == 'v' && str_f[1] == ' ' && !isEmpty(str_f) ){
            Data->vertices_amount++;

        } else if (str_f[0] == 'f' && str_f[1] == ' ' && !isEmpty(str_f) ){
            Data->polygons_amount++;
        }
    }
    //printf("%d %d\n",Data->vertices_amount, Data->polygons_amount);

    // если нет ни одной строки с v или f
    if (Data->vertices_amount == 0 || Data->polygons_amount == 0){
        err = 1;
    }
    
    return err;
}

int f_calloc(object_t *Data){
    // выделим память на vertex и polygon
    int err = 0;

    // выделим память на матрицу vertex 
    // Data->vertices_amount кол-во строк с double x y z
    Data->V = (double **)calloc(Data->vertices_amount,  sizeof(double *));
    if (Data->V ){
        for (int i = 0; i < Data->vertices_amount; i++){
            Data->V[i] = (double *)calloc(3, sizeof(double));
        }
    }

    Data->P = (polygon_t *)calloc(Data->polygons_amount,  sizeof(polygon_t));

    if(!Data->V || !Data->P){
        err = 1;
    }

    return err;
}

void f_free(object_t *Data){
    // выделим память на vertex и polygon
   
    if(Data->V){
        for (int i = 0; i < Data->vertices_amount; i++){
            if (Data->V[i]){
                free(Data->V[i]);
            }
            
        }
        free(Data->V);
    }
    
    
    if(Data->P){
       for(int i = 0; i < Data->polygons_amount; i++){
            if (Data->P[i].V_in_P){
                free(Data->P[i].V_in_P);
            }
       }
       free(Data->P);
    }

}

int f_getstruct(FILE *file, object_t *Data){
    int err = 0;
    int rowV = 0; // проверянных строк v
    int rowP = 0; // проверянных строк f

    // парсим построково 
    char str_f[256] = {'\0'};
    while (fgets(str_f, sizeof str_f, file) != NULL) {
         //printf("строка: %s\n", str_f);
        // printf("len = %d\n", strlen(str_f));
        if (strlen(str_f) < 7){
            continue;
        }
        // найдем 'v ', проверим на символы и запарсим
        if (str_f[0] == 'v' && str_f[1] == ' '){
            // если нет лишних символов, то парсим строку
            if (f_check(str_f) == 0){
                f_getV(str_f, Data, &rowV);
            }
            
        } else if (str_f[0] == 'f' && str_f[1] == ' '){
            if (f_check(str_f) == 0){
                if(f_check_P(str_f, Data, &rowP) == 0){
                    f_getP(str_f, Data, &rowP);
                } else {
                    err = 1; break;
                }
               
                
            }
        }
    }
    Data->vertices_amount = rowV;
    Data->polygons_amount = rowP;
  
    return err;
}

int f_check(char *str_f){
    // проверим разрешенные символы
    int err = 0;
    int i = 1;
    char ch; 
    
    
    // начинаем проверять строку без 'v/f' вначале
    if(str_f[0] == 'v') {
        while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')){
            if ( !(strchr("+-1234567890. ", ch))){
                err = 1;
                break;
            } 
            i++;
        }     
           
    } else if(str_f[0] == 'f') {
        while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')){
            if ( !(strchr("1234567890 /", ch))){
                err = 1;
                break;
            } 
            i++;
        }
    }

    return err;
}

int f_check_P(char *str_f, object_t *Data, int *rowP){
    // проверим разрешенные символы
    int err = 0;
    int i = 2; // начинаем проверять строку без 'v/f ' вначале
    char ch; 
    
    // сколько чисел в строке f 1 2 3 4 5 6 и запишем в amount_column_P
    while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')){
        if ( (strchr("1234567890", ch)) && (strchr(" ", str_f[i-1]))){
            Data->P[*rowP].amount_column_P++;

            // printf("ch = %c ", ch);
            // printf("*rowP = %d ", *rowP);
            // printf("Data->P[*rowP].amount_column_P = %d\n", Data->P[*rowP].amount_column_P);
        } 
        i++;
    }

    // выделим память под числа, зная их кол-во
    Data->P[*rowP].V_in_P = (int *)calloc(Data->P[*rowP].amount_column_P, sizeof(int));
    if( !Data->P[*rowP].V_in_P) {err = 1;}

    return err;
}


void f_getV(char *str_f, object_t *Data, int *rowV){
    // парс строки типа 'v 1 2 3'
    //printf("№ %d строка: %s", *rowV, str_f);
    
    // записываем координаты в структуру
    int success = 0; 
    char ch_space = ' ';
    char ch_space1 = '0';
    char ch_space2 = '0';
    double x, y, z;

    success = sscanf(str_f, "v %lf%c%lf%c%lf", &x, &ch_space1, &y, &ch_space2, &z);

    // printf("success = %d\n", success);
    // printf("x = %lf   y = %lf   z = %lf\n", x, y, z);
    // printf("ch_space1 = '%c'  ch_space1 = '%c'\n", ch_space1, ch_space2);
    
    // если успешно записали 3 координаты и 2 пробела, то меняем строку
    if (success == 5 && (ch_space1 == ch_space && ch_space2 == ch_space)){
        Data->V[*rowV][0] = x;
        Data->V[*rowV][1] = y;
        Data->V[*rowV][2] = z;
        //printf("%lf %lf %lf\n",  Data->V[*rowV][0],  Data->V[*rowV][1],  Data->V[*rowV][2]);
        (*rowV)++;
    }

   
}


void f_getP(char *str_f, object_t *Data, int *rowP){
    // парс строки типа 'f 1 2 3'
    //printf("№ %d строка: %s", *rowP, str_f);
    

    int success = 0; 
    int i = 2; // начинаем проверять строку без 'v/f ' вначале
    int j = 0;
    char ch; 
    
    if (Data->P[*rowP].amount_column_P >= 3){
        // сколько чисел в строке f 1 2 3 4 5 6 и запишем в vertices_amount_in_P
        while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')){
            if ( (strchr("1234567890", ch)) && (strchr(" ", str_f[i-1]))){
                
                Data->P[*rowP].V_in_P[j] = atoi(str_f + i);

                //printf("i = %d: %d \n",  i, Data->P[*rowP].V_in_P[j]);
                j++;
                success++;
            } 
            i++;
        }
       // printf("\n");
        

        // если успешно записали 
        if (success == Data->P[*rowP].amount_column_P){
            (*rowP)++;
        } 
    } 

    

}




void f_minmax(object_t *Data){
    Data->x_max = Data->V[0][0];
    Data->y_max = Data->V[0][1];
    Data->z_max = Data->V[0][2];
    Data->x_min = Data->V[0][0];
    Data->y_min = Data->V[0][1];
    Data->z_min = Data->V[0][2];

    for(int i = 0; i < Data->vertices_amount; i++){
        
        if(Data->V[i][0] < Data->x_min){ Data->x_min = Data->V[i][0];}
        if(Data->V[i][1] < Data->y_min){ Data->y_min = Data->V[i][1];}
        if(Data->V[i][2] < Data->z_min){ Data->z_min = Data->V[i][2];}

        if(Data->V[i][0] > Data->x_max){ Data->x_max = Data->V[i][0];}
        if(Data->V[i][1] > Data->y_max){ Data->y_max = Data->V[i][1];}
        if(Data->V[i][2] > Data->z_max){ Data->z_max = Data->V[i][2];}      

    }

}






void f_printf(object_t *Data){

    printf("V = %d   P = %d\n", Data->vertices_amount, Data->polygons_amount);
    printf("Xmin = %lf   Xmax = %lf\n", Data->x_min, Data->x_max);
    printf("Ymin = %lf   Ymax = %lf\n", Data->y_min, Data->y_max);
    printf("Zmin = %lf   Zmax = %lf\n\n", Data->z_min, Data->z_max);    
    // печать структуры
    for(int i = 0; i < Data->vertices_amount; i++){
        printf("%lf %lf %lf\n", Data->V[i][0], Data->V[i][1], Data->V[i][2]);
        // printf("%d", i);
    }
    for(int i = 0; i < Data->polygons_amount; i++){
        //printf("Data->P[i].amount_column_P = %d\n", Data->P[i].amount_column_P);
        for (int j = 0; j < Data->P[i].amount_column_P; j++){
            printf("%d  ", Data->P[i].V_in_P[j]);
        }
        printf("\n");
    }
    
}