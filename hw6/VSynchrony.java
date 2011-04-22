import java.lang.ClassLoader;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.HashMap;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.Properties;

import org.jgroups.JChannel;
import org.jgroups.Message;
import org.jgroups.ReceiverAdapter;
import org.jgroups.View;
import org.jgroups.util.Util;

public class VSynchrony extends ReceiverAdapter {
  JChannel channel;
  String user_name = System.getProperty("user.name","n/a");
  final List<String> state = new LinkedList<String>();
  HashMap<String,Integer> ledger = new HashMap<String,Integer>();
  HashMap<String,Stock> index = new HashMap<String,Stock>();

  private void start() throws Exception {
    channel = new JChannel();
    channel.connect("jordi-group");
    channel.setReceiver(this);
    if(!channel.getState(null, 10000)) {
      this.getInitStocks();
      this.makeInitLedger(4);
    }
    System.out.println(this.ledger);
    System.out.println(this.index);
    //eventLoop();
    channel.close();
  }

  public void viewAccepted(View new_view) {
    System.out.println("** view: " + new_view);
  }

  public void getInitStocks() {
    // Gets the initial stocks from index.properties file
    Properties indexProp = new Properties();
    InputStream in = getClass().getResourceAsStream("index.properties");
    try {
      indexProp.load(in);
      in.close();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
    // Initialize stocks for the properties
    Enumeration indexSyms = indexProp.propertyNames();
    while(indexSyms.hasMoreElements()) {
      String stockSym = (String)indexSyms.nextElement();
      String stockName = indexProp.getProperty(stockSym);
      index.put(stockSym, new Stock(stockName));
    }
  }

  public void makeInitLedger(int clients) {
    // Populates the ledger with a number of clients
    for(int i = 1; i <= clients; i++){
      String client = "C" + i;
      ledger.put(client, new Integer(10000));
    }
  }

  public void receive(Message msg) {
    String line = msg.getSrc() + ": " + msg.getObject();
    System.out.println(line);
    synchronized(state) {
      state.add(line);
    }
  }

  public byte[] getState() { // Called in the state provider
    synchronized(state) {
      try {
        return Util.objectToByteBuffer(state);
      }
      catch(Exception e) {
        e.printStackTrace();
        return null;
      }
    }
  }

  public void setState(byte[] new_state) { // Called in the state requester
    try {
      List<String> list = (List<String>)Util.objectFromByteBuffer(new_state);
      synchronized(state) {
        state.clear();
        state.addAll(list);
      }
      System.out.println("received state (" + list.size() + " messages in chat history):");
      for(String str: list) {
        System.out.println(str);
      }
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void eventLoop() {
    BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
    while(true) {
      try {
        System.out.print(">");
        System.out.flush();
        String line = in.readLine().toLowerCase();
        if(line.startsWith("quit") || line.startsWith("exit")){
          break;
        }
        line = "[" + user_name + "] " + line;
        Message msg = new Message(null,null,line);
        channel.send(msg);
      }
      catch(Exception e){
      }
    }
  }

  public static void main(String[] args) throws Exception
  {
    new VSynchrony().start();
  }
}
