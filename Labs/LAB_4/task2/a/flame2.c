#include "util.h"



extern int system_call();
#define STDIN 0
#define STDOUT 1 
#define STDERR 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_LSEEK 19
#define SYS_getdents 141
#define R_MODE 0
#define W_MODE 1
#define RW_MODE 2
#define A_MODE 1024
#define CREATE 64

typedef struct linux_dirent {
               unsigned long  d_ino;     /* Inode number */
               unsigned long  d_off;     /* Offset to next linux_dirent */
               unsigned short d_reclen;  /* Length of this linux_dirent */
               char           d_name[1];  /* Filename (null-terminated) */
                                 /* length is actually (d_reclen - 2 -
                                    offsetof(struct linux_dirent, d_name)) */
               
               char           pad;       /* Zero padding byte*/
               char           d_type;    /* File type (only since Linux*/
                                         /* 2.6.4); offset is (d_reclen - 1)*/
               
           }linux_dirent;


int main(int argc, char** argv){
    char buf[8192];
    linux_dirent * entp = (linux_dirent *) buf;
    int on_p = 0;
    char keychar[1];
      int j;
      for(j = 1; j < argc; j=j+1){
        if (strncmp(argv[j], "-p", 2)==0){
            on_p = 1;
        }
        keychar[0] = *(argv[j]+2);
      }

    int fd = system_call(SYS_OPEN, ".", R_MODE,0);
    int numOfBytes = system_call(SYS_getdents,fd ,entp,8192 );
    if (fd < 0 || numOfBytes < 0)
    {
        system_call(0x55, -1);
    }
       int i;
       for (i = 0; i < numOfBytes; i+= entp->d_reclen )
       {
         
           if(( on_p && strncmp(entp->d_name, keychar, 1) == 0) || on_p==0){
             system_call(SYS_WRITE, STDOUT,"name: ", 6);
             system_call(SYS_WRITE, STDOUT, entp->d_name, strlen(entp->d_name));
              system_call(SYS_WRITE, STDOUT,"   type: ", 9);
              int typeID = entp->d_type;
              system_call(SYS_WRITE, STDOUT, itoa(typeID), strlen(itoa(typeID)));

              system_call(SYS_WRITE, STDOUT,"\n", 1);
           }
        
         
        entp = (linux_dirent *) (buf+i);
       }
       
        

      
    return 0;
}