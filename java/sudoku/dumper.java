package sudoku;
import sun.misc.Signal;
import sun.misc.SignalHandler;
class state {
    s00d s;
    byte p;
    byte v;
}
class dumper {
    StringBuilder buf;
    state[] stack;
    byte top;
    byte pos;
    SignalHandler rq;
    void burn(){
        stack = null;
        buf = null;
    }
    dumper(){
        rq = new SignalHandler(){
            public void handle(Signal sig){
                System.err.println("dump requested");
                buf = new StringBuilder(8280); //TODO count recursion depth and use that to decide capacity!
                //TODO do i neet to set zeroes? or better use a byte arraylist?
            }
        };
        Signal.handle(new Signal("INT", rq);
    } 
}
int dmp(s00d *s, int p, int v){
    char *eye;
    b16 *sun;
    int l;

    for(l=0, eye=d.buf+d.pos, sun=s->i_v; l<405; sun++, eye+=5) l+=sprintf(eye, "%04x ", (int)*sun); //TODO java formatted output

    l+=sprintf(eye, "%02x ", (int)s->left); eye+=3;
    l+=sprintf(eye, "%02x ", p); eye+=3;
    l+=sprintf(eye, "%02x\n", v);

    return l;//the characters written d.pos to be incremented outside
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
            *eye=(b16)strtoul(this,&nxt,16);//TODO java formatted input
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
        line=strtok(0,"\r\n");//TODO java tokenization
	d.top++;
	depth--;
    }
    free(d.buf);
    d.buf=0;
    return 0;
}
