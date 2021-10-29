#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

void one(long number);
void two(long number);
void pass() { cout << "You pass :)\n"; }
void fail() { cout << "You've failed :(\n"; }
const char * passMessage = ":)";
const char * failMessage = ":(";

/**********************************************
 * MAIN : The top of the callstack.
 **********************************************/
int main()
{
   char text[8] = "*MAIN**";
   long number = 123456;
   void (*pointerFunction)() = fail;
   const char * message = failMessage;

   cout << "pointerFunction: " << hex << (long)*pointerFunction << endl;

   // display the initial values of the local variables
   cout << "main() : " << (void *)main << endl;
   cout << "\ttext:             " << text              << endl;
   cout << "\tnumber:           " << number            << endl;
   cout << "\tmessage:          " << message           << endl;
   cout << "\tfunction pointer: ";
   pointerFunction();

   // call the other functions
   one(number + 111111);     // 234567

   // display the new values of the local variables
   cout << "main() - after\n";
   cout << "\ttext:             " << text              << endl;
   cout << "\tnumber:           " << number            << endl;
   cout << "\tmessage:          " << message           << endl;
   cout << "\tfunction pointer: ";
   pointerFunction();

   return 0;
}

/************************************************
 * CONVERT TO STRING
 * Convert the data from p into a human-readable string
 * by removing all the unprintable characters and replacing
 * them with a dot
 ***********************************************/
string displayCharArray(const char * p)
{
   string output;
   for (int i = 0; i < 8; i++)
       output += string(" ") + (p[i] >= ' ' && p[i] <= 'z' ? p[i] : '.');
   return output;
}

/**********************************************
 * ONE : The next item on the call stack
 **********************************************/
void one(long number)               // 234567
{
   char text[8] = "**ONE**";

   cout << "one() : " << (void *)one << endl;
   cout << "\tmessage: " << (void *)failMessage << endl;
   cout << "\tfail():  " << (void *)fail        << endl;

   two(number + 111111);    // 345678
}

/**********************************************
 * TWO : The bottom of the call stack
 **********************************************/
void two(long number)              // 345678
{
   // start your display of the stack from this point
   long bow = number + 111111;     // 456789
   char text[8] = "**TWO**";
   long * pLong = NULL;
   char * pChar = NULL;

   // header for our table. Use these setw() offsets in your table
   cout << '[' << setw(2) << 'i' << ']'
        << setw(15) << "address"
        << setw(20) << "hexadecimal"
        << setw(20) << "decimal"
        << setw(18) << "characters"
        << endl;
   cout << "----+"
        << "---------------+"
        << "-------------------+"
        << "-------------------+"
        << "-----------------+\n";
   for (long i = 32; i >= -8; i--)   // You may need to change 24 to another number
   {
      ////////////////////////////////////////////////
      // Insert code here to display the callstack
      // Our anchor is the address of the `bow` variable. The current location
      // is that address + the offset `i`.
      long *address = &bow + i;

      // Prepend the "0x" to the hex value.
      ostringstream hexVal;
      hexVal << "0x" << hex << *address;
      
      // Output the current offset (i.e. [ 1])
      cout << "[" << setw(2) << i << "]";
      // Output the hex representation of the address
      cout << setw(16) << address;
      // Output the hex representation of the memory at this address
      cout << setw(20) << hexVal.str();
      // Output the decimal representation of the memory at this address
      cout << setw(20) << dec << *address;
      // Output the ASCII representation of the memory at this address
      cout << setw(18) << displayCharArray((char *)address) << endl;
      //
      ////////////////////////////////////////////////
   }

   ////////////////////////////////////////////////
   // Insert code here to change the variables in main()

   // Check assumptions about the local machine
   // Note: these are not perfect checks, only strong indicators that our
   // environment is what we expect. The following implementation will only
   // work on x86_64 - specifically AMD64 ABI.
   assert(sizeof(long) == 8);
   assert(sizeof(long *) == 8);

   // Fetch the current frame
   // The current frame starts at this offset from &bow (on the target machine)
   int frameOffset = 4; // 4 * 8 = 32 bytes

   // The current frame's frame pointer (which points to the frame pointer
   // for one())
   long *framePtr = (long *)(&bow + frameOffset);

   // Fetch the frame pointer for one() by following the current frame's
   // frame pointer
   long *frameOnePtr = (long *)*framePtr;

   // The main frame can be found by following frameOnePtr.
   long *frameMainPtr = (long *)*frameOnePtr;

   // change text in main() to "*main**"

   // Note: the following uses offsets relative to the main frame's
   // frame pointer. This is only necessary for the first object (the
   // *main char pointer). After finding that first object, all other
   // objects could just as easily be found by offsets relative to the
   // last object pointer.

   // The text is found 2 blocks before the main frame's frame pointer
   char *main = (char *)(frameMainPtr - 2);
   assert(strncmp(main, "*MAIN**", 8) == 0);
   strncpy(main, "*main**", 8);

   // change number in main() to 654321

   // The number is found 4 blocks before the main frame's frame pointer
   // (or 2 blocks before char *main found above)
   int *num = (int *)(frameMainPtr - 4);
   assert(*num == 123456);
   *num = 654321;

   // change pointerFunction in main() to point to pass

   // pointerFunction is found 5 blocks before the main frame's frame pointer
   // (or 1 block before int *num found above)
   long *pointerFunctionPtr = (long *)(frameMainPtr - 5);
   assert((long)*pointerFunctionPtr == (long)&fail);
   *pointerFunctionPtr = (long)&pass;

   // change message in main() to point to passMessage

   // message is found 6 blocks before the main frame's frame pointer
   // (or 1 block before long *pointerFunctionPtr found above)
   long *message = (long *)(frameMainPtr - 6);
   assert((long)*message == (long)&failMessage[0]);
   *message = (long)&passMessage[0];

   //
   ////////////////////////////////////////////////
}
