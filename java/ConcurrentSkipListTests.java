
import java.lang.Runnable;
import java.util.Map;
import java.util.concurrent.ConcurrentSkipListMap;

class InsertRunnable implements Runnable {
    ConcurrentSkipListMap sl;
    int start;
    int end;

    InsertRunnable(int start, int end, ConcurrentSkipListMap sl) {
        this.start = start;
        this.end = end;
        this.sl = sl;
    }

    public void run() {
        
        for(int i=start; i<end; i++) {
            sl.put(i, i);
        }

        for(int i=start; i<end; i++) {
            System.out.println(sl.get(i));
        }
    }
}

class ConcurrentSkipListTests {
    void getTest(int cnt, int tCnt) {
        Map sl = new ConcurrentSkipListMap<Integer, Integer>();
        int start = 0;
        int perThread = cnt / tCnt;

        Thread[] t = new Thread[tCnt];

        for(int i=0; i<tCnt; i++) {
            t[i] = new Thread(new InsertRunnable(start, start + perThread, sl));
            t[i].start();
            start += perThread;
        }

        for(int i=0; i<tCnt; i++) {
            t[i].join();
        }

    }

    public static void main(String[] args) {
        
        //ConcurrentSkipListTests slTest = new ConcurrentSkipListTests();
        //slTest.getTest(100);
    }
}

