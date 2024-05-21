#ifndef USER_H
#define USER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>

#define IS_NULL(ptr) ((ptr) == NULL)
#define MAX_USERS 50
#define MAX_SIZE       50
#define ZERO           0
#define SHORTEST_PWD   6
#define LEAP           4
#define SPECIAL_YEAR1  100
#define SPECIAL_YEAR2  400
#define SPECIAL_MONTH  2
#define DAY29          29
#define LEAST_MONTH    1
#define MAX_MONTH      12
#define LEAST_DAY      1
#define INDEX_ARR      1

#define ENTER_CHAR     13
#define BACKSPACE_CHAR 8

#define SHOW_SIGNIN        1
#define LOGIN_TRANSACTION  2
#define TRANFER_EXIT       3
#define LOG_OUT            4
#define EXIT_OP            5

typedef enum
{
  USER_SUCCESSFULLY_ADDED,
  USER_ALREADY_EXISTED,
  USER_ADMINISTRATION_FULL
} ERROR_CODE;

struct date
{
  int month, day, year;
};

typedef struct
{
  unsigned char username[MAX_SIZE];
  unsigned char pwd[MAX_SIZE];
  unsigned char address[MAX_SIZE];
  unsigned char phone[MAX_SIZE];
  unsigned char firstname[MAX_SIZE];
  unsigned char lastname[MAX_SIZE];
  float amt;
  struct date dob;
  struct date deposit;
  struct date withdraw;
} USER;

float getUserAmt(unsigned userIdx);

bool isValidFloat(const char *input);

bool userExists(char* username,char *userIdDetect);

void transfermoney(unsigned userIdx);

void deposit(unsigned userIdx, float amt);

void withdrawn(unsigned userIdx, float amt);

void display(unsigned userIdx);

bool my_strcmp(char* s1 , char* s2);

bool authenticateUser(char* username, char* pwd, unsigned char* userIdx);

bool addUser(USER *user);

#endif

