#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"

// (Interpreted Language) - [Lexer] -> [Parser] -> AST -> [Visitor]
// To run a command, set up the Roman executor in the global env path and then run in a new terminal session:
// Roman C:\C_APPS\Roman\src\examples\main.roman

char* read_file_to_string(const char *filename) {
    // 1. Open the file in binary mode
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // 2. Seek to the end to determine the file size
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Error seeking file");
        fclose(file);
        return NULL;
    }

    long file_size = ftell(file);
    if (file_size < 0) {
        perror("Error getting file size");
        fclose(file);
        return NULL;
    }

    // 3. Rewind back to the beginning of the file
    rewind(file);

    // 4. Allocate memory (+1 byte for the null terminator)
    char *buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // 5. Read the contents into the buffer
    size_t bytes_read = fread(buffer, 1, file_size, file);

    // 6. Null-terminate the string
    buffer[bytes_read] = '\0';

    // 7. Clean up and return
    fclose(file);
    return buffer;
}

bool ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) return false;

    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    // A string cannot end with a suffix longer than itself
    if (suffix_len > str_len) return false;

    // Move the pointer of str forward to where the suffix should start
    // Compare the remaining part of str with the suffix
    return strcmp(str + (str_len - suffix_len), suffix) == 0;
}

int main(int argc, char *argv[]) {

    // Executable runs with " Roman.exe "C:\C_APPS\Roman\src\examples\main.roman" "

    if (argv[1] != NULL) {
        if (!ends_with(argv[1], ".roman")) {
            printf("Invalid file type, try a '.roman' file extension.\n");
            exit(101);
            return 1;
        }

        char* example_contents = read_file_to_string(argv[1]);

        if (example_contents != NULL) {
            Lexer_T* Lexer = Init_Lexer(example_contents);
            Parser_T* Parser = Init_Parser(Lexer);

            AST_T* root = Parser_Parse(Parser, Parser->Scope);
            Visitor_T* visitor = Init_Visitor();
            AST_T* visit = Visitor_Visit(visitor, root);
            Visitor_Clean(visitor, root);
            free(visit);
            free(example_contents);  // frees memory
        } else {
            printf("FILE IS NULL.\n");
        }
    return 0;
    } else {
        printf("Invalid arguments, argument 1 is NULL or not a valid path.\n");
        exit(100);
    }
}
