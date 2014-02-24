import java.util.*;
import com.illposed.osc.*;
import java.net.*;

public class RemoteCommand {
    public static int MAX=255;
    public static void main(String[] arg) throws Exception {
        byte[] addr = {(byte)192, (byte)168, (byte)1, (byte)12};
        OSCPortOut sender =
        new OSCPortOut(InetAddress.getByAddress(addr),57000);

        // Format: 'pixel-index, R, G, B' ...
        Object args[] = new Object[4];

        for(int col=1; col<=3; col++) {
            for(int i=0; i<MAX; i+=51) {
                
                // Get the OSC message ready ...
                
                // Pixel-index is args[0] ...
                args[0] = new Integer(0);
                // Init the level of red here ...
                args[1] = new Integer(0);
                // Init the level of green here ...
                args[2] = new Integer(0);
                // Init the level of blue here ...
                args[3] = new Integer(0);

                args[col] = new Integer(i);
                OSCMessage msg = new OSCMessage("/ledstrip", args);
                System.out.println("R:"+args[1]+",G:"+args[2]
                   +"B:"+args[3]);

                try {
                    sender.send(msg);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                Thread.sleep(100);
            }
        }
    }
}
