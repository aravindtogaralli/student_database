#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Only for strcmp

typedef struct date {
    int day;
    int month;
    int year;
} DATE;

typedef struct student {
    char batch_id[10];
    char name[50];
    float avg_internal_marks;
    char assessment_status[5];
    DATE dob;
    DATE doj;
} ST;

void read_string(char *str, int size) {
    int i = 0;
    char ch;
    while (i < size - 1 && (ch = getchar()) != '\n') {
        str[i++] = ch;
    }
    str[i] = '\0';
}

void input_student(ST *s) {
    printf("Enter Batch ID: ");
    read_string(s->batch_id, 10);

    printf("Enter Name: ");
    read_string(s->name, 50);

    printf("Enter Average Internal Marks: ");
    scanf("%f", &s->avg_internal_marks);

    printf("Enter Assessment Status (r/nr): ");
    scanf("%s", s->assessment_status);

    printf("Enter Date of Birth (dd mm yyyy): ");
    scanf("%d%d%d", &s->dob.day, &s->dob.month, &s->dob.year);

    printf("Enter Date of Joining (dd mm yyyy): ");
    scanf("%d%d%d", &s->doj.day, &s->doj.month, &s->doj.year);
    getchar(); // clear newline from buffer
}

void save_to_file(ST *s) {
    char filename[20];
    int i = 0;
    while (s->batch_id[i] != '\0') {
        filename[i] = s->batch_id[i];
        i++;
    }
    filename[i++] = '.';
    filename[i++] = 't';
    filename[i++] = 'x';
    filename[i++] = 't';
    filename[i] = '\0';

    FILE *fp = fopen(filename, "ab");
    fwrite(s, sizeof(ST), 1, fp);
    fclose(fp);
}

void display(ST s) {
    printf("\nBatch ID: %s\n", s.batch_id);
    printf("Name: %s\n", s.name);
    printf("Marks: %.2f\n", s.avg_internal_marks);
    printf("Assessment: %s\n", s.assessment_status);
    printf("DOB: %02d-%02d-%04d\n", s.dob.day, s.dob.month, s.dob.year);
    printf("DOJ: %02d-%02d-%04d\n", s.doj.day, s.doj.month, s.doj.year);
}

void search_by_date(char *filename, char type[], int d, int m, int y) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("File not found.\n");
        return;
    }
    ST s;
    int found = 0;
    while (fread(&s, sizeof(ST), 1, fp)) {
        DATE dt = strcmp(type, "dob") == 0 ? s.dob : s.doj;
        if (dt.day == d && dt.month == m && dt.year == y) {
            display(s);
            found = 1;
        }
    }
    if (!found) printf("No matching records.\n");
    fclose(fp);
}

void search_by_name(char *filename, char *name) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return;
    ST s;
    int found = 0;
    while (fread(&s, sizeof(ST), 1, fp)) {
        int i = 0;
        while (s.name[i] == name[i] && s.name[i] != '\0' && name[i] != '\0')
            i++;
        if (s.name[i] == '\0' && name[i] == '\0') {
            display(s);
            found = 1;
        }
    }
    if (!found) printf("Not found.\n");
    fclose(fp);
}

void search_by_batchid(char *batch_id) {
    char filename[20];
    int i = 0;
    while (batch_id[i] != '\0') {
        filename[i] = batch_id[i];
        i++;
    }
    filename[i++] = '.';
    filename[i++] = 't';
    filename[i++] = 'x';
    filename[i++] = 't';
    filename[i] = '\0';

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Batch file not found.\n");
        return;
    }
    ST s;
    while (fread(&s, sizeof(ST), 1, fp)) {
        display(s);
    }
    fclose(fp);
}

void edit_record(char *filename, char *name) {
    FILE *fp = fopen(filename, "rb+");
    if (!fp) return;
    ST s;
    long pos;
    while ((pos = ftell(fp)), fread(&s, sizeof(ST), 1, fp)) {
        int i = 0;
        while (s.name[i] == name[i] && s.name[i] != '\0' && name[i] != '\0')
            i++;
        if (s.name[i] == '\0' && name[i] == '\0') {
            printf("Old Record:\n");
            display(s);
            printf("\nEnter new details:\n");
            input_student(&s);
            fseek(fp, pos, SEEK_SET);
            fwrite(&s, sizeof(ST), 1, fp);
            printf("Record updated.\n");
            break;
        }
    }
    fclose(fp);
}

void search_by_marks(char *filename, float min, float max) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return;
    ST s;
    while (fread(&s, sizeof(ST), 1, fp)) {
        if (s.avg_internal_marks >= min && s.avg_internal_marks <= max) {
            display(s);
        }
    }
    fclose(fp);
}

void search_by_status(char *filename, char *status) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return;
    ST s;
    while (fread(&s, sizeof(ST), 1, fp)) {
        int i = 0;
        while (s.assessment_status[i] == status[i] && s.assessment_status[i] != '\0' && status[i] != '\0')
            i++;
        if (s.assessment_status[i] == '\0' && status[i] == '\0') {
            display(s);
        }
    }
    fclose(fp);
}

void display_all(char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("File not found.\n");
        return;
    }
    ST s;
    while (fread(&s, sizeof(ST), 1, fp)) {
        display(s);
    }
    fclose(fp);
}

int main() {
    int ch, n, i;
    ST *s;
    char filename[20], name[50], batch_id[10], status[5];
    int d, m, y;
    float min, max;

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Add Student\n2. Search by DOB\n3. Search by DOJ\n4. Search by Name\n5. Search by Batch ID\n6. Edit Record\n7. Search by Marks\n8. Search by Status\n9. Display All\n0. Exit\nChoice: ");
        scanf("%d", &ch);
        getchar(); // clear buffer

        switch (ch) {
            case 1:
                printf("Enter number of students: ");
                scanf("%d", &n);
                getchar();
                s = (ST *)malloc(n * sizeof(ST));
                for (i = 0; i < n; i++) {
                    input_student(&s[i]);
                    save_to_file(&s[i]);
                }
                free(s);
                break;

            case 2:
                printf("Enter filename: ");
                read_string(filename, 20);
                printf("Enter DOB (dd mm yyyy): ");
                scanf("%d%d%d", &d, &m, &y);
                search_by_date(filename, "dob", d, m, y);
                break;

            case 3:
                printf("Enter filename: ");
                read_string(filename, 20);
                printf("Enter DOJ (dd mm yyyy): ");
                scanf("%d%d%d", &d, &m, &y);
                search_by_date(filename, "doj", d, m, y);
                break;

            case 4:
                printf("Enter filename: ");
                read_string(filename, 20);
                printf("Enter Name: ");
                read_string(name, 50);
                search_by_name(filename, name);
                break;

            case 5:
                printf("Enter Batch ID: ");
                read_string(batch_id, 10);
                search_by_batchid(batch_id);
                break;

            case 6:
                printf("Enter filename: ");
                read_string(filename, 20);
                printf("Enter Name to Edit: ");
                read_string(name, 50);
                edit_record(filename, name);
                break;

            case 7:
                printf("Enter filename: ");
                read_string(filename, 20);
                printf("Enter marks range (min max): ");
                scanf("%f%f", &min, &max);
                search_by_marks(filename, min, max);
                break;

            case 8:
                printf("Enter filename: ");
                read_string(filename, 20);
                printf("Enter Assessment Status (r/nr): ");
                read_string(status, 5);
                search_by_status(filename, status);
                break;

            case 9:
                printf("Enter filename: ");
                read_string(filename, 20);
                display_all(filename);
                break;

            case 0:
                return 0;
        }
    }

    return 0;
}

