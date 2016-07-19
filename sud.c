/*
Copyright (c) 2016, Prashant Karmakar, webster15july@gmail.com

 Permission to use, copy, modify, and/or distribute this software for
 any purpose with or without fee is hereby granted, provided that
 the above copyright notice and this permission notice appear in
 all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
 THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


Internet Systems Consortium License
http://en.wikipedia.org/wiki/ISC_license
*/

#include <stdio.h>
#include <stdlib.h>

void die (char *msg) {
  fputs (msg, stderr);
  exit (0);
}

typedef struct {
  unsigned short i_v[81];
  unsigned char left;
} sudoku;
unsigned short wait = 0x20;
unsigned short open = 0x10;
unsigned short may_b[9] = {
  0x4000, 0x2000, 0x1000,
  0x0800, 0x0400, 0x0200,
  0x0100, 0x0080, 0x0040
};

unsigned short data = 0xf;

#include <string.h>

int scope_gen (unsigned char scope[81 * 20]) {
  unsigned char i, j, r, c, gr, gc;
  unsigned char *eye;
  unsigned char scope_seive[81];
  /* malloc from stdlib.h */
  for (i = 0, eye = scope; i < 81; i++) {
    memset (scope_seive, 0, 81 * sizeof (unsigned char));
    /* from string.h */
    r = i / (9);
    c = i % (9);
    gr = (r / 3) * 3;
    gc = (c / 3) * 3;
    for (j = 0; j < 9; j++) {
      scope_seive[r * 9 + j]++;
      scope_seive[j * 9 + c]++;
      scope_seive[(gr + j / 3) * 9 + gc + j % 3]++;
    }
    scope_seive[i] = 0;
    for (j = 0; j < 81; j++)
      if (scope_seive[j]) {
        *eye = j;
        eye++;
      }
  }
  return 0;
}

unsigned char scope[81 * 20];
typedef struct {
  sudoku *s;
  int p;
  int v;
} sudoku_state;
typedef struct {
  char *buffer;
  int position;

  sudoku_state *stack;
  int top;
} dump_struct;
dump_struct dump_data;
#include <signal.h>
void dump_request (int sig) {
  fputs ("\ndump requested\n", stderr);
  if (!(dump_data.buffer = malloc (81 * 414)))
    die ("RAM denied");
  memset (dump_data.buffer, 0, 81 * 414);
}
int sudoku_dump (sudoku * s, int p, int v, char * buffer) {
  char *eye;
  unsigned short *sun;
  int l;

  for (l = 0, eye = buffer, sun = s->i_v; l < 405;
       sun++, eye += 5)
    l += sprintf (eye, "%04x ", (int) *sun);

  l += sprintf (eye, "%02x ", (int) s->left);
  eye += 3;
  l += sprintf (eye, "%02x ", p);
  eye += 3;
  l += sprintf (eye, "%02x\n", v);

  return l;                     /* the characters written dump_data.position to be
                                   incremented outside */
}
int burn (sudoku_state * n) {
  for (; n >= dump_data.stack; n--)
    free (n->s);
  free (dump_data.stack);
  dump_data.stack = 0;
  free (dump_data.buffer);
  dump_data.buffer = 0;
  return 1;
}
int dumpfile_try_read () {
  FILE *dumpfile;
  int depth, i;
  char *line, *this, *nxt;
  unsigned short *eye;
  sudoku_state *now;

  if (!(dumpfile = fopen ("dump", "rb")))
    return 2;
  if (!(dump_data.buffer = (char *) malloc (81 * 414)))
    die ("RAM denied\n");
  memset (dump_data.buffer, 0, 81 * 414);
  depth = fread (dump_data.buffer, 414, 81, dumpfile);  /* if CRLF & fread doesnt
                                                   copy the last incomplete
                                                   line */
  fclose (dumpfile);
  if (!depth)
    return 1;

  if (!(dump_data.stack = (sudoku_state *) malloc (sizeof (sudoku_state) * depth)))
    die ("RAM denied\n");
  memset (dump_data.stack, 0, sizeof (sudoku_state) * depth);

  line = strtok (dump_data.buffer, "\r\n");
  dump_data.top = 0;
  while (line && depth) {
    now = dump_data.stack + dump_data.top;
    if (!(now->s = (sudoku *) malloc (sizeof (sudoku))))
      die ("RAM denied\n");
    for (i = 0, this = line, eye = now->s->i_v; i < 81; i++, eye++) {
      *eye = (unsigned short) strtoul (this, &nxt, 16);
      if ((this + 5 != nxt) && (i != 0 || (this + 4 != nxt)))
        return burn (now);
      this = nxt;
    }
    now->s->left = (unsigned char) strtoul (this, &nxt, 16);
    if (this + 3 != nxt)
      return burn (now);
    this = nxt;
    now->p = (unsigned char) strtoul (this, &nxt, 16);
    if (this + 3 != nxt)
      return burn (now);
    this = nxt;
    now->v = (unsigned char) strtoul (this, &nxt, 16);
    if (this + 3 != nxt)
      return burn (now);
    line = strtok (0, "\r\n");
    dump_data.top++;
    depth--;
  }
  free (dump_data.buffer);
  dump_data.buffer = 0;
  return 0;
}

typedef struct {
  unsigned long *i;
  unsigned int n;
} iint;

iint *count;

iint *iint_new (unsigned bound) {
  long szl;
  iint *egg;
  if (!(egg = malloc (sizeof (iint))))
    die ("RAM denied\n");
  szl = sizeof (long);
  egg->n = bound / szl + ((bound % szl) && 1);
  if (!(egg->i = malloc (egg->n * szl)))
    die ("RAM denied\n");
  memset (egg->i, 0, egg->n * szl);
  return egg;
}



int iint_add (iint * eye, unsigned long x) {
  int ret = 0;
  *(eye->i) += x;
  if (*(eye->i) < x) {
    if (--(eye->n)) {
      eye->i++;
      ret = iint_add (eye, 1L);
      eye->i--;
    }
    else
      ret = 1;
    (eye->n)++;
  }
  return ret;
}

int iint_to_string (char *eye, iint * num) {
  static char zp_lu[7];
  unsigned i;
  i = num->n;
  sprintf (zp_lu, "%%0%lulx", 2 * sizeof (unsigned long));
  while (i--)
    eye += sprintf (eye, zp_lu, num->i[i]);     /* woah! */
  strcat (eye, "\n");
  return 0;
}

char def[10] = "123456789";
char out[83] = "";

int sudoku_first_waiting_cell (sudoku * puzl, unsigned char *p) {
  unsigned short *sun;
  for (sun = (&puzl->i_v[0]), *p = 0; *p < 81; (*p)++, sun++)
    if (((*sun) & wait))
      return 1;
  return 0;
}
void sudoku_init (char *in, sudoku * puzl) {
  unsigned short *eye, idea_v;
  unsigned char i;
  puzl->left = 81;
  eye = puzl->i_v;
  for (i = 0; i < 81 && *in; i++, eye++, in++) {
    if ((idea_v = (*in) - '1') < 9)
      *eye = idea_v | open | may_b[idea_v];
    else
      *eye = 0x7fe7;
  }
  for (; i < 81; i++, eye++)
    *eye = 0x7fe7;
}
void sudoku_to_string (char *buffer, sudoku * puzl) {
  unsigned short *sun;
  char *eye;
  unsigned char i;
  eye = buffer;
  for (sun = puzl->i_v, i = 0; i < 81; i++, sun++, eye++) {
    if ((*sun) & open)
      *eye = '?';
    else
      *eye = (unsigned char) ('1' + ((*sun) & data));
  }
  *eye = '\n';
  *(++eye) = '\0';
}
int sudoku_cell_to_fill (sudoku * puzl, unsigned char *p, unsigned char *v) {
  unsigned short *sun;
  for (sun = puzl->i_v, *p = 0; *p < 81; (*p)++, sun++) {
    if ((*sun) & open) {
      *v = (unsigned char) ((*sun) & data);
      return 1;
    }
  }
  return 0;
}
int remove_probable (unsigned short *eye, unsigned char v) {
  if ((*eye) & may_b[v]) {
    if (!((*eye) & wait))
      return 1;
    (*eye) &= ~may_b[v];
    (*eye)--;
    if ((*eye) & open) {
      for (v = 0; v < 9; v++)
        if ((*eye) & may_b[v])
          break;
      (*eye) = ((unsigned short) v) | open | may_b[v];
    }
  }
  return 0;
}
int cell_value_to_try (unsigned short *cell, unsigned char *v) {
  for (; *v < 9; (*v)++)
    if ((*cell) & may_b[*v])
      return 1;
  return 0;
}
int hook (sudoku * puzl) {
  unsigned char x;
  unsigned char *eye;
  unsigned char pos, val;
  if (dump_data.stack) {
    if (dump_data.top)
      return 0;
    free (dump_data.stack);
    dump_data.stack = 0;
    dump_data.top = -1;
    return 1;
  }
  while (sudoku_cell_to_fill (puzl, &pos, &val)) {
    for (x = 0, eye = &scope[pos * 20]; x < 20; x++, eye++) {
      if (remove_probable (&(puzl->i_v[*eye]), val))
        return -1;
    }
    puzl->i_v[pos] &= (~open);
    if (!(--(puzl->left))) {
      sudoku_to_string (out, puzl);
      fputs (out, stdout);
      iint_add (count, 1L);            /* cant possibly overflow, should i check? */
      if (dump_data.buffer) {
        dump_data.position = 0;
        return 2;
      }
      else
        return 1;
    }
  }
  return 0;
}
int crook (sudoku * master) {
  sudoku *copy;
  unsigned short *mc, *cc;
  unsigned char pos, val = 0, dummy;
  if (dump_data.stack) {
    if (!dump_data.top--)
      die ("stack underflow");
    copy = (dump_data.stack + dump_data.top)->s;
    pos = (unsigned char) (dump_data.stack + dump_data.top)->p;
    val = (unsigned char) (dump_data.stack + dump_data.top)->v;
    mc = &master->i_v[pos];
    cc = &copy->i_v[pos];
  }
  else {
    if (!(copy = (sudoku *) malloc (sizeof (sudoku))))
      die ("RAM denied\n");
    if (sudoku_first_waiting_cell (master, &pos)) {
      mc = &master->i_v[pos];
      cc = &copy->i_v[pos];
    }
    else
      return 1;
  }
  while (dump_data.stack || cell_value_to_try (mc, &val)) {
    if (!dump_data.stack) {
      *copy = *master;
      *cc = (((unsigned short) val) | open | may_b[val]);
    }
    switch (squash (copy)) {
    case 1:
      val++;
      break;
    case 2:
      dump_data.position += sudoku_dump (copy, (int) pos, (int) val, dump_data.buffer + dump_data.position);
      return 2;
    case -1:
      remove_probable (mc, val);             /* should i check? */
      ;
      if (sudoku_cell_to_fill (master, &pos, &dummy) && (dummy > val)) {
        free (copy);
        return 0;
      }
    }
  }
  free (copy);
  return 1;
}
int squash (sudoku * puzl) {
  int ret;
  do
    if (ret = hook (puzl))
      return ret;
  while (!(ret = crook (puzl)));
  return ret;
}




int main (int argc, char **argv) {
  sudoku *master;
  char *in;
  FILE *dumpfile;

  dump_data.buffer = 0;
  dump_data.stack = 0;
  dump_data.position = dump_data.top = -1;

  scope_gen (scope);

  in = argc > 1 ? argv[1] : def;

  if (argc > 1 || dumpfile_try_read ()) {
    /* we have not read a dumpfile */
    if (!(master = (sudoku *) malloc (sizeof (sudoku))))
      die ("RAM denied\n");
    sudoku_init (in, master);
  }
  else {
    /* we have read dumpfile */
    dump_data.top--;
    master = (dump_data.stack + dump_data.top)->s;
    /* pop a sudoku_state */
  }

  if (signal (SIGINT, dump_request) == SIG_ERR)
    fputs ("could not enable dump feature\n", stderr);
  else
    fputs ("dump feature enabled\n", stderr);

  count = iint_new (10);

  switch (squash (master)) {
  case -1:
    fputs ("no solution\n", stderr);
    break;
  case 2:
    sudoku_dump (master, 10, 10, dump_data.buffer + dump_data.position);
    if (!(dumpfile = fopen ("dump", "wb"))) {
      fputs ("couldn't create dumpfile\n", stderr);
      fputs (dump_data.buffer, stderr);
    }
    else {
      fputs (dump_data.buffer, dumpfile);
      fclose (dumpfile);
    }
    free (dump_data.buffer);
  }
  free (master);

  *out = '#';
  iint_to_string (out + 1, count);        /* haha! passing a global :D */
  fputs (out, stderr);

  return 0;
}
