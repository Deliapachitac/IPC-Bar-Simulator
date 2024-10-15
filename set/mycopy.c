#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 50

int main(int argc, char *argv[]){

    if (argc != 3) {
        printf( "The program has more arguments than needed");
        exit(EXIT_FAILURE);
    }

    int sourfile, destfile;

    // opening the files
    sourfile = open(argv[1], O_RDONLY);
    if (sourfile == NULL) {
        printf("The file is not opened. The program will ");
        exit(EXIT_FAILURE);
    }
    destfile = open(argv[2], O_RDONLY);
    if (destfile == NULL) {
        printf("The file is not opened. The program will ");
        exit(EXIT_FAILURE);
    }
    int c;
    while (( c = fgetc(sourfile)) != EOF)
    {
        fputc(c, destfile);
    }

    fclose(sourfile);
    fclose(destfile);
    return 0;
}



int main(int argc, char *argv[]) {
    int source_fd, dest_fd;  // file descriptors για το αρχείο προέλευσης και προορισμού
    ssize_t n_read, n_written; // Μεταβλητές για τον αριθμό των bytes που διαβάζονται/γράφονται
    char buffer[BUFFER_SIZE]; // buffer για την αποθήκευση δεδομένων

    // Έλεγχος για τον αριθμό των παραμέτρων γραμμής εντολών
    if (argc != 3) {
        fprintf(stderr, "Χρήση: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Άνοιγμα του αρχείου προέλευσης (source file)
    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1) {
        perror("Σφάλμα κατά το άνοιγμα του αρχείου προέλευσης");
        exit(EXIT_FAILURE);
    }

    // Άνοιγμα του αρχείου προορισμού (destination file)
    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Σφάλμα κατά το άνοιγμα του αρχείου προορισμού");
        close(source_fd);
        exit(EXIT_FAILURE);
    }

    // Αντιγραφή του περιεχομένου από το αρχείο προέλευσης στο αρχείο προορισμού
    while ((n_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        n_written = write(dest_fd, buffer, n_read);
        if (n_written != n_read) {
            perror("Σφάλμα κατά την εγγραφή στο αρχείο προορισμού");
            close(source_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (n_read == -1) {
        perror("Σφάλμα κατά την ανάγνωση από το αρχείο προέλευσης");
    }

    // Κλείσιμο των αρχείων
    close(source_fd);
    close(dest_fd);

    return 0;
}
