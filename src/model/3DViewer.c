#include "3DViewer.h"

int main(){
    int err = 0;
    viewer Data;


    // src:
    //      makefile
    //      папка model: 3DViewer.h 3DViewer.c 1.obj
    
    err = f_parser("model/1.obj", &Data);
    
    printf("\nv первая проверка строк: %d\n", Data.amount_V);
    printf("f первая проверка строк: %d\n", Data.amount_P);

    return err;
}


int f_parser(char *file_model, viewer *Data){
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
        // if (err == 0) {err = f_calloc(&Data);}

        //заполняем структуру vertex и polygon
        if (err == 0) {err = f_getstruct(file, Data);}

        // центрируем модель, перенесем в начало координат
        // if (err == 0) {f_center(&Data);}

        // если у нас выделилась память, но дальше была ошибка, освободим ее
        // if (err != 0) {err = f_free(&Data);}

        fclose(file);
    }
    
    return err;
}

void f_clean(viewer *Data){
    // зануляем структуру
    Data->V = NULL;
    Data->P = NULL;
    Data->amount_V = 0;
    Data->amount_P = 0;
    Data->X_max = 0;
    Data->Y_max = 0;
    Data->Z_max = 0;
    Data->X_min = 0;
    Data->Y_min = 0;
    Data->Z_min = 0;
}

int f_countvf(FILE *file, viewer *Data){
    int err = 0;

    // парсим построково и считаем строки V и P
    char str_f[256] = {'\0'};
    while (fgets(str_f, sizeof str_f, file) != NULL) {
        // если в строке меньше 7 символов, парсим дальше
        // тк мин нормальная строка = 7 символов 'v 1 2 3'
        if (strlen(str_f) <= 7){
            continue;
        }
        // если строка начинается с 'v ' будем выделять под нее память
        if (str_f[0] == 'v' && str_f[1] == ' '){
            Data->amount_V++;

        } else if (str_f[0] == 'f' && str_f[1] == ' '){
            Data->amount_P++;
        }
    }

    // если нет ни одной строки с v или f
    if (Data->amount_V == 0 || Data->amount_P == 0){
        err = 1;
    }
    
    return err;
}

// int f_calloc(viewer *Data){
//     // выделим память на vertex и polygon
//     int err = 0;

//     // Data->amount_V кол-во строк с double x y z

//     return err;
// }

int f_getstruct(FILE *file, viewer *Data){
    int err = 0;
    int rowV = 0; // проверянных строк v
    int rowP = 0; // проверянных строк f

    // парсим построково 
    char str_f[256] = {'\0'};
    while (fgets(str_f, sizeof str_f, file) != NULL) {

        if (strlen(str_f) <= 7){
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
                f_getP(str_f, Data, &rowP);
            }
        }
    }
    
    
    return err;
}

int f_check(char *str_f){
    // проверим разрешенные символы
    int err = 0;
    int i = 1;
    char ch; 
    
    // начинаем проверять строку без 'v/f' вначале
    while ((ch = str_f[i]) && (ch != '\0' && ch != '\n')){
        if ( !(strchr("+-1234567890. ", ch))){
            err = 1;
            break;
        } 
        i++;
    }

    return err;
}

void f_getV(char *str_f, viewer *Data, int *rowV){
    // парс строки типа 'v 1 2 3'
    printf("%s", str_f);

    (*rowV)++;
}

void f_getP(char *str_f, viewer *Data, int *rowP){
    // парс строки типа 'f 1 2 3'
    printf("%s", str_f);
  
    (*rowP)++;

}

// void f_center(viewer *Data){
//     // переносим модель в начало координат
// }

void f_printf(viewer *Data){
    // печать структуры
    printf("V = %d   P = %d\n", Data->amount_V, Data->amount_P);
}
