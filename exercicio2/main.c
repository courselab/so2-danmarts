#include "bios.h"
#include "utils.h"

#define PROMPT "$ "		/* Prompt sign.      */
#define SIZE 20			/* Read buffer size. */

char buffer[SIZE];		/* Read buffer.      */

// Function to get the available RAM memory
unsigned int get_available_memory() {
    unsigned int memory;
    __asm__("mov $0x88, %%eax;"
            "int $0x15;"
            "mov %%eax, %0;"
            : "=r" (memory)
            :
            : "%eax");
    return memory;
}

int main()
{
  clear();
  
  println("Boot Command 1.0");

  while (1)
    {
      print(PROMPT);		/* Show prompt.               */
      readln(buffer);		/* Read user input.            */

      if (buffer[0])		/* Execute built-in command.  */
	{
	    unsigned int memory = get_available_memory();
	    println("Available RAM memory: %d KB", memory);
	  }
	  else 
	    println("Unknown command.");
	}
    }
  
  return 0;
}
