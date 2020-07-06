// Copyright 2020 Calcan Elena-Claudia
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./utils.h"

int main(int argc, char *argv[]) {
  struct DoublyLinkedList *list;
  struct Node *cursor = NULL;
  struct ID3 song;
  FILE *fin, *fout;
  char comm[255], f_name[200];
  int q;
  // verifica parametrii liniei de comanda
  if (argc != 3) {
    printf("Error: Wrong number of arguments\n");
    return 1;
  }
  // deschide fisierul din care citim comenzile
  fin = fopen(argv[1], "rt");
  if (fin == NULL) {
    fprintf(stderr, "Cannot open %s\n", argv[1]);
    return 1;
  }
  fscanf(fin, "%d", &q);
  // deschide fisierul in care afisam output-ul
  fout = fopen(argv[2], "wt");
  if (fout == NULL) {
    fprintf(stderr, "Cannot open%s\n", argv[2]);
    return 1;
  }

  // initializare lista
  list = malloc(sizeof(struct DoublyLinkedList));
  init_list(list);
  // parcurgere fisier din care citim comenzile
  while (q >= 0) {
    fgets(comm, sizeof(comm), fin);
    comm[strlen(comm) - 1] = '\0';
    // verificam ce comanda am citit
    if (strstr(comm, "ADD_FIRST") != 0) {
      // creeaza calea cantre fisierul binar din care extragem informatiile
      snprintf(f_name, sizeof(f_name), "%s%s", "songs/", strchr(comm, ' ')+1);
      read_data(&song, f_name);
      if (list->size == 0) {
        add_head(list, &song);
        cursor = list->head;
      } else if (is_in_playlist(list, &song) == 0) {
        if (strcmp(list->head->data.title, song.title) == 0) {
          if (list->head == cursor) {
            cursor = list->head->next;
          }
          del_first(list);
          add_first(list, &song);
        } else if (strcmp(list->tail->data.title, song.title) == 0) {
          if (list->tail == cursor) {
            cursor = list->tail->prev;
          }
          del_last(list);
          add_first(list, &song);
        } else if (strcmp(cursor->data.title, song.title) == 0) {
          cursor = cursor->next;
          del_curr(list, cursor);
          add_first(list, &song);
        } else {
          remove_song(list, position(list, &song));
          add_first(list, &song);
        }
      } else if (is_in_playlist(list, &song) != 0) {
        add_first(list, &song);
      }
    } else if (strstr(comm, "ADD_LAST") != 0) {
      snprintf(f_name, sizeof(f_name), "%s%s", "songs/", strchr(comm, ' ') +1);
      read_data(&song, f_name);
      if (list->size == 0) {
        add_head(list, &song);
        cursor = list->tail;
      } else if (is_in_playlist(list, &song) == 0) {
        if (strcmp(list->head->data.title, song.title) == 0) {
          if (list->head == cursor) {
            cursor = list->head->next;
          }
          del_first(list);
          add_last(list, &song);
        } else if (strcmp(list->tail->data.title, song.title) == 0) {
          if (list->tail == cursor) {
            cursor = list->tail->prev;
          }
          del_last(list);
          add_last(list, &song);
        } else if (strcmp(cursor->data.title, song.title) == 0) {
          cursor = cursor->next;
          del_curr(list, cursor);
          add_last(list, &song);
        } else {
          remove_song(list, position(list, &song));
          add_last(list, &song);
        }
      } else if (is_in_playlist(list, &song) != 0) {
        add_last(list, &song);
      }
    } else if (strstr(comm, "ADD_AFTER") != 0 && cursor != NULL) {
      snprintf(f_name, sizeof(f_name), "%s%s", "songs/", strchr(comm, ' ')+1);
      read_data(&song, f_name);
      if (strcmp(cursor->data.title, song.title) != 0) {
        if (is_in_playlist(list, &song) != 0) {
          if (list->size == 1) {
            add_last(list, &song);
          } else if (cursor == list->head) {
            add_after(list, cursor, &song);
          } else if (cursor == list->tail) {
            add_last(list, &song);
          } else {
            add_after(list, cursor, &song);
          }
        } else if (is_in_playlist(list, &song) == 0) {
          if (strcmp(song.title, list->head->data.title) == 0) {
            del_first(list);
            add_after(list, cursor, &song);
          } else if (strcmp(song.title, list->tail->data.title) == 0) {
            del_last(list);
            add_after(list, cursor, &song);
          } else {
            remove_song(list, position(list, &song));
            add_after(list, cursor, &song);
          }
        }
      }

    } else if (strstr(comm, "DEL_FIRST") != 0) {
      if (list == NULL || list->head == NULL) {
        fprintf(fout, "Error: delete from empty playlist\n");
      } else if (list->size == 1) {
        del_head(list);
        cursor = NULL;
      } else {
        if (cursor == list->head) {
          cursor = list->head->next;
        }
        del_first(list);
      }
    } else if (strstr(comm, "DEL_LAST") != 0) {
      if (list == NULL || list->head == NULL) {
        fprintf(fout, "Error: delete from empty playlist\n");
      } else if (list->size == 1) {
        del_head(list);
        cursor = NULL;
      } else {
        if (cursor == list->tail) {
          cursor = list->tail->prev;
        }
        del_last(list);
      }
    } else if (strstr(comm, "DEL_CURR") != 0) {
      if (list == NULL || cursor == NULL) {
        fprintf(fout, "Error: no track playing\n");
      } else if (cursor == list->head && cursor == list->tail) {
        cursor = NULL;
        del_head(list);
      } else if (cursor == list->head && cursor != list->tail) {
        cursor = list->head->next;
        del_first(list);
      } else if (cursor != list->head && cursor == list->tail) {
        cursor = list->tail->prev;
        del_last(list);
      } else {
        cursor = cursor->next;
        del_curr(list, cursor);
      }
    } else if (strstr(comm, "DEL_SONG") != 0) {
      snprintf(f_name, sizeof(f_name), "%s%s", "songs/", strchr(comm, ' ')+1);
      read_data(&song, f_name);
      if (is_in_playlist(list, &song) == 1) {
        fprintf(fout, "Error: no song found to delete\n");
      } else {
        if (strcmp(cursor->data.title, song.title) == 0) {
          if (list->size == 1) {
            cursor = NULL;
            del_head(list);
          } else if (cursor == list->head && cursor != list->tail) {
            cursor = list->head->next;
            del_first(list);
          } else if (cursor != list->head && cursor == list->tail) {
            cursor = list->tail->prev;
            del_last(list);
          } else {
            cursor = cursor->next;
            remove_song(list, position(list, &song));
          }
        } else {
          remove_song(list, position(list, &song));
        }
      }
    } else if (strstr(comm, "MOVE_NEXT") != 0) {
      if (list == NULL || cursor == NULL) {
        fprintf(fout, "Error: no track playing\n");
      } else {
        if (cursor->next != NULL) {
          cursor = cursor->next;
        }
      }
    } else if (strstr(comm, "MOVE_PREV") != 0) {
      if (list == NULL || cursor == NULL) {
        fprintf(fout, "Error: no track playing\n");
      } else {
        if (cursor->prev != NULL) {
          cursor = cursor->prev;
        }
      }
    } else if (strstr(comm, "SHOW_FIRST") != 0) {
      if (list->head == NULL) {
        fprintf(fout, "Error: show empty playlist\n");
      } else {
        print_data(&list->head->data, fout);
      }
    } else if (strstr(comm, "SHOW_LAST") != 0) {
      if (list->tail == NULL) {
        fprintf(fout, "Error: show empty playlist\n");
      } else {
        print_data(&list->tail->data, fout);
      }
    } else if (strstr(comm, "SHOW_CURR") != 0) {
      if (list == NULL || cursor == NULL) {
        fprintf(fout, "Error: show empty playlist\n");
      } else {
        print_data(&cursor->data, fout);
      }
    } else if (strstr(comm, "SHOW_PLAYLIST") != 0) {
      if (list->size == 0) {
        fprintf(fout, "[]");
      } else {
        print_list(list, fout);
      }
    }
    q--;
  }
  // elibereaza memoria folosita de lista lista
  free_list(&list);
  // inchide fisiere
  fclose(fout);
  fclose(fin);
  return 0;
}
