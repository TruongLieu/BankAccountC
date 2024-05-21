#include "user.h"

static USER userList[MAX_USERS];
static char num_user = 0;

/* Log error with the time, username and text message to a file */
static void logError(char* username, char* text)
{
  FILE *INPUT;
  INPUT = fopen("logfile","a");
  if (INPUT == NULL) {
      perror("Failed to open logfile");
      return;
  }
  fseek(INPUT,0,EOF);

  time_t t; // time variable
  struct tm *tmp; // poiter to time type

  t = time(&t); // get the current time (s)
  tmp = localtime(&t); // Convert to day month time year

  fprintf(INPUT,"%s", asctime(tmp)); // asctime to covert time type to string
  fprintf(INPUT,"User: %s %s\n",username,text);
  fclose(INPUT);
}

/* Returns user amount. */
float getUserAmt(unsigned userIdx)
{
  return userList[userIdx].amt;
}

// Check the input is a float number
bool isValidFloat(const char *input)
{
    bool hasDot = false; // '.' check variable
    for (int i = ZERO; input[i] != '\0'; i++)
    {
        if (input[i] == '.')
        {
            // // Is '.' appear before ?
            if (hasDot)
            {
                return false; // a float cant have have 2 '.'
            }
            hasDot = true; // check had '.'
        }
        else if (!isdigit(input[i]))
        {
            return false; // return false if the character is not digits or '.'
        }
    }
    return true;
}

/* Returns true if user exists and false otherwise. */
bool userExists(char* username,char *userIdDetect)
{
  if (IS_NULL(username))
  {
    logError("System","Null poiter");
  }
  for (char userIdx = ZERO; userIdx < num_user; userIdx++)
  {
    if (my_strcmp(userList[userIdx].username , username))
    {
      *userIdDetect = userIdx;
      return true;
    }
  }
  return false;
}

/* Allow user to deposit money from account to account */
void transfermoney(unsigned userIdx)
{
    char targetUsername[MAX_SIZE];
    char targetUserIdx = -1;
    do
    {
        printf("Enter the username to transfer money to: ");
        scanf("%s", targetUsername);
        if (!userExists(targetUsername , &targetUserIdx))
        {
            printf("Username does not exist! Please enter a valid username.\n");
        }
    } while (targetUserIdx == -1);
    
    char input[MAX_SIZE];
    float amt;
    do
    {
        printf("Enter the amount to transfer: ");
        scanf("%s", input);
        if (!isValidFloat(input))
        {
            printf("Invalid input! Please enter a valid number.\n");
            continue;
        }
        amt = atof(input);
        if (getUserAmt(userIdx) < amt)
        {
            printf("Insufficient funds! Current balance: %.2f\n", getUserAmt(userIdx));
            continue;
        }
        break;
    } while (true);

    // Withdraw from the transferring user's account
    withdrawn(userIdx, amt);
    // Deposit into the target user's account
    deposit(targetUserIdx, amt);
    system("CLS");
    printf("Transfer successful. You have transferred %.2f to %s.\n", amt, targetUsername);
    getch();
}

/* Allow user to deposit money */
void deposit(unsigned userIdx, float amt) 
{
    userList[userIdx].amt += amt;
    system("CLS");
    printf("Deposit successful. New balance: %.2f\n", userList[userIdx].amt);
}

/* Allow user to withdrawn money */
void withdrawn(unsigned userIdx, float amt) 
{
    userList[userIdx].amt -= amt;
    system("CLS");
    printf("Withdrawn successful. New balance: %.2f\n", userList[userIdx].amt);
}

/* Display user information */
void display(unsigned userIdx)
{
	printf("Usename: %s\n",userList[userIdx].username);
  printf("Name: %s %s\n",userList[userIdx].firstname,userList[userIdx].lastname);
  printf("Address: %s\n",userList[userIdx].address);
  printf("Phone: %s\n",userList[userIdx].phone);
  printf("Amount: %f\n",userList[userIdx].amt);
  printf("Date of Birth: %02d-%02d-%04d\n", userList[userIdx].dob.day, userList[userIdx].dob.month, userList[userIdx].dob.year);
  getch();
}

// return true if s1 = s2 and false if s1 != s2
bool my_strcmp(char* s1 , char* s2)
{
  if (strlen(s1)==strlen(s2))
  {
    char maxlen=strlen(s1);
    for (char strIndex = ZERO; strIndex < maxlen; strIndex++)
    {
      if (s1[strIndex]!=s2[strIndex]) return false;
    }
    return true;
  }
  return false;
}

/* Returns true if the provided username and the provided password are correct for that user, 
   returns false otherwise. */
bool authenticateUser(char* username, char* pwd, unsigned char* userIdx_detect)
{
  if (IS_NULL(username)||IS_NULL(pwd))
  {
    logError("System","Null poiter");
  }
  for (int userIdx = ZERO; userIdx < num_user; userIdx++)
  {
    if (my_strcmp(userList[userIdx].username , username))
    {
      if(my_strcmp(userList[userIdx].pwd , pwd))
      {
        *userIdx_detect = userIdx;
        return true;
      }
      else
      {
        logError(username,"!!! Wrong Password");
        return false;
      }
    }
  }
  logError(username,"!!! User Does Not Exist");
  return false;
}


/* Adds a new user with the provided `username' and the provided password `pwd'
   Returns USER_SUCCESSFULLY_ADDED on success, USER_ALREADY_EXISTED
   in case provided username already exists and USER_ADMINISTRATION_FULL
   in case no more user can be added. */
bool addUser(USER *user)
{
  if (IS_NULL(user))
  {
    logError("System","Null poiter");
  }
  if (num_user==MAX_SIZE)
  {
    logError(user->username,"!!! Over Flow");
    return false;
  }
  num_user++;
  strcpy(userList[num_user-INDEX_ARR].username, user->username);
  strcpy(userList[num_user-INDEX_ARR].pwd, user->pwd);
  strcpy(userList[num_user-INDEX_ARR].firstname, user->firstname);
  strcpy(userList[num_user-INDEX_ARR].lastname, user->lastname);
  strcpy(userList[num_user-INDEX_ARR].address, user->address);
  strcpy(userList[num_user-INDEX_ARR].phone, user->phone);
  userList[num_user-INDEX_ARR].amt = user->amt;
  userList[num_user-INDEX_ARR].dob.day = user -> dob.day;
  userList[num_user-INDEX_ARR].dob.month = user -> dob.month;
  userList[num_user-INDEX_ARR].dob.year = user -> dob.year;
  return true;
}