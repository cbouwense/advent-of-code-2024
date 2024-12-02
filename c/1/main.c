#include "../guppy.h"

void part_one(int argc, char **argv) {
    int part_one_answer = 0;
    GupArena a = gup_arena_create();

    // example: [
    //   "3   4",
    //   "4   3",
    //   "2   5",
    // ]
    const char *file_path = argc > 1 ? argv[1] : "input.txt";

    GupArrayString file_lines = gup_file_read_lines_arena(&a, file_path);
    GupArrayInt left_ints = gup_array_int_create_arena(&a);
    GupArrayInt right_ints = gup_array_int_create_arena(&a);

    for (int i = 0; i < file_lines.count; i++) {
        const GupString line = file_lines.data[i];
        
        // "3   4" -> ["3", "4"]
        GupArrayString tokens = gup_string_split_by_cstr_arena(&a, line, "   ");
        int left_int, right_int = 0;
        
        gup_assert_verbose(gup_string_to_int(tokens.data[0], &left_int), "FATAL ERROR: Unable to parse integer out of the left number in the input line. You should probably print out the tokens and inspect if there's any sort of corruption or weirdo input.");
        gup_assert_verbose(gup_string_to_int(tokens.data[1], &right_int), "FATAL ERROR: Unable to parse integer out of the right number in the input line. You should probably print out the tokens and inspect if there's any sort of corruption or weirdo input.");

        gup_array_int_append_arena(&a, &left_ints, left_int);
        gup_array_int_append_arena(&a, &right_ints, right_int);
    }

    // Sort em baby
    GupArrayInt sorted_left = gup_array_int_sort_arena(&a, left_ints);
    GupArrayInt sorted_right = gup_array_int_sort_arena(&a, right_ints);

    for (int i = 0; i < sorted_left.count; i++) {
        const int distance = sorted_right.data[i] - sorted_left.data[i];
        if (distance < 0) {
            part_one_answer -= distance;
        } else {
            part_one_answer += distance;
        }
    }

    printf("part_one_answer: %d\n", part_one_answer);
    
    gup_arena_destroy(&a);
}

int main(int argc, char **argv) {
    part_one(argc, argv);
    return 0;
}
