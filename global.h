#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <curses.h>

#include "libmorf.h"

#include "valsi.h"
#include "ui.h"
#include "parse.h"
#include "info.h"
#include "table.h"
#include "transcription.h"
#include "choice.h"

#include "db/gismu.h"
#include "db/cmavo.h"
#include "db/lujvo.h"

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))
