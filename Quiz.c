#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct flashcard {
    char question[256];
    char answer[256];
} flash;

void subjects();
void loadFlashcards(const char *filename);
void createFlashcards(const char *filename);
void quizMode(FILE *file);
void removeFlashcards(const char *filename);
void showFlashcards(FILE *file);
void toLowerCase(char *str);

int main() {
    subjects();
    return 0;
}

void subjects() {
    int choice;
    while (1) {
        printf("\nChoose a subject:\n");
        printf("1. Mathematics\n");
        printf("2. Science\n");
        printf("3. History\n");
        printf("4. English\n");
        printf("5. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            loadFlashcards("mathematics.txt");
        } else if (choice == 2) {
            loadFlashcards("science.txt");
        } else if (choice == 3) {
            loadFlashcards("history.txt");
        } else if (choice == 4) {
            loadFlashcards("english.txt");
        } else if (choice == 5) {
            break;
        } else {
            printf("\nInvalid choice. Try again.\n");
        }
    }
}

void loadFlashcards(const char *filename) {
    int choice;
    while (1) {
        FILE *file = fopen(filename, "a+");
        if (!file) {
            printf("\nCould not open file.\n");
            return;
        }

        printf("\nOptions:\n");
        printf("1. Create Flashcards\n");
        printf("2. Quiz Mode\n");
        printf("3. Remove Flashcards\n");
        printf("4. Show Flashcards\n");
        printf("5. Back to Subjects\n");
        printf("6. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            fclose(file);
            createFlashcards(filename);
        } else if (choice == 2) {
            quizMode(file);
        } else if (choice == 3) {
            fclose(file);
            removeFlashcards(filename);
        } else if (choice == 4) {
            showFlashcards(file);
        } else if (choice == 5) {
            fclose(file);
            break;
        } else if (choice == 6) {
            fclose(file);
            exit(0);
        } else {
            printf("\nInvalid choice.\n");
        }
        fclose(file);
    }
}

void createFlashcards(const char *filename) {
    FILE *f = fopen(filename, "a");
    if (!f) {
        printf("\nError opening file.\n");
        return;
    }

    char question[256], answer[256];
    printf("\n--- Add Flashcards ---\n");
    while (1) {
        printf("\nEnter question (blank to stop): ");
        fgets(question, sizeof(question), stdin);
        question[strcspn(question, "\n")] = '\0';

        if (strlen(question) == 0) {
            break;
        }

        printf("\nEnter answer: ");
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = '\0';

        fprintf(f, "%s|%s\n", question, answer);
        printf("\nFlashcard saved!\n");
    }
    fclose(f);
}

void quizMode(FILE *file) {
    rewind(file);
    flash cards[100];
    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && count < 100) {
        sscanf(line, "%255[^|]|%255[^\n]", cards[count].question, cards[count].answer);
        count++;
    }

    if (count == 0) {
        printf("\nNo flashcards found.\n");
        return;
    }

    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        int j = rand() % count;
        flash temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }

    int score = 0;
    printf("\n--- Quiz Mode ---\n");
    for (int i = 0; i < count; i++) {
        char userAnswer[256];
        printf("Q: %s\n", cards[i].question);
        printf("Your answer: ");
        fgets(userAnswer, sizeof(userAnswer), stdin);
        userAnswer[strcspn(userAnswer, "\n")] = '\0';

        toLowerCase(userAnswer);
        toLowerCase(cards[i].answer);

        if (strcmp(userAnswer, cards[i].answer) == 0) {
            printf("\nCorrect!\n");
            score++;
        } else {
            printf("\nWrong! Correct answer: %s\n", cards[i].answer);
        }
    }
    printf("\nYour score: %d/%d\n", score, count);
}

void removeFlashcards(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("\nError opening file.\n");
        return;
    }

    flash cards[100];
    char line[512];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < 100) {
        sscanf(line, "%255[^|]|%255[^\n]", cards[count].question, cards[count].answer);
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("\nNo flashcards to remove.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("%d. %s | %s\n", i + 1, cards[i].question, cards[i].answer);
    }

    int choice;
    printf("\nEnter number to remove (0 to cancel): ");
    scanf("%d", &choice);
    getchar();

    if (choice <= 0 || choice > count) {
        printf("\nCanceled.\n");
        return;
    }

    file = fopen(filename, "w");
    for (int i = 0; i < count; i++) {
        if (i != choice - 1) {
            fprintf(file, "%s|%s\n", cards[i].question, cards[i].answer);
        }
    }
    fclose(file);
    printf("\nFlashcard removed.\n");
}

void showFlashcards(FILE *file) {
    rewind(file);
    char line[512];
    flash fc;
    printf("\n--- Flashcards ---\n");
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%255[^|]|%255[^\n]", fc.question, fc.answer);
        printf("Q: %s\nA: %s\n\n", fc.question, fc.answer);
    }
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

