/*
Copyright (c) 2014, Prashant Karmakar, pk80@live.com

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

typedef unsigned char b8;
typedef unsigned short b16;
typedef struct {
    b16 i_v[81];
    b8 left;
} s00d;
b16 wait=0x20;
b16 open=0x10;
b16 may_b[9]={
    0x4000, 0x2000, 0x1000,
    0x0800, 0x0400, 0x0200,
    0x0100, 0x0080, 0x0040
};
b16 data=0xf;
b8 clr[81*20]={
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 63, 27, 54, 45, 18, 19, 72, 20, 36,
    2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 46, 37, 28, 18, 19, 20, 55, 73, 0, 64,
    1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 74, 56, 47, 18, 19, 20, 65, 38, 29,
    1, 2, 4, 5, 6, 7, 8, 12, 13, 14, 0, 48, 66, 39, 30, 75, 21, 57, 23, 22,
    1, 2, 3, 5, 6, 7, 8, 12, 13, 14, 0, 67, 31, 49, 21, 76, 40, 22, 23, 58,
    1, 2, 3, 4, 6, 7, 8, 12, 13, 14, 0, 77, 50, 41, 32, 68, 21, 22, 23, 59,
    1, 2, 3, 4, 5, 7, 8, 15, 16, 24, 33, 26, 78, 60, 17, 51, 25, 42, 69, 0,
    1, 2, 3, 4, 5, 6, 8, 15, 16, 0, 25, 26, 52, 43, 17, 34, 79, 70, 61, 24,
    1, 2, 3, 4, 5, 6, 7, 15, 16, 24, 25, 35, 71, 17, 62, 53, 0, 80, 44, 26,
    1, 2, 10, 11, 12, 13, 14, 15, 16, 0, 63, 27, 54, 45, 17, 18, 36, 72, 20, 19,
    1, 2, 9, 11, 12, 13, 14, 15, 16, 46, 37, 28, 17, 18, 19, 20, 55, 73, 0, 64,
    1, 2, 9, 10, 12, 13, 14, 15, 16, 0, 74, 29, 47, 18, 19, 38, 17, 65, 56, 20,
    3, 4, 5, 9, 10, 11, 13, 14, 15, 16, 48, 66, 39, 30, 17, 75, 21, 22, 23, 57,
    3, 4, 5, 9, 10, 11, 12, 14, 15, 16, 67, 31, 17, 49, 21, 76, 40, 22, 23, 58,
    3, 4, 5, 9, 10, 11, 12, 13, 15, 16, 77, 50, 41, 32, 68, 17, 21, 22, 23, 59,
    6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 24, 33, 26, 78, 60, 17, 51, 42, 69, 25,
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 26, 70, 43, 17, 34, 79, 52, 61,
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 24, 25, 35, 71, 53, 62, 80, 44, 26,
    1, 2, 0, 63, 9, 36, 19, 10, 20, 21, 11, 22, 23, 24, 25, 26, 27, 54, 72, 45,
    1, 2, 0, 64, 9, 18, 10, 20, 21, 11, 22, 23, 46, 24, 25, 26, 73, 28, 55, 37,
    1, 2, 0, 9, 18, 19, 10, 21, 11, 22, 23, 47, 25, 26, 74, 65, 24, 56, 29, 38,
    30, 4, 66, 18, 19, 20, 22, 3, 23, 12, 24, 48, 25, 13, 26, 14, 75, 39, 57, 5,
    58, 4, 31, 18, 19, 20, 40, 21, 3, 23, 12, 24, 49, 13, 26, 14, 76, 67, 25, 5,
    59, 4, 32, 18, 19, 20, 21, 22, 3, 12, 24, 25, 50, 26, 68, 14, 77, 13, 41, 5,
    15, 60, 8, 16, 17, 18, 19, 20, 78, 21, 42, 22, 23, 25, 51, 7, 26, 69, 33, 6,
    15, 8, 16, 17, 34, 18, 70, 19, 20, 21, 22, 23, 24, 26, 52, 7, 61, 79, 43, 6,
    15, 8, 16, 17, 18, 19, 20, 21, 22, 44, 6, 24, 25, 71, 7, 80, 62, 53, 23, 35,
    0, 30, 31, 32, 33, 9, 18, 36, 37, 38, 45, 46, 47, 72, 35, 54, 28, 63, 29, 34,
    1, 30, 31, 32, 33, 34, 35, 36, 19, 10, 45, 46, 47, 73, 64, 27, 55, 38, 29, 37,
    2, 31, 32, 33, 34, 35, 36, 37, 38, 20, 11, 45, 46, 47, 65, 27, 74, 28, 56, 30,
    31, 32, 33, 34, 35, 39, 40, 21, 3, 12, 48, 49, 50, 66, 27, 75, 28, 29, 57, 41,
    58, 30, 4, 32, 33, 34, 35, 39, 40, 41, 22, 48, 49, 13, 27, 50, 28, 76, 29, 67,
    59, 31, 33, 34, 68, 5, 39, 40, 41, 23, 48, 49, 50, 77, 27, 14, 28, 35, 29, 30,
    15, 30, 60, 31, 32, 34, 35, 78, 42, 43, 44, 6, 24, 51, 52, 27, 28, 53, 29, 69,
    30, 31, 16, 32, 33, 35, 70, 42, 43, 44, 25, 53, 51, 52, 27, 61, 28, 79, 29, 7,
    30, 31, 62, 32, 17, 34, 80, 42, 43, 44, 52, 51, 26, 71, 53, 33, 28, 27, 29, 8,
    0, 63, 9, 18, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 27, 54, 28, 29, 72,
    1, 64, 36, 10, 39, 40, 41, 42, 43, 44, 45, 46, 47, 73, 27, 55, 28, 19, 29, 38,
    2, 36, 37, 20, 40, 41, 42, 43, 44, 45, 46, 47, 65, 74, 27, 39, 28, 56, 29, 11,
    30, 31, 32, 66, 36, 37, 38, 40, 21, 42, 43, 3, 12, 48, 49, 50, 75, 44, 57, 41,
    58, 30, 4, 31, 32, 36, 37, 38, 39, 41, 42, 22, 44, 48, 49, 13, 50, 76, 67, 43,
    30, 31, 32, 68, 36, 37, 38, 39, 40, 42, 43, 44, 23, 48, 49, 50, 14, 77, 59, 5,
    15, 60, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 6, 24, 51, 52, 53, 78, 69,
    61, 16, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 44, 25, 51, 52, 7, 79, 53, 70,
    8, 62, 17, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 26, 71, 53, 80, 52, 51, 33,
    0, 63, 9, 18, 36, 37, 38, 46, 47, 48, 49, 50, 51, 52, 27, 54, 28, 72, 29, 53,
    1, 64, 36, 19, 38, 73, 53, 45, 47, 48, 49, 50, 51, 52, 27, 28, 55, 10, 29, 37,
    2, 36, 37, 38, 20, 11, 74, 45, 46, 48, 49, 50, 51, 52, 27, 53, 28, 56, 29, 65,
    30, 31, 32, 66, 39, 40, 21, 3, 45, 12, 47, 49, 50, 51, 52, 53, 75, 46, 57, 41,
    58, 30, 4, 31, 32, 39, 40, 41, 22, 45, 46, 47, 48, 50, 51, 52, 53, 76, 67, 13,
    30, 31, 32, 68, 5, 39, 40, 41, 23, 46, 47, 48, 49, 51, 52, 53, 14, 77, 59, 45,
    15, 60, 33, 34, 35, 78, 42, 43, 44, 6, 46, 24, 48, 49, 50, 52, 53, 69, 45, 47,
    61, 16, 33, 34, 35, 70, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 7, 79, 53, 25,
    8, 62, 17, 34, 35, 80, 42, 43, 44, 45, 46, 47, 48, 49, 50, 26, 52, 71, 51, 33,
    58, 59, 60, 61, 62, 63, 64, 9, 18, 36, 72, 74, 45, 65, 73, 27, 55, 56, 57, 0,
    1, 58, 59, 60, 61, 62, 63, 64, 72, 10, 46, 65, 74, 37, 28, 73, 54, 56, 57, 19,
    58, 59, 60, 61, 62, 63, 64, 72, 38, 20, 11, 47, 65, 74, 2, 73, 54, 55, 29, 57,
    58, 30, 60, 61, 62, 66, 68, 39, 21, 3, 12, 48, 67, 77, 76, 75, 54, 55, 56, 59,
    59, 4, 31, 62, 66, 68, 76, 40, 22, 77, 49, 67, 61, 75, 60, 54, 55, 56, 57, 13,
    58, 60, 61, 62, 32, 66, 68, 5, 76, 41, 23, 67, 77, 75, 50, 54, 55, 56, 57, 14,
    15, 59, 61, 62, 33, 70, 78, 80, 42, 6, 24, 79, 69, 71, 51, 54, 55, 56, 57, 58,
    58, 59, 60, 62, 34, 70, 78, 80, 43, 79, 25, 71, 69, 52, 7, 54, 55, 56, 57, 16,
    58, 59, 60, 8, 17, 35, 70, 78, 80, 44, 79, 61, 69, 71, 53, 54, 55, 56, 57, 26,
    0, 64, 9, 68, 36, 72, 74, 45, 70, 73, 65, 67, 69, 71, 27, 54, 55, 56, 66, 18,
    1, 63, 66, 68, 70, 19, 10, 46, 74, 37, 65, 67, 69, 71, 73, 54, 55, 56, 28, 72,
    2, 63, 64, 66, 68, 70, 72, 38, 20, 11, 47, 67, 69, 71, 73, 54, 55, 56, 29, 74,
    58, 30, 63, 64, 68, 70, 39, 21, 3, 12, 48, 65, 67, 69, 71, 75, 77, 76, 57, 59,
    58, 59, 4, 31, 63, 64, 66, 68, 70, 76, 40, 22, 65, 13, 69, 71, 75, 77, 49, 57,
    58, 59, 32, 64, 66, 70, 5, 76, 41, 23, 65, 67, 69, 71, 75, 14, 77, 63, 57, 50,
    15, 60, 61, 62, 63, 33, 66, 68, 70, 78, 80, 42, 6, 24, 65, 67, 71, 79, 51, 64,
    60, 61, 62, 63, 64, 66, 68, 78, 80, 43, 65, 67, 69, 52, 7, 25, 71, 79, 16, 34,
    60, 8, 62, 63, 64, 66, 35, 70, 78, 80, 44, 65, 67, 69, 53, 26, 61, 79, 17, 68,
    0, 63, 64, 9, 18, 36, 74, 76, 78, 80, 45, 65, 79, 73, 27, 75, 55, 56, 77, 54,
    1, 63, 64, 19, 74, 76, 78, 80, 46, 79, 77, 65, 37, 54, 10, 75, 28, 56, 72, 55,
    2, 63, 64, 72, 20, 78, 80, 11, 47, 79, 65, 54, 77, 38, 73, 75, 55, 56, 29, 76,
    58, 59, 66, 68, 72, 74, 76, 78, 21, 3, 12, 48, 67, 80, 39, 73, 77, 79, 57, 30,
    58, 59, 4, 31, 66, 68, 72, 74, 40, 80, 22, 49, 67, 78, 73, 75, 77, 79, 57, 13,
    58, 59, 32, 66, 68, 72, 74, 76, 78, 41, 23, 80, 67, 14, 50, 73, 75, 79, 57, 5,
    15, 60, 61, 62, 33, 70, 72, 74, 76, 80, 42, 6, 24, 69, 71, 73, 75, 77, 79, 51,
    60, 61, 62, 34, 70, 72, 74, 76, 78, 80, 43, 25, 71, 69, 52, 7, 75, 77, 73, 16,
    60, 8, 62, 17, 35, 70, 72, 74, 76, 78, 44, 53, 69, 71, 73, 75, 77, 79, 61, 26,
};
typedef struct {
    s00d *s;
    int p;
    int v;
} state;
struct {
    char *buf;
    state *stack;
    int top;
    int pos;
} d;
#include <signal.h>
#include <string.h>
void rq(int sig){
    fputs("\ndump requested\n", stderr);
    if(!(d.buf=malloc(81*414))) die("RAM denied");
    memset(d.buf, 0, 81*414);
}
int dmp(s00d *s, int p, int v){
    char *eye;
    b16 *sun;
    int l;

    for(l=0, eye=d.buf+d.pos, sun=s->i_v; l<405; sun++, eye+=5) l+=sprintf(eye, "%04x ", (int)*sun);

    l+=sprintf(eye, "%02x ", (int)s->left); eye+=3;
    l+=sprintf(eye, "%02x ", p); eye+=3;
    l+=sprintf(eye, "%02x\n", v);

    return l;//the characters written d.pos to be incremented outside
}
int burn(state *n){
    for(;n>=d.stack;n--) free(n->s);
    free(d.stack);
    d.stack=0;
    free(d.buf);
    d.buf=0;
    return 1;
}
int rd(){
    FILE *dump;
    int depth, i;
    char *line, *this, *nxt;
    b16 *eye;
    state *now;

    if(!(dump=fopen("dump", "rb"))) return 2;
    if(!(d.buf=(char *)malloc(81*414))) die("RAM denied\n");
    memset(d.buf, 0, 81*414);
    depth=fread(d.buf, 414, 81, dump); //if CRLF & fread doesnt copy the last incomplete line
    fclose(dump);
    if(!depth) return 1;

    if(!(d.stack=(state *)malloc(sizeof(state)*depth))) die("RAM denied\n");
    memset(d.stack, 0, sizeof(state)*depth);

    line=strtok(d.buf,"\r\n");
    d.top=0;
    while(line&&depth){
        now=d.stack+d.top;
        if(!(now->s=(s00d *)malloc(sizeof(s00d)))) die("RAM denied\n");
        for(i=0,this=line,eye=now->s->i_v;i<81;i++,eye++){
            *eye=(b16)strtoul(this,&nxt,16);
            if((this+5!=nxt)&&(i!=0||(this+4!=nxt))) return burn(now);
            this=nxt; 
        }
        now->s->left=(b8)strtoul(this,&nxt,16);
        if(this+3!=nxt) return burn(now);
        this=nxt;
        now->p=(b8)strtoul(this,&nxt,16);
        if(this+3!=nxt) return burn(now);
        this=nxt;
        now->v=(b8)strtoul(this,&nxt,16);
        if(this+3!=nxt) return burn(now);
        line=strtok(0,"\r\n");
        d.top++;
        depth--;
    }
    free(d.buf);
    d.buf=0;
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
    while(i--) eye+=sprintf(eye, zp_lu, cnt->i[i]); // woah!
    strcat(eye,"\n");
    return 0;
}

b8 def[10]="123456789";
b8 out[83]="";

int first(s00d *puzl, b8 *p) {
    b16 * sun;
    for(sun=(&puzl->i_v[0]), *p=0; *p<81; (*p)++, sun++) if(((*sun)&wait)) return 1;
    return 0;
}
void S00D(b8 *in, s00d *puzl) {
    b16 *eye, idea_v;
    b8 i;
    puzl->left = 81;
    eye = puzl->i_v;
    for(i=0; i<81 && *in; i++, eye++, in++) {
        if((idea_v=(*in)-'1')<9) *eye = idea_v|open|may_b[idea_v];
        else *eye = 0x7fe7;
    }
    for(;i<81;i++,eye++) *eye = 0x7fe7;
}
void STR(b8 *buf, s00d *puzl) {
    b16 *sun;
    b8 *eye;
    b8 i;
    eye=buf;
    for (sun=puzl->i_v, i=0; i<81; i++, sun++, eye++) {
        if ((*sun) & open) *eye = '?';
        else *eye = (b8)('1' + ((*sun) & data));
    }
    *eye = '\n';
    *(++eye) = '\0';
}
int idea(s00d *puzl, b8 *p, b8 *v) {
    b16 *sun;
    for (sun=puzl->i_v, *p=0; *p<81; (*p)++, sun++) {
        if ((*sun)&open) {
            *v=(b8)((*sun)&data);
            return 1;
        }
    }
    return 0;
}
int rm(b16 *eye, b8 v) {
    if ((*eye)&may_b[v]) {
        if (!((*eye)&wait)) return 1;
        (*eye)&=~may_b[v];
        (*eye)--;
        if((*eye)&open) {
            for(v=0; v<9; v++) if((*eye)&may_b[v]) break;
            (*eye) = ((b16)v)|open|may_b[v];
        }
    }
    return 0;
}
int trial(b16 *cell, b8 *v) {
    for(; *v<9; (*v)++) if((*cell)&may_b[*v]) return 1;
    return 0;
}
int hook(s00d *puzl) {
    b8 x;
    b8 *eye; 
    b8 pos, val;
    if(d.stack) {
        if(d.top) return 0;
        free(d.stack);
        d.stack=0;
        d.top=-1;
        return 1;
    }
    while(idea(puzl, &pos, &val)) {
        for(x=0, eye=&clr[pos*20]; x<20; x++, eye++) {       
             if(rm(&(puzl->i_v[*eye]), val)) return -1;
        }
        puzl->i_v[pos]&=(~open);
        if (!(--(puzl->left))) {
            STR(out,puzl);
            fputs(out ,stdout);
            add(cnt,1L); //cant possibly overflow, should i check?
            if(d.buf){
                d.pos=0;
                return 2;
            } else return 1;
        }
    }
    return 0;
}
int crook(s00d *master) {
    s00d *copy;
    b16 *mc, *cc;
    b8 pos, val=0, dummy;
    if(d.stack){
        if(!d.top--) die("stack underflow");
        copy=(d.stack+d.top)->s;
        pos=(b8)(d.stack+d.top)->p;
        val=(b8)(d.stack+d.top)->v;
        mc=&master->i_v[pos];
        cc=&copy->i_v[pos];
    } else {
        if(!(copy=(s00d *)malloc(sizeof(s00d)))) die("RAM denied\n");
        if(first(master, &pos)) {
            mc=&master->i_v[pos];
            cc=&copy->i_v[pos];
        } else return 1;
    }
    while(d.stack||trial(mc, &val)) {
        if(!d.stack){
            *copy=*master;
            *cc=(((b16)val)|open|may_b[val]);
        }
        switch(squash(copy)){
            case 1:
            val++;
            break;
            case 2:
            d.pos+=dmp(copy, (int)pos, (int)val);
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
int squash(s00d *puzl){
    int ret;
    do if(ret=hook(puzl)) return ret; while (!(ret=crook(puzl)));
    return ret;
}




int main(int argc, char **argv){
    s00d *master;
    char *in;
    FILE *dump;

    d.buf=d.stack=0;
    d.pos=d.top=-1;

    if(argc>1) {
        in=argv[1];
    } else {
        if(rd()) in=def; else {
            d.top--;
            master=(d.stack+d.top)->s;
        }
    }

    if(!d.stack) {
        if(!(master=(s00d *)malloc(sizeof(s00d)))) die("RAM denied\n");
        S00D(in, master);
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
        if(!(dump=fopen("dump","wb"))) die("couldn't create dumpfile\n");
        //rather dump to stderr
        fputs(d.buf, dump);
        fclose(dump);
        free(d.buf);
    }
    free(master);

    *out='#'; vyu(out+1, cnt); // haha! passing a global :D
    fputs(out, stderr);

    return 0;
}
/* pk 20131110 2110 */
