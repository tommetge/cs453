#include <iostream>
#include <string>

using namespace std;

void safe() {
   printf("Safe function\n");
}

void unsafe() {
   printf("UNSAFE FUNCTION!!!!\n");
}

/*************************************
 * ARC VULNERABILTY
 * 1. There must be a function pointer used in the code.
 * 2. Through some vulnerability, there must be a way for user input 
 *    to overwrite the function pointer. This typically happens through 
 *    a stack buffer vulnerability.
 * 3. After the memory is overwritten, the function pointer must 
 *    be dereferenced.
 ****************************************/
// accept a buffer
void arcVulnerability(const char * input)
{
   // set the function ptr to the safe function
   void (* functionPtr)() = safe;
   // set the buffer
   char buffer[4] = {0};

   // create vulnerability from the buffer by not checking the length of the input
   memcpy(buffer, input, strlen(input));
  
   // call the now unsafe pointer
   functionPtr();
}

/**************************************
 * ARC WORKING
 * Call arcVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void arcWorking()
{
   arcVulnerability("Home");
}

/**************************************
 * ARC EXPLOIT
 * ...
 *************************************/
void arcExploit()
{

   // create buffer
   char bad[4 + sizeof(long *)] = {0};

   // create function pointer that points to safe
   bad[0] = 0xaa;
   bad[1] = 0xaa;
   bad[2] = 0xaa;
   bad[3] = 0xaa;

   // overflow the buffer to point to the unsafe function
   void (* unsafePtr)() = unsafe;
   memcpy(bad + 4, (long *)&unsafePtr, sizeof(long *));

   arcVulnerability(bad);
}

/*************************************
 * ANSII-UNICODE VULNERABILTY
 * 1. There must be a buffer where the basetype is greater than one.
 * 2. Validation of the buffer must check the size of the buffer rather 
      than the number of elements in the buffer.
 ****************************************/
void ansiiToUnicodeVulnerability(long * input)
{
   // Buffer that is vulnerable
   long buffer[32];
   // Not checking the number of elements and instead checking the size creates the vulnerability
   memcpy(buffer, input, sizeof(buffer) * sizeof(short));
   cout << buffer << endl;
}

/**************************************
 * ANSII-UNICODE WORKING
 * Call ansiiToUnicodeVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void ansiiToUnicodeWorking()
{
   long lng[4] = {0xaa, 0xaa, 0xaa, 0xaa};
   ansiiToUnicodeVulnerability(lng);
}

/**************************************
 * ANSII-UNICODE EXPLOIT
 * ...
 *************************************/
void ansiiToUnicodeExploit()
{
   // NOTE: This will cause the program to abort. To see the actual overflow use ASAN
   long bad[64] = {};
   ansiiToUnicodeVulnerability(bad);
}

static const char *SectionBoundary = "********************";

/**********************************************
 * MAIN : The main function.
 **********************************************/
int main()
{
   // cout << "Array Index" << endl;
   // cout << SectionBoundary << endl;

   // arrayVulnerability();
   // arrayWorking();
   // arrayExploit();

   cout << "\nARC" << endl;
   cout << SectionBoundary << endl;

   // arcVulnerability();
   arcWorking();
   arcExploit();

   // cout << "VTABLE" << endl;
   // cout << SectionBoundary << endl;

   // vtableVulnerability();
   // vtableWorking();
   // vtableExploit();

   // cout << "Stack Smashing" << endl;
   // cout << SectionBoundary << endl;

   // stackVulnerability();
   // stackWorking();
   // stackExploit();

   // cout << "Heap Spraying" << endl;
   // cout << SectionBoundary << endl;

   // heapVulnerability();
   // heapWorking();
   // heapExploit();

   // cout << "Integer Overflow" << endl;
   // cout << SectionBoundary << endl;

   // intOverflowVulnerability();
   // intOverflowWorking();
   // intOverflowExploit();

   cout << "\nANSI-UNICODE" << endl;
   cout << SectionBoundary << endl;

   // ansiiToUnicodeVulnerability();
   ansiiToUnicodeWorking();
   ansiiToUnicodeExploit();

   return 0;
}
