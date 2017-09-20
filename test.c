#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cpy_str(char *tar, const char *ori){
  char c;
  int i = 0;
  while ((c=ori[i])!='\0'){
    tar[i]=c;
    i++;
  }
  tar[i] = '\0';

  return;
}

char* reach_input(char** input, int argument)
{
	char* goal = malloc(sizeof(char));
	char* given = *input;
	char walker;
	int traveler = 0;
	int counter = 0;
	while (counter<argument)
	{
		if (given[traveler] == ' ')
		{
			counter++;
		}
		else if (given[traveler] == '\0')
		{
			return NULL;
		}

		traveler++;

		if (counter == argument)
		{
			break;
		}
	}

	counter = 0;
	while (given[traveler] != ' ')
	{
		if (given[traveler] == '\0')
		{
			goal[counter] = '\0';
			break;
		}
		goal[counter] = given[traveler];
		counter++; traveler++;
		goal = realloc(goal, sizeof(char)*(counter)+1);
	}

	return goal;
}

int main(){
	char* arreglo = malloc(sizeof(char)*12);
	arreglo = "init 10 2";
	char* arg = reach_input(&arreglo, 0);
	printf("%s\n", arg);

	return 0;
}