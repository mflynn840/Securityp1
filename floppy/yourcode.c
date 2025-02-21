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
	struct stat stat_obj_dir1;
	struct stat stat_obj_dir2;
	struct stat stat_obj_file;

	//see if we have search access to grandparent directory
	r1 = stat(dir1, &stat_obj_dir1);
	printf("dir1 permission Bits: %s\n", stat_obj_dir1.st_mode);

	//see if we have search access to parent directory
	r2 = stat(dir2, &stat_obj_dir2);
	printf("dir2 permission Bits: %s\n", stat_obj_dir2.st_mode);

	//see if we have read/write access to the file
	r3 = stat(pathname, &stat_obj_file);
	printf("dir1 permission Bits: %s\n", stat_obj_file.st_mode);
	printf("dir1=%s, dir2=%s, pathname=%s");

	return 0;
}
