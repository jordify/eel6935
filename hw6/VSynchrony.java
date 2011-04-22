import java.lang.ClassLoader;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.IOException;
import java.io.Serializable;

import java.util.List;
import java.util.ListIterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Enumeration;
import java.util.Properties;
import java.util.Iterator;
import java.util.HashMap;

import java.net.Socket;
import java.net.ServerSocket;

import org.jgroups.JChannel;
import org.jgroups.Message;
import org.jgroups.ReceiverAdapter;
import org.jgroups.View;
import org.jgroups.util.Util;
import org.jgroups.blocks.ReplicatedHashMap;

public class VSynchrony extends ReceiverAdapter {
  JChannel channel;
  Integer orderNum = 0;
  HashMap<String,Integer> ledger = null;
  HashMap<String,Stock> index = null;

  private void start(int clients, int port) throws Exception {
    channel = new JChannel();
    channel.connect("jordi-group");
    channel.setReceiver(this);
    // Set up replicated hash maps
    ledger = new HashMap<String,Integer>(channel);
    index = new HashMap<String,Stock>(channel);
    if(!channel.getState(null, 10000)) {
      this.getInitStocks();
      this.makeInitLedger(clients);
    }
    eventLoop(port);
    channel.close();
  }

  private void eventLoop(int port) throws IOException {
    // Loop to open a socket, process a request, and multicast the order
    ServerSocket serverSock = null;
    try {
      serverSock = new ServerSocket(port);
    } 
    catch (Exception e) {
      System.out.println("Could not listen on port: " + port);
      System.exit(-1);
    }
    Socket clientSock = null;
    while(true) {
      try {
        clientSock = serverSock.accept();
        ObjectInputStream ois = new ObjectInputStream(clientSock.getInputStream());
        String request = (String)ois.readObject();
        if(request.startsWith("quit") || request.startsWith("exit")){
          // Only way to do a graceful leave!
          ois.close();
          clientSock.close();
          break;
        } else {
          // Handle request, multicast the request
          this.handleRequest(new Request(request));
          Message msg = new Message(null,null,request);
          channel.send(msg);
        }
        ois.close();
        clientSock.close();
      }
      catch(Exception e){
        e.printStackTrace();
      }
    }
    serverSock.close();
  }

  private void handleRequest(Request request) {
    // Processes an incoming request
    // Checks the requested stock's buy or sell list and searches for a
    // trade match. If none found, the request gets added to the queued
    // trade lists of that stock.
    Stock stock = this.index.get(request.symbol);
    if(request.action.equalsIgnoreCase("SELL")) {
      ListIterator itr = stock.buyList.listIterator();
      Request buyRequest = null;
      while(itr.hasNext()) {
        Request testBuyRequest = (Request)itr.next();
        if(testBuyRequest.price >= request.price) { // A Match!
          buyRequest = testBuyRequest;
          // Remove the buy request
          stock.buyList.remove(buyRequest);
          break;
        }
      }
      if(buyRequest==null) {
        stock.sellList.addFirst(request);
      } else {
        this.handleTrade(stock,buyRequest,request);
      }
    } else if(request.action.equalsIgnoreCase("BUY")) { 
      ListIterator itr = stock.sellList.listIterator();
      Request sellRequest = null;
      while(itr.hasNext()) {
        Request testSellRequest = (Request)itr.next();
        if(testSellRequest.price <= request.price) { // A Match!
          sellRequest = testSellRequest;
          // Remove the sell request
          stock.sellList.remove(sellRequest);
          break;
        }
      }
      if(sellRequest==null) {
        stock.buyList.addFirst(request);
      } else {
        this.handleTrade(stock,request,sellRequest);
      }
    }
  }

  private void handleTrade(Stock stock, Request buyRequest, Request sellRequest) {
    // When a request matches a queued request this function performs
    // the trade by updating client account balances and stock price.
    // Set new stock price
    stock.price = sellRequest.price;
    int tradePrice = sellRequest.price*buyRequest.shares;
    // Update client balances
    int oldBalance = ledger.get(buyRequest.client);
    ledger.put(buyRequest.client,(oldBalance-tradePrice)); 
    oldBalance = ledger.get(sellRequest.client);
    ledger.put(sellRequest.client,(oldBalance+tradePrice)); 
  }

  private void getInitStocks() {
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

  private void makeInitLedger(int clients) {
    // Populates the ledger with a number of clients
    for(int i = 0; i < clients; i++){
      String client = "C" + i;
      ledger.put(client, new Integer(10000));
    }
  }

  public String requestToString(Request request) {
    String str = request.client + " " + request.action + " " + 
      request.symbol + " " + request.price + " " + request.shares; 
    return str;
  }

  public String ledgerToString() {
    Iterator itr = ledger.keySet().iterator();
    String str = "[";
    while(itr.hasNext()) {
      String id = (String)itr.next();
      str = str + id + " " + ledger.get(id) + ", ";
    }
    return (str.substring(0,(str.length()-2)) + "]");
  }

  public String requestListToString(LinkedList reqList) {
    String str = "[";
    ListIterator itr = reqList.listIterator();
    while(itr.hasNext()) {
      str = str + this.requestToString((Request)itr.next()) + ", ";
    }
    if(str.length()==1) {
      return (str + "]");
    } else {
      return (str.substring(0,(str.length()-2)) + "]");
    }
  }

  public void receive(Message msg) {
    String order = (String)msg.getObject();
    Request request = new Request(order);
    Stock stock = this.index.get(request.symbol);
    System.out.println("---------------------------------------------------------------------------------");
    System.out.println("Order " + this.orderNum + ": " + this.requestToString(request));
    System.out.println("Price: " + stock.price);
    System.out.println("Buys: " + this.requestListToString(stock.buyList));
    System.out.println("Sells: " + this.requestListToString(stock.sellList));
    System.out.println(this.ledgerToString());
    synchronized(orderNum) {
      orderNum += 1;
    }
  }

  public void viewAccepted(View new_view) {
    System.out.println("---------------------------------------------------------------------------------");
    System.out.println("** view: " + new_view);
  }

  public byte[] getState() { // Called in the state provider
    synchronized(orderNum) {
      try {
        return Util.objectToByteBuffer((Integer)orderNum);
      }
      catch(Exception e) {
        e.printStackTrace();
        return null;
      }
    }
  }

  public void setState(byte[] new_state) { // Called in the state requester
    try {
      int newNum = (Integer)Util.objectFromByteBuffer(new_state);
      synchronized(orderNum) {
        orderNum = newNum;
      }
      System.out.println("Got State: Current Order Num=" + orderNum);
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  public static void main(String[] args) throws Exception
  {
    int clients = 10;
    int port = 54321;
    if (args.length < 2) {
      System.out.println("Usage: VSynchrony <number of clients> <server port number>");
      System.exit(2);
    } else {
      clients = Integer.parseInt(args[0]);
      port = Integer.parseInt(args[1]);
    }
    new VSynchrony().start(clients, port);
  }
}
