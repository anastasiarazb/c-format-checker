#include <stdio.h>


int reverse(int str[], int i);

int main(void)
{
    int a[64], i, j, n, base;
    printf("Enter the number and the new base:\n");
    scanf("%d %d", &n, &base);
    for (i = 0; n; n = n / base, i++) a[i] = n % base;
    for (j = i - 1; j >= 0; j--){
        if (a[j] >= 0 && a[j] <= 9) a[j] += '0';
        else a[j] += 'a' - 10;
    }
    reverse(a, i);
    for (j=0; j<i; j++) printf("%c", a[j]);
    putchar('\n');
    return 0;
}
int reverse(int str[], int i){
    int temp, j;
    for (j = 0; j < i / 2;){
        temp = str[j];
        str[j++] = str[--i];
        str[i] = temp;
    }
    return (*str);
}
