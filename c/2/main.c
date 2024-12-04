#include "../guppy.h"

typedef enum Direction {
    DIRECTION_UNKNOWN = 1,
    DIRECTION_INCREASING,
    DIRECTION_DECREASING,
} Direction;

int part_one(int argc, char **argv) {
    GupArena a = gup_arena_create();

    int part_one_answer = 0;
    const char *file_path = argc > 1 ? argv[1] : "input.txt";
    GupArrayString file_lines = gup_file_read_lines_arena(&a, file_path);

    for (int i = 0; i < file_lines.count; i++) {
        GupString line = file_lines.data[i];
        GupArrayString tokens = gup_string_split_arena(&a, line, ' ');
        Direction dir = DIRECTION_UNKNOWN;
        bool is_safe = true;

        for (int j = 0; j < tokens.count - 1; j++) {
            GupString curr_level = tokens.data[j];
            GupString next_level = tokens.data[j+1];

            int curr_level_int, next_level_int = 0;
            gup_string_to_int(curr_level, &curr_level_int);
            gup_string_to_int(next_level, &next_level_int);

            int delta = next_level_int - curr_level_int;
            // If there is no change, this is definitely an unsafe report and
            // we can just move onto the next one.
            if (delta == 0) {
                is_safe = false;
                break;
            }
            // If at any point the change is more than 3, we know this report
            // is unsafe and we can move on.
            if (delta < -3 || delta > 3) {
                is_safe = false;
                break;
            }

            if (j == 0) {
                if (delta < 0) dir = DIRECTION_DECREASING;
                if (delta > 0) dir = DIRECTION_INCREASING;
            } else {
                if (dir == DIRECTION_DECREASING && delta > 0) {
                    is_safe = false;
                    break;
                }
                if (dir == DIRECTION_INCREASING && delta < 0) {
                    is_safe = false;
                    break;
                }
            }
            
        }

        if (is_safe) part_one_answer++;
    }

    gup_arena_destroy(&a);

    return part_one_answer;
}

bool is_line_safe(GupArrayString tokens) {
    Direction dir = DIRECTION_UNKNOWN;

    for (int i = 0; i < tokens.count - 1; i++) {
        GupString curr_level = tokens.data[i];
        GupString next_level = tokens.data[i+1];

        int curr_level_int, next_level_int = 0;
        gup_string_to_int(curr_level, &curr_level_int);
        gup_string_to_int(next_level, &next_level_int);

        int delta = next_level_int - curr_level_int;
        if (dir == DIRECTION_UNKNOWN) {
            if (delta < 0) dir = DIRECTION_DECREASING;
            if (delta > 0) dir = DIRECTION_INCREASING;
        }

        if (delta == 0) return false;
        if (delta < -3) return false;
        if (delta > 3)  return false;
        if (dir == DIRECTION_DECREASING && delta > 0) return false;
        if (dir == DIRECTION_INCREASING && delta < 0) return false;
    }

    return true;
}

int part_two(int argc, char **argv) {
    int part_two_answer = 0;

    GupArena a = gup_arena_create();

    const char *file_path = argc > 1 ? argv[1] : "input.txt";
    GupArrayString file_lines = gup_file_read_lines_arena(&a, file_path);

    for (int i = 0; i < file_lines.count; i++) {
        GupString line = file_lines.data[i];
        GupArrayString tokens = gup_string_split_arena(&a, line, ' ');

        if (is_line_safe(tokens)) {
            part_two_answer++;
        } else {
            // Brute force taking each token out and seeing if it's safe.
            for (int j = 0; j < tokens.count; j++) {
                GupArrayString tokens_without_one = gup_array_string_remove_at_index_arena(&a, tokens, j);

                if (is_line_safe(tokens_without_one)) {
                    part_two_answer++;
                    break;
                }
            }
        }
    }   

    gup_arena_destroy(&a);

    return part_two_answer;
}

int main(int argc, char **argv) {
    printf("part 1: %d\n", part_one(argc, argv));
    printf("part 2: %d\n", part_two(argc, argv));

    return 0;
}
