#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>

#include "include/catch.hpp"
#include "include/stream-redirect.hpp"

#include "interact.h"
#include "messages.h"

const char * MESSAGES_FILE_NAME = "data/messages.txt";
const char * TEST_MESSAGES_DIR  = "data/test";
const char * TEST_MESSAGES_FILE = "data/test/messages.txt";

namespace fs = std::filesystem;
using namespace std;

#define CLEAR_INPUT in.str(""); in.clear();
#define CLEAR_OUTPUT out.str(""); out.clear();
#define CLEAR_IN_OUT CLEAR_INPUT CLEAR_OUTPUT;

TEST_CASE("Messages", "[messages]")
{
  // Make a copy of the data file to work with
  fs::create_directories(TEST_MESSAGES_DIR);
  fs::remove(TEST_MESSAGES_FILE);
  fs::copy(MESSAGES_FILE_NAME, TEST_MESSAGES_FILE);

  SECTION("Display Users")
  {
    ostringstream out;
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    displayUsers();

    const char *expectedOutput =
      "\tAdmiralAbe\n"
      "\tCaptainCharlie\n"
      "\tSeamanSam\n"
      "\tSeamanSue\n"
      "\tSeamanSly\n";

    REQUIRE(out.str() == string(expectedOutput));
  }

  SECTION("Display Messages - Secret")
  {
    ostringstream out;
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("AdmiralAbe", "password", messages);
    interact.display();

    const char *expectedOutput =
      "\t[100] Message from Winston Churchill at 16 May 1940\n"
      "\t[101] Message from President Franklin D. Roosevelt at 29 December 1940\n"
      "\t[102] Message from Seaman Smith at 5 December 1941\n"
      "\t[103] Message from Walter the Weatherman at 7 December 1941\n"
      "\t[104] Message from President Franklin D. Roosevelt at 8 December 1941\n"
      "\t[105] Message from Lt. Kenneth Taylor at 9 December 1941\n"
      "\t[106] Message from Fleet Admiral Chester W. Nimitz at 10 December 1941\n"
      "\t[107] Message from Captain Buckmaster at 7 June 1942\n"
      "\t[108] Message from General George S. Patton at 1 February 1943\n"
      "\t[109] Message from J. Robert Oppenheimer at 16 July, 1945\n";

    REQUIRE(out.str() == string(expectedOutput));
  }

  SECTION("Display Messages - Privileged")
  {
    ostringstream out;
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("CaptainCharlie", "password", messages);
    interact.display();

    const char *expectedOutput =
      "\t[100] Message from Winston Churchill at 16 May 1940\n"
      "\t[101] Message from President Franklin D. Roosevelt at 29 December 1940\n"
      "\t[102] Message from Seaman Smith at 5 December 1941\n"
      "\t[103] Message from Walter the Weatherman at 7 December 1941\n"
      "\t[104] Message from President Franklin D. Roosevelt at 8 December 1941\n"
      "\t[105] Message from Lt. Kenneth Taylor at 9 December 1941\n"
      "\t[107] Message from Captain Buckmaster at 7 June 1942\n"
      "\t[108] Message from General George S. Patton at 1 February 1943\n";

    REQUIRE(out.str() == string(expectedOutput));
  }

  SECTION("Display Messages - Confidential")
  {
    ostringstream out;
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("SeamanSly", "password", messages);
    interact.display();

    const char *expectedOutput =
      "\t[100] Message from Winston Churchill at 16 May 1940\n"
      "\t[101] Message from President Franklin D. Roosevelt at 29 December 1940\n"
      "\t[102] Message from Seaman Smith at 5 December 1941\n"
      "\t[103] Message from Walter the Weatherman at 7 December 1941\n"
      "\t[104] Message from President Franklin D. Roosevelt at 8 December 1941\n"
      "\t[105] Message from Lt. Kenneth Taylor at 9 December 1941\n";

    REQUIRE(out.str() == string(expectedOutput));
  }

  SECTION("Display Messages - Public")
  {
    ostringstream out;
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("Anonymous", "password", messages);
    interact.display();

    const char *expectedOutput =
      "\t[101] Message from President Franklin D. Roosevelt at 29 December 1940\n"
      "\t[103] Message from Walter the Weatherman at 7 December 1941\n"
      "\t[104] Message from President Franklin D. Roosevelt at 8 December 1941\n";

    REQUIRE(out.str() == string(expectedOutput));
  }

  SECTION("Display Message - Secret")
  {
    ostringstream out;
    stringstream in("106\n");
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("AdmiralAbe", "password", messages, &in);
    interact.show();

    const char *expectedOutput =
      "Select the message ID to display: "
      "\tMessage: Good thing our carriers were out to sea on Sunday!\n";

    REQUIRE(out.str() == string(expectedOutput));

    CLEAR_IN_OUT;

    // Should also be able to read down (lower control level)

    in << "108" << endl;

    interact.show();

    const char *expectedOutput2 =
      "Select the message ID to display: "
      "\tMessage: May God have mercy upon my enemies, because I won't!\n";

    REQUIRE(out.str() == string(expectedOutput2));
  }

  SECTION("Display Message - Confidential")
  {
    ostringstream out;
    stringstream in("108\n");
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);

    // Reading up is not allowed

    Interact interact("AdmiralAbe", "password", messages, &in);
    interact.show();

    const char *expectedOutput =
      "Select the message ID to display: "; // No message content

    REQUIRE(out.str() == string(expectedOutput));

    // Should be able to read our control level

    CLEAR_IN_OUT;

    in << "100" << endl;

    interact.show();

    const char *expectedOutput2 =
      "Select the message ID to display: "
      "\tMessage: Good thing our carriers were out to sea on Sunday!\n";

    REQUIRE(out.str() == string(expectedOutput2));

    CLEAR_IN_OUT;

    // Should also be able to read down (lower control level)

    in << "105" << endl;

    interact.show();

    const char *expectedOutput3 =
      "Select the message ID to display: "
      "\tMessage: The weather will be perfect, not a cloud in the sky\n";

    REQUIRE(out.str() == string(expectedOutput3));
  }

  SECTION("Display Message - Privileged")
  {
    ostringstream out;
    stringstream in("100\n");
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("AdmiralAbe", "password", messages, &in);
    interact.show();

    const char *expectedOutput =
      "Select the message ID to display: "
      "\tMessage: No man can tame a tiger into a kitten by stroking it.\n";

    REQUIRE(out.str() == string(expectedOutput));
  }

  SECTION("Display Message - Confidential")
  {
    ostringstream out;
    stringstream in("100\n");
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("AdmiralAbe", "password", messages, &in);
    interact.show();

    const char *expectedOutput =
      "Select the message ID to display: "
      "\tMessage: You ask what is our aim? I can answer in one word: Victory!\n";  

    REQUIRE(out.str() == string(expectedOutput));
  }

  SECTION("Display Message - Public")
  {
    ostringstream out;
    stringstream in("100\n");
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("AdmiralAbe", "password", messages, &in);
    interact.show();

    const char *expectedOutput =
      "Select the message ID to display: "
      "\tMessage: You ask what is our aim? I can answer in one word: Victory!\n";  

    REQUIRE(out.str() == string(expectedOutput));
  }

  SECTION("Remove Message")
  {
    ostringstream out;
    stringstream in("100\n");
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("SeamanSly", "password", messages, &in);
    interact.display();

    const char *expectedOutput =
      "\t[100] Message from Winston Churchill at 16 May 1940\n"
      "\t[101] Message from President Franklin D. Roosevelt at 29 December 1940\n"
      "\t[102] Message from Seaman Smith at 5 December 1941\n"
      "\t[103] Message from Walter the Weatherman at 7 December 1941\n"
      "\t[104] Message from President Franklin D. Roosevelt at 8 December 1941\n"
      "\t[105] Message from Lt. Kenneth Taylor at 9 December 1941\n"
      "\t[106] Message from Fleet Admiral Chester W. Nimitz at 10 December 1941\n"
      "\t[107] Message from Captain Buckmaster at 7 June 1942\n"
      "\t[108] Message from General George S. Patton at 1 February 1943\n"
      "\t[109] Message from J. Robert Oppenheimer at 16 July, 1945\n";

    REQUIRE(out.str() == string(expectedOutput));

    interact.remove();

    out.str("");
    out.clear();

    interact.display();

    const char *expectedOutput2 =
      "\t[101] Message from President Franklin D. Roosevelt at 29 December 1940\n"
      "\t[102] Message from Seaman Smith at 5 December 1941\n"
      "\t[103] Message from Walter the Weatherman at 7 December 1941\n"
      "\t[104] Message from President Franklin D. Roosevelt at 8 December 1941\n"
      "\t[105] Message from Lt. Kenneth Taylor at 9 December 1941\n"
      "\t[106] Message from Fleet Admiral Chester W. Nimitz at 10 December 1941\n"
      "\t[107] Message from Captain Buckmaster at 7 June 1942\n"
      "\t[108] Message from General George S. Patton at 1 February 1943\n"
      "\t[109] Message from J. Robert Oppenheimer at 16 July, 1945\n";

    REQUIRE(out.str() == string(expectedOutput2));
  }

  SECTION("Add Message")
  {
    ostringstream out;
    stringstream in;
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("SeamanSly", "password", messages, &in);
    interact.display();

    const char *expectedOutput =
      "\t[100] Message from Winston Churchill at 16 May 1940\n"
      "\t[101] Message from President Franklin D. Roosevelt at 29 December 1940\n"
      "\t[102] Message from Seaman Smith at 5 December 1941\n"
      "\t[103] Message from Walter the Weatherman at 7 December 1941\n"
      "\t[104] Message from President Franklin D. Roosevelt at 8 December 1941\n"
      "\t[105] Message from Lt. Kenneth Taylor at 9 December 1941\n"
      "\t[106] Message from Fleet Admiral Chester W. Nimitz at 10 December 1941\n"
      "\t[107] Message from Captain Buckmaster at 7 June 1942\n"
      "\t[108] Message from General George S. Patton at 1 February 1943\n"
      "\t[109] Message from J. Robert Oppenheimer at 16 July, 1945\n";

    REQUIRE(out.str() == string(expectedOutput));

    in << "Just a test message" << endl; // The text of the new message
    in << "1 January, 1944" << endl; // Date of the new message

    interact.add();

    out.str("");
    out.clear();

    interact.display();

    const char *expectedOutput2 =
      "\t[100] Message from Winston Churchill at 16 May 1940\n"
      "\t[101] Message from President Franklin D. Roosevelt at 29 December 1940\n"
      "\t[102] Message from Seaman Smith at 5 December 1941\n"
      "\t[103] Message from Walter the Weatherman at 7 December 1941\n"
      "\t[104] Message from President Franklin D. Roosevelt at 8 December 1941\n"
      "\t[105] Message from Lt. Kenneth Taylor at 9 December 1941\n"
      "\t[106] Message from Fleet Admiral Chester W. Nimitz at 10 December 1941\n"
      "\t[107] Message from Captain Buckmaster at 7 June 1942\n"
      "\t[108] Message from General George S. Patton at 1 February 1943\n"
      "\t[109] Message from J. Robert Oppenheimer at 16 July, 1945\n"
      "\t[110] Message from SeamanSly at 1 January, 1944\n";

    REQUIRE(out.str() == string(expectedOutput2));
  }

  SECTION("Update Message")
  {
    ostringstream out;
    stringstream in;
    cout_redirect out_redir(out.rdbuf());

    Messages messages(TEST_MESSAGES_FILE);
    Interact interact("SeamanSly", "password", messages, &in);

    in << "Just a test message" << endl; // The text of the new message
    in << "1 January, 1944" << endl; // Date of the new message

    interact.add();

    out.str("");
    out.clear();

    in << "110" << endl; // The id of the message to update
    in << "Just an updated test message" << endl; // The text of the updated message

    interact.update();
    interact.display();

    const char *expectedOutput2 =
      "Select the message ID to update: Please provide a message: "
      "\t[100] Message from Winston Churchill at 16 May 1940\n"
      "\t[101] Message from President Franklin D. Roosevelt at 29 December 1940\n"
      "\t[102] Message from Seaman Smith at 5 December 1941\n"
      "\t[103] Message from Walter the Weatherman at 7 December 1941\n"
      "\t[104] Message from President Franklin D. Roosevelt at 8 December 1941\n"
      "\t[105] Message from Lt. Kenneth Taylor at 9 December 1941\n"
      "\t[106] Message from Fleet Admiral Chester W. Nimitz at 10 December 1941\n"
      "\t[107] Message from Captain Buckmaster at 7 June 1942\n"
      "\t[108] Message from General George S. Patton at 1 February 1943\n"
      "\t[109] Message from J. Robert Oppenheimer at 16 July, 1945\n"
      "\t[110] Message from SeamanSly at 1 January, 1944\n";

    REQUIRE(out.str() == string(expectedOutput2));

    out.str("");
    out.clear();

    in.str("");
    in.clear();

    in << "110" << endl; // The id of the message to display

    interact.show();

    const char *expectedOutput3 =
      "Select the message ID to display: "
      "\tMessage: Just an updated test message\n";  

    REQUIRE(out.str() == string(expectedOutput3));
  }
}
