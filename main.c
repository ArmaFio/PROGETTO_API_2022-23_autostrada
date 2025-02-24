#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "highwaystation.h"
#define MAXCOM 21

int main(){
    char parola[MAXCOM+2],a;
    int i, j, k, *m, *path,l;
    highway_station *highway = NULL;
    while((a =getchar_unlocked())!=EOF){
        parola[0]=a;
        i=1;
        do{
            a =getchar_unlocked();
            parola[i] = a;
            i++;
        } while (parola[i - 1] != ' ');
        parola[i] = '\0';
        if (strcmp(parola, "aggiungi-stazione ") == 0) {
            if (scanf( "%d", &j)) {
                if (scanf("%d", &k)) {
                    if(k!=0) {
                        m = malloc(sizeof(int) * k);
                        for (i = 0; i < k; i++) {
                            if (scanf("%d", &m[i])) {};
                        }
                        aggiungistazione(&highway, j, k, m);
                    }
                    else {
                        m = NULL;
                        aggiungistazione(&highway, j, k, m);
                    }
                }
            }
        } else if (strcmp(parola, "aggiungi-auto ") == 0) {
            if (scanf( "%d", &j))
            if (scanf( "%d", &k))
            aggiungiauto(highway, j, k);
        } else if (strcmp(parola, "demolisci-stazione ") == 0) {
            if (scanf( "%d", &j))
            demoliscistazione(&highway, j);
        } else if (strcmp(parola, "rottama-auto ") == 0) {
            if (scanf( "%d", &j))
            if (scanf( "%d", &k))
            rottamauto(&highway, j, k);
        } else if (strcmp(parola, "pianifica-percorso ") == 0) {
            if (scanf("%d", &j)) {
                if (scanf("%d", &k)) {
                    l = pianificapercorso(highway, j, k, &path);
                    if (l==0)
                        printf("nessun percorso");
                    else {
                        if(k>j) {
                            for (i = l - 1; i >= 0; i--) {
                                printf("%d ", path[i]);
                            }
                            printf("%d", k);
                        }
                        else{
                            printf("%d ", j);
                            for (i = 0; i < l; i++) {
                                if(i==l-1)
                                    printf("%d", path[i]);
                                else
                                    printf("%d ", path[i]);
                            }
                        }
                    }
                    free(path);
                }
            }
        }
        printf("\n");
        do{
            a=getchar_unlocked();
        }while(a!='\n'&&a!=EOF);
    }

    return 0;
}