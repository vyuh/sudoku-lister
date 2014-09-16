package sudoku;
    class cell {
        short v;
        static final short wait=0x20;
        static final short open=0x10;
        static final short[] may_b={
            0x4000, 0x2000, 0x1000,
            0x0800, 0x0400, 0x0200,
            0x0100, 0x0080, 0x0040
        };
        static final short data=0xf;
        cell(short i) {
            v=i;
        }
        String txt() {
            return ""+v;
        }
    }
