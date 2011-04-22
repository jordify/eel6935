import java.util.List;
import java.util.LinkedList;

class Stock {
  String name;
  int shares;
  int price;
  List<Request> sellList = new LinkedList<Request>();
  List<Request> buyList = new LinkedList<Request>();

  Stock(String name) {
    this.name = name;
    this.shares = 10000;
    this.price = 100;
  }
}
