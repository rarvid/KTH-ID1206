#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char *argv[]) {

  if(argc < 2) {
    perror("usage: myls <dir>\n");
    return -1;
  }

  char *path = argv[1];

  DIR *dirp = opendir(path);
  
  struct dirent *entry;

  struct stat file_st;


  while((entry = readdir(dirp)) != NULL) {
    switch(entry->d_type) {
      case DT_BLK :     // This is a block device
        printf("b:");
        break;
      case DT_CHR :     // This is a character device
        printf("c:");
        break;
      case DT_DIR :     // This is a directory
        printf("d:");
        break;
      case DT_FIFO :    // This is a named pipe
        printf("p:");
        break;
      case DT_LNK :     // This is a symbolic link
        printf("l:");
        break;
      case DT_REG :     // This is a regular file
        printf("f:");
        break;
      case DT_SOCK :    // This is a UNIX domain socket
        printf("s:");
        break;
      case DT_UNKNOWN : // The file type is unknown
        printf("u:");
        break;
    }
    fstatat(dirfd(dirp), entry->d_name, &file_st, 0);
    printf("\tinode: %lu", entry->d_ino);
    printf("\tdev: 0x%lx", file_st.st_dev);
    printf("\tinode: %lu", file_st.st_ino);
    printf("\tsize: %lu", file_st.st_size);
    printf("\tname: %s\n", entry->d_name);
  }
}
