/*
Copyright (c) 2014, Prashant Karmakar, webster15july@gmail.com

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

void
die (char *msg)
{
  fputs (msg, stderr);
  exit (0);
}

typedef unsigned char b8;
typedef unsigned short b16;
typedef struct
{
  b16 i_v[81];
  b8 left;
} s00d;
b16 wait = 0x20;
b16 open = 0x10;
b16 may_b[9] = {
  0x4000, 0x2000, 0x1000,
  0x0800, 0x0400, 0x0200,
  0x0100, 0x0080, 0x0040
};

b16 data = 0xf;

#include <string.h>

int
scope_gen (b8 clr[81 * 20])
{
  b8 i, j, r, c, gr, gc;
  b8 *eye;
  b8 scope[81];
  /* malloc from stdlib.h */
  for (i = 0, eye = clr; i < 81; i++)
    {
      memset (scope, 0, 81 * sizeof (b8));
      /* from string.h */
      r = i / (9);
      c = i % (9);
      gr = (r / 3) * 3;
      gc = (c / 3) * 3;
      for (j = 0; j < 9; j++)
        {
          scope[r * 9 + j]++;
          scope[j * 9 + c]++;
          scope[(gr + j / 3) * 9 + gc + j % 3]++;
        }
      scope[i] = 0;
      for (j = 0; j < 81; j++)
        if (scope[j])
          {
            *eye = j;
            eye++;
          }
    }
  return 0;
}

b8 clr[81 * 20];
typedef struct
{
  s00d *s;
  int p;
  int v;
} state;
struct
{
  char *buf;
  state *stack;
  int top;
  int pos;
} d;
#include <unistd.h>
char *bound;
b8 bounded;

#include <signal.h>
void
rq (int sig)
{
  if (!d.buf)
    {
      fputs ("\ndump requested\n", stderr);
      if (!(d.buf = malloc (81 * 414)))
        die ("RAM denied");
      memset (d.buf, 0, 81 * 414);
    }
}

int
dmp (s00d * s, int p, int v)
{
  char *eye;
  b16 *sun;
  int l;

  for (l = 0, eye = d.buf + d.pos, sun = s->i_v; l < 405; sun++, eye += 5)
    l += sprintf (eye, "%04x ", (int) *sun);

  l += sprintf (eye, "%02x ", (int) s->left);
  eye += 3;
  l += sprintf (eye, "%02x ", p);
  eye += 3;
  l += sprintf (eye, "%02x\n", v);

  return l;                     /*the characters written d.pos to be incremented outside */
}

int
burn (state * n)
{
  for (; n >= d.stack; n--)
    free (n->s);
  free (d.stack);
  d.stack = 0;
  free (d.buf);
  d.buf = 0;
  return 1;
}

int
rd ()
{
  FILE *dump;
  int depth, i;
  char *line, *this, *nxt;
  b16 *eye;
  state *now;

  if (!(dump = fopen ("dump", "rb")))
    return 2;
  if (!(d.buf = (char *) malloc (81 * 414)))
    die ("RAM denied\n");
  memset (d.buf, 0, 81 * 414);
  depth = fread (d.buf, 414, 81, dump); /*if CRLF & fread doesnt copy the last incomplete line */
  fclose (dump);
  if (!depth)
    return 1;

  if (!(d.stack = (state *) malloc (sizeof (state) * depth)))
    die ("RAM denied\n");
  memset (d.stack, 0, sizeof (state) * depth);

  line = strtok (d.buf, "\r\n");
  d.top = 0;
  while (line && depth)
    {
      now = d.stack + d.top;
      if (!(now->s = (s00d *) malloc (sizeof (s00d))))
        die ("RAM denied\n");
      for (i = 0, this = line, eye = now->s->i_v; i < 81; i++, eye++)
        {
          *eye = (b16) strtoul (this, &nxt, 16);
          if ((this + 5 != nxt) && (i != 0 || (this + 4 != nxt)))
            return burn (now);
          this = nxt;
        }
      now->s->left = (b8) strtoul (this, &nxt, 16);
      if (this + 3 != nxt)
        return burn (now);
      this = nxt;
      now->p = (b8) strtoul (this, &nxt, 16);
      if (this + 3 != nxt)
        return burn (now);
      this = nxt;
      now->v = (b8) strtoul (this, &nxt, 16);
      if (this + 3 != nxt)
        return burn (now);
      line = strtok (0, "\r\n");
      d.top++;
      depth--;
    }
  free (d.buf);
  d.buf = 0;
  return 0;
}

typedef struct
{
  unsigned long *i;
  unsigned int n;
} iint;

iint *cnt;

iint *
new (unsigned bound)
{
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



int
add (iint * eye, unsigned long x)
{
  int ret = 0;
  *(eye->i) += x;
  if (*(eye->i) < x)
    {
      if (--(eye->n))
        {
          eye->i++;
          ret = add (eye, 1L);
          eye->i--;
        }
      else
        ret = 1;
      (eye->n)++;
    }
  return ret;
}

int
vyu (char *eye, iint * num)
{
  static char zp_lu[7];
  unsigned i;
  i = num->n;
  sprintf (zp_lu, "%%0%lulx", 2 * sizeof (unsigned long));
  while (i--)
    eye += sprintf (eye, zp_lu, cnt->i[i]);     // woah!
  strcat (eye, "\n");
  return 0;
}

b8 def[10] = "123456789";
b8 out[83] = "";

int
first (s00d * puzl, b8 * p)
{
  b16 *sun;
  for (sun = (&puzl->i_v[0]), *p = 0; *p < 81; (*p)++, sun++)
    if (((*sun) & wait))
      return 1;
  return 0;
}

void
S00D (b8 * in, s00d * puzl)
{
  b16 *eye, idea_v;
  b8 i;
  puzl->left = 81;
  eye = puzl->i_v;
  for (i = 0; i < 81 && *in; i++, eye++, in++)
    {
      if ((idea_v = (*in) - '1') < 9)
        *eye = idea_v | open | may_b[idea_v];
      else
        *eye = 0x7fe7;
    }
  for (; i < 81; i++, eye++)
    *eye = 0x7fe7;
}

void
STR (b8 * buf, s00d * puzl)
{
  b16 *sun;
  b8 *eye;
  b8 i;
  eye = buf;
  for (sun = puzl->i_v, i = 0; i < 81; i++, sun++, eye++)
    {
      if ((*sun) & open)
        *eye = '?';
      else
        *eye = (b8) ('1' + ((*sun) & data));
    }
  *eye = '\n';
  *(++eye) = '\0';
}

int
idea (s00d * puzl, b8 * p, b8 * v)
{
  b16 *sun;
  for (sun = puzl->i_v, *p = 0; *p < 81; (*p)++, sun++)
    {
      if ((*sun) & open)
        {
          *v = (b8) ((*sun) & data);
          return 1;
        }
    }
  return 0;
}

int
rm (b16 * eye, b8 v)
{
  if ((*eye) & may_b[v])
    {
      if (!((*eye) & wait))
        return 1;
      (*eye) &= ~may_b[v];
      (*eye)--;
      if ((*eye) & open)
        {
          for (v = 0; v < 9; v++)
            if ((*eye) & may_b[v])
              break;
          (*eye) = ((b16) v) | open | may_b[v];
        }
    }
  return 0;
}

int
trial (b16 * cell, b8 * v)
{
  for (; *v < 9; (*v)++)
    if ((*cell) & may_b[*v])
      return 1;
  return 0;
}

int
hook (s00d * puzl)
{
  b8 x;
  b8 *eye;
  b8 pos, val;
  if (d.stack)
    {
      if (d.top)
        return 0;
      free (d.stack);
      d.stack = 0;
      d.top = -1;
      return 1;
    }
  while (idea (puzl, &pos, &val))
    {
      for (x = 0, eye = &clr[pos * 20]; x < 20; x++, eye++)
        {
          if (rm (&(puzl->i_v[*eye]), val))
            return -1;
        }
      puzl->i_v[pos] &= (~open);
      if (!(--(puzl->left)))
        {
          STR (out, puzl);
          fputs (out, stdout);
          add (cnt, 1L);        /*cant possibly overflow, should i check? */
          if (bounded)
            {
              vyu (out, cnt);   /* haha! passing a global :D */
              if (strstr (out, bound)
                  && strcmp (strstr (out, bound), bound) >= 0)
                raise (SIGINT);
            }
          vyu (out, cnt);
          if (d.buf)
            {
              d.pos = 0;
              return 2;
            }
          else
            return 1;
        }
    }
  return 0;
}

int
crook (s00d * master)
{
  s00d *copy;
  b16 *mc, *cc;
  b8 pos, val = 0, dummy;
  if (d.stack)
    {
      if (!d.top--)
        die ("stack underflow");
      copy = (d.stack + d.top)->s;
      pos = (b8) (d.stack + d.top)->p;
      val = (b8) (d.stack + d.top)->v;
      mc = &master->i_v[pos];
      cc = &copy->i_v[pos];
    }
  else
    {
      if (!(copy = (s00d *) malloc (sizeof (s00d))))
        die ("RAM denied\n");
      if (first (master, &pos))
        {
          mc = &master->i_v[pos];
          cc = &copy->i_v[pos];
        }
      else
        return 1;
    }
  while (d.stack || trial (mc, &val))
    {
      if (!d.stack)
        {
          *copy = *master;
          *cc = (((b16) val) | open | may_b[val]);
        }
      switch (squash (copy))
        {
        case 1:
          val++;
          break;
        case 2:
          d.pos += dmp (copy, (int) pos, (int) val);
          return 2;
        case -1:
          rm (mc, val);         /* should i check? */
          ;
          if (idea (master, &pos, &dummy) && (dummy > val))
            {
              free (copy);
              return 0;
            }
        }
    }
  free (copy);
  return 1;
}

int
squash (s00d * puzl)
{
  int ret;
  /* TODO make an alternative method.
   * that does only one solution per call
   * call it try_again or next
   *
   * REF: SICP section 3.5
   * just remember that the thing this method
   * generates is not a pure stream! it mutates
   * the object it is called with!
   * whenever `guess` function finds
   * that the probable it chose
   * can not produce any valid solution. it
   * _removes that probable from original structure_
   * and calls ` hook` function on it.
   */
  do
    if (ret = hook (puzl))
      return ret;
  while (!(ret = crook (puzl)));
  return ret;
}

#include <ctype.h>
int
main (int argc, char **argv)
{
  s00d *master;
  char *in;
  FILE *dump;
  int opt;

  d.buf = 0;
  d.stack = 0;
  d.pos = d.top = -1;

  scope_gen (clr);

  while ((opt = getopt (argc, argv, "n:")) != -1)
    {
      switch (opt)
        {
        case 'n':
          if ((strlen (optarg) - strspn (optarg, "0")) > 33)
            {
              fprintf (stderr,
                       "bound exceeds size of solution set. hence ignored.\n");
              break;
            }
          if (strlen (optarg) != strspn (optarg, "0123456789abcdefABCDEF"))
            {
              fprintf (stderr, "bound must be a hexadecimal number\n");
              exit (EXIT_FAILURE);
            }
          bound = optarg;
          bounded = 1;
          break;
        default:               /* '?' */
          fprintf (stderr, "Usage: %s [-n bound] [sudoku]\n", argv[0]);
          exit (EXIT_FAILURE);
        }
    }


  if (optind < argc)
    {
      in = argv[optind];
    }
  else
    {
      if (rd ())
        in = def;
      else
        {
          d.top--;
          master = (d.stack + d.top)->s;
        }
    }

  if (!d.stack)
    {
      if (!(master = (s00d *) malloc (sizeof (s00d))))
        die ("RAM denied\n");
      S00D (in, master);
    }

  if (signal (SIGINT, rq) == SIG_ERR)
    fputs ("could not enable dump feature\n", stderr);
  if (signal (SIGPIPE, rq) == SIG_ERR)
    fputs ("could not enable dump feature\n", stderr);
  else
    fputs ("dump feature enabled\n", stderr);

  cnt = new (10);

  switch (squash (master))
    {
    case -1:
      fputs ("no solution\n", stderr);
      break;
    case 2:
      dmp (master, 10, 10);
      if (!(dump = fopen ("dump", "wb")))
        die ("couldn't create dumpfile\n");
      //rather dump to stderr
      fputs (d.buf, dump);
      fclose (dump);
      free (d.buf);
    }
  free (master);

  *out = '#';
  vyu (out + 1, cnt);           // haha! passing a global :D
  fputs (out, stderr);

  return 0;
}

/* pk 20131110 2110 */
