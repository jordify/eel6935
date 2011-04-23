import java.io.Serializable;

class Request implements Serializable{
  String client;
  String action;
  String symbol;
  int price;
  int shares;

  Request(String request) {
    String[] temp;
    temp = request.toUpperCase().split(" ");
    this.client = temp[0];
    this.action = temp[1];
    this.symbol = temp[2];
    this.price = Integer.parseInt(temp[3]);
    this.shares = Integer.parseInt(temp[4]);
  }
}
