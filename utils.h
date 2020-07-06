// Copyright 2020 Calcan Elena-Claudia
#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ID3 {
  char antet[3];
  char title[30];
  char artist[30];
  char album[30];
  char year[4];
};

struct Node {
  struct Node *next, *prev;
  struct ID3 data;
};

struct DoublyLinkedList {
  struct Node *head, *tail;
  int size;
};

// extrag informatiile despre melodie din fisier binar
void read_data(struct ID3 *x, char *filename);

// afiseaza toate informatiile despre melodie
void print_data(struct ID3 *x, FILE *fout);

// initializeaza lista
void init_list(struct DoublyLinkedList *list);

// functia este apelata atunci cand dimensiunea fisierului == 0
// adauga head-ul in lista
void add_head(struct DoublyLinkedList *list, struct ID3 *data);

// adauga un element pe prima pozitie
void add_first(struct DoublyLinkedList *list, struct ID3 *data);

// adauga un element pe ultima pozitie
void add_last(struct DoublyLinkedList *list, struct ID3 *data);

// adauga un element dupa cursor
void add_after(struct DoublyLinkedList *l, struct Node *cur, struct ID3 *data);

// functia verifica daca melodia exita sau nu in playlist
int is_in_playlist(struct DoublyLinkedList *list, struct ID3 *data);

// sterge head-ul din lista
// functia este apelata atunci cand dimensiune fisierului == 1
void del_head(struct DoublyLinkedList *list);

// sterge primul element din lista
void del_first(struct DoublyLinkedList *list);

// sterge ultimul elemnt din lista
void del_last(struct DoublyLinkedList *list);

// sterge elementul care este pointat de cursor
void del_curr(struct DoublyLinkedList *list, struct Node *cursor);

// functia returnea pozitia pe care se afla o melodie existenta
int position(struct DoublyLinkedList *list, struct ID3 *data);

// functia sterge melodia care se afla pe a n-a pozitie
void remove_song(struct DoublyLinkedList *list, int n);

// afiseaza playlist-ul
void print_list(struct DoublyLinkedList *list, FILE *f);

// functia returneaza lungimea listei
int get_size(struct DoublyLinkedList *list);

// elibereaza memoria folosita de lista
void free_list(struct DoublyLinkedList **pp_list);

#endif  // UTILS_H_
