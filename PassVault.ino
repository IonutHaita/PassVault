#include <BluetoothSerial.h>

#define MAX_ADMINS 1
#define MAX_PASSWORDS 256

struct Admin 
{
  String adminUsername;
  String adminPassword;
};

struct Passwords
{
  String site;
  String username;
  String password;
};

Admin admin[MAX_ADMINS];
Passwords password[MAX_PASSWORDS];

int currentUserIndex = -1;

BluetoothSerial SerialBT;


void initializeAdmins() 
{
  admin[0] = {"Vault", "12345"};
}

void initializePasswords()
{
  password[0] = {"Facebook", "Vault", "12345"};
}

int cnp = 1; //This variable keeps track on the number of passwords used. It in used in registerPassword()

void preMenu()
{
  SerialBT.println("======================");
  delay(100);
  SerialBT.println("==VAULT 1.0 IS HERE!!====");
  delay(100);
  SerialBT.println("==WELCOME AND ENJOY!=");
  delay(100);
  SerialBT.println("======================");
  delay(100);
  mainMenu();
}
void mainMenu() 
{

  initializeAdmins();
  initializePasswords();
  SerialBT.print('\n');
  SerialBT.print('\n');
  SerialBT.println("====== Main Menu ======");
  SerialBT.println("1. User Login");
  SerialBT.println("======================");
  SerialBT.println("Enter option number: ");
  while (1) 
  {
    if (SerialBT.available()) 
    {
      int option = SerialBT.parseInt();
      SerialBT.read(); 
      switch (option) 
      {
      case 1:
      userLogin();
      break;
      default:
      SerialBT.println("Invalid option. Try again!");
      mainMenu();
      }
    }
  }
}



void userLogin() 
{
  if (currentUserIndex == -1) 
  {
    delay(300);
    SerialBT.print('\n');
    SerialBT.print('\n');
    SerialBT.println("====== User Login ======");
    SerialBT.println("Enter username: ");
    while (!SerialBT.available()) {delay(5);}
    String username = SerialBT.readStringUntil('\n');
    username.trim();
    delay(300);
    SerialBT.println("Enter password: ");
    while (!SerialBT.available()) {delay(5);}
    String password = SerialBT.readStringUntil('\n');
    password.trim();
    delay(300);

    for (int i = 0; i < MAX_ADMINS; i++)
    {
      if (admin[i].adminUsername == username && admin[i].adminPassword == password) 
      {
        currentUserIndex = i;
        SerialBT.println("Login successful.");
        loggedInMenu();
      }
    }

    if (currentUserIndex == -1) 
    {
      SerialBT.println("Invalid username or password. Please try again.");
      mainMenu();
    } 
    else 
    {
      loggedInMenu();
    }

  } 
  else 
  {
    SerialBT.println("User is already logged in. Please log out before logging in as a different user.");
    loggedInMenu();
  }
}

void loggedInMenu() 
{
  SerialBT.print('\n');
  SerialBT.print('\n');
  SerialBT.println("===== Logged In Menu =====");
  SerialBT.println("1. Display a stored password");
  SerialBT.println("2. Register a new password");
  SerialBT.println("3. Log out current user");
  SerialBT.println("========================");
  SerialBT.println("Enter option number: ");
  while (1) 
    {
      if (SerialBT.available()) 
      {
        delay(300);
        int option = SerialBT.parseInt();
        SerialBT.read();
        switch (option) 
        {
          case 1:
            delay(300);
            displayPassword();
            break;
          case 2:
            delay(300);
            registerPassword();
            break;
          case 3:
            delay(300);
            logoutUser();
            break;
          default:
            SerialBT.println("Invalid option. Try again.");
            }
          break;
      }
    }
}

void displayPassword() 
{
  SerialBT.print('\n');
  SerialBT.print('\n');
  SerialBT.println("Enter the site to display a password: ");
  delay(300);
  SerialBT.readStringUntil('\n');
  SerialBT.flush();
  while (!SerialBT.available()) {delay(5);}
  String site = SerialBT.readStringUntil('\n');
  site.trim();

  for (int i = 0; i <MAX_PASSWORDS; i++) 
  {
    if (password[i].site == site) 
    {
      SerialBT.print("Site: ");
      SerialBT.println(password[i].site);
      SerialBT.print("Username: ");
      SerialBT.println(password[i].username);
      SerialBT.print("Password: ");
      SerialBT.println(password[i].password);
      loggedInMenu();
      return;
    }
  }
  SerialBT.println("Site not found in the current user's vault.");
  loggedInMenu();
}

void registerPassword() 
{
  SerialBT.print('\n');
  SerialBT.print('\n');
  SerialBT.println("===== Register New Password =====");
  delay(300);
  if (currentUserIndex != -1) 
  {
    for(int i = cnp + 1; i < MAX_PASSWORDS; i++) 
    {
      SerialBT.println("Enter site: ");
      SerialBT.flush();
      while (!SerialBT.available()) {delay(5);}
      password[cnp].site = SerialBT.readStringUntil('\n');
      password[cnp].site.trim();
      delay(300);
      SerialBT.println("Enter username: ");
      SerialBT.flush();
      while (!SerialBT.available()) {delay(5);}
      password[cnp].username = SerialBT.readStringUntil('\n');
      password[cnp].username.trim();
      delay(300);
      SerialBT.println("Enter password: ");
      SerialBT.flush();
      while (!SerialBT.available()) {delay(5);}
      password[cnp].password = SerialBT.readStringUntil('\n');
      password[cnp].password.trim();
      delay(300);
      SerialBT.println("Password registered successfully.");
      loggedInMenu();
    } 
  }
  else 
  {
    SerialBT.println("No Admin is currently logged in. Please log in first!");
    mainMenu();
  }
}

void logoutUser() 
{
  currentUserIndex = -1;
  SerialBT.println("Admin logged out successfully.");
  mainMenu();
}

void setup() 
{
  SerialBT.begin("BT_Headset");// Set the name of the BT device here
  SerialBT.setPin("1243");       
}

void loop() 
{
  while(!SerialBT.available())
  {
    SerialBT.println("Send any character to begin");
    delay(2500);
  }
  SerialBT.parseInt();
  SerialBT.read();
  preMenu();
}