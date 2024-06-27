#include <stdio.h>
#include <stdlib.h>
#include <string.h>



enum Class {
    A,
    B,
    C,
    D
};

const char* class_names[] = {"A","B","C","D"};


enum Year {
    ONE = 0,
    TWO = 1,
    THREE = 2,
    FOUR = 3,
    FIVE = 4,
    SIX = 5
};

const char* years[] = {"ONE","TWO","THREE","FOUR","FIVE","SIX"};

typedef struct {
    int id;
    enum Class class;
    enum Year year;
    char name[50];
}Student;

void print_manage_student_menu(void);
void print_main_menu(void);
void add_student(void);
Student *create_student(int id, enum Class class, enum Year year, char name[]);
int read_line(char s[],int maxlen);
void find_student();

void print_main_menu() {
    char choice[2];
    printf("Please select one of the following or -1 if want to exit\n");
    printf("1. Manage Students\n");
    printf("\t- Add Student\n");

    read_line(choice, 2);

    switch (atoi(choice)) {
        default: return;

        case 1: print_manage_student_menu();

    }
}

void print_manage_student_menu() {
    char choice[2];

    printf("Select on of the following options or -1 to exit\n");
    printf("1. Add student\n");

    read_line(choice, 2);

    switch (atoi(choice)) {
        default: return;

        case 1: add_student();
        case 2: find_student();
    }

}

void find_student() {
    char id[2];

    printf("Please enter the ID of the Student");
    read_line(id, 2);

    FILE * file = fopen("student.dat", "r");

    fseek(file,2, SEEK_SET);

    char buff[24];

    fread(buff, 5, 1, file);

    buff[23] = "\0";

    printf(buff);
    /*
    *Explanation
    Struct Definition:

    The Record struct is defined with a fixed length for both id and content.
    writeRecord Function:

    The function ensures that each record is written in a fixed-length format using snprintf and fwrite.
    readRecordByNumber Function:

    Calculates the byte offset based on the record number.
    Uses fseek to jump directly to the record's position.
    Reads the fixed-length record using fread and parses the data.
    Main Function:

    Opens the file and writes a few sample records.
    Prompts the user to enter a record number to read.
    Reads and displays the specified record.
    By ensuring that each entry has a fixed length, you can efficiently access any entry in the file directly using fseek, achieving 𝑂(1)
    O(1) time complexity for accessing specific records.
     */


}

void add_student() {
    char name[50];
    char class_input[2];
    char year_input_str[3];
    int year_input;
    enum Year year;
    enum Class class;
    int id;

    printf("Name of the student:\n");
    read_line(name, 50);

    printf("Year:\n");
    read_line(year_input_str, 2);
    year_input = atoi(year_input_str);

    printf("Class:\n");
    read_line(class_input, 2);


    switch (year_input) {
        default: exit(0);
        case 1: year = ONE;
        case 2: year = TWO;
        case 3: year = THREE;
        case 4: year = FOUR;
        case 5: year = FIVE;
        case 6: year = SIX;
    }

    if (strcmp(class_input, "A") == 0) {
        class = A;
    } else if (strcmp(class_input, "B") == 0) {
        class = B;
    } else if (strcmp(class_input, "C") == 0) {
        class = C;
    }else if (strcmp(class_input, "D") == 0) {
        class = D;
    } else {
        printf("Invalid class input.\n");
        exit(0);
    }

    //find out if the file exists, if it does then find the size of the file.
    //if the file size is less than 54 chars then no newline.

    FILE * file = fopen("student.dat", "r");

    fseek(file, 0, SEEK_END);

    bool file_empty = ftell(file);

    size_t file_size = ftell(file);
    printf("%llu",file_size);

    if(!file_empty) {
        id = 1;
        fclose(file);
    }else {
        fclose(file);

        file = fopen("student.dat", "r");

        long max_len = 55+1; //max_len is 56, the plus 1 is to signify the null terminator
        char buff[max_len]; //buffer is 56 characters long

        fseek(file, -max_len, SEEK_END);

        fread(buff, max_len-1, 1, file);

        fclose(file);

        buff[max_len-1] = '\0'; // [55] which is the last character in the array is null terminated!
        char *last_newline = strrchr(buff, '\n'); // finds the last occurence of a newline in the buffer.
        char *last_line = last_newline+1; //the last line will always be after the newline char therefore the plus one
        if (last_newline != NULL) {
            int count = 0;
            char id_str[4]; //max for characters for an ID
            while (*last_line != ',' || count < 5) { //until the last_line character doesnt equal a comma contunue the loop
                id_str[count] = *last_line; //add the last line char to the ID char.
                *last_line++; //check next last line character
                count++; // counter increases
            }
            id = atoi(id_str)+1; // take the last ID from the entry and now increase it by one, thats the id of the next entry.
        }
    }

    char entry[56];
    sprintf(entry,"%04d,NAME:%s,YEAR:%d,CLASS:%s", id, name, year, class_input); //pad ID with leading 0s max of four
    size_t entry_length = strlen(entry);

    if (entry_length < 56) {
        for (int i = entry_length; i < 56; ++i) {
            entry[i] = ' ';
        }
        entry[55] = "\0";
    }

    file = fopen("student.dat","a");

    //fprintf(file, "\n%04d,NAME:%s,YEAR:%d,CLASS:%s", id, name, year, class_input);
    // If the file is empty then no need for \n character.

    if (!file_empty) {
        fprintf(file, "%s", entry);
    }else {
        fprintf(file, "\n%s", entry);

    }


    fclose(file);

    /*
    *   FILE * file = fopen("student.dat", "r");
    int id = 3;
    long offset = (id-1)*54+(id-1);

    if (file==NULL) {
        printf("File not found");
    }else {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);

        if (file_size <= 54) {
            char buff[55];
            fseek(file, 0, SEEK_SET);
            fread(buff, 1, 54, file);
            buff[54] = "\0";
            printf(buff);
        }else {
            char buff[55];
            fseek(file, offset, SEEK_SET);
            fread(buff, 1, 54, file);
            buff[54] = "\000";
            printf(buff);
        }
     */
}


Student *create_student(int id, enum Class class, enum Year year, char name[]) {
    Student *new_student = (Student*)malloc(sizeof(Student));

    new_student->id = id;
    new_student->class = class;
    new_student->year = year;
    strncpy(new_student->name, name, sizeof(new_student->name) - 1);

    return new_student;
}


int read_line(char s[], int maxlen) {
    char ch;
    int i = 0;
    int chars_remain = 1;
    while (chars_remain) {
        ch=getchar();
        if (ch == '\n' || ch == EOF) {
            chars_remain = 0;
        }else if (i < maxlen - 1) {
            s[i] = ch;
            i++;
        }
    }
    s[i] = '\0';

    return i;
}



int main(void) {
   char str[10] = "BalazsG";
   size_t strlenght = strlen(str);
   print_main_menu();
   /*
    *
    for (int i = strlenght; i < 10; ++i) {
        printf("\n%d", i);
        str[i] = 'A';
    }
    printf("\n%s", str);
    *
    */



}
