import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;

import java.net.Socket;

public class VClient {
  public static void main(String[] args) throws IOException {
    Socket sock = null;
    ObjectOutputStream oos = null;
    BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
    while(true) {
      try {
        sock = new Socket("localhost",54321);
        oos = new ObjectOutputStream(sock.getOutputStream());
      } catch(IOException e){
        System.out.println("Fail.");
      }
      
      System.out.print(">");
      System.out.flush();
      String line = in.readLine();
      if(line.startsWith("quit")||line.startsWith("exit")){
        break;
      }
      oos.writeObject(line);
      oos.flush();
      oos.close();
      sock.close();
    }
  }
}
