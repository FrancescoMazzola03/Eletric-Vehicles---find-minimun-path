#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// DICHIARAZIONE ALBERO ROSSO NERO
struct Tree
{
    int elem;            // stazione
    int color;           // 1-red, 0-black
    int park[513];       // parcheggio macchine
    struct Tree *parent; // parent
    struct Tree *right;  // right-child
    struct Tree *left;   // left child
};
typedef struct Tree RBTree;

// DICHIARAZIONE ALBERO PER PIANIFICA-PERCORSO
struct Tree2
{
    int elem;             // elemento
    int color;            // 1-red, 0-black
    int park[3];          // indicatore posizione,distanza,stazione
    struct Tree2 *parent; // parent
    struct Tree2 *right;  // right-child
    struct Tree2 *left;   // left child
};
typedef struct Tree2 RBTree2;

// FUNZIONI PER ALBERO ROSSO NERO
void rb_right_rotate(RBTree **radice, RBTree *x)
{
    RBTree *left = x->left;

    if (x->left != NULL)
    {

        x->left = left->right;

        if (x->left != NULL)
        {
            x->left->parent = x;
        }

        left->parent = x->parent;

        if (x->parent == NULL)
        {
            *radice = left;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = left;
        }
        else
        {
            x->parent->right = left;
        }

        left->right = x;
        x->parent = left;
    }
}
//----------------------------
void rb_left_rotate(RBTree **radice, RBTree *x)
{
    RBTree *right = x->right;

    if (x->right != NULL)
    {

        x->right = right->left;

        if (x->right != NULL)
        {
            x->right->parent = x;
        }

        right->parent = x->parent;

        if (x->parent == NULL)
        {
            *radice = right;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = right;
        }
        else
        {
            x->parent->right = right;
        }

        right->left = x;
        x->parent = right;
    }
}
//----------------------------
void rb_insert_fixup(RBTree **radice, RBTree *z)
{
    while (z->parent != NULL && z->parent->color == 1)
    {
        if (z->parent == z->parent->parent->left)
        {
            RBTree *y = z->parent->parent->right;
            if (y != NULL && y->color == 1)
            {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    rb_left_rotate(radice, z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                rb_right_rotate(radice, z->parent->parent);
            }
        }
        else
        {
            RBTree *y = z->parent->parent->left;
            if (y != NULL && y->color == 1)
            {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rb_right_rotate(radice, z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                rb_left_rotate(radice, z->parent->parent);
            }
        }
    }
    (*radice)->color = 0;
}
//----------------------------
int rb_insert(RBTree **radice, int elem, int num_auto)
{

    RBTree *z = (RBTree *)malloc(sizeof(RBTree));
    z->right = NULL;
    z->left = NULL;
    z->parent = NULL;
    z->elem = elem;
    z->color = 1;
    memset(z->park, 0, 513 * sizeof(int));

    RBTree *y = NULL;
    RBTree *x = *radice;

    while (x != NULL)
    {
        y = x;
        if (z->elem == x->elem)
        {
            return 0;
        }
        if (z->elem < x->elem)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == NULL)
    {
        *radice = z;
    }
    else if (z->elem < y->elem)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }

    z->left = NULL;
    z->right = NULL;
    z->color = 1;
    rb_insert_fixup(radice, z);

    int i, max = 0, station = 0, pos = 0;
    for (i = 0; i < num_auto; i++)
    {
        if (fscanf(stdin, "%d", &station) != EOF)
        {
            z->park[i] = station;
            if (z->park[i] > max)
            {
                max = z->park[i];
                pos = i;
            }
        }
    }
    int swap;
    swap = z->park[0];
    z->park[pos] = swap;
    z->park[0] = max;

    return 1;
}
//----------------------------
void rb_transplant(RBTree **radice, RBTree *u, RBTree *v)
{
    if (u->parent == NULL)
    {
        *radice = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if (v != NULL)
    {
        v->parent = u->parent;
    }
}
//----------------------------
void rb_delete_fixup(RBTree **radice, RBTree *x)
{
    while (x != *radice && x != NULL && x->color == 0)
    {
        if (x == x->parent->left)
        {
            RBTree *w = x->parent->right;
            if (w != NULL && w->color == 1)
            {
                w->color = 0;
                x->parent->color = 1;
                rb_left_rotate(radice, x->parent);
                w = x->parent->right;
            }
            if (w != NULL && w->left != NULL && w->right != NULL && w->left->color == 0 && w->right->color == 0)
            {
                if (w != NULL)
                    w->color = 1;
                x = x->parent;
            }
            else
            {
                if (w != NULL && w->right != NULL && w->right->color == 0)
                {
                    if (w->left != NULL)
                    {
                        w->left->color = 0;
                    }
                    if (w != NULL)
                    {
                        w->color = 1;
                    }
                    rb_right_rotate(radice, w);
                    w = x->parent->right;
                }
                if (w != NULL)
                {
                    w->color = x->parent->color;
                }
                x->parent->color = 0;
                if (w != NULL && w->right != NULL)
                {
                    w->right->color = 0;
                }
                if (x->parent != NULL)
                {
                    rb_left_rotate(radice, x->parent);
                }
                x = *radice;
            }
        }
        else
        {

            RBTree *w = x->parent->left;
            if (w != NULL && w->color == 1)
            {
                w->color = 0;
                x->parent->color = 1;
                rb_right_rotate(radice, x->parent);
                w = x->parent->left;
            }
            if (w != NULL && w->right != NULL && w->left != NULL && w->right->color == 0 && w->left->color == 0)
            {
                if (w != NULL)
                    w->color = 1;
                x = x->parent;
            }
            else
            {
                if (w != NULL && w->left != NULL && w->left->color == 0)
                {
                    if (w->right != NULL)
                    {
                        w->right->color = 0;
                    }
                    if (w != NULL)
                    {
                        w->color = 1;
                    }
                    rb_left_rotate(radice, w);
                    w = x->parent->left;
                }
                if (w != NULL)
                {
                    w->color = x->parent->color;
                }
                x->parent->color = 0;
                if (w != NULL && w->left != NULL)
                {
                    w->left->color = 0;
                }
                rb_right_rotate(radice, x->parent);
                x = *radice;
            }
        }
    }
    if (x != NULL)
        x->color = 0;
}
//----------------------------
RBTree *rb_minimum(RBTree *node)
{
    while (node != NULL && node->left != NULL)
    {
        node = node->left;
    }
    return node;
}
//----------------------------
void rb_delete(RBTree **radice, int elem)
{

    RBTree *z = *radice;
    while (z != NULL)
    {
        if (elem < z->elem)
        {
            z = z->left;
        }
        else if (elem > z->elem)
        {
            z = z->right;
        }
        else
            break;
    }

    if (z != NULL)
    {

        RBTree *x = NULL;
        RBTree *y = z;
        int y_original_color = y->color;
        if (z->left == NULL)
        {
            x = z->right;
            rb_transplant(radice, z, z->right);
        }
        else if (z->right == NULL)
        {
            x = z->left;
            rb_transplant(radice, z, z->left);
        }
        else
        {
            y = rb_minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y == z->right)
            {
                if (x != NULL)
                {
                    x->parent = y;
                }
            }
            else
            {
                rb_transplant(radice, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rb_transplant(radice, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        if (y_original_color == 0)
            rb_delete_fixup(radice, x);
    }
}
//----------------------------
void stampa_inorder(RBTree *albero)
{
    if (albero == NULL)
    {
        return;
    }
    stampa_inorder(albero->left);
    printf("%d ", albero->elem);
    stampa_inorder(albero->right);
}
//----------------------------
void stampa_unorder(RBTree *albero)
{
    if (albero == NULL)
    {
        return;
    }
    stampa_unorder(albero->right);
    printf("%d ", albero->elem);
    stampa_unorder(albero->left);
}
// FINE FUNZIONI ALBERO ROSSO NERO

// INIZIO FUNZIONI PER IL PERCORSO

//----------------------------------------------------------------

int rimuovi_stazione(RBTree **radice, int stazione)
{

    RBTree *z = *radice;

    while (z != NULL && z->elem != stazione)
    {
        if (stazione < z->elem)
            z = z->left;
        else if (stazione > z->elem)
            z = z->right;
    }

    if (z == NULL || z->elem != stazione)
    {
        return 0;
    }
    else
    {

        rb_delete(radice, stazione);

        return 1;
    }
}

//----------------------------------------------------------------
int inserisci_macchina(RBTree **radice, int stazione, int autonomia)
{

    RBTree *z = *radice;
    int i = 0;
    while (z != NULL && z->elem != stazione)
    {
        if (stazione < z->elem)
            z = z->left;
        else if (stazione > z->elem)
            z = z->right;
    }

    if (z == NULL)
    {
        return 0;
    }
    else
    {
        while (z->park[i] != 0 && i != 512)
        {
            if (z->park[i] == -1)
            {
                z->park[512]--;
                break;
            }
            i++;
        }
        if (z->park[0] < autonomia)
        {
            z->park[i] = z->park[0];
            z->park[0] = autonomia;
        }
        else
        {
            z->park[i] = autonomia;
        }
        return 1;
    }
}
//----------------------------
int rottamazione(RBTree **radice, int stazione, int autonomia)
{

    RBTree *z = *radice;

    int i = 0, cont = 0, j = 1, max = 0;
    while (z != NULL && z->elem != stazione)
    {
        if (stazione < z->elem)
            z = z->left;
        else if (stazione > z->elem)
            z = z->right;
    }
    if (z == NULL)
    {
        return 0;
    }
    else
    {
        if (z->park[0] == autonomia)
        {
            while (j < 512)
            {
                if (max < z->park[j])
                {
                    max = z->park[j];
                    i = j;
                }
                if (z->park[j] == -1)
                {
                    cont++;
                }
                if ((z->park[j] == 0 || j == 511) && cont == z->park[512])
                {
                    z->park[0] = max;
                    z->park[i] = 0;
                    z->park[512]++;
                }
                j++;
            }
            return 1;
        }
        else
        {
            i = 0;
            while (i < 512)
            {
                if (z->park[i] == autonomia)
                {
                    z->park[i] = -1;
                    z->park[512]++;
                    return 1;
                }
                if (z->park[i] == -1)
                {
                    cont++;
                }
                if (z->park[i] == 0 && cont == z->park[512])
                {
                    return 0;
                }
                i++;
            }
        }
    }
    return 0;
}
//----------------------------------

// FUNZIONI ASSISTENTI PER IL PIANIFICA PERCORSO

RBTree *ricerca(RBTree *Tree, int elem)
{
    RBTree *z = Tree;
    while (z != NULL && z->elem != elem)
    {
        if (elem < z->elem)
            z = z->left;
        else if (elem > z->elem)
            z = z->right;
    }
    if (z != NULL)
    {
        return z;
    }
    else
    {
        return NULL;
    }
}
//----------------------------
void hash_inserimento(int elemento, int table[], int dimensione)
{

    int pos = elemento % dimensione;
    int position = elemento % dimensione;
    int pos_2 = 1 + (elemento % (dimensione - 1));

    if (table[position] == -1)
    {
        table[position] = elemento;
    }
    else
    {
        int i = 1;
        while (table[pos] != -1)
        {
            pos = (position + i * pos_2) % dimensione;
            i++;
        }
        table[pos] = elemento;
    }
}
//----------------------------
int hash_ricerca(int elemento, int table[], int dimensione)
{

    int position = elemento % dimensione, i = 1;
    int pos_2 = 1 + (elemento % (dimensione - 1));
    int pos = elemento % dimensione;

    while (table[pos] != -1)
    {
        if (table[pos] == elemento)
        {
            return pos;
        }
        pos = (position + i * pos_2) % dimensione;
        i++;
    }
    return -1;
}

//----------------------------
void trova_elem(RBTree *Tree, int *array, int *pos, int min, int max, int table[], int size, int *dimensione)
{
    if (Tree == NULL)
    {
        return;
    }
    trova_elem(Tree->left, array, pos, min, max, table, size, dimensione);
    if (Tree->elem >= min && Tree->elem <= max)
    {
        array[(*pos)++] = Tree->elem;
        hash_inserimento(Tree->elem, table, size);
        (*dimensione)++;
    }
    trova_elem(Tree->right, array, pos, min, max, table, size, dimensione);
}
//----------------------------
int *array_percorso(RBTree *root, int size, int min, int max, int table[], int size2, int *dimensione)
{
    int *array = (int *)malloc(size * sizeof(int));
    int pos = 0;
    trova_elem(root, array, &pos, min, max, table, size2, dimensione);
    return array;
}
//----------------------------
void rb_right_rotate2(RBTree2 **radice, RBTree2 *x)
{
    RBTree2 *left = x->left;

    if (x->left != NULL)
    {

        x->left = left->right;

        if (x->left != NULL)
        {
            x->left->parent = x;
        }

        left->parent = x->parent;

        if (x->parent == NULL)
        {
            *radice = left;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = left;
        }
        else
        {
            x->parent->right = left;
        }

        left->right = x;
        x->parent = left;
    }
}
//----------------------------
void rb_left_rotate2(RBTree2 **radice, RBTree2 *x)
{
    RBTree2 *right = x->right;

    if (x->right != NULL)
    {

        x->right = right->left;

        if (x->right != NULL)
        {
            x->right->parent = x;
        }

        right->parent = x->parent;

        if (x->parent == NULL)
        {
            *radice = right;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = right;
        }
        else
        {
            x->parent->right = right;
        }

        right->left = x;
        x->parent = right;
    }
}
//----------------------------
void rb_insert_fixup2(RBTree2 **radice, RBTree2 *z)
{
    while (z->parent != NULL && z->parent->color == 1)
    {
        if (z->parent == z->parent->parent->left)
        {
            RBTree2 *y = z->parent->parent->right;
            if (y != NULL && y->color == 1)
            {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    rb_left_rotate2(radice, z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                rb_right_rotate2(radice, z->parent->parent);
            }
        }
        else
        {
            RBTree2 *y = z->parent->parent->left;
            if (y != NULL && y->color == 1)
            {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rb_right_rotate2(radice, z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                rb_left_rotate2(radice, z->parent->parent);
            }
        }
    }
    (*radice)->color = 0;
}
//----------------------------
void inserimento(RBTree2 **radice, int distanza, int posizione, int stazione)
{

    RBTree2 *z = (RBTree2 *)malloc(sizeof(RBTree2));
    z->right = NULL;
    z->left = NULL;
    z->parent = NULL;
    z->elem = distanza;
    z->color = 1;
    z->park[0] = posizione;
    z->park[1] = stazione;

    RBTree2 *y = NULL;
    RBTree2 *x = *radice;

    while (x != NULL)
    {
        y = x;
        if (z->elem == x->elem)
        {
            break;
        }
        if (z->elem < x->elem)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == NULL)
    {
        *radice = z;
    }
    else if (z->elem < y->elem)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }

    z->left = NULL;
    z->right = NULL;
    z->color = 1;
    rb_insert_fixup2(radice, z);
}
//----------------------------
void rb_transplant2(RBTree2 **radice, RBTree2 *u, RBTree2 *v)
{
    if (u->parent == NULL)
    {
        *radice = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if (v != NULL)
    {
        v->parent = u->parent;
    }
}
//----------------------------
void rb_delete_fixup2(RBTree2 **radice, RBTree2 *x)
{
    while (x != *radice && x != NULL && x->color == 0)
    {
        if (x == x->parent->left)
        {
            RBTree2 *w = x->parent->right;
            if (w != NULL && w->color == 1)
            {
                w->color = 0;
                x->parent->color = 1;
                rb_left_rotate2(radice, x->parent);
                w = x->parent->right;
            }
            if (w != NULL && w->left != NULL && w->right != NULL && w->left->color == 0 && w->right->color == 0)
            {
                if (w != NULL)
                {
                    w->color = 1;
                }
                x = x->parent;
            }
            else
            {
                if (w != NULL && w->right != NULL && w->right->color == 0)
                {
                    if (w->left != NULL)
                    {
                        w->left->color = 0;
                    }
                    if (w != NULL)
                    {
                        w->color = 1;
                    }
                    rb_right_rotate2(radice, w);
                    w = x->parent->right;
                }
                if (w != NULL)
                {
                    w->color = x->parent->color;
                }
                x->parent->color = 0;
                if (w != NULL && w->right != NULL)
                {
                    w->right->color = 0;
                }
                if (x->parent != NULL)
                {
                    rb_left_rotate2(radice, x->parent);
                }
                x = *radice;
            }
        }
        else
        {

            RBTree2 *w = x->parent->left;
            if (w != NULL && w->color == 1)
            {
                w->color = 0;
                x->parent->color = 1;
                rb_right_rotate2(radice, x->parent);
                w = x->parent->left;
            }
            if (w != NULL && w->right != NULL && w->left != NULL && w->right->color == 0 && w->left->color == 0)
            {
                if (w != NULL)
                {
                    w->color = 1;
                }
                x = x->parent;
            }
            else
            {
                if (w != NULL && w->left != NULL && w->left->color == 0)
                {
                    if (w->right != NULL)
                    {
                        w->right->color = 0;
                    }
                    if (w != NULL)
                    {
                        w->color = 1;
                    }
                    rb_left_rotate2(radice, w);
                    w = x->parent->left;
                }
                if (w != NULL)
                {
                    w->color = x->parent->color;
                }
                x->parent->color = 0;
                if (w != NULL && w->left != NULL)
                {
                    w->left->color = 0;
                }
                rb_right_rotate2(radice, x->parent);
                x = *radice;
            }
        }
    }
    if (x != NULL)
        x->color = 0;
}
//----------------------------
RBTree2 *rb_minimum2(RBTree2 *node)
{
    while (node != NULL && node->left != NULL)
    {
        node = node->left;
    }
    return node;
}
//----------------------------
void rb_delete2(RBTree2 **radice, int elem)
{

    RBTree2 *z = *radice;
    while (z != NULL)
    {
        if (elem < z->elem)
        {
            z = z->left;
        }
        else if (elem > z->elem)
        {
            z = z->right;
        }
        else
            break;
    }

    if (z != NULL)
    {

        RBTree2 *x = NULL;
        RBTree2 *y = z;
        int y_original_color = y->color;
        if (z->left == NULL)
        {
            x = z->right;
            rb_transplant2(radice, z, z->right);
        }
        else if (z->right == NULL)
        {
            x = z->left;
            rb_transplant2(radice, z, z->left);
        }
        else
        {
            y = rb_minimum2(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y == z->right)
            {
                if (x != NULL)
                    x->parent = y;
            }
            else
            {
                rb_transplant2(radice, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            rb_transplant2(radice, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        if (y_original_color == 0)
            rb_delete_fixup2(radice, x);
    }
}
// FINE FUNZIONI ASSISTENTI PER IL PIANIFICA PERCORSO
void plan(RBTree **radice, int source, int destination, int lenght)
{

    int v, u, MAX = INT_MAX, j = 0, i = 0;
    RBTree *root = *radice;
    int size = 0;
    if(lenght<10000){
        size = 10007;
    }else{
        size = 19841;
    }
    int dimensione = 0;
    int *table;
    table = (int *)malloc(size * sizeof(int));
    int dist[size], pred[size], flag = 0;

    for (i = 0; i < size; i++)
    {
        table[i] = -1;
        dist[i] = MAX;
        pred[i] = 0;
    }

    int *percorso = array_percorso(root, lenght, source, destination, table, size, &dimensione);
    int pos_source = hash_ricerca(source, table, size);
    dist[pos_source] = 0;
    RBTree2 *distanza = NULL;
    inserimento(&distanza, dist[pos_source], pos_source, source);
    for (int z = 0; z < dimensione - 1; z++)
    {

        RBTree2 *nodo = rb_minimum2(distanza);
        if (nodo == NULL)
        {
            printf("nessun percorso\n");
            flag = 1;
            z = size;
        }
        else
        {
            u = nodo->park[1];
            j = nodo->park[0];
            rb_delete2(&distanza, nodo->elem);
            RBTree *adiacente = ricerca(root, u);

            for (i = z + 1; i < dimensione; i++)
            {
                int stazione = percorso[i];

                if (adiacente->park[0] + adiacente->elem < stazione)
                {
                    i = size;
                }
                if (adiacente->elem < stazione && adiacente->park[0] + adiacente->elem >= stazione)
                {
                    int posizione = hash_ricerca(stazione, table, size);

                    if (dist[posizione] > (dist[j] + stazione - u))
                    {
                        dist[posizione] = (dist[j] + stazione - u);
                        inserimento(&distanza, dist[posizione], posizione, stazione);
                        pred[posizione] = u;
                        if (stazione == destination)
                        {
                            z = dimensione;
                        }
                    }
                }
            }
        }
    }
    if (flag != 1)
    {
        int stampa[dimensione];
        int cont = 1;
        stampa[0] = destination;
        v = pred[hash_ricerca(destination, table, size)];
        if (v == 0)
        {
            printf("nessun percorso\n");
        }
        else
        {
            for (int i = 0; i < dimensione; i++)
            {
                stampa[i + 1] = v;
                cont++;
                if (v == source)
                {
                    i = dimensione;
                }
                else
                {
                    v = pred[hash_ricerca(v, table, size)];
                }
            }

            for (i = cont - 1; i >= 0; i--)
            {
                printf("%d ", stampa[i]);
            }

            printf("\n");
        }
    }
}

void plan_invertito(RBTree **radice, int source, int destination, int lenght)
{

    int v, u, MAX = INT_MAX, j = 0, i = 0;
    RBTree *root = *radice;
    int size = 0;
    if(lenght<10000){
        size = 10007;
    }else{
        size = 19841;
    }
    int dimensione = 0;
    int *table;
    table = (int *)malloc(size * sizeof(int));
    int dist[size], pred[size], flag = 0;

    for (i = 0; i < size; i++)
    {
        table[i] = -1;
        dist[i] = MAX;
        pred[i] = 0;
    }

    int *percorso = array_percorso(root, lenght, destination, source, table, size, &dimensione);
    int pos_source = hash_ricerca(source, table, size);
    dist[pos_source] = 0;
    RBTree2 *distanza = NULL;
    int cont = dimensione - 1;
    int var = 1;
    inserimento(&distanza, dist[pos_source], pos_source, source);
    for (int z = 0; z < dimensione; z++)
    {

        RBTree2 *nodo = rb_minimum2(distanza);
        if (nodo == NULL)
        {
            printf("nessun percorso\n");
            flag = 1;
            z = size;
        }
        else
        {
            u = nodo->park[1];
            if (u == destination)
            {
                z = size;
            }
            j = nodo->park[0];
            rb_delete2(&distanza, nodo->elem);

            RBTree *adiacente = ricerca(root, u);
            int cost = size;
            var++;
            if (var % 4 == 0)
            {
                cont--;
            }
            for (i = cont; i >= 0; i--)
            {
                if (adiacente->elem < percorso[i])
                {
                    i--;
                }
                else
                {
                    if (adiacente->elem - adiacente->park[0] > percorso[i])
                    {
                        i = -1;
                    }

                    if (adiacente->elem > percorso[i] && adiacente->elem - adiacente->park[0] <= percorso[i])
                    {
                        int posizione = hash_ricerca(percorso[i], table, size);
                        if (dist[posizione] > (dist[j] + cost))
                        {
                            dist[posizione] = (dist[j] + cost);
                            inserimento(&distanza, dist[posizione], posizione, percorso[i]);
                            pred[posizione] = u;
                            if (percorso[i] == destination)
                            {
                                z = size;
                            }
                        }
                        cost--;
                    }
                }
            }
        }
    }
    if (flag != 1)
    {
        int stampa[dimensione];
        int cont = 1;
        stampa[0] = destination;
        v = pred[hash_ricerca(destination, table, size)];
        if (v == 0)
        {
            printf("nessun percorso\n");
        }
        else
        {
            for (int i = 0; i < dimensione; i++)
            {
                stampa[i + 1] = v;
                cont++;
                if (v == source)
                {
                    i = dimensione;
                }
                else
                {
                    v = pred[hash_ricerca(v, table, size)];
                }
            }
            for (i = cont - 1; i >= 0; i--)
            {
                printf("%d ", stampa[i]);
            }
            printf("\n");
        }
    }
}

//----------------------------------
int main()
{

    int lenght = 0; // lenght tiene il conto delle stazioni
    char input[30];
    char add[20] = "aggiunta", not [20] = "non aggiunta", rott[20] = "rottamata", not_rott[20] = "non rottamata";
    char dem[20] = "demolita", not_dem[20] = "non demolita";

    RBTree *albero = NULL;

    while (fscanf(stdin, "%s", input) != EOF)
    {

        // AGGIUNGI STAZIONE
        if (strcmp(input, "aggiungi-stazione") == 0)
        {
            int stazione, num_auto;
            if (fscanf(stdin, "%d %d", &stazione, &num_auto) != EOF)
            {
                if (rb_insert(&albero, stazione, num_auto) == 1)
                {
                    puts(add);
                    lenght++;
                }
                else
                {
                    puts(not );
                }
            }
        }

        // DEMOLISCI STAZIONE
        if (strcmp(input, "demolisci-stazione") == 0)
        {
            int stazione;
            if (fscanf(stdin, "%d", &stazione) != EOF)
            {
                if (rimuovi_stazione(&albero, stazione) == 0)
                {
                    puts(not_dem);
                }
                else
                {
                    puts(dem);
                    lenght--;
                }
            }
        }

        // AGGIUNGI AUTO
        if (strcmp(input, "aggiungi-auto") == 0)
        {
            int stazione, autonomia;
            if (fscanf(stdin, "%d %d", &stazione, &autonomia) != EOF)
            {
                if (inserisci_macchina(&albero, stazione, autonomia) == 0)
                {
                    puts(not );
                }
                else
                {
                    puts(add);
                }
            }
        }

        // ROTTAMA AUTO
        if (strcmp(input, "rottama-auto") == 0)
        {
            int stazione, autonomia;
            if (fscanf(stdin, "%d %d", &stazione, &autonomia) != EOF)
            {
                if (rottamazione(&albero, stazione, autonomia) == 0)
                {
                    puts(not_rott);
                }
                else
                {
                    puts(rott);
                }
            }
        }

        // PIANIFICA-PERCORSO
        if (strcmp(input, "pianifica-percorso") == 0)
        {
            int source, destination;
            if (fscanf(stdin, "%d %d", &source, &destination) != EOF)
            {
                if (source < destination)
                {
                    plan(&albero, source, destination, lenght);
                }
                else
                {
                    plan_invertito(&albero, source, destination, lenght);
                }
            }
        }
    };

    free(albero);
    return 0;
}