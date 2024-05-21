#include "user.c"

USER userLogin;

// Function to get the password and hint it when get the input
void getPassword(char *password, int maxLength)
{
    int i = ZERO;
    char ch;

    while (true)
    {
        ch = getch();

        // Enter to out
        if (ch == ENTER_CHAR)
        {
            break;
        }

        // Delete action
        if (ch == BACKSPACE_CHAR && i > ZERO)
        {
            printf("\b \b"); // Clear character
            i--;
            continue;
        }

        // Save and show "*"
        if ((i < maxLength - INDEX_ARR) && (ch != BACKSPACE_CHAR))
        {
            password[i++] = ch;
            printf("*");
        }
    }

    password[i] = '\0'; // add the end character
}

// Check password is more than 5 character include digit and aphabet
bool isValidPassword(const char *password)
{
    if (strlen(password) < SHORTEST_PWD)
    {
        return false;
    }

    bool hasLetter = false;
    bool hasDigit = false;

    for (int i = ZERO; i < strlen(password); i++)
    {
        if (isalpha(password[i]))
        {
            hasLetter = true;
        }
        if (isdigit(password[i]))
        {
            hasDigit = true;
        }
    }

    return hasLetter && hasDigit;
}

// Check phone is all of digit
bool isValidPhone(const char *phone)
{
    for (int i = ZERO; i < strlen(phone); i++)
    {
        if (!isdigit(phone[i]))
        {
            return false;
        }
    }
    return true;
}

// function to take the valid Amount
float enterAmount()
{
    char input[MAX_SIZE];
    float amount;

    do
    {
        printf("Enter amount: ");
        scanf("%s", input);

        // check Is the number valid?
        if (!isValidFloat(input))
        {
            printf("Invalid amount! Please enter a number.\n");
        }
        else
        {
            // If number valid, convert to float and assign it for amount
            amount = atof(input);
            break; // exit funtion
        }
    } while (true);
    return amount;
}

// Function to check if a year is a leap year
bool isLeapYear(int year)
{
    if ((year % LEAP == ZERO && year % SPECIAL_YEAR1 != ZERO) || (year % SPECIAL_YEAR2 == ZERO))
    {
        return true; // Leap year
    }
    return false; // Not a leap year
}

// Function to check if a date is valid
bool isValidDate(struct date dob)
{
    // Array to store the number of days in each month
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Check for leap year
    if (dob.month == SPECIAL_MONTH && isLeapYear(dob.year))
    {
        daysInMonth[SPECIAL_MONTH] = DAY29; // February has 29 days in a leap year
    }

    // Check if day, month, and year are within valid range
    if (dob.month < LEAST_MONTH || dob.month > MAX_MONTH || dob.day < LEAST_DAY || dob.day > daysInMonth[dob.month])
    {
        return false; // Invalid date
    }

    return true; // Valid date
}

// Function to allow user to enter date
struct date enterDOB()
{
    struct date dob; // Variable to store DOB

    // Loop until a valid date is entered
    do
    {
        printf("Enter date of birth (DD MM YYYY): ");
        scanf("%d %d %d", &dob.day, &dob.month, &dob.year);

        // Check if the entered date is valid
        if (!isValidDate(dob))
        {
            printf("Invalid date! Please enter a valid date.\n");
        }
        else
        {
            break; // Exit loop if valid date is entered
        }
    } while (true);

    return dob;
}

/* Allow user to enter username and password then add user,
   print USER SUCCESSFULLY ADDED if added successfully, USER ALREADY EXISTED if already existed
   and USER ALREADY FULL if no more space for new user */
void account(void)
{
    USER useradd;
    bool IsExist = false;
    bool isPasswordValid = true;
    char passwordConfirm[MAX_SIZE];
    char userIdx;
    do
    {
        if (IsExist)
            printf("!!!Username Exist\n");
        printf("Enter username: ");
        scanf("%s", &useradd.username);
        IsExist = userExists(useradd.username,&userIdx);
    } while (IsExist);

    do
    {
        if (!isPasswordValid)
        {
            printf("!!!Password must be longer than 5 characters and contain both letters and numbers.\n");
        }

        bool passwordsMatch = false;
        do
        {
            printf("Enter password: ");
            getPassword(useradd.pwd, MAX_SIZE);
            printf("\n");

            printf("Confirm password: ");
            getPassword(passwordConfirm, MAX_SIZE);
            printf("\n");

            if (!my_strcmp(useradd.pwd, passwordConfirm))
            {
                printf("Passwords do not match. Try again.\n");
            }
            else
            {
                passwordsMatch = true;
            }
        } while (!passwordsMatch);
        isPasswordValid = isValidPassword(useradd.pwd);
    } while (!isPasswordValid);

    printf("Enter first name: ");
    scanf("%s", &useradd.firstname);

    printf("Enter last name: ");
    scanf("%s", &useradd.lastname);

    printf("Enter address: ");
    scanf("%s", &useradd.address);

    bool isPhoneValid = false;
    do
    {
        printf("Enter phone: ");
        scanf("%s", useradd.phone);
        isPhoneValid = isValidPhone(useradd.phone);
        if (!isPhoneValid)
        {
            printf("!!!Phone number must contain only digits.\n");
        }
    } while (!isPhoneValid);

    useradd.amt = enterAmount();

    // Get date of birth from user
    useradd.dob = enterDOB();

    printf("!!! Create %s", addUser(&useradd) ? "Success" : "Fail"); // Assuming add_user is defined in user.c
    getch();                                                         // Wait for user to press a key
}

/* Allow user to enter username and password then authenticate user,
   print LOGIN SUCCESSFUL if login successfully, otherwise print LOGIN FAILED */
bool login(unsigned char *userIdx)
{
    char usename[MAX_SIZE];
    char password[MAX_SIZE];
    bool IsLogin = false;

    printf("Enter username: ");
    scanf("%s", &usename);

    printf("Enter password: ");
    // scanf("%s", &password);
    getPassword(password, MAX_SIZE);
    printf("\n");

    IsLogin = authenticateUser(usename, password, userIdx);
    printf("!!! Login %s", IsLogin ? "Success" : "Fail");
    getch(); // Wait for user to press a key
    return IsLogin;
}

/* Allow user to deposit or withdrawn money from his/her account */
void transaction(unsigned userIdx)
{
    int choice;
    char input[100];
    float amt;
    do
    {
        printf("1. Deposit\n");
        printf("2. Withdrawn\n");
        printf("3. Return\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            do
            {
                printf("Enter amount to deposit: ");
                scanf("%s", input);
                if (!isValidFloat(input))
                {
                    printf("Invalid input! Please enter a valid number.\n");
                    continue;
                }
                amt = atof(input);
                if (amt <= 0)
                {
                    printf("Invalid amount! The amount must be greater than 0.\n");
                    continue;
                }
                break;
            } while (true);
            deposit(userIdx, amt);
            break;

        case 2:
            do
            {
                printf("Enter amount to withdrawn: ");
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
            withdrawn(userIdx, amt);
            break;

        case 3:
            break;

        default:
            system("CLS");
            printf("Invalid choice!\n");
            break;
        }
    } while (choice != 3);
}

int main()
{
    int choice;
    bool auth = false;
    unsigned char userIdx;

    while (true)
    {
        system("cls");
        printf("\nWELCOME TO BANK ACCOUNT SYSTEM\n\n");
        printf("**********************************\n");

        if (!auth)
        {
            printf("1.CREATE A BANK ACCOUNT\n");
            printf("2.SIGN IN\n");
            printf("3.EXIT\n\n");
        }
        else
        {
            printf("1.DISPLAY INFORMATION\n");
            printf("2.DEPOSIT OR WITHDRAWN\n");
            printf("3.TRANSFER MONEY\n");
            printf("4.LOG OUT\n");
            printf("5.EXIT\n\n");
        }

        printf("\n\nENTER YOUR CHOICE..");
        scanf("%d", &choice);

        switch (choice)
        {
        case SHOW_SIGNIN:
            system("cls");
            if (auth)
            {
                display(userIdx);
            }
            else
            {
                account();
            }
            break;

        case LOGIN_TRANSACTION:
            system("cls");
            if (!auth)
            {
                auth = login(&userIdx);
            }
            else
            {
                transaction(userIdx);
            }
            break;

        case TRANFER_EXIT:
            system("cls");
            if (auth)
            {
                transfermoney(userIdx);
            }
            else
            {
                exit(ZERO);
            }
            break;

        case 4:
            if (auth)
            {
                system("cls");
                auth = false;
            }
            break;

        case EXIT_OP:
            if (auth)
            {
                exit(ZERO);
            }
        }
    }
    return 0;
}