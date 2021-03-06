package se.chalmers.moppe.ovecontrol;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;

/**
 * Created by Erik on 2017-09-24.
 */

public abstract class SocketHandler{

    private static PrintWriter out = null;
    public static Socket socket = null;
    private static ArrayList<ObserverStatic> observerList = new ArrayList();


    /*
	 * Initialize the output stream for the socket.
	 */
    public static void init(Socket socket) {
        SocketHandler.socket = socket;
        try {
            out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(
                    socket.getOutputStream())), true);
        } catch (IOException e) {
            e.printStackTrace();
        }
        notifyObservers();
    }
    /*
	 * Send a message through the socket.
	 */
    public static void send(final Object message) {
        if (socket != null){
            new Thread(new Runnable() {
                @Override
                public void run() {
                    out.println(message);
                }
            }).start();
        }
    }

    public static void disconnect() {
        try {
            if (socket != null) {
                socket.close();
                socket = null;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        notifyObservers();
    }

    public static boolean isConnected() {
        return socket != null && socket.isConnected();
    }

    public static void addObserver(ObserverStatic o){
        observerList.add(o);
    }

    private static void notifyObservers(){
        for (ObserverStatic observer : observerList){
            System.out.println("Updateing" + isConnected());
            observer.update(isConnected());
        }
    }
}
