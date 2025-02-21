/*
 * Code for students to implement for MP3.
 */
#define _GNU_SOURCE
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

	uid_t real_uid;
	uid_t effective_uid;
	uid_t saved_uid;

	getresuid(&real_uid, &effective_uid, &saved_uid);

	printf("e: %d r: %d s:%d");

	if (setresuid(real_uid, saved_uid, effective_uid) == -1){
		perror("cannot swap uid");
	}
	
	//Modify permissions for dir1 to allow other execute and read access
	int r1 = stat(dir1, &stat_dir1);
	output_permissions(stat_dir1.st_mode);
	if(chmod(dir1, stat_dir1.st_mode | S_IXOTH | S_IROTH) == -1){
		perror("unable to set permissions for dir1");
		exit(1);
	}
	int n1 = stat(dir1, &new_stat_dir1);
	output_permissions(new_stat_dir1.st_mode);


	//modify permissions for dir 2 to allows other execute and read access
	if (stat(dir2, &stat_dir2) == -1){
		perror("cannot stat dir 2")
	}
	output_permissions(stat_dir2.st_mode);
	if(chmod(dir2, stat_dir2.st_mode | S_IXOTH | S_IROTH)){
		perror("unable to set permissions for dir2")
	}
	int n2 = stat(dir2, &new_stat_dir2);
	output_permissions(new_stat_dir2.st_mode);



	//modify permissions for file to allow other read and write
	int r3 = stat(pathname, &stat_file);
	output_permissions(stat_file.st_mode);

	if(chmod(pathname, stat_file.st_mode | S_IROTH | SIWOTH) == -1){
		perror("could not modify permissions for file")
	}
	int n3 = stat(dir2, &new_stat_file);
	output_permissions(new_stat_file.st_mode);

	
	printf("dir1=%s, dir2=%s, pathname=%s", dir1, dir2, pathname);

	return 0;
}
