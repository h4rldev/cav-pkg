#include "../../include/cli.h"
#include "../../include/meta.h"

#include <stdio.h>
#include <string.h>

/*
 * Args:
 *
 * First-level
 * --help, -h    0
 * --version, -v 1
 * --sync, -s    2
 * --update, -u  3
 * --remove, -r  4
 *
 * Second-level
 * [option] --upgrade, -u   0
 * [option] --verbose, -v   1
 * [option] --noconfirm, -y 2
 * [remove] --recursive, -r 3
 * [remove] --cached, -c    4
 *
 * Third-level
 * [option] [option] --noconfirm, -y 0
 * [option] [option] --verbose, -v   1
 */

static int get_argument_int(char *arg, char *argument_list[], int arg_len) {
    if (arg == 0 || (arg[0] == '\0')) {
	return -1;
    }

    for (int i = 0; i < arg_len; i++) {
	if (strncmp(arg, argument_list[i], strlen(arg)) == 0)
	return i / 2;
    }
  
    return -1;
}

static int get_arg_int_level(int *argc, char **argv[], enum Level level) {
    int arg_int = 0;
    
    char *arg_to_scan = *(*argv + level + 1);
    printf("arg_to_scan: %s\n", arg_to_scan);
    
    char *level1_args[] = {
	"--help",    "-h",            // 0 - NOTE: This doesn't have a subflag
	"--version", "-v",            // 1 - NOTE: This doesn't have a subflag
	"--sync", "-s",               // 2
	"--update", "-u",             // 3
	"--remove", "-r"              // 4
    };
    
    char *level2_args[] = {
	"--upgrade",   "-u",          // 0
	"--verbose",   "-v",          // 1
	"--noconfirm", "-y",          // 2
	"--recursive", "-r",          // 3
	"--cached",    "-c"           // 4
    };

    char *level3_args[] = {
	"--noconfirm", "-y",          // 1
	"--verbose", "-v"             // 2
    };

    switch (level) {
    case First:
	arg_int = get_argument_int(arg_to_scan, level1_args, 10);
	break;
    case Second:
	arg_int = get_argument_int(arg_to_scan, level2_args, 10);
	break;
    case Third:
	arg_int = get_argument_int(arg_to_scan, level3_args, 4);
	break;
    default:
	fprintf(stderr, "! Something went terribly wrong.\n");
	return -1;
    }

    return arg_int;
}

static int print_version(void) {
    printf("%s v%s\n", __PROJ_NAME__, __PROJ_VERSION__);
    return 0;
}

static int print_help(void) {
    if (print_version() != 0) return -1;
    printf(
    "USAGE: cav [Options] [Package]\n"
    "Options: \n"
    "    -h, --help                 Displays this message.\n"
    "    -v, --version              Shows program version.\n"
    "    -s, --sync                 Sync/Install a package from repository.\n"
    "     ↳  -u, --upgrade              Upgrade selected package.\n"
    "         ↳  -y, --noconfirm            Accept all prompts for package upgrade.\n"
    "            -v, --verbose              Show extra info during command execution.\n"
    "        -v, --verbose              Show extra info during command execution.\n"
    "        -y, --noconfirm            Accepts all prompts for install.\n"
    "    -u, --update               Update repository.\n"
    "     ↳  -u, --upgrade              Upgrade all outdated packages.\n"
    "         ↳  -y, --noconfirm            Accept all prompts for upgrade of all packages.\n"
    "            -v, --verbose              Show extra info during command execution.\n"
    "        -v, --verbose              Show extra info during command execution.\n"
    "    -r, --remove               Remove a package.\n"
    "     ↳  -h, --help                 Display more help about --remove.\n"
    "        -v, --verbose              Show extra info during command execution.\n"
    "        -y, --noconfirm            Accepts all prompts for removal.\n"
    "        -r, --recursive            Removes dependencies recursively.\n"
    "         ↳  -y, --noconfirm            Accept all prompts for upgrade of all packages.\n"
    "            -v, --verbose              Show extra info during command execution.\n"
    "        -c, --cached               Clears cache for package.\n"
    "         ↳  -y, --noconfirm            Accept all prompts for upgrade of all packages.\n"
    "            -v, --verbose              Show extra info during command execution.\n\n"

    "© h4rl & Malwarepad, 2024\n"
    "This program is licensed under the %s.\n"
    "Made with ❤ by h4rl\n",

    __PROJ_LICENSE__
    );

    return 0;
}



static int cav_remove(int (*arg_ints)[], int* argc, char **argv[]) {
    return 0;
}



static int cav_update(int (*arg_ints)[], int* argc, char **argv[]) {
    int *local_arg_ints = *arg_ints;
    
    switch (local_arg_ints[2]) {
    case 0:
	break;
    default:
	printf("cav_update");
	break;
    }

    return 0;
}



static int cav_sync_upgrade(int (*arg_ints)[], int* argc, char **argv[]) {
    return 0;
}

static int cav_sync(int (*arg_ints)[], int* argc, char **argv[]) {
    int local_argc = *argc;
    char **local_argv = *argv;
    int *local_arg_ints = *arg_ints;
    
    int package_beginning = 0;
    
    switch (local_arg_ints[1]) {
    case 0:
	printf("Query and upgrade package \n");
	printf("local_arg_ints[1]: %d\n", local_arg_ints[1]);
	break;
    default:
	package_beginning = local_argc-2;

	printf("Query and download packages: ");
	for (int pkgi = 0; (pkgi + package_beginning) < local_argc; pkgi++) {
	    printf("%s ", local_argv[pkgi+package_beginning]);
	}
	printf("\n");
    }

    return 0;
} 

int process_args(int *argc, char **argv[]) {
    int local_argc = *argc;
    char **local_argv = *argv;
    
    // 3 to indicate possible flag levels, stack allocated as it doesn't need the heap, filled with 0s to avoid undefined behavior
    int arg_ints[3] = {0};
    int program_result = 0;

    // TODO: figure out how to check between packages and flags

    int argi = 0;
    int arg_count = 1;

    
    for (int argi = 0; argi != 3; argi++) {
	int arg_int = get_arg_int_level(&local_argc, &local_argv, argi);
	printf("Arg int: %d\n", arg_int);
	arg_ints[argi] = arg_int;
    } 

    switch (arg_ints[0]) {
	case 0: // --help
	    program_result = print_help();
	    break;

	case 1: // --version
	    program_result = print_version();
	    break;

	case 2: // --sync
	    program_result = cav_sync(&arg_ints, &local_argc, &local_argv);
	    break;

	case 3: // --update
	    program_result = cav_update(&arg_ints, &local_argc, &local_argv);
	    break;

	case 4: // --remove
	    program_result = cav_remove(&arg_ints, &local_argc, &local_argv);
	    break;

	default: // unrecognized flag
	    fprintf(stderr, "! Unknown flag\n");
	    program_result = -1;
	    break;
    }

    return program_result;
}
