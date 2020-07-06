// Copyright 2020 <Calcan Elena-Claudia>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./utils.h"

void read_data(struct ID3 *x, char *filename) {
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    fprintf(stderr, "Cannot open %s\n", filename);
    return;
  }
  // ma pozitionez cu 97 de pozitii mai in spate fata de finalul fisierului
  // deorece acolo se afla informatiile pe care le extragem despre melodie
  fseek(f, -97, SEEK_END);
  fread(x->antet, sizeof(struct ID3), 3, f);
  fread(x->title, sizeof(struct ID3), 30, f);
  fread(x->artist, sizeof(struct ID3), 30, f);
  fread(x->album, sizeof(struct ID3), 30, f);
  fread(x->year, sizeof(struct ID3), 4, f);
  fclose(f);
}

void print_data(struct ID3 *x, FILE *fout) {
  fprintf(fout, "Title: %.30s\n", x->title);
  fprintf(fout, "Artist: %.30s\n", x->artist);
  fprintf(fout, "Album: %.30s\n", x->album);
  fprintf(fout, "Year: %.4s\n", x->year);
}

void init_list(struct DoublyLinkedList *list) {
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
}

void add_head(struct DoublyLinkedList *list, struct ID3 *data) {
  struct Node *new;
  new = (struct Node *)malloc(sizeof(struct Node));
  memcpy(new->data.antet, data->antet, sizeof(data->antet));
  memcpy(new->data.title, data->title, sizeof(data->title));
  memcpy(new->data.artist, data->artist, sizeof(data->artist));
  memcpy(new->data.album, data->album, sizeof(data->artist));
  memcpy(new->data.year, data->year, sizeof(data->year));
  new->next = NULL;
  new->prev = NULL;
  list->head = new;
  list->tail = new;
  list->size++;
}

void add_first(struct DoublyLinkedList *list, struct ID3 *data) {
  struct Node *new;
  new = (struct Node *)malloc(sizeof(struct Node));
  memcpy(new->data.antet, data->antet, sizeof(data->antet));
  memcpy(new->data.title, data->title, sizeof(data->title));
  memcpy(new->data.artist, data->artist, sizeof(data->artist));
  memcpy(new->data.album, data->album, sizeof(data->artist));
  memcpy(new->data.year, data->year, sizeof(data->year));
  new->next = list->head;
  list->head->prev = new;
  new->prev = NULL;
  list->head = new;
  list->size++;
}

void add_last(struct DoublyLinkedList *list, struct ID3 *data) {
  struct Node *new;
  new = (struct Node *)malloc(sizeof(struct Node));
  memcpy(new->data.antet, data->antet, sizeof(data->antet));
  memcpy(new->data.title, data->title, sizeof(data->title));
  memcpy(new->data.artist, data->artist, sizeof(data->artist));
  memcpy(new->data.album, data->album, sizeof(data->artist));
  memcpy(new->data.year, data->year, sizeof(data->year));
  new->prev = list->tail;
  list->tail->next = new;
  new->next = NULL;
  list->tail = new;
  list->size++;
}

void add_after(struct DoublyLinkedList *l, struct Node *cur, struct ID3 *data) {
  struct Node *new;
  new = (struct Node *)malloc(sizeof(struct Node));
  memcpy(new->data.antet, data->antet, sizeof(data->antet));
  memcpy(new->data.title, data->title, sizeof(data->title));
  memcpy(new->data.artist, data->artist, sizeof(data->artist));
  memcpy(new->data.album, data->album, sizeof(data->artist));
  memcpy(new->data.year, data->year, sizeof(data->year));
  new->next = cur->next;
  new->prev = cur;
  cur->next->prev = new;
  cur->next = new;
  l->size++;
}

int is_in_playlist(struct DoublyLinkedList *list, struct ID3 *data) {
  struct Node *curr;
  if (list == NULL) {
    return 1;
  }
  curr = list->head;
  while (curr != NULL) {
    if (strcmp(curr->data.title, data->title) == 0) {
      return 0;
    }
    curr = curr->next;
  }
  return 1;
}

void del_head(struct DoublyLinkedList *list) {
  struct Node *tmp;
  tmp = list->head;
  list->head = NULL;
  list->tail = NULL;
  free(tmp);
  list->size--;
}

void del_first(struct DoublyLinkedList *list) {
  struct Node *tmp;
  tmp = list->head;
  tmp->next->prev = NULL;
  list->head = tmp->next;
  free(tmp);
  list->size--;
}

void del_last(struct DoublyLinkedList *list) {
  struct Node *tmp;
  tmp = list->tail;
  tmp->prev->next = NULL;
  list->tail = tmp->prev;
  free(tmp);
  list->size--;
}

void del_curr(struct DoublyLinkedList *list, struct Node *cursor) {
  struct Node *tmp;
  tmp = cursor->prev;
  tmp->prev->next = cursor;
  cursor->prev = tmp->prev;
  free(tmp);
  list->size--;
}

int position(struct DoublyLinkedList *list, struct ID3 *data) {
  struct Node *curr;
  int cnt = 0;
  curr = list->head;
  while (strcmp(curr->data.title, data->title) != 0) {
    cnt++;
    curr = curr->next;
  }
  return cnt;
}

void remove_song(struct DoublyLinkedList *list, int n) {
  if (list->size == 1) {
    del_head(list);
  } else if (n == 0) {
    del_first(list);
  } else if (n >= list->size - 1) {
    del_last(list);
  } else {
    struct Node *curr, *tmp;
    curr = list->head;
    for (int i = 0; i < n - 1; i++) {
      curr = curr->next;
    }
    tmp = curr->next;
    curr->next = tmp->next;
    tmp->next->prev = curr;
    free(tmp);
    list->size--;
  }
}

void print_list(struct DoublyLinkedList *list, FILE *f) {
  struct Node *curr;
  curr = list->head;
  fprintf(f, "[");
  while (curr->next != NULL) {
    fprintf(f, "%.30s; ", curr->data.title);
    curr = curr->next;
  }
  fprintf(f, "%s", curr->data.title);
  fprintf(f, "]\n");
}

int get_size(struct DoublyLinkedList *list) {
  if (list == NULL) {
    return -1;
  } else {
    return list->size;
  }
}

void free_list(struct DoublyLinkedList **list) {
  while (get_size(*list) != 0) {
    remove_song(*list, 0);
  }
  free(*list);
}
