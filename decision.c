#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


double calculate(double x, double y, char operation)
{
  double res;
  switch (operation) // plus
  {
  case '+':
  {
    res = x + y;
    break;
  }
  case '-': // minus
  {
    res = x - y;
    break;
  }
  case '/': // divide
  {
    if (y != 0)
      res = x / y; // Checking that y is not equal to 0.
    else
      res = 0;
    break;
  }
  case '*': // multiplication
  {
    res = x * y;
    break;
  }
  case '^': // exponent 
  {
    res = 1;
    for (int i = 0; i < y; i++)
      res = res * x; 
    break;
  }
  case '!': // factorial 
  {
    res = 1;
    if (y >= 0)
      for (int i = y; i != 1; i--)
        res = res * i; 
    else
      res = 0;
    break;
  }
  }
  return res;
}

int main(int argc, char *argv[])
{
  int fd[2]; // file descriptors init
  // fd[0] - read
  // fd[1] - write

  if (pipe(fd) == -1)
  {
    printf("Error opening to pipe\n"); 
    return 1;
  }

  int pid = fork();
  if (pid == -1)
  {
    printf("Error with fork\n");
    return 2;
  }
  if (pid == 0)
  {
    close(fd[0]);
    double a, b;
    char operation;

    printf("Input a first number: ");
    scanf("%lf", &a);

    printf("Input a second number: ");
    scanf("%lf", &b);

    printf("Input an operation: ");
    scanf(" %c", &operation);

    if (write(fd[1], &a, sizeof(double)) == -1)
    {
      printf("Error writing to pipe\n");
      return 3;
    }

    if (write(fd[1], &b, sizeof(double)) == -1)
    {
      printf("Error writing to pipe\n");
      return 4;
    }

    if (write(fd[1], &operation, sizeof(char)) == -1)
    {
      printf("Error writing to pipe\n");
      return 5;
    }

    close(fd[1]);
  }
  else
  {

    close(fd[1]);

    double x, y;
    char recieved_operation;

    if (read(fd[0], &x, sizeof(double)) == -1)
    {
      printf("Error reading from pipe\n");
      return 6;
    }

    if (read(fd[0], &y, sizeof(double)) == -1)
    {
      printf("Error reaing from pipe\n");
      return 6;
    }

    if (read(fd[0], &recieved_operation, sizeof(char)) == -1)
    {
      printf("Error reading from pipe\n");
      return 6;
    }

    close(fd[0]);
    printf("%.2lf %c %.2lf = %.2lf\n", x, recieved_operation, y, calculate(x, y, recieved_operation));
  }
}