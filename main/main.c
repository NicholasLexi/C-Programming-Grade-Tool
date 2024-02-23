/*
Advanced C Final Project - Student Grading System
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<ctype.h>
#include<math.h>
#include<windows.h> // only in windows
#include<stdbool.h> // only in c99

// definitions
#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 30
#define MAX_COURSE_QUANTITY 5
#define MAX_STUDENTS 10000

// external variables
int course_hours[MAX_COURSE_QUANTITY] = {4, 4, 3, 3, 4};
int student_count = 0;

struct student
{
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char gender;
    int age;
    int student_id;
    int grades[MAX_COURSE_QUANTITY];
    float points[MAX_COURSE_QUANTITY]; // will be calculated
    char alphabet_grades[MAX_COURSE_QUANTITY][3]; // will  be calculated
    float final_grade; // will be calculated
};

// the main array of structure variable
struct student student_data[MAX_STUDENTS]; 

// function prototypes
int read_student_data(const char *, struct student *);
void calculate_alphabet_grade(struct student *);
void calculate_final_grade(struct student *);
void show_file_read_success(void);
void show_all_student_data(int);
void add_student_data(struct student *, const char *);
void delete_student_data(struct student *, const char *);
void find_student_data(struct student *);
void create_backup(struct student *);
void exit_program(void);
void swap(struct student *, struct student *);
void quicksort_ascending(struct student *, int);
void quicksort_recursion_ascending(struct student *, int, int);
int partition_ascending(struct student *, int, int);
void quicksort_desecending(struct student *, int);
void quicksort_recursion_descending(struct student *, int, int);
int partition_descending(struct student *, int, int);
void save_changes_to_csv(struct student *, const char *);
void quicksort_ascending_name(struct student *, int);
void quicksort_recursion_ascending_name(struct student *, int , int);
int partition_ascending_name(struct student *, int, int);
int partition_ascending(struct student *, int, int);
void quicksort_descending_name(struct student *, int);
void quicksort_recursion_descending_name(struct student *, int, int);
int partition_descending_name(struct student *, int, int);
void see_basic_statistics(struct student *, int);
void course_average(struct student *, int);
void final_grades_median(struct student *, int);
void quicksort_median_array(float [], int, int);
int partition_median_array(float arr[], int, int);
void swap_array(float *, float *);
void final_grades_mean(struct student *, int);
float final_grades_variance(struct student *, int);
float final_grades_standard_deviation(float);
void visualize_statistics(struct student *, int);
void grade_distribution(struct student *, int);
void box_whisker_plot(struct student *, int);
void scatter_plot_age_vs_final_grade(struct student *, int); 
void create_gender_comparison_histogram(struct student *, int);

int main(void)
{
    char file_name_input[50];

    // primary function
    puts("============ Welcome ============");
    printf("Enter file name (csv) to proceed: ");
    scanf("%49[^\n]", file_name_input);
    system("cls");

    const char *filename = file_name_input;

    student_count = read_student_data(filename, student_data);
    calculate_alphabet_grade(student_data);
    calculate_final_grade(student_data);

    // main menu
    int choice;
    do
    {
        puts("============ MENU ============");
        puts("1. Show current list");
        puts("2. Add student");
        puts("3. Delete student");
        puts("4. Find student");
        puts("5. Sort by ascending grade");
        puts("6. Sort by descending grade");
        puts("7. Sort by alphabetical order (ascending)");
        puts("8. Sort by alphabetical order (descending)");
        puts("9. Basic statistics preview");
        puts("10. Visualize statistics");
        puts("11. Create backup");
        puts("0. Exit");
        puts("Enter command:");
        scanf("%d", &choice);
        switch (choice)
        {
            case 0:
            exit_program();
            break;

            case 1:
            system("cls");
            show_all_student_data(student_count);
            break;
            
            case 2:
            system("cls");
            add_student_data(student_data, filename);
            break;

            case 3:
            system("cls");
            delete_student_data(student_data, filename);
            break;

            case 4:
            system("cls");
            find_student_data(student_data);
            break;

            case 5:
            system("cls");
            quicksort_ascending(student_data, student_count);
            show_all_student_data(student_count);
            save_changes_to_csv(student_data, filename);
            break;

            case 6:
            system("cls");
            quicksort_desecending(student_data, student_count);
            show_all_student_data(student_count);
            save_changes_to_csv(student_data, filename);
            break;

            case 7:
            system("cls");
            quicksort_ascending_name(student_data, student_count);
            show_all_student_data(student_count);
            save_changes_to_csv(student_data, filename);
            break;

            case 8:
            system("cls");
            quicksort_descending_name(student_data, student_count);
            show_all_student_data(student_count);
            save_changes_to_csv(student_data, filename);
            break;

            case 9:
            system("cls");
            see_basic_statistics(student_data, student_count);
            break;

            case 10:
            system("cls");
            visualize_statistics(student_data, student_count);
            break;
            
            case 11:
            system("cls");
            create_backup(student_data);
            break;
        }
    
    } while (choice != 0);
    
    system("cls");

    return 0;
}

/*  read_student_data function: 

    - > reads csv file
    - > paramaters: csv file name, structure pointer 
    - > uses tokenization (strtok)
    - > turns strings to integers using atoi()
    - > store in structure
    - > after reading, returns the number of student (int return type)
*/

int read_student_data(const char *filename, struct student *students)
{
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        perror("File failed to open\n");
        exit(EXIT_FAILURE);
    }

    char buffer[1000];
    int count = 0;

    while(fgets(buffer, sizeof(buffer), file) != NULL && count < MAX_STUDENTS)
    {
        //tokenize csv (delimeter is ,)
        char *token = strtok(buffer, ",");
        if(token == NULL)
        {
            continue; // skips blank row
        }

        // reads name
        strcpy(students[count].name, token);

        token = strtok(NULL, ",");

        if(token == NULL)
        {
            fprintf(stderr, "Error reading CSV file: Insufficient fields\n");
            exit(EXIT_FAILURE);
        }

        // read email
        strcpy(students[count].email, token);

        token = strtok(NULL, ",");

        if(token == NULL)
        {
            fprintf(stderr, "Error reading CSV file: Insufficient fields\n");
            exit(EXIT_FAILURE);
        }        

        students[count].gender = toupper(token[0]);

        token = strtok(NULL, ",");

        if(token == NULL)
        {
            fprintf(stderr, "Error reading CSV file: Insufficient fields\n");
            exit(EXIT_FAILURE);
        }        

        // read age (atoi converts string to int)
        students[count].age = atoi(token);

        token = strtok(NULL, ",");

        if(token == NULL)
        {
            fprintf(stderr, "Error reading CSV file: Insufficient fields\n");
            exit(EXIT_FAILURE);
        }

        // read student id
        students[count].student_id = atoi(token);

        // read array of grades
        for(int i = 0 ; i < MAX_COURSE_QUANTITY ; i++)
        {
            token = strtok(NULL, ",");
            if (token == NULL)
            {
                fprintf(stderr, "Error reading CSV file: Insufficient fields for grades\n");
                exit(EXIT_FAILURE);
            }
            students[count].grades[i] = atoi(token);
        }
        count++;
    }
    fclose(file);
    show_file_read_success(); 
    return count;
}

/*  calculate_alphabet_grade function:

    char alphabet_grades[MAX_COURSE_QUANTITY][3] has three columns, 
    char alphabet_grades[MAX_COURSE_QUANTITY][0] will have the base alphabet grade 
    char alphabet_grades[MAX_COURSE_QUANTITY][1] will have the sign +,-, or none
    char alphabet_grades[MAX_COURSE_QUANTITY][2] will have '\0'
*/

void calculate_alphabet_grade(struct student *students)
{    
    for(int i = 0 ; i < student_count ; i++)
    {
        for(int j = 0 ; j < MAX_COURSE_QUANTITY ; j++)
        {
            if(students[i].grades[j] >= 97)
            {
                students[i].alphabet_grades[j][0] = 'A';
                students[i].alphabet_grades[j][1] = '+';
                students[i].points[j] = 4.0f;
            }
            else if(students[i].grades[j] >= 93)
            {
                students[i].alphabet_grades[j][0] = 'A';
                students[i].alphabet_grades[j][1] = ' ';
                students[i].points[j] = 4.0f;
            }
            else if(students[i].grades[j] >= 90)
            {
                students[i].alphabet_grades[j][0] = 'A';
                students[i].alphabet_grades[j][1] = '-';
                students[i].points[j] = 3.7f;
            }
            else if(students[i].grades[j] >= 87)
            {
                students[i].alphabet_grades[j][0] = 'B';
                students[i].alphabet_grades[j][1] = '+';
                students[i].points[j] = 3.3f;
            }
            else if(students[i].grades[j] >= 83)
            {
                students[i].alphabet_grades[j][0] = 'B';
                students[i].alphabet_grades[j][1] = ' ';
                students[i].points[j] = 3.0f;
            }
            else if(students[i].grades[j] >= 80)
            {
                students[i].alphabet_grades[j][0] = 'B';
                students[i].alphabet_grades[j][1] = '-';
                students[i].points[j] = 2.7f;
            }
            else if(students[i].grades[j] >= 77)
            {
                students[i].alphabet_grades[j][0] = 'C';
                students[i].alphabet_grades[j][1] = '+';
                students[i].points[j] = 2.3f;
            }
            else if(students[i].grades[j] >= 73)
            {
                students[i].alphabet_grades[j][0] = 'C';
                students[i].alphabet_grades[j][1] = ' ';
                students[i].points[j] = 2.0f;
            }
            else if(students[i].grades[j] >= 70)
            {
                students[i].alphabet_grades[j][0] = 'C';
                students[i].alphabet_grades[j][1] = '-';
                students[i].points[j] = 1.7f;
            }
            else if(students[i].grades[j] >= 67)
            {
                students[i].alphabet_grades[j][0] = 'D';
                students[i].alphabet_grades[j][1] = '+';
                students[i].points[j] = 1.3f;
                
            }
            else if(students[i].grades[j] >= 65)
            {
                students[i].alphabet_grades[j][0] = 'D';
                students[i].alphabet_grades[j][1] = ' ';
                students[i].points[j] = 1.0f;
            }
            else if(students[i].grades[j] < 65)
            {
                students[i].alphabet_grades[j][0] = 'F';
                students[i].alphabet_grades[j][1] = ' ';
                students[i].points[j] = 0.0f;

            }
            students[i].alphabet_grades[j][2] = '\0';
        }
    }
}

/*  calculate_final_grade function: 
    method of calculating final grade:
    course credit hours * points

    final grade = sum of quality points / sum of credit hours
*/

void calculate_final_grade(struct student *students)
{
    int total_credit_hours = 18;

    for (int i = 0; i < student_count; i++)
    {
        float total_quality_points = 0.0f;  // Declare inside the loop

        for (int j = 0; j < MAX_COURSE_QUANTITY; j++)
        {
            total_quality_points += (float)course_hours[j] * students[i].points[j];
        }

        // Update final_grade outside the inner loop
        students[i].final_grade = total_quality_points / (float)total_credit_hours;
    }
}

void show_file_read_success(void)
{
    puts("============ SYSTEM ============");
    puts("File sucessfully evaluated\n");
    puts("Enter any key to proceed to main menu: ");
    getch();
    system("cls");
}

void show_all_student_data(int student_count) 
{
    printf("%-35s%-35s%-10s%-8s%-8s%-8s%-8s%-8s%-s%-s\n", "Name", "E-mail", "ID", "LAG", "CAL", "STAT", "CPEN", "ALP", "", "Final Grade");
    puts("------------------------------------------------------------------------------------------------------------------------------------");

    for (int i = 0; i < student_count; i++) {
        printf("%-35s%-35s%-10d", student_data[i].name, student_data[i].email, student_data[i].student_id);

        for (int j = 0; j < MAX_COURSE_QUANTITY; j++) {
            printf("%-8s", student_data[i].alphabet_grades[j]);
        }

        printf("%-15.2f\n", student_data[i].final_grade);
    }
    puts("------------------------------------------------------------------------------------------------------------------------------------");
    getch();
    system("cls");
}

void add_student_data(struct student *students, const char *filename)
{
    int new_students = 0;
    printf("How many more students do you want to add? ");
    scanf("%d", &new_students);
    
    getchar(); // removes newline

    if (student_count + new_students > MAX_STUDENTS)
    {
        printf("Student list full!\n");
        getch();
        system("cls");
        return;
    }

    FILE *file = fopen(filename, "a");

    for (int i = student_count; i < student_count + new_students; i++)
    {

        printf("\nEnter details for Student %d:\n", i + 1);

        printf("Name: ");
        fgets(students[i].name,MAX_NAME_LENGTH,stdin);
        students[i].name[strcspn(students[i].name, "\n")] = '\0'; // Remove the newline character

        printf("E-mail: ");
        fgets(students[i].email, MAX_EMAIL_LENGTH, stdin);
        students[i].email[strcspn(students[i].email, "\n")] = '\0'; // Remove the newline character

        printf("Gender (M/F): ");
        scanf(" %c", &students[i].gender);

        printf("Age: ");
        scanf("%d", &students[i].age);

        printf("Student ID: ");
        scanf("%d", &students[i].student_id);

        printf("Grades for %d courses:\n", MAX_COURSE_QUANTITY);
        for (int j = 0; j < MAX_COURSE_QUANTITY; j++)
        {
            printf("Course %d: ", j + 1);
            scanf("%d", &students[i].grades[j]);
        }

        fprintf(file, "%s,%s,%c,%d,%d,%d,%d,%d,%d,%d\n", 
        students[i].name, students[i].email, students[i].gender, students[i].age, 
        students[i].student_id, students[i].grades[0], students[i].grades[1], students[i].grades[2], students[i].grades[3], students[i].grades[4]);

        // Clear the input buffer
        while (getchar() != '\n')
            ;
    }
    fclose(file);

    system("cls");

    student_count += new_students;

    calculate_alphabet_grade(student_data);
    calculate_final_grade(student_data);
    printf("\nStudent data added successfully!\n");
    getch();
    system("cls");
}

void delete_student_data(struct student *students, const char *filename)
{
    int student_id_to_delete, i;
    bool found_id = false;

    printf("Enter student ID to delete: ");
    scanf("%d", &student_id_to_delete);

    // search for id
    for(i = 0 ; i < student_count ; i++)
    {
        if(students[i].student_id == student_id_to_delete)
        {
            found_id = true;
            break;
        }
    }

    if(found_id == false)
    {
        printf("Student ID %d is not found\n", student_id_to_delete);
        return;
    }

    // i is used for indexing
    for( ; i < student_count - 1; i++)
    {
        students[i] = students[i + 1];
    }

    student_count--;

    FILE *file = fopen(filename, "w");

    if(file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);        
    }

    // re writing content
    for(int j = 0 ; j < student_count ; j++)
    {
        fprintf(file, "%s,%s,%c,%d,%d,%d,%d,%d,%d,%d\n",
                students[j].name, students[j].email, students[j].gender, students[j].age,
                students[j].student_id, students[j].grades[0], students[j].grades[1],
                students[j].grades[2], students[j].grades[3], students[j].grades[4]);        
    }

    fclose(file);
    printf("Student ID %d successfully deleted\n", student_id_to_delete);
    getch();
    system("cls");
}

void find_student_data(struct student *students)
{
    int student_id_to_find, i;
    bool id_found = false;

    printf("Enter student ID: ");
    scanf("%d", &student_id_to_find);

    // i is used for indexing
    for(i = 0 ; i < student_count ; i++)
    {
        if(students[i].student_id == student_id_to_find)
        {
            id_found = true;
            break;
        }
    }

    system("cls");
    if(id_found)
    {
        printf("%-35s%-35s%-10s%-8s%-8s%-8s%-8s%-8s%-s%-s\n", "Name", "E-mail", "ID", "LAG", "CAL", "STAT", "CPEN", "ALP", "", "Final Grade");
        puts("------------------------------------------------------------------------------------------------------------------------------------");
        printf("%-35s%-35s%-10d", student_data[i].name, student_data[i].email, student_data[i].student_id);
        printf("%-8s%-8s%-8s%-8s%-8s", student_data[i].alphabet_grades[0], student_data[i].alphabet_grades[1], student_data[i].alphabet_grades[2], student_data[i].alphabet_grades[3], student_data[i].alphabet_grades[4]);
        printf("%-15.2f\n", student_data[i].final_grade);
        puts("------------------------------------------------------------------------------------------------------------------------------------");
        getch();
        system("cls");
    }
    else
    {
        printf("Student ID %d is not found\n", student_id_to_find);
        getch();
        system("cls");
        return;
    }
}

void create_backup(struct student *students)
{   
    getchar(); // removes new line
    
    system("cls");

    char new_file_name[50];
    printf("Enter a new file name: ");
    scanf("%49[^\n]", new_file_name);

    FILE *new_file = fopen(new_file_name, "w");

    if(new_file == NULL)
    {
        printf("Error opening the file\n");
        getch();
        system("clr");
        return;
    }

    for(int i = 0 ; i < student_count ; i++)
    {
        fprintf(new_file, "%s,%s,%c,%d,%d,%d,%d,%d,%d,%d\n",
                students[i].name, students[i].email, students[i].gender, students[i].age,
                students[i].student_id, students[i].grades[0], students[i].grades[1],
                students[i].grades[2], students[i].grades[3], students[i].grades[4]);        
    }

    fclose(new_file);
    printf("File %s has been created!\n", new_file_name);
    getch();
    system("cls");
}

void quicksort_ascending(struct student *students, int student_count)
{
    quicksort_recursion_ascending(students, 0, student_count - 1);
}

void quicksort_recursion_ascending(struct student *students, int low, int high)
{
    if(low < high)
    {
    int pivot_index = partition_ascending(students, low, high);
    quicksort_recursion_ascending(students, low, pivot_index - 1);
    quicksort_recursion_ascending(students, pivot_index + 1, high);
    }
}

int partition_ascending(struct student *students, int low, int high)
{
    int pivot_value = students[high].final_grade;
    
    int i = low;

    for(int j = low ; j < high ; j++)
    {
        if(students[j].final_grade <= pivot_value)
        {
            swap(&students[i], &students[j]);
            i++;
        }
    }

    swap(&students[i], &students[high]);

    return i;
}

void quicksort_desecending(struct student *students, int student_count)
{
    quicksort_recursion_descending(students, 0, student_count - 1);
}

void quicksort_recursion_descending(struct student *students, int low, int high)
{
    if(low < high)
    {
        int pivot_index = partition_descending(students, low, high);
        quicksort_recursion_descending(students, low, pivot_index - 1);
        quicksort_recursion_descending(students, pivot_index + 1, high);
    }
}

int partition_descending(struct student *students, int low, int high)
{
    int pivot_value = students[high].final_grade;
    
    int i = low;

    for(int j = low ; j < high ; j++)
    {
        if(students[j].final_grade >= pivot_value)
        {
            swap(&students[i], &students[j]);
            i++;
        }
    }

    swap(&students[i], &students[high]);

    return i;
}

void swap(struct student *student_struct1, struct student *student_struct2)
{
    struct student temp = *student_struct1;
    *student_struct1 = *student_struct2;
    *student_struct2 = temp;
}

void quicksort_ascending_name(struct student *students, int student_count)
{
    quicksort_recursion_ascending_name(students, 0, student_count - 1);
}

void quicksort_recursion_ascending_name(struct student *students, int low, int high)
{
    if(low < high)
    {
        int pivot_index = partition_ascending_name(students, low, high);
        quicksort_recursion_ascending_name(students, low, pivot_index - 1);
        quicksort_recursion_ascending_name(students, pivot_index + 1, high);
    }
}

int partition_ascending_name(struct student *students, int low, int high)
{
    char pivot_value = students[high].name[0];

    int i = low;

    for(int j = low ; j < high ; j++)
    {
        if(students[j].name[0] <= pivot_value)
        {
            swap(&students[i], &students[j]);
            i++;
        }
    }

    swap(&students[i], &students[high]);

    return i;
}

void quicksort_descending_name(struct student *students, int student_count)
{
    quicksort_recursion_descending_name(students, 0, student_count - 1);
}

void quicksort_recursion_descending_name(struct student *students, int low, int high)
{
    if(low < high)
    {
        int pivot_index = partition_descending_name(students, low, high);
        quicksort_recursion_descending_name(students, low, pivot_index - 1);
        quicksort_recursion_descending_name(students, pivot_index + 1, high);
    }
}

int partition_descending_name(struct student *students, int low, int high)
{
    char pivot_value = students[high].name[0];

    int i = low;

    for(int j = low ; j < high ; j++)
    {
        if(students[j].name[0] >= pivot_value)
        {
            swap(&students[i], &students[j]);
            i++;
        }
    }

    swap(&students[i], &students[high]);

    return i;
}


void save_changes_to_csv(struct student *students, const char *filename)
{
    char user_input;
    printf("Would you like to save changes? (Y\\N) ");
    scanf(" %c", &user_input);

    if(toupper(user_input) != 'Y' && toupper(user_input) != 'N')
    {
        printf("\n\nPlease enter a valid input: \n");
 
        while(toupper(user_input) != 'Y' && toupper(user_input) != 'N')
        {
            scanf(" %c", &user_input);
        }
    }

    if(toupper(user_input) == 'Y')
    {
        FILE *file = fopen(filename, "w");
        if (file == NULL)
        {
            perror("Error opening file for writing");
            exit(EXIT_FAILURE);
        }

        // re writing content
        for(int j = 0 ; j < student_count ; j++)
        {
            fprintf(file, "%s,%s,%c,%d,%d,%d,%d,%d,%d,%d\n",
                    students[j].name, students[j].email, students[j].gender, students[j].age,
                    students[j].student_id, students[j].grades[0], students[j].grades[1],
                    students[j].grades[2], students[j].grades[3], students[j].grades[4]);        
        }

        fclose(file);
        printf("Changes made sucessfully saved\n");
        getch();
        system("cls");
    }
    else if(toupper(user_input) == 'N')
    {
        system("cls");
        return;
    }
}

void see_basic_statistics(struct student *students, int student_count)
{
    float variance = final_grades_variance(students, student_count);
    float standard_deviation = final_grades_standard_deviation(variance);

    course_average(students, student_count);
    final_grades_median(students, student_count);
    final_grades_mean(students, student_count);

    printf("Final grades variance: %.3f\n", variance);
    printf("Final grades standard deviation: %.3f\n", standard_deviation);
    
    getch();

    system("cls");

}

void course_average(struct student *students, int student_count)
{
    int lag = 0, cal = 0, stat = 0, cpen = 0, alp = 0;

    for(int i = 0 ; i < student_count - 1 ; i++)
    {
        lag += students[i].grades[0];
        cal += students[i].grades[1];
        stat += students[i].grades[2];
        cpen += students[i].grades[3];
        alp += students[i].grades[4];
    }

    lag /= student_count;
    cal /= student_count;
    stat /= student_count;
    cpen /= student_count;
    alp /= student_count;

    printf("Average grades for each courses:\n");
    printf("%-8s%-8s%-8s%-8s%-8s\n","LAG", "CAL", "STAT", "CPEN", "ALP");
    puts("------------------------------------------------");
    printf("%-8d%-8d%-8d%-8d%-8d\n", lag, cal, stat, cpen, alp);
    puts("------------------------------------------------");

}

void final_grades_median(struct student *students, int student_count)
{
    float array_for_calculation[student_count];

    for(int i = 0 ; i < student_count ; i++)
    {
        array_for_calculation[i] = students[i].final_grade;
    }

    quicksort_median_array(array_for_calculation, 0, student_count);

    printf("\n");
    if(student_count % 2 == 0)
    {
        printf("Median final grade: %.3f\n", (array_for_calculation[student_count/2] + array_for_calculation[student_count/ 2 + 1]) / 2);
    }
    else
    {
        printf("Median final grade: %.3f\n", array_for_calculation[student_count / 2 + 1]);
    }
}

void quicksort_median_array(float final_grades_array[], int low, int high)
{
    if(low < high)
    {
        int pivot_index = partition_median_array(final_grades_array, low, high);
        quicksort_median_array(final_grades_array, low, pivot_index - 1);
        quicksort_median_array(final_grades_array, pivot_index + 1, high);
    }

}


int partition_median_array(float final_grades_array[], int low, int high)
{
    int pivot_value = final_grades_array[high];
    
    int i = low;

    for(int j = low ; j < high ; j++)
    {
        if(final_grades_array[j] <= pivot_value)
        {
            swap_array(&final_grades_array[i], &final_grades_array[j]);
            i++;
        }
    }

    swap_array(&final_grades_array[i], &final_grades_array[high]);

    return i;
}

void swap_array(float *array_element1, float *array_element2)
{
    float temp = *array_element1;
    *array_element1 = *array_element2;
    *array_element2 = temp;
}

void final_grades_mean(struct student *students, int student_count)
{
    float mean = 0;

    for(int i = 0 ; i < student_count ; i++)
    {
        mean += students[i].final_grade;
    }

    printf("Mean final grades: %.3f\n", mean / student_count);
}

float final_grades_variance(struct student *students, int student_count)
{
    float mean = 0.0, variance = 0.0;

    // Calculate mean
    for (int i = 0; i < student_count; i++) {
        mean += students[i].final_grade;
    }
    mean /= student_count;

    // Calculate variance
    for (int i = 0; i < student_count; i++) {
        variance += pow(students[i].final_grade - mean, 2);
    }
    variance /= student_count;

    return variance;
}

float final_grades_standard_deviation(float variance)
{
    return sqrt(variance);
}

void visualize_statistics(struct student *students, int student_count)
{   
    int user_input;

    printf("What would you like to visualize?\n");
    printf("1. Distribution of grades\n");
    printf("2. Box and Whisker plot\n");
    printf("3. Age - grade correlation\n");
    printf("4. Gender - grade correlation\n");
    scanf("%d", &user_input);

    switch (user_input)
    {
        case 1:
        grade_distribution(students, student_count);
        system("cls");
        break;

        case 2:
        box_whisker_plot(students, student_count);
        system("cls");
        break;

        case 3:
        scatter_plot_age_vs_final_grade(students, student_count); 
        system("cls");
        break;

        case 4:
        create_gender_comparison_histogram(students, student_count);
        system("cls");
        break;
    }
}

void grade_distribution(struct student *students, int student_count)
{
    FILE *file = fopen("grade_distribution.dat", "w");

    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    // Find the minimum and maximum grades
    float min_grade = students[0].final_grade;
    float max_grade = students[0].final_grade;

    for (int i = 0; i < student_count; i++)
    {
        float grade = students[i].final_grade;

        if (grade < min_grade)
        {
            min_grade = grade;
        }

        if (grade > max_grade)
        {
            max_grade = grade;
        }
    }

    int num_bins = 10;
    float bin_width = (max_grade - min_grade) / num_bins;

    int histogram[10] = {0};

    for (int i = 0; i < student_count; i++)
    {
        float grade = students[i].final_grade;
        int bin = (int)((grade - min_grade) / bin_width);

        if (bin >= 0 && bin < num_bins)
        {
            histogram[bin]++;
        }
    }

    // Write data to file
    for (int i = 0; i < num_bins; i++)
    {
        fprintf(file, "%lf %d\n", min_grade + i * bin_width, histogram[i]);
    }

    fclose(file);

    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set boxwidth %f\n", bin_width * 0.8); // Adjust width based on preference
    fprintf(gnuplotPipe, "set style fill solid\n");
    fprintf(gnuplotPipe, "set xlabel 'Final Grade (0.0 - 4.0)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Frequency'\n");
    fprintf(gnuplotPipe, "set title 'Grade Distribution'\n");
    fprintf(gnuplotPipe, "plot 'grade_distribution.dat' with boxes notitle\n");
    pclose(gnuplotPipe);
}

void box_whisker_plot(struct student *students, int student_count)
{
    FILE *file = fopen("grade_boxplot.dat", "w");

    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    // Write data to file in a format suitable for boxplot
    for (int i = 0; i < student_count; i++)
    {
        for (int j = 0; j < MAX_COURSE_QUANTITY; j++)
        {
            fprintf(file, "%d\t", students[i].grades[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    // Create a file to store course names
    FILE *courseNamesFile = fopen("course_names.dat", "w");

    if (courseNamesFile == NULL)
    {
        fprintf(stderr, "Error opening file for writing course names\n");
        return;
    }

    // Write course names to the file
    fprintf(courseNamesFile, "LAG\nCAL\nSTAT\nCPEN\nALP\n");

    fclose(courseNamesFile);

    // Create box and whisker plot
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set boxwidth 0.5\n");
    fprintf(gnuplotPipe, "set style fill solid\n");
    fprintf(gnuplotPipe, "set style data boxplot\n");
    fprintf(gnuplotPipe, "set xtics ('LAG' 1, 'CAL' 2, 'STAT' 3, 'CPEN' 4, 'ALP' 5) rotate by -45\n");
    fprintf(gnuplotPipe, "set xtics format ''\n");
    fprintf(gnuplotPipe, "set xlabel 'Courses'\n");
    fprintf(gnuplotPipe, "set ylabel 'Grades'\n");
    fprintf(gnuplotPipe, "plot for [i=1:5] 'grade_boxplot.dat' using (i):i notitle\n");
    pclose(gnuplotPipe);
}


void scatter_plot_age_vs_final_grade(struct student *students, int student_count) 
{
    FILE *file = fopen("scatter_age_final_grade.dat", "w");

    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    // Write data to file in a format suitable for scatter plot
    for (int i = 0; i < student_count; i++) {
        fprintf(file, "%d\t%f\n", students[i].age, students[i].final_grade);
    }

    fclose(file);

    // Create scatter plot
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set style data points\n");
    fprintf(gnuplotPipe, "set xlabel 'Age'\n");
    fprintf(gnuplotPipe, "set ylabel 'Final Grade'\n");
    fprintf(gnuplotPipe, "set pointsize 1.5\n");
    fprintf(gnuplotPipe, "set key autotitle columnheader\n");
    fprintf(gnuplotPipe, "plot 'scatter_age_final_grade.dat' using 1:2 with points title 'Data'\n");
    pclose(gnuplotPipe);
}


void create_gender_comparison_histogram(struct student *students, int student_count)
 {
    FILE *file = fopen("gender_comparison_distribution.dat", "w");

    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    // Find the minimum and maximum grades
    float min_grade = students[0].final_grade;
    float max_grade = students[0].final_grade;

    for (int i = 0; i < student_count; i++) {
        float grade = students[i].final_grade;

        if (grade < min_grade) {
            min_grade = grade;
        }

        if (grade > max_grade) {
            max_grade = grade;
        }
    }

    int num_bins = 10;
    float bin_width = (max_grade - min_grade) / num_bins;

    int histogram_male[10] = {0};
    int histogram_female[10] = {0};

    for (int i = 0; i < student_count; i++) {
        float grade = students[i].final_grade;
        int bin = (int)((grade - min_grade) / bin_width);

        if (bin >= 0 && bin < num_bins) {
            if (students[i].gender == 'M') {
                histogram_male[bin]++;
            } else if (students[i].gender == 'F') {
                histogram_female[bin]++;
            }
        }
    }

    // Write data to file
    for (int i = 0; i < num_bins; i++) {
        fprintf(file, "%lf %d %d\n", min_grade + i * bin_width, histogram_male[i], histogram_female[i]);
    }

    fclose(file);

    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set boxwidth %f\n", bin_width * 0.8); // Adjust width based on preference
    fprintf(gnuplotPipe, "set style fill solid\n");
    fprintf(gnuplotPipe, "set xlabel 'Final Grade (0.0 - 4.0)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Frequency'\n");
    fprintf(gnuplotPipe, "set title 'Gender Comparison of Grade Distribution'\n");
    fprintf(gnuplotPipe, "plot 'gender_comparison_distribution.dat' using 1:2 with boxes title 'Male', '' using 1:3 with boxes title 'Female'\n");
    pclose(gnuplotPipe);
}

void exit_program(void)
{

    system("cls");
    int i;
    char ThankYou[100]     = "       ========= Thank You =========\n";
    char SeeYouSoon[100]   = " ========= Advanced C Final Project =========\n";
    for(i=0; i<strlen(ThankYou); i++)
    {
        printf("%c",ThankYou[i]);
        Sleep(40);
    }
    for(i=0; i<strlen(SeeYouSoon); i++)
    {
        printf("%c",SeeYouSoon[i]);
        Sleep(40);
    }
    exit(EXIT_SUCCESS);
}
