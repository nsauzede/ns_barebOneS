#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define EXPECT_EQ(v1, v2, ...) do{if (v1 == v2){printf("[ SUCCESS ]\n");}else{printf("[ FAIL ] ");printf(__VA_ARGS__);printf("\n");exit(1);}}while(0)
#define EXPECT_STR_EQ(s1, s2) EXPECT_EQ(strcmp(s1, s2), 0, "expected strings s1=[%s] s2=[%s] equal", s1, s2)
