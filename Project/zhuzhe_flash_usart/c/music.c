#include "music.h"
#include "s1.c"
#include "s2.c"
#include "s3.c"
#include "s4.c"
#include "s5.c"

const char * music_index[MUSIC_NUM]={s1,s2,s3,s4,s5};
unsigned int music_len[MUSIC_NUM]={sizeof(s1),sizeof(s2),sizeof(s3),sizeof(s4),sizeof(s5)};
