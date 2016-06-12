//'use strict';

//WOAH https://gist.github.com/ryanflorence/1198466
(function (name, definition){
  if (typeof define === 'function'){ // AMD
    define(definition);
  } else if (typeof module !== 'undefined' && module.exports) { // Node.js
    module.exports = definition();
  } else { // Browser
    var theModule = definition(), global = this, old = global[name];
    theModule.noConflict = function () {
      global[name] = old;
      return theModule;
    };
    global[name] = theModule;
  }
})('sud', function () {
    var pk = {};

    function scope_gen(n) {
        // for normal sudoku latin square, n is 3
        // a latin square has n^4 cells
        var i, j, r, c, gr, gc, l, p;
        var out = [];
        l = n * n * n * n;
        /* malloc from stdlib.h */
        for (i = 0; i < l; i++) {
            var scope = new Array(l);
            r = Math.floor(i / (n * n));
            c = i % (n * n);
            gr = Math.floor(r / n) * n;
            gc = Math.floor(c / n) * n;
            for (j = 0; j < n * n; j++) {
                scope[r * n * n + j] = true;
                scope[j * n * n + c] = true;
                scope[(gr + Math.floor(j / n)) * n * n + gc + j %
                    n] = true;
            }
            scope[i] = false;
            var eye = [];
            for (j = 0; j < l; j++)
                if (scope[j]) {
                    eye.push(j);
                }
            out.push(eye);
        }
        return out;
    }
    //the cell class
    var cell = function (i) {
        //cell object constructor
        if (i) {
            if (typeof i === "object") {
                this.v = i.v;
            } else {
                var idea = i - 1;
                if ((idea < 9) && (idea >= 0)) {
                    this.putIdea(idea);
                } else {
                    this.v = 0x7fe7;
                }
            }
        } else {
            this.v = 0x7fe7;
        }
    };
    cell.prototype.putIdea = function (idea) {
        this.v = (idea | open | may_b[idea]);
    };
    cell.prototype.copyIn = function (i) {
        this.v = i.v;
    };
    cell.prototype.txt = function () {
        return this.v.toString();
    };
    cell.prototype.yo = function (mask) {
        return ((this.v & mask) !== 0);
    };
    cell.prototype.reset = function (mask) {
        //reset the mask
        this.v &= ~mask;
    };
    cell.prototype.value = function () {
        //value of an open or filled cell
        return (this.v & data);
    };
    cell.prototype.rm = function (i) {
        // returns success or failure (false or true respectively)
        if (this.yo(may_b[i])) {
            //has it in probable
            if (!this.yo(wait)) {
                return true;
                // it is the only probable!!
            }
            this.reset(may_b[i]);
            this.v -= 1;
            if (this.yo(open)) {
                // only one probable left now
                for (i = 0; i < 9; i += 1) {
                    if (this.yo(may_b[i])) {
                        break;
                    }
                }
                this.putIdea(i);
            }
        }
        return false;
        //removed ok
    };
    cell.prototype.trial = function (i) {
        // returns trial value or >=9 if no trial available
        for (; i < 9; i += 1) {
            if (this.yo(may_b[i])) {
                break;
            }
        }
        return i;
    };
    //the package globals
    var wait = 0x20;
    var open = 0x10;
    var data = 0xf;
    var may_b = [
        0x4000, 0x2000, 0x1000,
        0x0800, 0x0400, 0x0200,
        0x0100, 0x0080, 0x0040
    ];
    var clr = scope_gen(3);
    //the sud class. models a sudoku
    var sud = function (inp) {
        if (inp) {
            if (typeof inp === "object") {
                this.left = inp.left;
                this.i_v = [];
                for (var i = 0; i < 81; i += 1) {
                    this.i_v[i] = new cell(inp.i_v[i]);
                }
                return;
            } else inp = inp.toString()
        } else inp = '123456789'
        var len = inp.length
        this.left = 81
        this.i_v = []
        var i = 0
        for (; i < len && i < 81; i += 1) {
            this.i_v[i] = new cell(inp.charAt(i))
        }
        for (; i < 81; i += 1) this.i_v[i] = new cell()
        arguments.callee.__out__ = []
    }
    sud.prototype.copyIn = function (t) {
        this.left = t.left;
        for (var i = 0; i < 81; i += 1) this.i_v[i].copyIn(t.i_v[
            i]);
    }
    sud.prototype.first = function () {
        // first waiting cell or >=81; for ordered listing of solutions
        var i = 0
        for (; i < 81; i += 1) {
            if (this.i_v[i].yo(wait)) {
                break;
            }
        }
        return i
    }
    sud.prototype.idea = function () {
        //first open cell or >=81
        var i = 0
        for (; i < 81; i += 1) {
            if (this.i_v[i].yo(open)) {
                break
            }
        }
        return i
    }
    sud.prototype.toString = function () {
        var buf = ''
        for (var i = 0; i < 81; i += 1) {
            if (this.i_v[i].yo(open)) {
                buf += '?';
            } else {
                buf += (1 + this.i_v[i].value());
            }
        }
        return buf;
    }
    sud.prototype.hook = function () {
        // returns { wrong, stale, solved, dumping } = -1, 0, 1, 2
        var x, pos, val
        while ((pos = this.idea()) < 81) {
            val = this.i_v[pos].value()
            for (x = 0; x < 20; x += 1) {
                if (this.i_v[clr[pos][x]].rm(val)) {
                    return -1;
                }
            }
            this.i_v[pos].reset(open)
            if ((this.left -= 1) === 0) {
                this.constructor.__out__.push(this.toString())
                return this.constructor.__out__.length === this.constructor
                    .__n__ ? 2 : 1;
            }
        }
        return 0
    }
    sud.prototype.crook = function () {
        var mc, cc, pos, val = 0
        var copy = new sud(this)
        if ((pos = this.first()) < 81) {
            mc = this.i_v[pos]
            cc = copy.i_v[pos]
        } else return 1
        while ((val = mc.trial(val)) < 9) {
            copy.copyIn(this)
            cc.putIdea(val)
            switch (copy.squash()) {
            case 1:
                val += 1
                break
            case 2:
                return 2
            case -1:
                mc.rm(val) // should i check? 
                if (
                        ((pos = this.idea()) < 81)
                        &&
                        (this.i_v[pos].value() > val)
                ) {
                    copy = null
                    return 0
                }
            }
        }
        copy = null
        return 1
    }
    sud.prototype.squash = function () {
        var ret
        do
            if ((ret = this.hook()) !== 0) return ret;
        while (0 === (ret = this.crook())) return ret
    }
    //the list class. subclass of sud.
    //solution lister engine
    //an iterator
    var list = function (inp) {
        // calling superclass constructor
        sud.call(this, inp)
        this.status = function (ls, pos, val) {
            this.l = new list(ls)
            this.p = pos
            this.v = val
        }
        if (typeof inp === 'object') return
        pk.nxt = []
        switch (this.squash()) {
        case -1:
            break
        case 2:
        }
    }
    // subclass extends superclass
    // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/create
    list.prototype = Object.create(sud.prototype)
    list.prototype.constructor = list
    list.prototype.hook = function () {
        // returns { wrong, stale, solved, dumping } = -1, 0, 1, 2
        var x, pos, val
        if (pk.stata) {
            if (pk.stata.length === 0) {
                delete pk.stata
                return 1
            } else return 0
        }
        while ((pos = this.idea()) < 81) {
            val = this.i_v[pos].value()
            for (x = 0; x < 20; x += 1) {
                if (this.i_v[clr[pos][x]].rm(val)) return -1
            } // try removing braces
            this.i_v[pos].reset(open)
            if ((this.left -= 1) === 0) {
                pk.nxt.push(this.toString())
                if (pk.nxt.length === pk.n) {
                    pk.stata = []
                    // i hope empty array is true in js
                    return 2
                } else return 1
            }
        }
        return 0
    }
    list.prototype.crook = function () {
        var mc, cc, pos, val = 0,
            copy
        if (pk.stata) {
            copy = pk.stata.pop();
            mc = this.i_v[copy.p];
            cc = copy.l.i_v[copy.p];
        } else {
            if ((pos = this.first()) >= 81) return 1
            else {
                copy = new this.status(this, pos, 0)
                mc = this.i_v[copy.p]
                cc = copy.l.i_v[copy.p]
            }
        }
        while (pk.stata || ((copy.v = mc.trial(copy.v)) < 9)) {
            if (!pk.stata) {
                copy.l.copyIn(this);
                cc.putIdea(copy.v);
            }
            switch (copy.l.squash()) {
            case 1:
                copy.v += 1;
                break;
            case 2:
                pk.stata.push(copy);
                return 2;
            case -1:
                mc.rm(copy.v); /* should i check? */
                if (
                    ((pos = this.idea()) < 81) && (this.i_v[pos].value() >
                        copy.v)) return 0
            }
        }
        return 1
    }
    list.prototype.next = function () {
        if (pk.nxt.length === 0) return undefined
        pk.out = pk.nxt
        pk.nxt = []
        this.squash()
        return pk.out
    }
    list.prototype.hasNext = function () {
        return pk.nxt.length !== 0;
    }
    //exports

    var o = {};
    o.iter = function (constraints, n) {
        pk.n = n || 2
        return new list(constraints)
    }
    o.list = function (constraints, n) {
        var master = new sud(constraints)
        master.constructor.__n__ = n || 2;
        switch (master.squash()) {
        case -1:
            console.log("no solution")
            break
        case 2:
        }
        return master.constructor.__out__
    }
    return o;
});
/*
TODO:
change pk.nxt pk.out pk.n pk.stata. /pk\.[^.]* /
change /stat/
for class list
use ed
add semicolons to statements
add braces to loop or if/else bodies
use semver.org and publish to npm
*/
