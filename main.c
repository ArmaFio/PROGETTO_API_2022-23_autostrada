#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCARS 512
#define MAXCOM 21

typedef struct highway_station {
    int distance;
    int nCars;
    int maxcarautonomy;
    struct highway_station *prec;
    struct highway_station *left;
    struct highway_station *right;
    struct car *root;
} highway_station;


typedef struct car{
    int autonomy;
    int n;
    struct car *prec;
    struct car *left;
    struct car *right;
} car;

typedef struct path{
    int stop;
    struct path *next;
    struct path *prec;
}path;


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


void aggiungistazione (highway_station **highway, int distanza, int nAuto, int *autonomia){
    highway_station *new=NULL,*prec=NULL,*next=NULL;
    car *c=NULL,*carnext=NULL,*carprec=NULL;
    int i,done=0;
    next=*highway;
    while(next!=NULL) {
        prec = next;
        if (distanza < next->distance)
            next = next->left;
        else if (distanza > next->distance)
            next = next->right;
        else {
            printf("non aggiunta\n");
            return;
        }
    }
    new=malloc(sizeof (highway_station));
    new->distance=distanza;
    new->nCars=0;
    new->root=NULL;
    new->right=NULL,
    new->left=NULL;
    new->prec=NULL;
    if (prec==NULL)
        *highway=new;
    else {
        if (prec->distance > distanza)
            prec->left = new;
        else
            prec->right = new;
        new->prec = prec;
    }
    for(i=0;i<nAuto&&i<MAXCARS;i++){
        carnext=new->root;
        while(carnext!=NULL&&done==0) {
            carprec = carnext;
            if (autonomia[i] < carnext->autonomy)
                carnext = carnext->left;
            else if (autonomia[i] > carnext->autonomy)
                carnext = carnext->right;
            else {
                carnext->n++;
                new->nCars++;
                done=1;
            }
        }
        if(done==0){
            c= malloc(sizeof (car));
            c->autonomy=autonomia[i];
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
    printf("aggiunta\n");
}

void aggiungiauto(highway_station **highway, int distanza,int autonomia){
    highway_station *curr=NULL;
    car *c=NULL,*carnext=NULL,*carprec=NULL;
    int done=0;
    curr= cercastazione(highway, distanza);
    if(curr==NULL) {
        printf("non aggiunta\n");
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
            curr->nCars++;
            printf("aggiunta\n");
            return;
        }
    }
    else
        printf("non aggiunta\n");
}


void demoliscistazione(highway_station **highway, int distance){
    highway_station *curr=NULL, *curr2;
    curr=*highway;
    while(curr!=NULL) {
        if (distance < curr->distance)
            curr = curr->left;
        else if (distance > curr->distance)
            curr = curr->right;
        else {
            curr2= rimuovistazione(highway, &curr);
            free(curr2);
            printf("demolita\n");
            return;
        }
    }
    printf("non demolita\n");
}


void rottamauto(highway_station **highway, int distance, int autonomy){
    highway_station *curr=NULL;
    car *curr2=NULL, *curr3=NULL;
    curr= cercastazione(highway,distance);
    if (curr==NULL||curr->nCars==0) {
        printf("non rottamata\n");
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
                        while (curr2->right != NULL)
                            curr2 = curr2->right;
                        curr->maxcarautonomy = curr2->autonomy;
                    }
                    free(curr3);
                }
                printf("rottamata\n");
                return;
            }
        }
        printf("non rottamata\n");
        return;
    }
}

void trovapercorso(highway_station** partenza, highway_station** arrivo, int length, path **curr, path **bestp ,int *best){
    int a;
    path* stop;
    highway_station *succ;
    if((*partenza)->maxcarautonomy==0&&(((*arrivo)->distance)!=((*partenza)->distance)))
        return;
    stop=malloc(sizeof(path));
    stop->stop=(*partenza)->distance;
    stop->next=NULL;
    stop->prec = (*curr)->prec;
    free(*curr);
    *curr = stop;
    length++;
    if((*partenza)->maxcarautonomy>=((*arrivo)->distance)-((*partenza)->distance)) {
        if (*best == 0 || length < *best) {
            stop->next = malloc(sizeof(path));
            stop->next->stop = (*arrivo)->distance;
            stop->next->prec = stop;
            stop->next->next=NULL;
            *bestp = stop->next;
            *best = length;
        }
        else
            free(stop);
        return;
    }
    else {
       a = *best;
       succ = stsuccessor(partenza);
       stop->next=malloc(sizeof(path));
       stop->next->prec=stop;
       while (succ != NULL && (length <= (*best) - 2||*best==0) && succ->distance < (*arrivo)->distance&&succ->distance<=(*partenza)->distance+(*partenza)->maxcarautonomy) {
           trovapercorso(&succ, arrivo, length, &((*curr)->next), bestp, best);
           succ = stsuccessor(&succ);
       }
       if (a == *best)
           free((*curr));
       else
           (*curr)->next=NULL;
   }

}





void trovapercorsobackwards(highway_station** partenza, highway_station** arrivo, int length, path **curr, path **bestp ,int *best) {
    path *stop;
    highway_station *pred;
    int a;
    if ((*partenza)->maxcarautonomy==0&&(*partenza)->distance!=(*arrivo)->distance){
        return;
    }
    stop=malloc(sizeof(path));
    stop->stop=(*partenza)->distance;
    stop->prec=*curr;
    stop->next=NULL;
    stop->prec = (*curr)->prec;
    free(*curr);
    *curr = stop;
    length++;
    if ((*partenza)->maxcarautonomy>=((*arrivo)->distance)-((*partenza)->distance)) {
        if (*best == 0 || length < *best) {
            stop->next = malloc(sizeof(path));
            stop->next->stop = (*arrivo)->distance;
            stop->next->prec = stop;
            stop->next->next=NULL;
            *bestp = stop->next;
            *best = length;
        }
        else
            free(stop);
        return;
    } else {
        a=*best;
        pred= predecessor((partenza));
        stop->next=malloc(sizeof(path));
        stop->next->prec=stop;
        while(pred!=NULL&&pred->distance>=(*partenza)->distance-(pred->maxcarautonomy)&&(length <= (*best) - 2||(*best)==0)&&pred->distance > (*arrivo)->distance){
            trovapercorsobackwards(&pred,arrivo,length,&((*curr)->next),bestp,best);
            pred= predecessor(&pred);
        }
        if (a == *best)
            free((*curr));
        (*curr)->next=NULL;
    }
}

path* pianificapercorso(highway_station **highway, int partenza, int arrivo){
    highway_station *part, *arr;
    path *curr=NULL, *best=NULL;
    int b=0;
    if(partenza==arrivo) {
        best = malloc(sizeof(path));
        best->stop = partenza;
    }
    else {
        part= cercastazione(highway,partenza);
        arr= cercastazione(highway,arrivo);
        if(part!=NULL&&arr!=NULL) {
            curr = malloc(sizeof(path));
            curr->prec=NULL;
            if (partenza < arrivo)
                trovapercorso(&part, &arr, 0, &curr, &best, &b);
            else
                trovapercorsobackwards(&part, &arr, 0, &curr, &best, &b);
            while (best != NULL ) {
                if (best->prec != NULL)
                    best = best->prec;
                else
                    break;
            }
        }
    }
    return best;
}



int main(){
    char parola[MAXCOM], a;
    int i=0, j, k, *m;
    highway_station *highway = NULL;
    path *path;
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
                    m = malloc(sizeof(int) * k);
                    for (i = 0; i < k; i++)
                        if (scanf("%d", &m[i]));
                    aggiungistazione(&highway, j, k, m);
                    free(m);
                }
            }
        } else if (strcmp(parola, "aggiungi-auto ") == 0) {
            if (scanf( "%d", &j))
            if (scanf( "%d", &k))
            aggiungiauto(&highway, j, k);
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
                    path = pianificapercorso(&highway, j, k);
                    if (path == NULL)
                        printf("nessun percorso\n");
                    else {
                        printf("%d ", path->stop);
                        while (path->next != NULL) {
                            path = path->next;
                            printf("%d ", path->stop);
                            free(path->prec);
                        }
                        printf("\n");
                        free(path);
                    }
                }
            }
        }
        i=0;
        do{
            a=getchar_unlocked();
        }while(a!='\n'&&a!=EOF);
    }

    return 0;
}