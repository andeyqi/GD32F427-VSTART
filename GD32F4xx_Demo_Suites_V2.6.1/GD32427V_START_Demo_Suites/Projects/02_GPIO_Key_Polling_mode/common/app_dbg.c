#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"
#include "littleshell.h"


#define logp(format, ...) do { \
    printf(format, ##__VA_ARGS__); \
    printf("\r\n"); \
} while (0)

static unsigned int prv_issue_breakpoint(char argc, char *argv[]) {
  __asm("bkpt 1");
  return 0;
}
LTSH_FUNCTION_EXPORT(prv_issue_breakpoint,"issue breakponit");

static unsigned int prv_dump_fpb_config(char argc, char *argv[]) {
  fpb_dump_breakpoint_config();
  return 0;
}
LTSH_FUNCTION_EXPORT(prv_dump_fpb_config,"prv_dump_fpb_config");


static int prv_fpb_set_breakpoint(char argc, char *argv[]) {
  if (argc < 3) {
    logp("Expected [Comp Id] [Address]");
    return -1;
  }

  size_t comp_id = strtoul(argv[1], NULL, 0x0);
  uint32_t addr = strtoul(argv[2], NULL, 0x0);

  bool success = fpb_set_breakpoint(comp_id, addr);
  logp("Set breakpoint on address 0x%x in FP_COMP[%d] %s", addr,
              (int)comp_id, success ? "Succeeded" : "Failed");

  return success ? 0 : -1;
}
LTSH_FUNCTION_EXPORT(prv_fpb_set_breakpoint,"prv_fpb_set_breakpoint");


static unsigned int dbg_enable(char argc,char ** argv)
{
    debug_monitor_enable();
    return 0;
}
LTSH_FUNCTION_EXPORT(dbg_enable,"debug monitor enable");


unsigned int dbg_disable(char argc,char ** argv)
{
    debug_monitor_disable();
    return 0;
}
LTSH_FUNCTION_EXPORT(dbg_disable,"debug monitor disable");

