Organization:
Request Class
      They are used in the buy/sell lists in the stock class
      Contain the data passed in by a request from a client
Stock Class
      Constructor takes the Symbol Name String pair
              Sets shares and price to default
      Contains price, shares, name, and buy/sell queues
Ledger
      Hash map that could be replicated on the channel
      Maps client ID string to Integer account balance
Index
      Hash map that could be replicated on the channel
      Maps String symbols to Stock objects

Running the program is as follows:
      make
      java VSynchrony <number of clients> <port number to serve on>
Note that typing "make run" will run with a default of 10 clients on port 54321
