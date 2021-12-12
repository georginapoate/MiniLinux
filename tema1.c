#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

void freeFiles(Dir* home) {

	File* f = home->head_children_files;
	File* next = NULL;
	while (f) {
		next = f->next;
		free(f->name);
		free(f);
		f = next;
	}
}

void freeDrts(Dir* home) {
	Dir* next = NULL;
	while (home) {
		next = home->next;
		freeFiles(home);
		if (home->head_children_dirs) {
			freeDrts(home->head_children_dirs);
		}
		free(home->name);
		free(home);
		home = next;
	}
}

File* contains1(Dir* parent, char* name) {
	File* fis = parent->head_children_files;
	while (fis) {
		if (strcmp(fis->name, name) == 0)
			break;
		fis = fis->next;
	}
	return fis;
}

Dir* contains2(Dir* parent, char* name) {
	Dir* drt = parent->head_children_dirs;
	while (drt) {
		if (strcmp(drt->name, name) == 0)
			break;
		drt = drt->next;
	}
	return drt;
}


void touch (Dir* parent, char* name) {
	if (contains1(parent, name) == NULL) {
		File* newfile = calloc(1, sizeof(File));
		newfile->parent = parent;
		newfile->name = strdup(name);
		File* fil = parent->head_children_files;
		if (fil == NULL) {
			parent->head_children_files = newfile;
		} else { 
			while (fil->next) {
				fil = fil->next;
			}
			fil->next = newfile;
		}
	} else {
		printf("File already exists\n");
	}
}

void mkdir (Dir* parent, char* name) {
	if (contains2(parent, name) == NULL){
		Dir* newdir = calloc(1, sizeof(Dir));
		newdir->parent = parent;
		newdir->name = strdup(name);
		Dir* dir = parent->head_children_dirs;
		if (dir == NULL) {
			parent->head_children_dirs = newdir;
		} else { 
			while (dir->next) {
				dir = dir->next;
			}
			dir->next = newdir;
		}
	} else {
		printf("Directory already exists\n");
	}
}

void ls (Dir* parent) {
	Dir* drt = parent->head_children_dirs;
	File* fil = parent->head_children_files;
	while (drt) {
		printf("%s\n", drt->name);
		drt = drt->next;
	}
	while (fil) {
		printf("%s\n", fil->name);
		fil = fil->next;
	}
}

void rm (Dir* parent, char* name) {
	File* delf = contains1(parent, name);
	File* adt = parent->head_children_files;
	//cazul in care contains1 nu a gasit file-ul:
	if (delf == NULL) {
		printf("Could not find the file\n");
	// cazul in care fisierul este primul din director:
	} else if (delf == adt) {
		parent->head_children_files = delf->next;
		free(delf->name);
		free(delf);
	// cazul in care fisierul este la mijloc/la finalul dirului:
	} else {
		while (adt->next != delf) {
			adt = adt->next;
		} 
		adt->next = delf->next;
		free(delf->name);
		free(delf);
	}
		
}

void rmdir (Dir* parent, char* name) {
	Dir* deld = contains2(parent, name);
	Dir* adt = parent->head_children_dirs;
	if (deld == NULL) {
		printf("Could not find the dir\n");
	} else if (deld == adt) {
		parent->head_children_dirs = deld->next;
		deld->next = NULL;
		freeDrts(deld);
	} else {
		while (adt->next != deld) {
			adt = adt->next;
		}
		adt->next = deld->next;
		deld->next = NULL;
		freeDrts(deld);
	}
}

void cd(Dir** target, char *name) {
	Dir* chgd = *target;
	Dir* contd = contains2(chgd, name);
	if (strcmp(name, "..") == 0) {
		if (chgd->parent == NULL) return;
		else *target = chgd->parent;
	} else if (contd == NULL) {
		printf("No directories found!\n");
	} else {
		*target = contd;
	}
}

char *pwd (Dir* target) {
	if (target->parent != NULL) {
		char* str = pwd(target->parent); // pwd + target->name
		char* name = target->name;
		char* path = malloc(sizeof(char) * (strlen(str)+1+strlen(name)+1));
		sprintf(path, "%s/%s", str, name);
		free(str);
		return path;
	}
	return strdup(target->name); //am facut duplicate la numele dirului
}

void stop (Dir* target) {
	freeDrts(target);
	return;
}

void tree (Dir* parent, int level) {
	Dir* drt = parent->head_children_dirs;
	File* fil = parent->head_children_files;
	while (drt) {
		for (int i = 0; i < level; i++) {
			printf("    ");
		}
		printf("%s\n", drt->name);
		tree(drt, level+1);
		drt = drt->next;
	}
	while (fil) {
		for (int i = 0; i < level; i++) {
			printf("    ");
		}
		printf("%s\n", fil->name);
		fil = fil->next;
	}
}

void mv(Dir* parent, char *oldname, char *newname) {
	// check if oldname exists for files and dirs:

	if (contains2(parent, oldname) || contains1(parent, oldname)) {

		// check if newname exists:

		if (contains2(parent, newname) || contains1(parent, newname)) {
			printf("File/Director already exists\n");
	
		} else if (contains1(parent, oldname) != NULL) {
			rm(parent, oldname);
			touch(parent, newname);

		} else if (contains2(parent, oldname) != NULL) { 
			rmdir(parent, oldname);
			mkdir(parent, newname);
		}

	// case if oldname doesn't exist:

	} else printf("File/Director not found\n");
}

int main() {
	Dir* home = calloc(1, sizeof(Dir));

	//i-am setat numele directorului root:
	
	home->name = strdup("/home");
	Dir* curr = home;
	char *line = malloc(sizeof(char) * MAX_INPUT_LINE_SIZE);
	const char delim[] = " \n";

	// voi folosi strtok ca sa separ stringurile in
	// (numele functiei, parametru)

	char *ptok = NULL;
	char *param = NULL;
	char *param2 = NULL;
	while (fgets(line, MAX_INPUT_LINE_SIZE, stdin)) {
		ptok = strtok(line, delim);
		if (strcmp(ptok, "touch") == 0) {
			param = strtok(NULL, delim);
			touch(curr, param);
		} else if (strcmp(ptok, "ls") == 0) {
			ls(curr);
		} else if (strcmp(ptok, "mkdir") == 0){
			param = strtok(NULL, delim);
			mkdir(curr, param);
		} else if (strcmp(ptok, "rm") == 0){
			param = strtok(NULL, delim);
			rm(curr, param);
		} else if (strcmp(ptok, "rmdir") == 0){
			param = strtok(NULL, delim);
			rmdir(curr, param);
		} else if (strcmp(ptok, "stop") == 0){
			param = strtok(NULL, delim);
			stop(home);
			break;
		} else if (strcmp(ptok, "cd") == 0){
			param = strtok(NULL, delim);
			cd(&curr, param);
		} else if (strcmp(ptok, "tree") == 0){
			param = strtok(NULL, delim);
			tree(curr, 0);
		} else if (strcmp(ptok, "pwd") == 0){
			char *loc = (pwd(curr));
			printf("%s", loc);
			free(loc);
		} else if (strcmp(ptok, "mv") == 0){
			param = strtok(NULL, delim);
			param2 = strtok(NULL, delim);
			mv(curr, param, param2);
		}
	}
	free(line);
	return 0;
}
