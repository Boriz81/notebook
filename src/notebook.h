#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#define MAX_TITLE_LENGTH 100
#define MAX_CONTENT_LENGTH 1000
#define MAX_TAGS 5
#define MAX_TAG_LENGTH 20
#define FILENAME "notes.dat"

// Структура для хранения даты
typedef struct {
    int day;
    int month;
    int year;
} Date;

// Структура для хранения заметки
typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    char tags[MAX_TAGS][MAX_TAG_LENGTH];
    int tag_count;
    Date created_date;
    Date modified_date;
    int is_deleted; // Флаг для "мягкого" удаления
} Note;

// Прототипы функций
void print_menu();
void add_note();
void view_all_notes();
void search_notes();
void delete_notes();
void edit_note();
void view_note_by_id();
void save_note(Note *note);
void load_notes();
Date get_current_date();
void print_note(Note note);
int get_next_id();

#endif
