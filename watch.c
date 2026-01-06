/* watch.c - Auto-compilador que detecta cambios y recompila */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_FILES 128
#define MAX_PATH 512
#define MAX_CMD 1024

typedef struct {
    char path[MAX_PATH];
    time_t mtime;
} File;

static File files[MAX_FILES];
static int nfiles = 0;
static char command[MAX_CMD];
static int clear_screen = 1;

static time_t get_mtime(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return st.st_mtime;
}

static int should_watch(const char *name) {
    size_t len = strlen(name);
    if (len < 2) return 0;
    
    const char *ext = name + len - 2;
    if (strcmp(ext, ".c") == 0 || strcmp(ext, ".h") == 0)
        return 1;
    
    if (len >= 3) {
        ext = name + len - 3;
        if (strcmp(ext, ".cc") == 0)
            return 1;
    }
    
    if (len >= 4) {
        ext = name + len - 4;
        if (strcmp(ext, ".cpp") == 0 || strcmp(ext, ".hpp") == 0)
            return 1;
    }
    
    return 0;
}

static void scan_files(const char *dir) {
    DIR *d = opendir(dir);
    if (!d) return;
    
    struct dirent *e;
    while ((e = readdir(d))) {
        if (e->d_name[0] == '.') continue;
        
        char path[MAX_PATH];
        snprintf(path, MAX_PATH, "%s/%s", dir, e->d_name);
        
        struct stat st;
        if (stat(path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                scan_files(path);
            } else if (should_watch(e->d_name)) {
                if (nfiles < MAX_FILES) {
                    strncpy(files[nfiles].path, path, MAX_PATH - 1);
                    files[nfiles].mtime = st.st_mtime;
                    nfiles++;
                }
            }
        }
    }
    closedir(d);
}

static int check_changes(void) {
    for (int i = 0; i < nfiles; i++) {
        time_t current = get_mtime(files[i].path);
        if (current != files[i].mtime) {
            files[i].mtime = current;
            return 1;
        }
    }
    return 0;
}

static void run_command(void) {
    if (clear_screen) {
        system("clear");
    }
    
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = 0;
    
    printf("═══════════════════════════════════════════════\n");
    printf("⟳ Compilando... [%s]\n", time_str);
    printf("═══════════════════════════════════════════════\n\n");
    
    int ret = system(command);
    
    printf("\n");
    if (ret == 0) {
        printf("✓ Éxito\n");
    } else {
        printf("✗ Error (código %d)\n", ret);
    }
    printf("═══════════════════════════════════════════════\n");
    printf("Observando cambios...\n");
}

static void usage(void) {
    printf("watch - auto-compilador en vivo\n\n");
    printf("Uso:\n");
    printf("  watch [opciones] <comando>\n\n");
    printf("Opciones:\n");
    printf("  -n    no limpiar pantalla\n\n");
    printf("Ejemplos:\n");
    printf("  watch gcc main.c -o app\n");
    printf("  watch make\n");
    printf("  watch ./blaze\n");
    printf("  watch -n gcc test.c\n");
    exit(0);
}

int main(int argc, char **argv) {
    if (argc < 2) usage();
    
    int arg_start = 1;
    
    if (strcmp(argv[1], "-n") == 0) {
        clear_screen = 0;
        arg_start = 2;
        if (argc < 3) usage();
    }
    
    command[0] = '\0';
    for (int i = arg_start; i < argc; i++) {
        if (i > arg_start) strcat(command, " ");
        strncat(command, argv[i], MAX_CMD - strlen(command) - 1);
    }
    
    nfiles = 0;
    scan_files(".");
    
    printf("Observando %d archivos en el directorio actual\n", nfiles);
    printf("Comando: %s\n\n", command);
    
    run_command();
    
    while (1) {
        sleep(1);
        
        if (check_changes()) {
            printf("\n→ Cambio detectado!\n\n");
            run_command();
        }
    }
    
    return 0;
}