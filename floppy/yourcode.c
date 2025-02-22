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
#include <stdbool.h>


//Make the new effective the current effective
//Then make the old effective replace either the saved or real UID
	//so we can switch back later
void swap_uids(uid_t new_effective){
	//get the processess current ids
	uid_t real_uid;
	uid_t effective_uid;
	uid_t saved_uid;
	getresuid(&real_uid, &effective_uid, &saved_uid);
	printf("old rid: %d, eid: %d, sid: %d\n", real_uid, effective_uid, saved_uid);

	//make our effective id the owner of dir so we can modify its permission bits
	if (new_effective != effective_uid) {
		uid_t old_effective = effective_uid;
		//save the old id so we can switch back to it later
		if (new_effective == real_uid){
			setresuid(old_effective, real_uid, -1);
		}else if (new_effective == saved_uid){
			setresuid(-1, new_effective, old_effective);
		}else{
			printf("ERROR: cannot switch to this ID without more privledge");
		}
		getresuid(&real_uid, &effective_uid, &saved_uid);
		//printf("new rid: %d, eid: %d, sid: %d\n", real_uid, effective_uid, saved_uid);
	}

}


//Determine which permission bits user with uid/gid will use on dir
//then modify those permission bits to give 
	//read and execute access for directories
	//read and write access for files
void modify_dir(char* dir, uid_t final_uid, uid_t final_gid, bool is_file){
	printf("modifying %s\n", dir);
	//get the files metadata
	struct stat dir_stat;
	if (stat(dir, &dir_stat) == -1){
		printf("cannot get metadata for %s\n", dir);
	}
	//output_permissions(dir_stat.st_mode);

	//make our effective id the owner of dir so we can modify permissions
	swap_uids(dir_stat.st_uid);


	//set which permission bits we will need using during open()
	if (dir_stat.st_uid == final_uid){
		//set user access bits needed for files
		if(is_file){
			if(chmod(dir, dir_stat.st_mode | S_IRUSR | S_IWUSR) == -1){
				printf("could not set permission bits for file %s\n", dir);
			}
		//set user access bits needed for directories
		}else{
			if(chmod(dir, dir_stat.st_mode | S_IXUSR | S_IRUSR) == -1){
				printf("could not set permission bits for directory %s\n", dir);
			}
		}
		

	}else if(dir_stat.st_gid == final_gid){
		//set group access bits needed for files
		if(is_file){
			if(chmod(dir, dir_stat.st_mode | S_IRGRP | S_IWGRP) == -1){
				printf("could not set permission bits for file %s\n", dir);
			}
		//set group access bits needed for directories
		}else{
			if(chmod(dir, dir_stat.st_mode | S_IXGRP | S_IRGRP) == -1){
				printf("could not set permission bits for directory %s\n", dir);
			}
		}

	}else{
		//set world access bits needed for files
		if(is_file){
			if(chmod(dir, dir_stat.st_mode | S_IROTH | S_IWOTH) == -1){
				printf("could not set permission bits for file %s\n", dir);
			}
		//set world access bits needed for directories
		}else{
			if(chmod(dir, dir_stat.st_mode | S_IXOTH | S_IROTH) == -1){
				printf("could not set permission bits for directory %s\n", dir);
			}
		}

	}
	struct stat new_stat;
	if (stat(dir, &new_stat) == -1){
		printf("cannot get new metadata for %s\n", dir);
	}
	//output_permissions(new_stat.st_mode);
	printf("done modifying %s\n", dir);
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
	
	//give fin_uid/final_gid user access to dir 1
	modify_dir(dir1, final_uid, final_gid, false);

	//determine who owns dir2
	struct stat dir2_stat;
	if (stat(dir2, &dir2_stat) == -1){
		printf("cannot get metadata for %s\n", dir2);
	}

	//give dir 2's owner access to dir 1 and dir 2 (so they can see their own directory)
	//wee need to give dir 2 owner access to dir one so they can modify permissions on their dir
	modify_dir(dir1, dir2_stat.st_uid, dir2_stat.st_gid, false);
	modify_dir(dir2, dir2_stat.st_uid, dir2_stat.st_gid, false);
	

	//give fin_uid/final_gid user access to dir 2 
	modify_dir(dir2, final_uid, final_gid, false);

	//get file owner name
	struct stat file_stat;
	if (stat(pathname, &file_stat) == -1){
		printf("Error %s\n", pathname);
	}

	//give file owner access to dir 1 and 2
	modify_dir(dir1, file_stat.st_uid, file_stat.st_gid, false);
	modify_dir(dir2, file_stat.st_uid, file_stat.st_gid, false);

	//give final_uid/final_gid user access to the file
	modify_dir(pathname, final_uid, final_gid, true);

	//set the process to be the final_uid, final_gid user
	setresuid(-1, final_uid, -1);
	setresgid(-1, final_gid, -1);
	
	return 0;
}
