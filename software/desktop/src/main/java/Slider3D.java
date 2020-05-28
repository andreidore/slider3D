import java.io.IOException;
import org.firmata4j.Consumer;
import org.firmata4j.IODevice;
import org.firmata4j.IODeviceEventListener;
import org.firmata4j.IOEvent;
import org.firmata4j.Pin;
import org.firmata4j.firmata.FirmataDevice;
import org.firmata4j.firmata.FirmataMessageFactory;
import org.firmata4j.firmata.parser.FirmataEventType;
import org.firmata4j.fsm.Event;

public class Slider3D {


    public static void main(String[] args) throws IOException, InterruptedException {
        System.out.println("Start");


        // construct a Firmata device instance
        IODevice device = new FirmataDevice("/dev/cu.usbmodem143101"); // using the name of a port
        // IODevice device = new FirmataDevice(new NetworkTransport("192.168.1.18:4334")); // using
        // a network address
        // subscribe to events using device.addEventListener(...);
        // and/or device.getPin(n).addEventListener(...);
        device.start(); // initiate communication to the device
        device.ensureInitializationIsDone(); // wait for initialization is done

        device.sendMessage(FirmataMessageFactory.stringMessage("test"));


        device.addProtocolMessageHandler(FirmataEventType.SYSEX_CUSTOM_MESSAGE,
                new Consumer<Event>() {
                    @Override
                    public void accept(Event evt) {
                        byte[] message =
                                (byte[]) evt.getBodyItem(FirmataEventType.SYSEX_CUSTOM_MESSAGE);
                        byte messageType = message[0];
                        System.out.println(messageType);
                        // and so on
                    }
                });

        Thread.sleep(10000);

        // sending commands to the board
        device.stop(); // stop communication to the device

    }

}
