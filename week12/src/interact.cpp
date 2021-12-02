/***********************************************************************
 * COMPONENT:
 *    INTERACT
 * Author:
 *    Br. Helfrich, <your name here if you made a change>
 * Summary:
 *    This class allows one user to interact with the system
 ************************************************************************/

#include <iostream>   // standard input and output
#include <string>     // for convenience
#include <cassert>    // because I am paraniod
#include <stdlib.h>   // for atoi 
#include "messages.h" // to interact with the collection of messages
#include "control.h"  // all the Bell-LaPadula stuff
#include "interact.h" // the interact class and User structure

using namespace std;

/**************************************************************
 * USERS
 * All the users currently in the system
 *************************************************************/
const User users[] =
{
   { "AdmiralAbe",     "password", SECRET },  
   { "CaptainCharlie", "password", PRIVILEGED }, 
   { "SeamanSam",      "password", CONFIDENTIAL },
   { "SeamanSue",      "password", CONFIDENTIAL },
   { "SeamanSly",      "password", CONFIDENTIAL }
};

const int ID_INVALID = -1;

/****************************************************
 * INTERACT constructor
 * Authenticat ethe user and get him/her all set up
 ***************************************************/
Interact::Interact(const string & userName,
                   const string & password,
                   Messages & messages,
                   std::istream * in)
{
   authenticate(userName, password);
   this->userName = userName;
   this->pMessages = &messages;
   this->in = in;
}

/****************************************************
 * INTERACT :: SHOW
 * show a single message
 ****************************************************/
void Interact::show() const
{
   Control control = controlFromUser(userName);
   pMessages->show(control,
                   promptForId("display"));
}
   
/****************************************************
 * INTERACT :: DISPLAY
 * display the set of messages
 ***************************************************/
void Interact::display() const
{
   Control control = controlFromUser(userName);
   pMessages->display(control);
}

/****************************************************
 * INTERACT :: ADD
 * add a single message
 ****************************************************/
void Interact::add()
{
   Control control = controlFromUser(userName);
   pMessages->add(controlToString(control),
                  promptForLine("message"),
                  userName,
                  promptForLine("date"));
}

/****************************************************
 * INTERACT :: UDPATE
 * update a single message
 ****************************************************/
void Interact::update()
{
   Control control = controlFromUser(userName);
   int id = promptForId("update");
   pMessages->update(control,
                     id,
                     promptForLine("message"));
}

/****************************************************
 * INTERACT :: REMOVE
 * remove one message from the list
 ***************************************************/
void Interact::remove()
{
   Control control = controlFromUser(userName);
   pMessages->remove(control,
                     promptForId("delete"));
}

/****************************************************
 * INTERACT :: DISPLAY USERS
 * display the set of users in the system
 ***************************************************/
void displayUsers()
{
   for (int idUser = 0; idUser < sizeof(users) / sizeof(users[0]); idUser++)
      cout << "\t" << users[idUser].name << endl;
}
   
/****************************************************
 * INTERACT :: PROMPT FOR LINE
 * prompt for a line of input
 ****************************************************/
string Interact::promptForLine(const char * verb) const
{
   string line;
   cout << "Please provide a " << verb << ": ";
   getline(*in, line);
   return line;
}

/****************************************************
 * INTERACT :: PROMPT FOR ID
 * prompt for a message ID
 ***************************************************/
int Interact::promptForId(const char * verb) const
{
   string id;
   cout << "Select the message ID to " << verb << ": ";
   *in  >> id;
   in->ignore();
   return atoi(id.c_str());
}

/****************************************************
 * INTERACT :: AUTHENTICATION
 * authenticate the user: find their control level
 ****************************************************/
void Interact::authenticate(const string & userName,
                            const string & password)
{
   int id = idFromUser(userName);
   authenticated = false;
   if (ID_INVALID != id && password == string(users[id].password))
      authenticated = true;
}

/****************************************************
 * INTERACT :: ID FROM USER
 * Find the ID of a given user
 ****************************************************/
int Interact::idFromUser(const string & userName) const
{
   for (int idUser = 0; idUser < sizeof(users) / sizeof(users[0]); idUser++)
      if (userName == string(users[idUser].name))
         return idUser;
   return ID_INVALID;
}

/****************************************************
 * INTERACT :: CONTROL FROM USER
 * Find the Control of a given user
 ****************************************************/
Control Interact::controlFromUser(const string & userName) const
{
   Control control = PUBLIC;
   if (authenticated) {
      int id = idFromUser(userName);
      control = users[id].control;
   }
   return control;
}