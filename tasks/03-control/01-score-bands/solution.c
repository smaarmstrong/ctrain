#include <stdio.h>

int main(void)
{
    int score;
    while (scanf("%d", &score) == 1) {
        if (score < 0 || score > 100) {
            printf("%d: invalid\n", score);
            continue;
        }
        char letter;
        if (score >= 90)
            letter = 'A';
        else if (score >= 80)
            letter = 'B';
        else if (score >= 70)
            letter = 'C';
        else if (score >= 60)
            letter = 'D';
        else
            letter = 'F';
        printf("%d: %c (%s)\n", score, letter, score >= 60 ? "pass" : "fail");
    }
    return 0;
}
