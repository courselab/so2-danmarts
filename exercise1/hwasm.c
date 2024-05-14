#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_CODE_LENGTH 65536

int main() {
    FILE *input_file, *output_file;
    char input_filename[] = "hw.S";
    char output_filename[] = "hw.bin";
    char line[MAX_LINE_LENGTH];
    unsigned char code[MAX_CODE_LENGTH];
    int code_size = 0;

    // Open input file
    input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Unable to open input file '%s'\n", input_filename);
        return 1;
    }

    // Read input file line by line
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Check if line is not a comment or empty
        if (line[0] != ';' && line[0] != '\0') {
            // Split line into tokens
            char *token = strtok(line, " \t,");

            // Process tokens
            while (token != NULL) {
                // Check if token is a hexadecimal number
                if (token[0] == '0' && (token[1] == 'x' || token[1] == 'X')) {
                    // Convert hexadecimal string to integer
                    code[code_size++] = (unsigned char)strtol(token, NULL, 16);
                }

                // Get next token
                token = strtok(NULL, " \t,");
            }
        }
    }

    // Close input file
    fclose(input_file);

    // Open output file for writing in binary mode
    output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Error: Unable to open output file '%s'\n", output_filename);
        return 1;
    }

    // Write assembled code to output file
    fwrite(code, sizeof(unsigned char), code_size, output_file);

    // Close output file
    fclose(output_file);

    printf("Assembly completed. Output file: %s\n", output_filename);

    return 0;
}
