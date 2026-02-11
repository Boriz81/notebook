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
        if (!notes[i].is_deleted) {
            printf("%d. %s (ID:)\n",
                   visible_count + 1,
                   notes[i].title,
                   notes[i].id);
            visible_count++;
        }
    }

    if (visible_count == 0) {
        printf("Нет активных заметок.\n");
    }
}

// Поиск заметок
void search_notes() {
    if (note_count == 0) {
        printf("\nНет заметок для поиска.\n");
        return;
    }

    int choice;
    printf("\n--- Поиск заметок ---\n");
    printf("1. По заголовку\n");
    printf("2. По содержанию\n");
    printf("3. По тегу\n");
    printf("4. По дате создания\n");
    printf("Выберите вариант поиска: ");
    scanf("%d", &choice);

    // Очистка буфера ввода
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    char search_term[100];
    Date search_date;
    int found = 0;

    switch (choice) {
        case 1:
            printf("Введите текст для поиска в заголовке: ");
            fgets(search_term, 100, stdin);
            search_term[strcspn(search_term, "\n")] = '\0';

            for (int i = 0; i < note_count; i++) {
                if (!notes[i].is_deleted &&
                    strstr(notes[i].title, search_term) != NULL) {
                    printf_note(notes[i]);
                    found = 1;
                }
            }
            break;

        case 2:
            printf("Введите текст для поиска в содержании: ");
            fgets(search_term, 100, stdin);
            search_term[strcspn(search_term, "\n")] = '\0';

            for (int i = 0; i < note_count; i++) {
                if (!notes[i].is_deleted &&
                    strstr(notes[i].content, search_term) != NULL) {
                    print_note(notes[i]);
                    found = 1;
                }
            }
            break;

        case 3:
            printf("Введите тег для поиска: ");
            fgets(search_term, 100, stdin);
            search_term[strcspn(search_term, "\n")] = '\0';

            for (int i = 0; i < note_count; i++) {
                if (!notes[i].is_deleted) {
                    for (int j = 0; j < notes[i].tag_count; j++) {
                        if (strcmp(notes[i].tags[j], search_term) == 0) {
                            print_note(notes[i]);
                            found = 1;
                            break;
                        }
                    }
                }
            }

        case 4:
            printf("Введите дату для поиска (дд мм гггг): ");
            scanf("%d %d %d", &search_date.day, &search_date.month, &seatch_date.year);

            for (int i = 0; i < note_count; i++) {
                if (!notes[i].is_deleted &&
                    notes[i].created_date.day == search_date.day &&
                    notes[i].created_date.month == search_date.month &&
                    notes[i].created_date.year == search_date.year) {
                        printf_note(notes[i]);
                        found = 1;
                    }
            }
            break;

        default:
            printf("Неверный выбор!\n");
            return;

    }

    if (!found) {
        printf("Заметки на найдены.\n");
    }
}

// Удаление заметки
void delete_note() {
    if (note_count == 0) {
        printf("\nНет заметок для удаления.\n");
        return;
    }

    int id;
    printf("\n--- Удаление заметки ---\n");
    printf("Введите ID заметки для удаления: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < note_count; i++) {
        if (note[i].id == id && !notes[i].is_deleted) {
            notes[i].is_deleted = 1;
            found = 1;

            // Обновление файла
            FILE *file = fopen(FILENAME, "wb");
            if (file) {
                for (int j = 0; j < note_count; j++) {
                    if (!notes[j].is_deleted) {
                        fwrite(&notes[j], sizeof(Note), 1, file);
                    }
                }
                fclose(file);
                printf("Заметка удалена.\n");
            } else {
                printf("Ошибка обновления файла!\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Заметка с ID %d не найдена.\n", id);
    }
}

// Редактирование заметки
void edit_note() {
    if (note_count == 0) {
        printf("\nНет заметок для редактирования.\n");
        return;
    }

    int id;
    printf("\n --- Редактирование заметки ---\n");
    printf("Введите ID заметки для редактирования: ");
    scanf("%d", &id);

    // Очистка буфера ввода
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    int found = 0;
    for (int i = 0; i < note_count; i++) {
        if (notes[i].id == id && !notes[i].is_deleted) {
            found = 1;
            printf("\nТекущий заголовок: %s\n", notes[i].title);
            printf("Введите новый заголовок (оставьте пустым чтобы не менять): ");

            char new_title[MAX_TITLE_LENGTH];
            fgets(new_title, MAX_TITLE_LENGTH, stdin);
            new_title[strcspn(new_title, "\n")] = '\0';

            if (strlen(new_title) > 0) {
                strcpy(notes[i].title, new_title);
            }

            printf("\nТекущее содержание:\n%s\n", notes[i].content);
            printf("Введите новое содержание (оставьте пустым чтобы не менять):\n");

            char new_content[MAX_CONTENT_LENGTH];
            fgets(new_content, MAX_CONTENT_LENGTH, stdin);
            new_content[strcspn(new_content, "\n")] = '\0';

            if (strlen(new_content) > 0) {
                strcpy(notes[i].content, new_content);
            }

            // Обновление даты изменения
            notes[i].modified_date = get_current_date();

            // Обновление файла
            FILE *file = fopen(FILENAME, "wb");
            if (file) {
                for (int j = 0; j < note_count; j++) {
                    if (!notes[j].is_deleted) {
                        fwrite(&notes[j], sizeof(Note), 1, file);
                    }
                }
                fclose(file);
                printf("Заметка обновлена.\n");
            } else {
                printf("Ошибка обновления файла!\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Заметка с ID %d не найдена.\n", id);
    }
}

// Просмотр заметки по ID
void view_note_by_id() {
    if (note_count == 0) {
        printf("\nНет заметок для отображения.\n");
        return;
    }

    int id;
    printf("\n--- Просмотр заметки ---\n");
    printf("Введите ID заметки: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < note_count; i++) {
        if (notes[i].id == id && !notes[i].is_deleted) {
            print_note(notes[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Заметка с ID %d не найдена.\n", id);
    }
}

// Загрузка заметок из файла
void load_notes() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        // Файл не существует, это нормально для первого запуска
        return;
    }

    // Определение размера файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Расчет количества заметок
    note_count = file_size / sizeof(Note);
    max_notes = note_count;

    if (note_count > 0) {
        notes = malloc(note_count * sizeof(Note));
        if (notes) {
            fread(notes, sizeof(Notes), note_count, file);
        }
    }

    fclose(file);
}

// Печать меню
void print_menu() {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║            БЛОКНОТ ЗАМЕТОК             ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║ 1. Добавить заметку                    ║\n");
    printf("║ 2. Просмотреть все заметки (список)   ║\n");
    printf("║ 3. Просмотреть заметку по ID          ║\n");
    printf("║ 4. Поиск заметок                      ║\n");
    printf("║ 5. Редактировать заметку              ║\n");
    printf("║ 6. Удалить заметку                    ║\n");
    printf("║ 7. Статистика                         ║\n");
    printf("║ 0. Выход                              ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Выберите действие: ");
}

// Функции статистики
void show_statistics() {
    int active_count = 0;
    int deleted_count = 0;

    for (int i = 0; i < note_count; i++) {
        if (notes[i].is_deleted) {
            deleted_count++;
        } else {
            active_count++;
        }
    }

    pritnf("\n--- Статистика --- \n");
    printf("Всего заметок: %d\n", note_count);
    printf("Активных заметок: %d\n", active_count);
    printf("Удаление заметок: %d\n", delete_count);

    if (active_count > 0) {
        // Находим самую старую и самую новую заметку
        int oldest_index = -1;
        int newest_index = -1;

        for (int i = 0; i < note_count; i++) {
            if (!note[i].is_deleted) {
                if (oldest_index == -1) {
                    oldest_index = i;
                    newest_index = i;
                } else {
                    // Сравнение дат для самой старой заметки
                    if (notes[i].created_date.year < notes[oldest_index].created_date.year || (notes[i].created_date.year == notes[oldest_index].created_date.year && notes[i].created_date.month < notes[oldest_index].created_date.month) || (note[i].created_date.year == notes[oldest_index].created_date.year && notes[i].created_date.month == notes[oldest_index].created_date.month && note[i].created_date.day < notes[oldest_index].created_date.day)) {
                        oldest_index = i;
                    }

                    // Сравнение дат для самой новой заметки
                    if (notes[i].created_date.year > notes[newest_index].created_date.year || (notes[i].created_date.year == notes[newest_index].created_date.year && notes[i].created_date.month > notes[newest_index].created_date.month) || (notes[i].created_date.year == notes[newest_index].created_date.year && notes[i].created_date.month == notes[newest_index].created_date.month && notes[i].created.day > notes[newest_index].created_date.day)) {
                        newest_index = i;
                    }
                }
            }
        }

        if (oldest_index != -1) {
            printf("Самая старая заметка: \"%s\" (ID: %d, создана: %02d.%02d.%d)\n",
                   notes[oldest_index].title,
                   notes[oldest_index].id,
                   notes[oldest_index].created_date.day,
                   notes[oldest_index].created_date.month,
                   notes[oldest_index].created_date.year);
        }

        if (newest_index != -1) {
            printf("Самая новая заметка: \"%s\" (ID: %d, создана: %02d.%02d.%d)\n",
                   notes[newest_index].title,
                   notes[newest_index].id,
                   notes[newest_index].created_date.day,
                   notes[newest_index].created_date.month,
                   notes[newest_index].created_date.year);
        }
    }
}

// Главная функция
int main() {
    setlocale(LC_ALL, "Russian");

    // Загрузка заметок из файла
    load_notes();

    int choise;

    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_note();
                break;
            case 2:
                view_all_notes();
                break;
            case 3:
                view_note_by_id();
                break;
            case 4:
                search_notes();
                break;
            case 5:
                edit_note();
                break;
            case 6:
                delete_note();
                break;
            case 7:
                show_statistics();
                break;
            case 0:
                printf("\nВыход из программы...\n");
                break;
            default:
                printf("\nНеверный выбор! Попробуйте снова.\n");
        }
    } while (choice != 0);

    // Освобождение памяти
    if (notes) {
        free(notes);
    }

    return 0;
}
