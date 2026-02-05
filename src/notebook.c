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
    
    // Ввод заголовка
    printf("Введите заголовок: ");
    fgets(new_note.title, MAX_TITLE_LENGTH, stdin);
    new_note.title[strcspn(new_note.title, "\n")] = '\0';
    
    // Ввод содержимого
    printf("Введите содержание (для завершения введите точку на новой строке):\n");
    char line[100];
    new_note.content[0] = '\0';

    while (1) {
        fgets(line, 100, stdin);
        if (strcmp(line, ".\n") == 0) break;
        strcat(new_note.content, line);
    }
    new_note.content[strcspn(new_note.content, "\n")] = '\0';

    // Ввод тегов
    printf("Введите теги (через запятую, максимум %d): ", MAX_TAGS);
    char tags_input[200];
    fgets(tags_input, 200, stdin);


    new_note.tag_count = 0;
    char *token = strtok(tags_input, ",;\n");
    while (token != NULL && new_note.tag_count < MAX_TAGS) {
        // Удаление пробелов в начале и конце
        while (isspace(*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) end--;
        *(end + 1) = '\0';

        if (strlen(token) > 0) {
            strcpy(new_note.tags[new_note.tag_count], token);
            new_note.tag_count++;
        }
        token = strtok(NULL, ",;\n");
    }

    // Установка дат
    Date current_date = get_current_date();
    new_note.created_date = current_date;
    new_note.modified_date = current_date;

    // Сохранение в массив
    if (note_count >= max_notes) {
        max_notes = (max_notes == 0) ? 10 : max_notes * 2;
        notes = realloc(notes, max_notes * sizeof(Note));
        if (!notes) {
            printf("Ошибка выделения памяти!\n");
            return;
        }
    }

    notes[note_count] = new_note;
    note_count++;

    // Сохранение в файл
    FILE *file = fopen(FILENAME, "ab");
    if (file) {
        fwrite(&new_note, sizeof(Note), 1, file);
        fclose(file);
        printf("Заметка успешно сохранено! (ID: %d)\n", new_note.id);
    } else {
        printf("Ошибка сохранения в файл!\n");
    }
}

// Просмотр всех заметок

void view_all_notes() {
    if (note_count == 0) {
        printf("\nНет заметок для отображения.\n");
        return;
    }
    
    printf("\n--- Все заметки ---\n");
    int visible_count = 0;

    for (int i = 0; i < note_count; i++) {
        if
    }
}
