#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "highwaystation.h"
#define MAXCARS 512


highway_station* stsuccessor(highway_station **node){
    highway_station *curr=NULL,*curr2=NULL;
    curr=(*node);
    if (curr->right!=NULL) {
        curr=curr->right;
        while (curr->left != NULL)
            curr = curr->left;
        return curr;
    }
    curr=(*node)->prec;
    curr2=(*node);
    while(curr!=NULL&& curr2==curr->right){
        curr2=curr;
        curr=curr->prec;
    }
    return curr;
}

highway_station* predecessor(highway_station **node){
    highway_station *curr=NULL,*curr2=NULL;
    curr=(*node);
    if (curr->left!=NULL) {
        curr=curr->left;
        while (curr->right != NULL)
            curr = curr->right;
        return curr;
    }
    curr=(*node)->prec;
    curr2=(*node);
    while(curr!=NULL&& curr2==curr->left){
        curr2=curr;
        curr=curr->prec;
    }
    return curr;
}

car* carsuccessor(car **node){
    car *curr=NULL,*curr2=NULL;
    curr=(*node);
    if (curr->right!=NULL) {
        curr=curr->right;
        while (curr->left != NULL)
            curr = curr->left;
        return curr;
    }
    curr=(*node)->prec;
    curr2=(*node);
    while(curr!=NULL&& curr2==curr->right){
        curr2=curr;
        curr=curr->prec;
    }
    return curr;
}

car* rimuoviauto(car **root, car **node){
    car *y=NULL, *x=NULL;
    if ((*node)->right==NULL || (*node)->left==NULL)
        y=(*node);
    else
        y = carsuccessor(node);
    if(y->left!=NULL)
        x=y->left;
    else x=y->right;
    if (x!=NULL)
        x->prec=y->prec;
    if (y->prec==NULL)
        *root=x;
    else if(y==y->prec->left)
        y->prec->left=x;
    else
        y->prec->right=x;
    if (y!=(*node)){
        (*node)->autonomy=y->autonomy;
        (*node)->n=y->n;
    }
    return y;
}
highway_station* rimuovistazione(highway_station **root, highway_station **node){
    highway_station *y=NULL, *x=NULL;
    if ((*node)->right==NULL || (*node)->left==NULL)
        y=(*node);
    else y= stsuccessor(node);
    if(y->left!=NULL)
        x=y->left;
    else x=y->right;
    if (x!=NULL)
        x->prec=y->prec;
    if (y->prec==NULL)
        *root=x;
    else if(y==y->prec->left)
        y->prec->left=x;
    else
        y->prec->right=x;
    if (y!=(*node)){
        (*node)->distance=y->distance;
        (*node)->maxcarautonomy=y->maxcarautonomy;
        (*node)->root=y->root;
        (*node)->nCars=y->nCars;
    }
    return y;
}

highway_station* cercastazione(highway_station **highway, int distance){
    highway_station *curr=NULL;
    curr=*highway;
    while(curr!=NULL) {
        if (distance < curr->distance)
            curr = curr->left;
        else if (distance > curr->distance)
            curr = curr->right;
        else {
            return curr;
        }
    }
    return NULL;
}


void aggiungistazione (highway_station **highway, int distanza, int nAuto, int **autonomia){
    highway_station *new=NULL,*prec=NULL,*next=NULL;
    car *c=NULL,*carnext=NULL,*carprec=NULL;
    int i,done;
    next=*highway;
    while(next!=NULL) {
        prec = next;
        if (distanza < next->distance)
            next = next->left;
        else if (distanza > next->distance)
            next = next->right;
        else {
            printf("non aggiunta");
            free(*autonomia);
            return;
        }
    }
    new=malloc(sizeof (highway_station));
    new->maxcarautonomy=0;
    new->distance=distanza;
    new->nCars=0;
    new->root=NULL;
    new->right=NULL,
    new->left=NULL;
    new->prec=prec;
    if (prec==NULL)
        *highway=new;
    else {
        if (prec->distance > distanza) {
            prec->left = new;
        }
        else if (prec->distance < distanza) {
            prec->right = new;
        }
    }
    for(i=0;i<nAuto&&i<MAXCARS;i++){
        done=0;
        carnext=new->root;
        while(carnext!=NULL&&done==0) {
            carprec = carnext;
            if ((*autonomia)[i]< carnext->autonomy)
                carnext = carnext->left;
            else if ((*autonomia)[i] > carnext->autonomy)
                carnext = carnext->right;
            else {
                carnext->n++;
                new->nCars++;
                done=1;
            }
        }
        if(done==0){
            c= malloc(sizeof (car));
            c->autonomy=(*autonomia)[i];
            c->n=1;
            c->left=NULL;
            c->right=NULL;
            c->prec=NULL;
            if (carprec==NULL) {
                new->root = c;
                new->maxcarautonomy = c->autonomy;
            }
            else {
                if (carprec->autonomy > c->autonomy)
                    carprec->left = c;
                else
                    carprec->right = c;
                c->prec = carprec;
                if (c->autonomy>new->maxcarautonomy)
                    new->maxcarautonomy=c->autonomy;
            }
            new->nCars++;
        }
    }
    printf("aggiunta");
    if(*autonomia!=NULL)
        free(*autonomia);
}

void aggiungiauto(highway_station **highway, int distanza,int autonomia){
    highway_station *curr=NULL;
    car *c=NULL,*carnext=NULL,*carprec=NULL;
    int done=0;
    curr= cercastazione(highway, distanza);
    if(curr==NULL) {
        printf("non aggiunta");
        return;
    }
    carnext=curr->root;
    if(curr->nCars<MAXCARS) {
        while (carnext != NULL) {
            carprec = carnext;
            if (autonomia < carnext->autonomy)
                carnext = carnext->left;
            else if (autonomia > carnext->autonomy)
                carnext = carnext->right;
            else {
                carnext->n++;
                curr->nCars++;
                done = 1;
                printf("aggiunta");
                break;
            }
        }
        if (done == 0) {
            c = malloc(sizeof(car));
            c->autonomy = autonomia;
            c->right=NULL;
            c->left=NULL;
            c->prec=NULL;
            c->n=1;
            if (carprec == NULL)
                curr->root = c;
            else {
                if (carprec->autonomy > c->autonomy)
                    carprec->left = c;
                else
                    carprec->right = c;
                c->prec = carprec;
            }
            if (c->autonomy>curr->maxcarautonomy)
                curr->maxcarautonomy=c->autonomy;
            curr->nCars++;
            printf("aggiunta");
            return;
        }
    }
    else
        printf("non aggiunta");
}


void demoliscistazione(highway_station **highway, int distance){
    highway_station *curr=NULL, *curr2=NULL;
    curr=*highway;
    while(curr!=NULL) {
        if (distance < curr->distance)
            curr = curr->left;
        else if (distance > curr->distance)
            curr = curr->right;
        else {
            printf("demolita");
            curr2= rimuovistazione(highway,&curr);
            free(curr2);
            return;
        }
    }
    printf("non demolita");
}


void rottamauto(highway_station **highway, int distance, int autonomy){
    highway_station *curr=NULL;
    car *curr2=NULL, *curr3=NULL;
    curr= cercastazione(highway,distance);
    if (curr==NULL||curr->nCars==0) {
        printf("non rottamata");
        return;
    }
    else{
        curr2=curr->root;
        while(curr2!=NULL) {
            if (autonomy < curr2->autonomy)
                curr2 = curr2->left;
            else if (autonomy > curr2->autonomy)
                curr2 = curr2->right;
            else {
                curr2->n--;
                if(curr2->n==0){
                    curr3= rimuoviauto(&(curr->root),&curr2);
                    if (curr2->autonomy==curr->maxcarautonomy) {
                        curr2=curr->root;
                        if(curr2==NULL)
                            curr->maxcarautonomy=0;
                        else {
                            while (curr2->right != NULL)
                                curr2 = curr2->right;
                            curr->maxcarautonomy = curr2->autonomy;
                        }
                    }
                    free(curr3);
                }
                curr->nCars--;
                printf("rottamata");
                return;
            }
        }
        printf("non rottamata");
        return;
    }
}

void trovapercorso(highway_station** partenza, highway_station** arrivo, int *length, int **path){
    highway_station *succ, *temp;
    temp=(*arrivo);
    do{
        succ=(*partenza);
        while(succ->maxcarautonomy<temp->distance-succ->distance&&succ!=NULL&&succ->distance<temp->distance){
           succ= stsuccessor(&succ);
        }
        if(!(succ!=NULL&&succ->distance<temp->distance)) {
            (*length) = 0;
            return;
        }
        (*length)++;
        temp=succ;
        (*path)=realloc((*path), sizeof(int)*(*length));
        (*path)[(*length)-1]=succ->distance;
    }while(temp!=(*partenza));
}





void trovapercorsobackwards(highway_station **highway, highway_station** partenza, highway_station** arrivo, int *length,  int **path ) {
    int nearest, exnearest, *toverify= malloc(sizeof(int)), l=0, i;
    highway_station *succ, *temp=NULL, *curr=NULL, *curr2=NULL, *best=NULL, *curr3=NULL;
    temp=(*partenza);
    exnearest=temp->distance;
    do{
        succ=temp;
        nearest=temp->distance;
        while(succ!=NULL&&succ!=(*arrivo)&&temp->maxcarautonomy>=temp->distance-succ->distance&&succ->distance>(*arrivo)->distance){
            succ= predecessor(&succ);
            if(temp->maxcarautonomy>=temp->distance-succ->distance) {
                if(succ==(*arrivo)) {
                    best = succ;
                    nearest= succ->distance;
                    break;
                }
                curr=succ;
                while(curr!=(*arrivo)&&curr!=NULL&&curr->distance>=succ->distance-succ->maxcarautonomy) {
                    curr2 = curr;
                    curr = predecessor(&curr);
                }
                if(!(curr==(*arrivo)&&curr->distance>=succ->distance-succ->maxcarautonomy)) {
                    curr = curr2;
                    while (curr->maxcarautonomy == 0)
                        curr = stsuccessor(&curr);
                }
                if(curr->distance<=nearest) {
                    nearest = curr->distance;
                    best = succ;
                }
            }
        }
        if(!(succ!=NULL&&succ->distance>=(*arrivo)->distance&&curr!=NULL&&nearest!=temp->distance)) {
            (*length) = 0;
            return;
        }
        if(best->distance>exnearest){
            l++;
            toverify= realloc(toverify, sizeof (int)*l);
            toverify[l-1]=(*length) - 1;
        }
        (*length)++;
        (*path) = realloc((*path), sizeof(int) * (*length));
        (*path)[(*length) - 1] = best->distance;
        temp=best;
        exnearest=nearest;
    }while(best!=(*arrivo));
    for(i=l-1;i>=0;i--) {
        curr = cercastazione(highway, (*path)[toverify[i]]);
        if(i>=1)
            curr2 = cercastazione(highway, (*path)[toverify[i] - 1]);
        else
            curr2=(*partenza);
        curr3 = cercastazione(highway, (*path)[toverify[i] + 1]);
        while (curr2->maxcarautonomy >= curr2->distance - curr->distance) {
            curr = predecessor(&curr);
            if (curr2->maxcarautonomy >= curr2->distance - curr->distance &&
                curr->maxcarautonomy >= curr->distance - curr3->distance)
                (*path)[toverify[i]] = curr->distance;
        }
    }
}


int pianificapercorso(highway_station **highway, int partenza, int arrivo, int**path){
    highway_station *part, *arr;
    int b=0, *best= malloc(sizeof(int));
    if(partenza==arrivo) {
        best[0] = partenza;
        b=1;
    }
    else {
        part= cercastazione(highway,partenza);
        arr= cercastazione(highway,arrivo);
        if(part!=NULL&&arr!=NULL&&!((part)->maxcarautonomy==0&&(((arr)->distance)!=((part)->distance)))) {
            if (partenza < arrivo)
                trovapercorso(&part, &arr, &b,&best);
            else
                trovapercorsobackwards(highway,&part, &arr, &b, &best);
        }
    }
    *path=best;

    return b;
}

