package sudoku;
import java.util.Iterator;
import java.util.Stack;
import java.util.NoSuchElementException;
class cell {
    short v;
    final static short wait=0x20;
    final static short open=0x10;
    final static short[] may_b={
        0x4000, 0x2000, 0x1000,
        0x0800, 0x0400, 0x0200,
        0x0100, 0x0080, 0x0040
    };
    final static short data=0xf;
    cell() { 
        v=0x7fe7; //may_be_0..8 wait !open 9_probables_left
    }
    cell(char i) { // may be only the given value, open, and the value
        byte idea=(byte)((short)i-(short)'1');
        if((idea<9)&&(idea>=0)) putIdea(idea);
        else v = 0x7fe7;
    }
    cell(cell t){//deep copier
        v = t.v;
    }
    void putIdea(byte idea){
        // mutation
        v=(short)(idea|open|may_b[idea]);
    }
    void copyin(cell t){
        // mutation
        v=t.v;
    }
    String txt() { //for non semantic dump. though portable.
        return ""+v;
    }
    boolean test_mask(short mask) { //the mask set or not
        if((v&mask)!=0) return true;
        else return false;
    }
    boolean may_be(byte i) {
        return test_mask(may_b[i]);
    }
    boolean waiting() {
        return test_mask(wait);
    }
    boolean is_open() {
        return test_mask(open);
    }
    void reset(short mask) { //reset the mask
        // mutation
        v&=~mask;
    }
    void lock() {
        reset(open);
    }
    byte value(){ //value of an open or filled cell
        return (byte)(v&data);
    }
    boolean rm(byte i) { // returns success or failure (false or true respectively)
        if (may_be(i)) { //has it in probable
            if (!waiting()) return true; // it is the only probable!!
            reset(may_b[i]);
            v--; // mutation
            if(is_open()) {// only one probable left now
                for(i=0; i<9; i++) if(may_be(i)) break;
                putIdea(i);
            }
        }
        return false; //removed ok
    }
    byte trial(byte i) { // returns trial value or >=9 if no trial available
        for(; i<9; i++) if(may_be(i)) break;
        return i;
    }
}
class s00d {
    static int n; //number of solutions to print 
    cell[] i_v;
    byte left;
    static final byte[][] clr={
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 63, 27, 54, 45, 18, 19, 72, 20, 36 },
        { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 46, 37, 28, 18, 19, 20, 55, 73, 0, 64 },
        { 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 74, 56, 47, 18, 19, 20, 65, 38, 29 },
        { 1, 2, 4, 5, 6, 7, 8, 12, 13, 14, 0, 48, 66, 39, 30, 75, 21, 57, 23, 22 },
        { 1, 2, 3, 5, 6, 7, 8, 12, 13, 14, 0, 67, 31, 49, 21, 76, 40, 22, 23, 58 },
        { 1, 2, 3, 4, 6, 7, 8, 12, 13, 14, 0, 77, 50, 41, 32, 68, 21, 22, 23, 59 },
        { 1, 2, 3, 4, 5, 7, 8, 15, 16, 24, 33, 26, 78, 60, 17, 51, 25, 42, 69, 0 },
        { 1, 2, 3, 4, 5, 6, 8, 15, 16, 0, 25, 26, 52, 43, 17, 34, 79, 70, 61, 24 },
        { 1, 2, 3, 4, 5, 6, 7, 15, 16, 24, 25, 35, 71, 17, 62, 53, 0, 80, 44, 26 },
        { 1, 2, 10, 11, 12, 13, 14, 15, 16, 0, 63, 27, 54, 45, 17, 18, 36, 72, 20, 19 },
        { 1, 2, 9, 11, 12, 13, 14, 15, 16, 46, 37, 28, 17, 18, 19, 20, 55, 73, 0, 64 },
        { 1, 2, 9, 10, 12, 13, 14, 15, 16, 0, 74, 29, 47, 18, 19, 38, 17, 65, 56, 20 },
        { 3, 4, 5, 9, 10, 11, 13, 14, 15, 16, 48, 66, 39, 30, 17, 75, 21, 22, 23, 57 },
        { 3, 4, 5, 9, 10, 11, 12, 14, 15, 16, 67, 31, 17, 49, 21, 76, 40, 22, 23, 58 },
        { 3, 4, 5, 9, 10, 11, 12, 13, 15, 16, 77, 50, 41, 32, 68, 17, 21, 22, 23, 59 },
        { 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 24, 33, 26, 78, 60, 17, 51, 42, 69, 25 },
        { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 26, 70, 43, 17, 34, 79, 52, 61 },
        { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 24, 25, 35, 71, 53, 62, 80, 44, 26 },
        { 1, 2, 0, 63, 9, 36, 19, 10, 20, 21, 11, 22, 23, 24, 25, 26, 27, 54, 72, 45 },
        { 1, 2, 0, 64, 9, 18, 10, 20, 21, 11, 22, 23, 46, 24, 25, 26, 73, 28, 55, 37 },
        { 1, 2, 0, 9, 18, 19, 10, 21, 11, 22, 23, 47, 25, 26, 74, 65, 24, 56, 29, 38 },
        { 30, 4, 66, 18, 19, 20, 22, 3, 23, 12, 24, 48, 25, 13, 26, 14, 75, 39, 57, 5 },
        { 58, 4, 31, 18, 19, 20, 40, 21, 3, 23, 12, 24, 49, 13, 26, 14, 76, 67, 25, 5 },
        { 59, 4, 32, 18, 19, 20, 21, 22, 3, 12, 24, 25, 50, 26, 68, 14, 77, 13, 41, 5 },
        { 15, 60, 8, 16, 17, 18, 19, 20, 78, 21, 42, 22, 23, 25, 51, 7, 26, 69, 33, 6 },
        { 15, 8, 16, 17, 34, 18, 70, 19, 20, 21, 22, 23, 24, 26, 52, 7, 61, 79, 43, 6 },
        { 15, 8, 16, 17, 18, 19, 20, 21, 22, 44, 6, 24, 25, 71, 7, 80, 62, 53, 23, 35 },
        { 0, 30, 31, 32, 33, 9, 18, 36, 37, 38, 45, 46, 47, 72, 35, 54, 28, 63, 29, 34 },
        { 1, 30, 31, 32, 33, 34, 35, 36, 19, 10, 45, 46, 47, 73, 64, 27, 55, 38, 29, 37 },
        { 2, 31, 32, 33, 34, 35, 36, 37, 38, 20, 11, 45, 46, 47, 65, 27, 74, 28, 56, 30 },
        { 31, 32, 33, 34, 35, 39, 40, 21, 3, 12, 48, 49, 50, 66, 27, 75, 28, 29, 57, 41 },
        { 58, 30, 4, 32, 33, 34, 35, 39, 40, 41, 22, 48, 49, 13, 27, 50, 28, 76, 29, 67 },
        { 59, 31, 33, 34, 68, 5, 39, 40, 41, 23, 48, 49, 50, 77, 27, 14, 28, 35, 29, 30 },
        { 15, 30, 60, 31, 32, 34, 35, 78, 42, 43, 44, 6, 24, 51, 52, 27, 28, 53, 29, 69 },
        { 30, 31, 16, 32, 33, 35, 70, 42, 43, 44, 25, 53, 51, 52, 27, 61, 28, 79, 29, 7 },
        { 30, 31, 62, 32, 17, 34, 80, 42, 43, 44, 52, 51, 26, 71, 53, 33, 28, 27, 29, 8 },
        { 0, 63, 9, 18, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 27, 54, 28, 29, 72 },
        { 1, 64, 36, 10, 39, 40, 41, 42, 43, 44, 45, 46, 47, 73, 27, 55, 28, 19, 29, 38 },
        { 2, 36, 37, 20, 40, 41, 42, 43, 44, 45, 46, 47, 65, 74, 27, 39, 28, 56, 29, 11 },
        { 30, 31, 32, 66, 36, 37, 38, 40, 21, 42, 43, 3, 12, 48, 49, 50, 75, 44, 57, 41 },
        { 58, 30, 4, 31, 32, 36, 37, 38, 39, 41, 42, 22, 44, 48, 49, 13, 50, 76, 67, 43 },
        { 30, 31, 32, 68, 36, 37, 38, 39, 40, 42, 43, 44, 23, 48, 49, 50, 14, 77, 59, 5 },
        { 15, 60, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 6, 24, 51, 52, 53, 78, 69 },
        { 61, 16, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 44, 25, 51, 52, 7, 79, 53, 70 },
        { 8, 62, 17, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 26, 71, 53, 80, 52, 51, 33 },
        { 0, 63, 9, 18, 36, 37, 38, 46, 47, 48, 49, 50, 51, 52, 27, 54, 28, 72, 29, 53 },
        { 1, 64, 36, 19, 38, 73, 53, 45, 47, 48, 49, 50, 51, 52, 27, 28, 55, 10, 29, 37 },
        { 2, 36, 37, 38, 20, 11, 74, 45, 46, 48, 49, 50, 51, 52, 27, 53, 28, 56, 29, 65 },
        { 30, 31, 32, 66, 39, 40, 21, 3, 45, 12, 47, 49, 50, 51, 52, 53, 75, 46, 57, 41 },
        { 58, 30, 4, 31, 32, 39, 40, 41, 22, 45, 46, 47, 48, 50, 51, 52, 53, 76, 67, 13 },
        { 30, 31, 32, 68, 5, 39, 40, 41, 23, 46, 47, 48, 49, 51, 52, 53, 14, 77, 59, 45 },
        { 15, 60, 33, 34, 35, 78, 42, 43, 44, 6, 46, 24, 48, 49, 50, 52, 53, 69, 45, 47 },
        { 61, 16, 33, 34, 35, 70, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 7, 79, 53, 25 },
        { 8, 62, 17, 34, 35, 80, 42, 43, 44, 45, 46, 47, 48, 49, 50, 26, 52, 71, 51, 33 },
        { 58, 59, 60, 61, 62, 63, 64, 9, 18, 36, 72, 74, 45, 65, 73, 27, 55, 56, 57, 0 },
        { 1, 58, 59, 60, 61, 62, 63, 64, 72, 10, 46, 65, 74, 37, 28, 73, 54, 56, 57, 19 },
        { 58, 59, 60, 61, 62, 63, 64, 72, 38, 20, 11, 47, 65, 74, 2, 73, 54, 55, 29, 57 },
        { 58, 30, 60, 61, 62, 66, 68, 39, 21, 3, 12, 48, 67, 77, 76, 75, 54, 55, 56, 59 },
        { 59, 4, 31, 62, 66, 68, 76, 40, 22, 77, 49, 67, 61, 75, 60, 54, 55, 56, 57, 13 },
        { 58, 60, 61, 62, 32, 66, 68, 5, 76, 41, 23, 67, 77, 75, 50, 54, 55, 56, 57, 14 },
        { 15, 59, 61, 62, 33, 70, 78, 80, 42, 6, 24, 79, 69, 71, 51, 54, 55, 56, 57, 58 },
        { 58, 59, 60, 62, 34, 70, 78, 80, 43, 79, 25, 71, 69, 52, 7, 54, 55, 56, 57, 16 },
        { 58, 59, 60, 8, 17, 35, 70, 78, 80, 44, 79, 61, 69, 71, 53, 54, 55, 56, 57, 26 },
        { 0, 64, 9, 68, 36, 72, 74, 45, 70, 73, 65, 67, 69, 71, 27, 54, 55, 56, 66, 18 },
        { 1, 63, 66, 68, 70, 19, 10, 46, 74, 37, 65, 67, 69, 71, 73, 54, 55, 56, 28, 72 },
        { 2, 63, 64, 66, 68, 70, 72, 38, 20, 11, 47, 67, 69, 71, 73, 54, 55, 56, 29, 74 },
        { 58, 30, 63, 64, 68, 70, 39, 21, 3, 12, 48, 65, 67, 69, 71, 75, 77, 76, 57, 59 },
        { 58, 59, 4, 31, 63, 64, 66, 68, 70, 76, 40, 22, 65, 13, 69, 71, 75, 77, 49, 57 },
        { 58, 59, 32, 64, 66, 70, 5, 76, 41, 23, 65, 67, 69, 71, 75, 14, 77, 63, 57, 50 },
        { 15, 60, 61, 62, 63, 33, 66, 68, 70, 78, 80, 42, 6, 24, 65, 67, 71, 79, 51, 64 },
        { 60, 61, 62, 63, 64, 66, 68, 78, 80, 43, 65, 67, 69, 52, 7, 25, 71, 79, 16, 34 },
        { 60, 8, 62, 63, 64, 66, 35, 70, 78, 80, 44, 65, 67, 69, 53, 26, 61, 79, 17, 68 },
        { 0, 63, 64, 9, 18, 36, 74, 76, 78, 80, 45, 65, 79, 73, 27, 75, 55, 56, 77, 54 },
        { 1, 63, 64, 19, 74, 76, 78, 80, 46, 79, 77, 65, 37, 54, 10, 75, 28, 56, 72, 55 },
        { 2, 63, 64, 72, 20, 78, 80, 11, 47, 79, 65, 54, 77, 38, 73, 75, 55, 56, 29, 76 },
        { 58, 59, 66, 68, 72, 74, 76, 78, 21, 3, 12, 48, 67, 80, 39, 73, 77, 79, 57, 30 },
        { 58, 59, 4, 31, 66, 68, 72, 74, 40, 80, 22, 49, 67, 78, 73, 75, 77, 79, 57, 13 },
        { 58, 59, 32, 66, 68, 72, 74, 76, 78, 41, 23, 80, 67, 14, 50, 73, 75, 79, 57, 5 },
        { 15, 60, 61, 62, 33, 70, 72, 74, 76, 80, 42, 6, 24, 69, 71, 73, 75, 77, 79, 51 },
        { 60, 61, 62, 34, 70, 72, 74, 76, 78, 80, 43, 25, 71, 69, 52, 7, 75, 77, 73, 16 },
        { 60, 8, 62, 17, 35, 70, 72, 74, 76, 78, 44, 53, 69, 71, 73, 75, 77, 79, 61, 26 }
    };
    byte first() { // first waiting cell or >=81; for ordered listing of solutions
        byte i = 0;
        for(; i<81; i++) if(i_v[i].waiting()) break;
        return i;
    }
    byte best() { // best waiting cell or >=81; first() substitute for fast first convergence
        byte i=0, level=9, b=81;
        for(; i<81; i++) {
            if(i_v[i].waiting()) {
                if(i_v[i].value()<level) {
                    level=(byte)i_v[i].value();
                    b=i;
                }
            }
        }
        return b;
    }
    /*  returns the cell with minimum 
        number of probables
        converges to a solution faster
        than selecting the first empty cell
    */
    
    byte idea() { //first open cell or >=81
        byte i = 0;
        for (; i<81; i++) if (i_v[i].is_open()) break;
        return i;
    }
    public String toString(){
        byte i;
        StringBuilder buf = new StringBuilder(81);
        for (i=0; i<81; i++) {
            if (i_v[i].is_open()) buf.append('?');
            else buf.append(1+i_v[i].value());
        }
        return buf.toString();
    }


    int hook() { // returns { wrong, stale, solved, dumping } = -1, 0, 1, 2
        // mutation
        byte x, pos, val;
        while((pos = idea()) < 81) {
            val = i_v[pos].value();
            for(x=0; x<20; x++) {       
                 if(i_v[clr[pos][x]].rm(val)) return -1;
            }
            i_v[pos].lock();
            if (--left == 0) {
                System.out.println(toString());
                n--; if(n==0) return 2;
                return 1;
            }
        }
        return 0;
    }
    int crook(){ // returns { hint_found, solved_all, dumping } = 0, 1, 2
        // mutation
        s00d copy;
        cell mc, cc;
        byte pos, val=0;
            copy = new s00d(this);
            if((pos=first())<81) {
                mc=i_v[pos];
                cc=copy.i_v[pos];
            } else return 1;
            while((val=mc.trial(val))<9){
                copy.copyin(this);
                cc.putIdea(val);
                switch(copy.squash()){
                    case 1:
                    val++;
                    break;
                    case 2:
                    return 2;
                    case -1:
                    mc.rm(val); /* should i check? */
                    if(((pos = idea())<81)&&(i_v[pos].value()>val)) {
                        copy = null;
                        return 0;
                    }
                }
            }
        copy = null;
        return 1;
    }
    int squash(){ // returns { wrong, solved_all, dumping } = -1, 1, 2
        int ret;
        do if((ret=hook())!=0) return ret; while (0==(ret=crook()));
        return ret;
    }
    s00d(String in) {
        byte i;
        if(in==null) in = "123456789";
        int len = in.length();
        left = 81;
        i_v = new cell[81];
        for(i=0; i<len && i<81; i++) {
            i_v[i] = new cell(in.charAt(i));
        }
        for(;i<81;i++) i_v[i] = new cell();
        n=2;
    }
    s00d(s00d t){//deep copier
        left = t.left;
        i_v = new cell[81];
        for(byte i = 0; i<81; i++) i_v[i] = new cell(t.i_v[i]);
    }
    void copyin(s00d t){
        // mutation
        left = t.left;
        for(byte i = 0; i<81; i++) i_v[i].copyin(t.i_v[i]);
    }
    public static void main(String[] args) {

        s00d master;
        String in;

        if(args.length!=0) in=args[0]; else in="123456789";

        master = new s00d(in);
        master.n =500;
        switch(master.squash()) {
            case -1:
            System.err.println("no solution");
            break;
            case 2:
        }
    }
}

class list extends s00d implements Iterator<String>, Iterable<String> {
    static int d; //sols done in this call
    byte id; //debug
    class status {
        list s;
        byte p;
        byte v;
        byte i; //debug
        status(list l, byte pos, byte val){
            s = new list(l);
            p = pos;
            v = val;
        }
    }
    static Stack<status> stata;
    static StringBuilder out;
    static StringBuilder nxt;

    list() {
        this(null,1);
    }
    list(int npc){
        this(null,npc);
    }
    list(String in){
        this(in,1);
    }

    list(String in, int npc){
        super(in);
        int i = 82*npc;
        n = npc;
        d = 0; 
        id = 0; //debug
        out = new StringBuilder(i);
        nxt = new StringBuilder(i);
        switch(squash()){
            case -1:
            nxt.delete(0, nxt.length());
            break;
            case 2:
            
        }
    }
    list(list l){ //deep copier
        super(l);
        id = (byte)(l.id+ 1);//debug
    }
    public boolean hasNext() {
        return nxt.length() != 0;
    }
    public String next(){
        if(nxt.length() == 0) throw new NoSuchElementException();
        out.delete(0, out.length());
        out.append(nxt);
        squash();
        return out.toString();
    }
    public void remove(){
        throw new UnsupportedOperationException("Whatta fuck ya doin?\n");
    }
    public Iterator<String> iterator(){
        return this;
    }
    int hook() { // returns { wrong, stale, solved, dumping } = -1, 0, 1, 2
        byte x, pos, val;
        if(stata!=null) {
            if(stata.empty()) {
                stata=null;
                return 1;
            } else return 0;
        }
        while((pos = idea()) < 81) {
            val = i_v[pos].value();
            for(x=0; x<20; x++) {       
                 if(i_v[clr[pos][x]].rm(val)) return -1;
            }
            i_v[pos].lock();
            if (--left == 0) {
                nxt.append(toString()+"\n");
                d++;
                if(d==n) {
                    stata = new Stack<status>();
                    d=0;
                    return 2;
                } else return 1;
            }
        }
        return 0;
    }
    int crook(){
        cell mc, cc;
        status copy;
        if(stata!=null) {
            copy=stata.pop();
            mc=i_v[copy.p];
            cc=copy.s.i_v[copy.p];
        } else {
            byte pos;
            if((pos=first())>=81) {
                return 1;
            } else {
                copy = new status(this, pos, (byte)0);
                mc=i_v[copy.p];
                cc=copy.s.i_v[copy.p];
            }
        }
        while((stata!=null)||((copy.v=mc.trial(copy.v))<9)){
            if(stata == null){
                copy.s.copyin(this);
                cc.putIdea(copy.v);
            }
            switch(copy.s.squash()){
                case 1:
                copy.v++;
                break;
                case 2:
                stata.push(copy);
                return 2;
                case -1:
                mc.rm(copy.v); /* should i check? */
                byte pos;
                if(((pos = idea())<81)&&(i_v[pos].value()>copy.v)) {
                    return 0;
                }
            }
        }
        return 1;
    }
    public static void main(String[] args){
        list pk = new list(5);
        if(pk.hasNext()) System.out.print(pk.next());
    }
        
}
