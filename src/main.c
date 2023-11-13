#include "../include/test.h"

void print_node(FILE *file, struct node *node) {

    printf("{\n");
    printf("%sid : %lu\n", TABULATION, node->id);
    printf("%svertex : %lu\n", TABULATION, node->d);

    uint64_t *nodes = read_file(file, node->nodes, sizeof(uint64_t) * node->d);

    for (uint64_t i = 0; i < node->d; i++) {
        struct node *tmp_node = read_file(file, nodes[i], sizeof(struct node));
        printf("%sneighbour %lu, id : %lu\n", TABULATION, i + 1, tmp_node->id);
        free(tmp_node);
    }
    //free(nodes);

    printf("%sinteger : %lu\n", TABULATION, node->intgr);
    double dbl;
    memcpy(&dbl, &node->dbl, sizeof(node->dbl));
    printf("%sdouble : %.3f\n", TABULATION, dbl);
    printf("%sboolean : %lu\n", TABULATION, node->bln);

    struct string_save *strings = read_file(file, node->strings_array_ptr,
                                            sizeof(struct string_save) * node->n_strings);

    printf("%sstring : %s\n", TABULATION,
           (char *) read_file(file, strings[0].string_line_ptr, strings[0].size_of_string));

    printf("}\n");

    free(strings);
}

void print_node_list(FILE *file, struct node_list *node_list) {
    if (node_list != NULL) {
        struct node_list *next;
        while (node_list != NULL) {
            next = node_list->prev;
            print_node(file, node_list->value);
            node_list = next;
        }
    }
}

void print_all_nodes(FILE *file) {
    struct header *file_header = read_file(file, 0, sizeof(struct header));
    uint64_t node_ptr = file_header->first_node_ptr;
    free(file_header);

    struct node *iter_node;
    while (node_ptr != INVALID_PTR) {
        printf("{\n");
        iter_node = read_file(file, node_ptr, sizeof(struct node));
        printf("%sid : %lu\n", TABULATION, iter_node->id);
        printf("%svertex : %lu\n", TABULATION, iter_node->d);
        printf("%sinteger : %lu\n", TABULATION, iter_node->intgr);
        double dbl;
        memcpy(&dbl, &iter_node->dbl, sizeof(iter_node->dbl));
        printf("%sdouble : %.3f\n", TABULATION, dbl);
        printf("%sboolean : %lu\n", TABULATION, iter_node->bln);


        struct string_save *strings = read_file(file, iter_node->strings_array_ptr,
                                                sizeof(struct string_save) * iter_node->n_strings);

        printf("%sstring : %s\n", TABULATION,
               (char *) read_file(file, strings[0].string_line_ptr, strings[0].size_of_string));

        free(strings);

        printf("}\n");
        node_ptr = iter_node->next_ptr;
        free(iter_node);
    }
}

void free_node_list(struct node_list *node_list) {
    if (node_list != NULL) {
        struct node_list *next;
        while (node_list != NULL) {
            next = node_list->prev;
            free(node_list->value);
            free(node_list);
            node_list = next;
        }
    }
}


int main(int argc, char **argv) {

    FILE *file = open_file("data.txt");


    uint64_t intgr = 10;
    uint64_t dbl;
    double d = 1011.11;
    memcpy(&dbl, &d, sizeof(d));
    uint64_t bln = 1;
    uint64_t str = (uint64_t) "new element";

    printf("////////////////////// CREATE NODE ////////////////////////////\n");

    create_node(file, intgr, dbl, bln, str);
    print_all_nodes(file);

    printf("////////////////////// FIND NODE BY ID 1 ////////////////////////////\n");
    struct node *node = malloc(sizeof(struct node));
    find_node_by_id(file, 1, node);

    print_node(file, node);
    free(node);

    printf("////////////////////// CONNECT NODES 1 AND 2 ////////////////////////////\n");
    connect_nodes(file, 1, 2);
    struct node_list *node_list2 = NULL;
    find_all(file, &node_list2);
    print_node_list(file, node_list2);
    free_node_list(node_list2);

    printf("////////////////////// DELETE NODE BY ID 1 ////////////////////////////\n");
    delete_node_by_id(file, 1);
    print_all_nodes(file);

    printf("////////////////////// FIND ALL ////////////////////////////\n");
    struct node_list *node_list3 = NULL;
    find_all(file, &node_list3);
    print_node_list(file, node_list3);
    free_node_list(node_list3);

    printf("//////////////////////// FIND BY FIELD - INTEGER VALUE - 10 ////////////////////\n");
    struct node_list *node_list = NULL;
    find_node_by_filed_value(file, (char *) "integer", 10, &node_list);
    print_node_list(file, node_list);
    free_node_list(node_list);

    printf("////////////////////// DELETE ALL ////////////////////////////\n");
    delete_all(file);
    print_all_nodes(file);

    printf("////////////////////// CREATE NODE ////////////////////////////\n");

    create_node(file, intgr, dbl, bln, str);
    print_all_nodes(file);

    return 0;
}

/* CHECK FILE SIZE FOR NODES
   for (int i = 0; i < 100; i++) {
       create_node(file, intgr, dbl, bln, str);
       fseek(file, 0, SEEK_END);
       printf("%ld\n", ftell(file));
   }
*/


// FIRST CHECK

/*
   uint64_t intgr = 10;
   uint64_t dbl;
   double d = 1011.11;
   memcpy(&dbl, &d, sizeof(d));
   uint64_t bln = 1;
   uint64_t str = (uint64_t) "new element";

   printf("////////////////////// CREATE NODE ////////////////////////////\n");

   create_node(file, intgr, dbl, bln, str);
   print_all_nodes(file);

   printf("////////////////////// FIND NODE BY ID 1 ////////////////////////////\n");
   struct node *node = malloc(sizeof(struct node));
   find_node_by_id(file, 1, node);

   print_node(file, node);
   free(node);

   printf("////////////////////// CONNECT NODES 1 AND 2 ////////////////////////////\n");
   connect_nodes(file, 1 ,2);
   struct node_list *node_list2 = NULL;
   findAll(file, &node_list2);
   print_node_list(file, node_list2);
   free_node_list(node_list2);

   printf("////////////////////// DELETE NODE BY ID 1 ////////////////////////////\n");
   delete_node_by_id(file, 1);
   print_all_nodes(file);

   printf("////////////////////// FIND ALL ////////////////////////////\n");
   struct node_list *node_list = NULL;
   findAll(file, &node_list);
   print_node_list(file, node_list);
   free_node_list(node_list);

   printf("//////////////////////// FIND BY FIELD - INTEGER VALUE - 10 ////////////////////\n");
   struct node_list *node_list = NULL;
   find_node_by_filed_value(file, (char *) "integer", 10, &node_list);
   print_node_list(file, node_list);
   free_node_list(node_list);

   printf("////////////////////// DELETE ALL ////////////////////////////\n");
   deleteAll(file);
   print_all_nodes(file);

   printf("////////////////////// CREATE NODE ////////////////////////////\n");

   create_node(file, intgr, dbl, bln, str);
   print_all_nodes(file);
   */

