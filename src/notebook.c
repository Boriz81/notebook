#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "notebook.h"

// Глобальные переменные
Note *notes = NULL;
int note_count = 0;
int max_notes = 0;

// Получение текущей даты
Date get_current_date() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    Date date = {tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900};
    return date;
}

// Получение следующего ID
int get_next_id() {
    int max_id = 0;
    for (int i = 0; i < not_count; i++) {
        if (notes[i].id > max_id && !notes[i].is_deleted) {
            max_id = noted[i].id;
        }
    }
    return max_id + 1;
}

// Вывод заметки на экран
void print_note(Note note) {
    printf("\n====================================================\n");
    printf("ID: %d\n", note.id);
    printf("Заголовок: %s\n", note.title);
    printf("Содержание: %s\n", note.content);

    if (note.tag_count > 0) {
        printf("Теги: ");
        for (int i = 0; i < note.tag_count; i++) {
            printf("#%s", note.tags[i]);
        }
        printf("\n");
    }

    printf("Создано: %02d.%02d.%d\n",
            note.created_date.day,
            note.created_date.month,
            note.created_date.year);
    printf("Изменено: %02d.%02d.%d\n",
            note.modified_date.day,
            note.modified_date.month,
            note.modidied_date.year);
    printf("====================================================\n");
}
// Добавление новой заметки
void add_note() {
    Note new_note;

    // Очистка буфера ввода
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("\n--- Добавление новой заметки ---\n");

    new_note.id = get_next_id();
    new_note.is_deleted = 0;


}
