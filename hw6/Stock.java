import java.util.LinkedList;
import java.io.Serializable;
//import java.io.StringCharacterIterator;

public class Stock implements Serializable{
  private static final long serialVersionUID = 7526472295622776147L;
  String name;
  int shares;
  int price;
  LinkedList<Request> sellList = new LinkedList<Request>();
  LinkedList<Request> buyList = new LinkedList<Request>();

  Stock(String name) {
    this.name = name;
    this.shares = 10000;
    this.price = 100;
  }
}
