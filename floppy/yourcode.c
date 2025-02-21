/*
 * Code for students to implement for MP3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "openfile.h"




void output_permissions(mode_t m)
{
    putchar( m & S_IRUSR ? 'r' : '-' );
    putchar( m & S_IWUSR ? 'w' : '-' );
    putchar( m & S_IXUSR ? 'x' : '-' );
    putchar( m & S_IRGRP ? 'r' : '-' );
    putchar( m & S_IWGRP ? 'w' : '-' );
    putchar( m & S_IXGRP ? 'x' : '-' );
    putchar( m & S_IROTH ? 'r' : '-' );
    putchar( m & S_IWOTH ? 'w' : '-' );
    putchar( m & S_IXOTH ? 'x' : '-' );
    putchar('\n');
}


/*
 * Function: makeAccessWork()
 *
 * Description:
 * 	Implement your code here to allow this process to open the file for
 * 	reading and writing.
 *
 * Inputs:
 * 	dir1     - The absolute pathname of the file's grandparent directory.
 * 	dir2     - The absolute pathname of the file's parent directory.
 * 	pathname - The absolute pathname of the file to open.
 */



int
makeAccessWork (char * dir1, char * dir2, char * pathname) {
	/*
	 * Implement your code here.
	 */
	struct stat stat_dir1;
	struct stat stat_dir2;
	struct stat stat_file;
	struct stat new_stat_dir1;
	struct stat new_stat_dir2;
	struct stat new_stat_file;


	//remember old permissions, add read access to parent and grandparents
	//by adding one more bit mask on
	int r1 = stat(dir1, &stat_dir1);
	output_permissions(stat_dir1.st_mode);
	int x1 = chmod(dir1, stat_dir1.st_mode | S_IXOTH);
	if (x1!=0){
		perror("unable to set permissions");
		exit(1);
	}

	int n1 = stat(dir1, &new_stat_dir1);
	output_permissions(new_stat_dir1.st_mode);


	int r2 = stat(dir2, &stat_dir2);
	output_permissions(stat_dir2.st_mode);
	chmod(dir2, stat_dir2.st_mode | S_IXOTH);
	int n2 = stat(dir2, &new_stat_dir2);
	output_permissions(new_stat_dir2.st_mode);


	int r3 = stat(pathname, &stat_file);
	output_permissions(stat_file.st_mode);
	mode_t new_file_mode = stat_file.st_mode | S_IROTH | S_IWOTH;
	chmod(pathname, new_file_mode);
	int n3 = stat(dir2, &new_stat_file);
	output_permissions(new_stat_file.st_mode);

	

	printf("dir1=%s, dir2=%s, pathname=%s", dir1, dir2, pathname);

	return 0;
}
