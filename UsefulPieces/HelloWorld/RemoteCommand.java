import java.util.*;
import com.illposed.osc.*;
import java.net.*;

public class RemoteCommand {
    public static int MAX=255;
    public static void main(String[] arg) throws Exception {
        byte[] addr = {(byte)159, (byte)149, (byte)159, (byte)213};
        OSCPortOut sender =
        new OSCPortOut(InetAddress.getByAddress(addr),57600);

        Object args[] = new Object[4];

        args[0] = new Integer(10);
        args[1] = new Integer(10);
        args[2] = new Integer(20);
        args[3] = new Integer(5);

        OSCMessage msg = new OSCMessage("/ping_frw", args);

        try {
            sender.send(msg);
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        Thread.sleep(100);

        }
}
