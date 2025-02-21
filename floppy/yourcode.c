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


void swap_ids(char* new_effective, uid_t* old_uids){

	
}

void modify_dir(char* dir, uid_t final_uid, uid_t final_gid, bool is_file){

	//get the files metadata
	struct stat dir_stat;
	if (stat(dir, &dir_stat) == -1){
		perror("cannot get metadata for %s", dir);
	}

	output_permissions(dir_stat.st_mode)
	//get the processess current ids
	uid_t real_uid;
	uid_t effective_uid;
	uid_t saved_uid;
	getresuid(&real_uid, &effective_uid, &saved_uid);


	//make our effective id the owner of dir so we can modify its permission bits
	if (dir_stat.st_uid != effective_uid) {
		uid_t old_effective = effective_uid;
		//save the old id so we can switch back to it later
		if (dir_stat.st_uid == real_uid){
			setresuid(old_effective, real_uid, -1);
		}else if (dir_stat.st_uid == saved_uid){
			setresuid(-1, dir_stat.st_uid, old_effective);
		}else{
			print("ERROR: cannot switch to this ID without root privledge")
		}
		
	}

	//decide which permission bits we will be using during open()
	if (dir_stat.st_uid == final_uid){
		//set user access bits needed for files
		if(is_file){
			if(chmod(dir, stat_dir.st_mode | S_IRUSR | S_IWUSR) == -1){
				perror("coult not set permission bits for file")
			}
		//set user access bits needed for directories
		}else{
			if(chmod(dir, stat_dir.st_mode | S_IXUSR) == -1){
				perror("could not set permission bits for directory")
			}
		}
		

	}else if(dir_stat.st_gid == final_gid){
		//set user access bits needed for files
		if(is_file){
			if(chmod(dir, stat_dir.st_mode | S_IRGRP | S_IWGRP) == -1){
				perror("coult not set permission bits for file")
			}
		//set user access bits needed for directories
		}else{
			if(chmod(dir, stat_dir.st_mode | S_IXGRP) == -1){
				perror("could not set permission bits for directory")
			}
		}

	}else{
		//set user access bits needed for files
		if(is_file){
			if(chmod(dir, stat_dir.st_mode | S_IROTH | S_IWOTH) == -1){
				perror("coult not set permission bits for file")
			}
		//set user access bits needed for directories
		}else{
			if(chmod(dir, stat_dir.st_mode | S_IXOTH) == -1){
				perror("could not set permission bits for directory")
			}
		}

	}

	struct stat new_stat;
	if (stat(dir, &new_stat) == -1){
		perror("cannot get metadata for %s", dir);
	}

	output_permissions(new_stat.st_mode)

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


	//1. Decide who we want to be (group and user) when the program is done executing
	//2.  We need to know whether or not we will be using owner, 
	      //group or other permission bits at each level

	uid_t final_uid = jim;
	gid_t final_gid = 102;
	
	//to hold file/directory metadata
	struct stat stat_dir1;
	struct stat stat_dir2;
	struct stat stat_file;
	struct stat new_stat_dir1;
	struct stat new_stat_dir2;
	struct stat new_stat_file;

	//our current ids
	uid_t real_uid;
	uid_t effective_uid;
	uid_t saved_uid;

	getresuid(&real_uid, &effective_uid, &saved_uid);
	printf("e: %d r: %d s:%d");

	//Get dir1's stat
	int r1 = stat(dir1, &stat_dir1);
	output_permissions(stat_dir1.st_mode);

	//make our effective uid the owner of dir1 (which is home so its jim)
	if(setresuid(judy, jim, john) == -1){
		perror("cannot swap to dir1 owner")
	}

	//turn on world permission bits for read and execute
	if(chmod(dir1, stat_dir1.st_mode | S_IXOTH | S_IROTH) == -1){
		perror("unable to set permissions for dir1");
		exit(1);
	}
	int n1 = stat(dir1, &new_stat_dir1);
	output_permissions(new_stat_dir1.st_mode);


	//become the owner of dir 1/2/3
	if (stat(dir2, &stat_dir2) == -1){
		perror("cannot stat dir 2");
	}
	output_permissions(stat_dir2.st_mode);
	uid_t old_id = effective_uid;
	setresuid(-1, stat_dir2.st_)

	if(chmod(dir2, stat_dir2.st_mode | S_IXOTH | S_IROTH) == -1){
		perror("unable to set permissions for dir2");
	}
	int n2 = stat(dir2, &new_stat_dir2);
	output_permissions(new_stat_dir2.st_mode);



	//modify permissions for file to allow other read and write
	int r3 = stat(pathname, &stat_file);
	output_permissions(stat_file.st_mode);

	if(chmod(pathname, stat_file.st_mode | S_IROTH | S_IWOTH) == -1){
		perror("could not modify permissions for file");
	}

	int n3 = stat(dir2, &new_stat_file);
	output_permissions(new_stat_file.st_mode);

	
	printf("dir1=%s, dir2=%s, pathname=%s", dir1, dir2, pathname);

	return 0;
}
