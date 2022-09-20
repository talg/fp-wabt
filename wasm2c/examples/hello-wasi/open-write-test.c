#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char * argv[]) {
    printf("printing args");
    for (int i = 0; i < argc ; i++) {
	printf("[%d] %s \n", i, argv[i]);
    }
    printf("Running simple file I/O test\n");
    char * hello_str = strdup("hello world");
    puts(hello_str);
    printf("writing test.out\n");
    FILE * fp = fopen("test.out", "w+");
    assert(fp != NULL);
    fprintf(fp,"hello filesystem\n");
    fclose(fp);
    printf("writing /tmp/test.out\n");
    fp = fopen("/tmp/test.out", "w+");
    assert(fp != NULL);
    fprintf(fp,"hello filesystem\n");
    fclose(fp);
    return 0;
}
