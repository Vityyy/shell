#ifndef EXEC_H
#define EXEC_H

#include "defs.h"
#include "types.h"
#include "utils.h"
#include "freecmd.h"
#include "wrappers.h"

extern struct cmd *parsed;
extern stack_t ss;

void exec_cmd(struct cmd *c);

#endif  // EXEC_H
