#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/utsname.h>

struct Output{
  char os[128];
  char kernel[128];
  char cpu[128];
  char memory[128];  
};

char *get_os() 
{
  FILE *os_release;
  char line[1000];
  os_release = fopen("/etc/os-release", "r");
  static char os[1000];

  while (NULL != fgets(line, 1000, os_release)) {
    if (1 == sscanf(line, "PRETTY_NAME=\"%[^\"]",os)) {
      break;
    }
  }
  fclose(os_release);

  return os;
}

char *get_kernel()
{
  FILE *version;
  char line[1000];
  version = fopen("/proc/version", "r");
  static char kernel[1000];

  while (NULL != fgets(line, 1000, version)) {
    if (1 == sscanf(line, "Linux version %[^ ]",kernel)) {
      break;
    }
  }
  fclose(version);

  return kernel;
}

char *get_cpu()
{
  FILE *cpuinfo;
  char line[1000];
  cpuinfo = fopen("/proc/cpuinfo", "r");
  static char cpu[1000];

  while (NULL != fgets(line, 1000, cpuinfo)) {
    if (1 == sscanf(line, "model name : %[^\n]",cpu)){
      break;
    }
  }
  fclose(cpuinfo);

  return cpu;
}

char *get_memory()
{
  FILE *meminfo;
  char line[1000];
  meminfo = popen("free -h", "r");
  char totmem[1000];
  char usemem[1000];
  static char mem[1000];

  while (NULL != fgets(line, 1000, meminfo)) {
    if (2 == sscanf(line, "Mem: %[^ ] %[^ ]",totmem, usemem)) {
      break;
    }
  }
  fclose(meminfo);

  strcat(totmem, "B");
  strcat(usemem, "B");

  strcpy(mem, usemem);
  strcat(mem, " / ");
  strcat(mem, totmem);
  return mem;
}

int main(void) {
  char *os_name = get_os();
  printf("OS: %s\n", os_name);
  char *kernel = get_kernel();
  printf("Kernel: %s\n", kernel);
  char *cpu = get_cpu();
  printf("CPU: %s\n", cpu);
  char *mem = get_memory();
  printf("Memory: %s\n", mem);

  return 0;
}
