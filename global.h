#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <curses.h>

#include "libmorf.h"

#include "valsi.h"
#include "ui.h"
#include "parse.h"
#include "info.h"
#include "table.h"

#include "db/gismu.h"
#include "db/cmavo.h"

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))
