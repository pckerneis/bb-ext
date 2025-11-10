#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if(argc < 2) {
    fprintf(stderr, "Usage: %s \"FORMULA\"\n", argv[0]);
    fprintf(stderr, "   or: %s -f <formula_file>\n", argv[0]);
    fprintf(stderr, "Options:\n  -sr <rate>   Sample rate (default 8000)\n");
    fprintf(stderr, "Example: %s \"sine(t, 440)\"\n", argv[0]);
    fprintf(stderr, "         %s -f examples/sine440.bbx -sr 11025\n", argv[0]);
    return 1;
  }

  const char *formula = NULL;
  const char *formula_path = NULL;
  char *file_buf = NULL;
  unsigned int sample_rate = 8000;

  for(int i=1;i<argc;i++){
    if(strcmp(argv[i], "-f") == 0 && i+1 < argc){
      formula_path = argv[++i];
    }else if(strcmp(argv[i], "-sr") == 0 && i+1 < argc){
      int sr = atoi(argv[++i]);
      if(sr > 0) sample_rate = (unsigned int)sr;
    }else if(argv[i][0] != '-'){
      if(!formula) formula = argv[i];
    }
  }

  if(formula_path){
    FILE *ff = fopen(formula_path, "rb");
    if(!ff){ perror("fopen formula file"); return 1; }
    if(fseek(ff, 0, SEEK_END) != 0){ perror("fseek"); fclose(ff); return 1; }
    long sz = ftell(ff);
    if(sz < 0){ perror("ftell"); fclose(ff); return 1; }
    if(fseek(ff, 0, SEEK_SET) != 0){ perror("fseek"); fclose(ff); return 1; }
    file_buf = (char*)malloc((size_t)sz + 1);
    if(!file_buf){ perror("malloc"); fclose(ff); return 1; }
    size_t rd = fread(file_buf, 1, (size_t)sz, ff);
    fclose(ff);
    file_buf[rd] = '\0';
    formula = file_buf;
  }

  if(!formula){
    fprintf(stderr, "No formula provided. Use inline formula or -f <file>.\n");
    return 1;
  }

  fprintf(stdout, "Create temp file...\n");

  FILE *f=fopen("/tmp/bbtmp.c", "w");
  if(!f){perror("fopen");return 1;}

  fprintf(stdout, "Writing...\n");

  fprintf(
    f,
    "#include <stdio.h>\n"
    "#include <stdint.h>\n"
    "#include <stdlib.h>\n"
    "#include <time.h>\n"
    "#include <math.h>\n"
    "#ifndef M_PI\n#define M_PI 3.14159265358979323846\n#endif\n"
    "#define SR %u\n"
    "static inline double sine3(double pos, double freq, double phase){ double tsec = pos / (double)SR; return sin(2.0*M_PI*(freq*tsec) + phase); }\n"
    "static inline double sine2(double pos, double freq){ return sine3(pos, freq, 0.0); }\n"
    "#define GET_MACRO(_1,_2,_3,NAME,...) NAME\n"
    "#define sine(...) GET_MACRO(__VA_ARGS__, sine3, sine2)(__VA_ARGS__)\n"
    /* Phase helpers and additional waveforms */
    "static inline double _phase_frac(double pos, double freq, double phase){ double tsec = pos / (double)SR; double theta = 2.0*M_PI*(freq*tsec) + phase; double cyc = fmod(theta, 2.0*M_PI); if(cyc < 0) cyc += 2.0*M_PI; return cyc / (2.0*M_PI); }\n"
    "static inline double saw3(double pos, double freq, double phase){ double frac = _phase_frac(pos,freq,phase); return 2.0*frac - 1.0; }\n"
    "static inline double saw2(double pos, double freq){ return saw3(pos,freq,0.0); }\n"
    "#define saw(...) GET_MACRO(__VA_ARGS__, saw3, saw2)(__VA_ARGS__)\n"
    "static inline double sq3(double pos, double freq, double phase){ double frac = _phase_frac(pos,freq,phase); return (frac < 0.5) ? 1.0 : -1.0; }\n"
    "static inline double sq2(double pos, double freq){ return sq3(pos,freq,0.0); }\n"
    "#define sq(...) GET_MACRO(__VA_ARGS__, sq3, sq2)(__VA_ARGS__)\n"
    "static inline double tri3(double pos, double freq, double phase){ double x = 2.0*_phase_frac(pos,freq,phase) - 1.0; return 1.0 - 2.0*fabs(x); }\n"
    "static inline double tri2(double pos, double freq){ return tri3(pos,freq,0.0); }\n"
    "#define tri(...) GET_MACRO(__VA_ARGS__, tri3, tri2)(__VA_ARGS__)\n"
    /* Random helpers */
    "static inline double rnd(void){ return rand() / (RAND_MAX + 1.0); }\n"
    "static inline double rndf1(double max){ return rnd() * max; }\n"
    "static inline double rndf2(double min, double max){ return min + rnd() * (max - min); }\n"
    "#define GET_MACRO2(_1,_2,NAME,...) NAME\n"
    "#define rndf(...) GET_MACRO2(__VA_ARGS__, rndf2, rndf1)(__VA_ARGS__)\n"
    "static inline int rndi1(int max){ if(max <= 0) return 0; return rand() % (max + 1); }\n"
    "static inline int rndi2(int min, int max){ if(max <= min) return min; return min + (rand() % (max - min + 1)); }\n"
    "#define rndi(...) GET_MACRO2(__VA_ARGS__, rndi2, rndi1)(__VA_ARGS__)\n"
    "int main(){ srand((unsigned)time(NULL)); unsigned int t=0; for(;;t++){ double v=(%s); if(v>1) v=1; if(v<-1) v=-1; int16_t s=(int16_t)lrint(v*32767.0); putchar(s & 0xFF); putchar((s>>8)&0xFF);} }\n",
    sample_rate,
    formula
  );
  fclose(f);


  fprintf(stdout, "Compiling...\n");

  char cmd[256];
  snprintf(cmd, sizeof(cmd), "gcc /tmp/bbtmp.c -o /tmp/bbtmp -lm && /tmp/bbtmp | aplay -f S16_LE -r %u", sample_rate);
  system(cmd);
  if(file_buf) free(file_buf);
  return 0;
}
