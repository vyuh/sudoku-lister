package sudoku;
import java.util.Iterator;
import java.util.Stack;
class solutions extends sudoku.s00d implements Iterator<String>, Iterable<String> {
    final static String def = "123456789";
    static int sols_per_call;
    static int sols_done;
    static String in;
    class status {
        solutions s;
        byte p;
        byte v;
        status(status mars){
            s=mars.s;
            p=mars.p;
            v=mars.v;
        }
        status(){
            v=0;
        }
    }
    static Stack<status> stata;
    static StringBuilder out;
    static StringBuilder nxt;
    static boolean readin;

    solutions() {
        this(def,1);
    }
    solutions(int n){
        this(def,n);
    }
    solutions(String puzl){
        this(puzl,1);
    }
    solutions(String puzl, int n){
        super(puzl);
        int i = 82*n;
        in = puzl;
        readin = false;
        sols_per_call = n;
        sols_done= 0; 
        out = new StringBuilder(i);
        nxt = new StringBuilder(i);
        stata = new Stack<status>();
        switch(squash()){
            case -1:
            nxt.delete(0, nxt.length());
            break;
            case 2:
            readin = true;
        }
    }
    solutions(solutions mars){ // just copies
        super(mars);
    }
    public boolean hasNext() {
        return nxt.length() != 0;
    }
    public String next(){
        out.delete(0, nxt.length());
        out.append(nxt);
        switch(squash()){
            case 2:
            readin = true;
        }
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

        if(readin) {
            if(stata.empty()) {
                readin=false;
                return 1;
            } else return 0;
        }
        while((pos = idea()) < 81) {
            val = i_v[pos].value();
            for(x=0; x<20; x++) {       
                 if(i_v[clr[pos][x]].rm(val)) return -1;
            }
            i_v[pos].reset(cell.open);
            if (--left == 0) {
                nxt.append(toString()+"\n");
                sols_done++;
                if(sols_done==sols_per_call) return 2;
                else return 1;
            }
        }
        return 0;
    }
    int crook(){
        cell mc, cc;
        status copy;
        if(readin){
            copy=stata.pop();
        } else {
            copy = new status();
            copy.s = new solutions(this);
            if((copy.p=first())>=81) return 1;
        }
            System.err.println(copy.p);
        mc=i_v[copy.p];
        cc=copy.s.i_v[copy.p];
        while(readin||((copy.v=mc.trial(copy.v))<9)){
            if(!readin){
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
                if(((copy.p = idea())<81)&&(i_v[copy.p].value()>copy.v)) {
                    return 0;
                }
            }
        }
        return 1;
    }
    public static void main(String[] args) {
        solutions yo = new solutions();
        if(yo.hasNext()) System.out.print(yo.next());
        // for(String i:yo) System.out.print(i);
    }
        
}
