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

void die(char *msg){
    fputs(msg, stderr);
    exit(0);
}

typedef struct {
    unsigned short i_v[81];
    unsigned char left;
} sudoku;
unsigned short wait=0x20;
unsigned short open=0x10;
unsigned short may_b[9]={
    0x4000, 0x2000, 0x1000,
    0x0800, 0x0400, 0x0200,
    0x0100, 0x0080, 0x0040
};
unsigned short data=0xf;

#include <string.h>

int scope_gen(unsigned char scope[81*20]){
   unsigned char i, j,
      r, c, gr, gc;
   unsigned char *eye;
   unsigned char scope_seive[81];
   /* malloc from stdlib.h */
   for (i=0, eye=scope;i<81;i++) {
      memset(scope_seive, 0, 81*sizeof(unsigned char));
      /* from string.h */
      r=i/(9);
      c=i%(9);
      gr=(r/3)*3;
      gc=(c/3)*3;
      for (j=0; j<9; j++) {
         scope_seive[r*9+j]++;
         scope_seive[j*9+c]++;
         scope_seive[(gr+j/3)*9+gc+j%3]++;
      }
      scope_seive[i]=0;
      for(j=0; j<81; j++) if (scope_seive[j]) {*eye = j; eye++;}
   }
   return 0;
}
unsigned char scope[81*20];
typedef struct {
    sudoku *s;
    int p;
    int v;
} sudoku_state;
struct {
    char *buffer;
    int position;

    sudoku_state *stack;
    int top;
} d;
#include <signal.h>
void rq(int sig){
    fputs("\ndump requested\n", stderr);
    if(!(d.buffer=malloc(81*414))) die("RAM denied");
    memset(d.buffer, 0, 81*414);
}
int dmp(sudoku *s, int p, int v){
    char *eye;
    unsigned short *sun;
    int l;

    for(l=0, eye=d.buffer+d.position, sun=s->i_v; l<405; sun++, eye+=5) l+=sprintf(eye, "%04x ", (int)*sun);

    l+=sprintf(eye, "%02x ", (int)s->left); eye+=3;
    l+=sprintf(eye, "%02x ", p); eye+=3;
    l+=sprintf(eye, "%02x\n", v);

    return l; /* the characters written d.position to be incremented outside */
}
int burn(sudoku_state *n){
    for(;n>=d.stack;n--) free(n->s);
    free(d.stack);
    d.stack=0;
    free(d.buffer);
    d.buffer=0;
    return 1;
}
int dumpfile_try_read(){
    FILE *dumpfile;
    int depth, i;
    char *line, *this, *nxt;
    unsigned short *eye;
    sudoku_state *now;

    if(!(dumpfile=fopen("dump", "rb"))) return 2;
    if(!(d.buffer=(char *)malloc(81*414))) die("RAM denied\n");
    memset(d.buffer, 0, 81*414);
    depth=fread(d.buffer, 414, 81, dumpfile); /* if CRLF & fread doesnt copy the last incomplete line */
    fclose(dumpfile);
    if(!depth) return 1;

    if(!(d.stack=(sudoku_state *)malloc(sizeof(sudoku_state)*depth))) die("RAM denied\n");
    memset(d.stack, 0, sizeof(sudoku_state)*depth);

    line=strtok(d.buffer,"\r\n");
    d.top=0;
    while(line&&depth){
        now=d.stack+d.top;
        if(!(now->s=(sudoku *)malloc(sizeof(sudoku)))) die("RAM denied\n");
        for(i=0,this=line,eye=now->s->i_v;i<81;i++,eye++){
            *eye=(unsigned short)strtoul(this,&nxt,16);
            if((this+5!=nxt)&&(i!=0||(this+4!=nxt))) return burn(now);
            this=nxt;
        }
        now->s->left=(unsigned char)strtoul(this,&nxt,16);
        if(this+3!=nxt) return burn(now);
        this=nxt;
        now->p=(unsigned char)strtoul(this,&nxt,16);
        if(this+3!=nxt) return burn(now);
        this=nxt;
        now->v=(unsigned char)strtoul(this,&nxt,16);
        if(this+3!=nxt) return burn(now);
        line=strtok(0,"\r\n");
        d.top++;
        depth--;
    }
    free(d.buffer);
    d.buffer=0;
    return 0;
}

typedef struct {
    unsigned long *i;
    unsigned int n;
} iint;

iint *cnt;

iint *new (unsigned bound) {
    long szl;
    iint *egg;
    if(!(egg=malloc(sizeof(iint)))) die("RAM denied\n");
    szl=sizeof(long);
    egg->n=bound/szl+((bound%szl)&&1);
    if(!(egg->i=malloc(egg->n*szl))) die("RAM denied\n");
    memset(egg->i, 0, egg->n*szl);
    return egg;
}



int add(iint *eye, unsigned long x) {
    int ret=0;
    *(eye->i)+=x;
    if (*(eye->i)<x){
        if(--(eye->n)){
        eye->i++;
            ret=add(eye, 1L);
            eye->i--;
    } else ret=1;
        (eye->n)++;
    }
    return ret;
}

int vyu(char *eye, iint *num) {
    static char zp_lu[7];
    unsigned i;
    i=num->n;
    sprintf(zp_lu, "%%0%lulx", 2*sizeof(unsigned long));
    while(i--) eye+=sprintf(eye, zp_lu, cnt->i[i]); /* woah! */
    strcat(eye,"\n");
    return 0;
}

char def[10]="123456789";
char out[83]="";

int first(sudoku *puzl, unsigned char *p) {
    unsigned short * sun;
    for(sun=(&puzl->i_v[0]), *p=0; *p<81; (*p)++, sun++) if(((*sun)&wait)) return 1;
    return 0;
}
void sudoku_init(char *in, sudoku *puzl) {
    unsigned short *eye, idea_v;
    unsigned char i;
    puzl->left = 81;
    eye = puzl->i_v;
    for(i=0; i<81 && *in; i++, eye++, in++) {
        if((idea_v=(*in)-'1')<9) *eye = idea_v|open|may_b[idea_v];
        else *eye = 0x7fe7;
    }
    for(;i<81;i++,eye++) *eye = 0x7fe7;
}
void sudoku_to_string(char *buffer, sudoku *puzl) {
    unsigned short *sun;
    char *eye;
    unsigned char i;
    eye=buffer;
    for (sun=puzl->i_v, i=0; i<81; i++, sun++, eye++) {
        if ((*sun) & open) *eye = '?';
        else *eye = (unsigned char)('1' + ((*sun) & data));
    }
    *eye = '\n';
    *(++eye) = '\0';
}
int idea(sudoku *puzl, unsigned char *p, unsigned char *v) {
    unsigned short *sun;
    for (sun=puzl->i_v, *p=0; *p<81; (*p)++, sun++) {
        if ((*sun)&open) {
            *v=(unsigned char)((*sun)&data);
            return 1;
        }
    }
    return 0;
}
int rm(unsigned short *eye, unsigned char v) {
    if ((*eye)&may_b[v]) {
        if (!((*eye)&wait)) return 1;
        (*eye)&=~may_b[v];
        (*eye)--;
        if((*eye)&open) {
            for(v=0; v<9; v++) if((*eye)&may_b[v]) break;
            (*eye) = ((unsigned short)v)|open|may_b[v];
        }
    }
    return 0;
}
int trial(unsigned short *cell, unsigned char *v) {
    for(; *v<9; (*v)++) if((*cell)&may_b[*v]) return 1;
    return 0;
}
int hook(sudoku *puzl) {
    unsigned char x;
    unsigned char *eye;
    unsigned char pos, val;
    if(d.stack) {
        if(d.top) return 0;
        free(d.stack);
        d.stack=0;
        d.top=-1;
        return 1;
    }
    while(idea(puzl, &pos, &val)) {
        for(x=0, eye=&scope[pos*20]; x<20; x++, eye++) {
             if(rm(&(puzl->i_v[*eye]), val)) return -1;
        }
        puzl->i_v[pos]&=(~open);
        if (!(--(puzl->left))) {
            sudoku_to_string(out,puzl);
            fputs(out ,stdout);
            add(cnt,1L); /* cant possibly overflow, should i check? */
            if(d.buffer){
                d.position=0;
                return 2;
            } else return 1;
        }
    }
    return 0;
}
int crook(sudoku *master) {
    sudoku *copy;
    unsigned short *mc, *cc;
    unsigned char pos, val=0, dummy;
    if(d.stack){
        if(!d.top--) die("stack underflow");
        copy=(d.stack+d.top)->s;
        pos=(unsigned char)(d.stack+d.top)->p;
        val=(unsigned char)(d.stack+d.top)->v;
        mc=&master->i_v[pos];
        cc=&copy->i_v[pos];
    } else {
        if(!(copy=(sudoku *)malloc(sizeof(sudoku)))) die("RAM denied\n");
        if(first(master, &pos)) {
            mc=&master->i_v[pos];
            cc=&copy->i_v[pos];
        } else return 1;
    }
    while(d.stack||trial(mc, &val)) {
        if(!d.stack){
            *copy=*master;
            *cc=(((unsigned short)val)|open|may_b[val]);
        }
        switch(squash(copy)){
            case 1:
            val++;
            break;
            case 2:
            d.position+=dmp(copy, (int)pos, (int)val);
            return 2;
            case -1:
            rm(mc,val); /* should i check? */
            ;
            if(idea(master, &pos, &dummy)&&(dummy>val)) {
                free(copy);
                return 0;
            }
        }
    }
    free(copy);
    return 1;
}
int squash(sudoku *puzl){
    int ret;
    do if(ret=hook(puzl)) return ret; while (!(ret=crook(puzl)));
    return ret;
}




int main(int argc, char **argv){
    sudoku *master;
    char *in;
    FILE *dumpfile;

    d.buffer=0;
    d.stack=0;
    d.position=d.top=-1;

    scope_gen(scope);

    in = argc > 1 ? argv[1] : def;

    if(argc > 1 || dumpfile_try_read()) {
        /* we have not read a dumpfile */
        if(!(master=(sudoku *)malloc(sizeof(sudoku)))) die("RAM denied\n");
        sudoku_init(in, master);
    } else {
        /* we have read dumpfile */
        d.top--;
        master=(d.stack+d.top)->s;
        /* pop a sudoku_state */
    }

    if(signal(SIGINT, rq)==SIG_ERR) fputs("could not enable dump feature\n", stderr);
    else fputs("dump feature enabled\n", stderr);

    cnt=new(10);

    switch(squash(master)) {
        case -1:
        fputs("no solution\n",stderr);
        break;
        case 2:
        dmp(master,10,10);
        if(!(dumpfile=fopen("dump","wb"))) die("couldn't create dumpfile\n");
        /* rather dump to stderr */
        fputs(d.buffer, dumpfile);
        fclose(dumpfile);
        free(d.buffer);
    }
    free(master);

    *out='#'; vyu(out+1, cnt); /* haha! passing a global :D */
    fputs(out, stderr);

    return 0;
}
/* pk 20131110 2110 */
