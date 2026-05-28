
/*
   Test program for checking CTask DOS TSR compatibility.

   This program goes TSR, and then repeatedly reads a file, and
   outputs status messages to the serial port.

   The program is called with res [filename] [port]
   where [port] is  "-"    output to COM1 (relative)
                    "-1"   output to COM1 (relative)
                    "-2"   output to COM2 (relative)
                    "1"    output to COM1 (absolute)
   If no filename is given, nothing is done in the background.
   If no port is given, COM1 (absolute) is used.
*/

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <dos.h>
#include <errno.h>

#include "tsk.h"
#include "tskprf.h"

#define PORT   1
#define BAUD   1152

#define STACKSIZE 2048

unsigned int _stklen = 2 * STACKSIZE;  /* For Turbo C: One task + main task Stack */

tcb tcb1;

byte fbuf[512];
char fname[128];

int ofile;

#if (TSK_TURBO)
#define stayres(len)    keep (0, (len))
#else
#define stayres(len)    _dos_keep (0, (len))
#define sound(x)
#define nosound()
#endif

struct mem_control {
                   byte id;
                   word owner;
                   word paragraphs;
                   };

void exit_resident (void)
{
   struct mem_control far *mem;

   mem = MK_FP (_psp - 1, 0);
   stayres (mem->paragraphs);
}


/*
   Task1 does something in the backgound.
*/

void far task1 (void)
{
  int retval;
  unsigned nr;

   while (1) 
      {
      t_delay (36L);  /* 2 seconds = 36 */
      if (fname [0]) 
         {
         if (_dos_open (fname, O_RDONLY, &ofile)) 
            {
            tsk_cprintf ("\n*OF ERR %d*\007\n", errno);
            sound (800);
            t_delay (2L);
            nosound ();
            }
         else 
            {
            tsk_cprintf ("O");
            while (!(retval = _dos_read (ofile, fbuf, 512, &nr)))
               {
               if (!nr)
                  break;
	            t_delay (2L);
               tsk_cprintf ("r");
               }
            if (retval)
               {
               tsk_cprintf ("\n*Rd ERR %d*\007\n", errno);
	            sound (200);
               t_delay (8L);
               nosound ();
               }
            else 
               {
               tsk_cprintf ("C");
               sound (4000);
               t_delay (1L);
               nosound ();
               }
            _dos_close (ofile);
            }
         }
      else 
         {
         sound (200);
         t_delay (1L);
         nosound ();
         }
      }
}


int main (int argc, char *argv [])
{
   char stack1 [STACKSIZE];
   int port, i;
   
   if (argc >= 2) 
      strcpy (fname, argv [1]);
   else 
      fname[0] = '\0';

   port = PORT;
   if (argc > 2)
      {
      port = 1;
      i = 0;
      if (argv [2][0] == '-')
         i = 1;
      if (argv [2][i] >= '1' && argv [2][i] <= '4')
         port = argv [2][i] - '0';
      }

   install_tasker (0, 0, IFL_STD, "Tres");
   tsk_cprint_init (port, BAUD);

   create_task (&tcb1, task1, stack1, STACKSIZE, PRI_STD, NULL TN("SIO-TASK"));

   start_task (&tcb1);
   set_priority (NULL, PRI_STD);
   preempt_on ();
   yield ();
   exit_resident ();
   return 0; /* dummy */
}


