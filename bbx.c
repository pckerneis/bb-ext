#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if(argc < 2) {
    fprintf(stderr, "Usage: %s \"FORMULA\"\n", argv[0]);
    fprintf(stderr, "Example: %s \"sine(t, 440)\"\n", argv[0]);
    return 1;
  }

  const char *formula=argv[1];

  fprintf(stdout, "Create temp file...\n");

  FILE *f=fopen("/tmp/bbtmp.c", "w");
  if(!f){perror("fopen");return 1;}

  fprintf(stdout, "Writing...\n");

  fprintf(
    f,
    "#include <stdio.h>\n"
    "#include <stdint.h>\n"
    "#include <math.h>\n"
    "#ifndef M_PI\n#define M_PI 3.14159265358979323846\n#endif\n"
    "#define SR 8000\n"
    "static inline double sine3(double pos, double freq, double phase){ double tsec = pos / (double)SR; return sin(2.0*M_PI*(freq*tsec) + phase); }\n"
    "static inline double sine2(double pos, double freq){ return sine3(pos, freq, 0.0); }\n"
    "#define GET_MACRO(_1,_2,_3,NAME,...) NAME\n"
    "#define sine(...) GET_MACRO(__VA_ARGS__, sine3, sine2)(__VA_ARGS__)\n"
    "int main(){unsigned int t=0;for(;;t++){double v=(%s);if(v>1) v=1; if(v<-1) v=-1; int16_t s=(int16_t)lrint(v*32767.0); putchar(s & 0xFF); putchar((s>>8)&0xFF);} }\n",
    formula
  );
  fclose(f);


  fprintf(stdout, "Compiling...\n");

  system("gcc /tmp/bbtmp.c -o /tmp/bbtmp -lm && /tmp/bbtmp | aplay -f S16_LE -r 8000");
  return 0;
}
