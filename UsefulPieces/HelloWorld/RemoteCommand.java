import java.util.*;
import com.illposed.osc.*;
import java.net.*;

public class RemoteCommand {
    public static int MAX=255;
    public static void main(String[] arg) throws Exception {
        byte[] addr = {(byte)192, (byte)168, (byte)1, (byte)6};
        OSCPortOut sender =
        new OSCPortOut(InetAddress.getByAddress(addr),57600);

        /*
        Object args[] = new Object[4];

        args[0] = new Integer(1);
        args[1] = new Integer(0);
        args[2] = new Integer(0);
        args[3] = new Integer(255);

        OSCMessage msg = new OSCMessage("/single_led", args);
        */

        OSCMessage msg = new OSCMessage("/off");

        try {
            sender.send(msg);
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        Thread.sleep(100);

        }
}
