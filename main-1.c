#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *malloc(size_t size);

typedef struct student
{
  int id;
  char *lname;
  float *scores;
  float std_avg;
} student;

typedef struct course
{
  char *course_name;
  int num_sections;
  student **sections;
  int *num_students;
  int *num_scores;
} course;

student **read_sections(FILE *inputFile, int num_students[], int num_scores[], int num_sections)
{
  student** studentArray = malloc(num_sections*sizeof(student*));

  for(int x = 0; x < num_sections; x++)
  {

    fscanf(inputFile, "%d %d", &num_students[x], &num_scores[x]);

    studentArray[x] = malloc(num_students[x]*sizeof(student));

    for(int y = 0; y < num_students[x]; y++)
    {
      float totalPoints = 0;
      studentArray[x][y].lname = malloc(20*sizeof(char));
      fscanf(inputFile, "%d %s", &studentArray[x][y].id, studentArray[x][y].lname);

      studentArray[x][y].scores = malloc(num_scores[x] * sizeof(float));
      for(int z = 0; z < num_scores[x]; z++)
      {
        fscanf(inputFile, "%f", &studentArray[x][y].scores[z]);
        totalPoints += studentArray[x][y].scores[z];
      }
      studentArray[x][y].std_avg = totalPoints/(float)num_scores[x];
    }
  }
  return studentArray;
}

course *read_courses(FILE *inputFile, int *num_courses)
{
  course *temp = malloc(*num_courses * sizeof(course));
  for(int x = 0; x < *num_courses; x++)
  {
    temp[x].course_name = malloc(20*sizeof(char));
    fscanf(inputFile, "%s", temp[x].course_name);
    fscanf(inputFile, "%d", &temp[x].num_sections);

    temp[x].num_students = malloc(temp[x].num_sections * sizeof(int));
    temp[x].num_scores = malloc(temp[x].num_sections * sizeof(int));

    temp[x].sections = read_sections(inputFile, temp[x].num_students, temp[x].num_scores, temp[x].num_sections);
  }
  return temp;
  free(temp);
  for(int x = 0; x < *num_courses; x++)
  {
    free(temp[x].course_name);
    free(temp[x].num_students);
    free(temp[x].num_scores);
  }
}

void process_courses(course *courses, int num_courses)
{
  for(int x = 0; x < num_courses; x++)
  {
    int passingCount = 0;
    student best;
    int check = 0;
    float *sectionavg = malloc(courses[x].num_sections*sizeof(float));
    printf("\n%s ", courses[x].course_name);
    for(int y = 0; y < courses[x].num_sections; y++)
    {
      float sectionTotal = 0;
      for(int z = 0; z < courses[x].num_students[y]; z++)
      {
        student temp = courses[x].sections[y][z];
        sectionTotal += temp.std_avg;
        if(temp.std_avg >= 70)
        {
          passingCount++;
        }
        if(check == 0)
        {
          best = temp;
          check = 1;
        }
        else if(best.std_avg < temp.std_avg)
        {
          best = temp;
        }
      }
      sectionavg[y] = sectionTotal/(float)courses[x].num_students[y];
    }
    printf("%d ", passingCount);
    for(int y = 0; y < courses[x].num_sections; y++)
    {
      printf("%.2f ", sectionavg[y]);
    }
    printf("%d %s %.2f", best.id, best.lname, best.std_avg);
    free(sectionavg);
  }
}

void release_courses(course *courses, int num_courses)
{
  for(int x = 0; x < num_courses; x++)
  {
    for(int y = 0; y < courses[x].num_sections; y++)
    {
      for(int z = 0; z < courses[x].num_students[y]; z++)
      {
        free(courses[x].sections[y][z].lname);
        free(courses[x].sections[y][z].scores);
      }
      free(courses[x].sections[y]);
    }
    free(courses[x].num_scores);
    free(courses[x].num_students);
    free(courses[x].sections);
    free(courses[x].course_name);
  }
  free(courses);
}

int main(void)
{
  FILE *inputFile = fopen("assignment1input.txt", "r");

  int num_testcases = 0;
  fscanf(inputFile, "%d", &num_testcases);

  for(int x = 1; x <= num_testcases; x++)
  {
    int num_courses = 0;
    fscanf(inputFile, "%d", &num_courses);
    course *temp = read_courses(inputFile, &num_courses);
    printf("test case %d", x);
    process_courses(temp, num_courses);
    printf("\n\n");
    release_courses(temp, num_courses);
  }
  return 0;
}