#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#include <UniversalTelegramBot.h>

#define SSID "indu"
#define PASSWORD "123456789"

#define BOT_TOKEN "5717504404:AAErAz8cunQui15KsvNFhqwx2TkYcuvfP-M"

String GOOGLE_SCRIPT_ID = "AKfycbxaA7v0dTM09ZWQfAb_J5OBLWXC0vlhcTSNC2Tu6LYRZyz4tCobaw0hp7QGW7lcTzo";

String username = "";
String pass = "";
String f = "";
String num = "";
int flag = 0;

unsigned long BOT_MTBS = 1000; 

WiFiClientSecure secured_client;

UniversalTelegramBot bot(BOT_TOKEN, secured_client);


bool Start = false;

void handleNewMessages(int numNewMessages)
{
  Serial.println("handleNewMessages");
  
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    if (text == "/start")
    {
      String welcome = "hello,";
      welcome += "wellcome to ATM\n";
      bot.sendMessage(chat_id, welcome);
      bot.sendMessage(chat_id,"Enter your Username:");
      while(bot.getUpdates(bot.last_message_received + 1)==0){}
      username = bot.messages[i].text;
      bot.sendMessage(chat_id,"Enter your Password:");
      while(bot.getUpdates(bot.last_message_received + 1)==0){}
      pass = bot.messages[i].text;
      Serial.println(username);
      Serial.println(pass);
    if (username != "" && pass != ""){
    
     String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+
    "/exec?"+"username=" + username + "&password=" + pass +"&flag=0";
    
    Serial.print ("POST data to spreadsheet:");
    
    Serial.println (urlFinal);
    
    HTTPClient http;
    http.begin (urlFinal.c_str () );
    http.setFollowRedirects (HTTPC_STRICT_FOLLOW_REDIRECTS) ;
    int httpCode = http.GET () ;
    f = http.getString();
    Serial.print ("HTTP Status Code: ");
    
    Serial.println (httpCode);
    
    Serial.print("payload: ");
    
    Serial.println(f);
    
    http.end () ;
    if(f=="true"){
      String h = "Wellcome, "+username+".\n";
      h = h + "/credit: for add amount  to bank.\n";
      h = h + "/debit: for take amount from bank.\n";      
      bot.sendMessage(chat_id,h);
      
      while(bot.getUpdates(bot.last_message_received + 1)==0){}
      
     String mode = bot.messages[i].text;
      if(mode == "/credit"){
        flag = 2;   
        
       bot.sendMessage(chat_id,"enter how many 100 ruppe notes do you want credit:");
       
       while(bot.getUpdates(bot.last_message_received + 1)==0){}   
       num = bot.messages[i].text  ;       
      }else if(mode == "/debit"){
       flag = 1;
       bot.sendMessage(chat_id,"enter how many 100 ruppe notes do you want withdraw:");
       while(bot.getUpdates(bot.last_message_received + 1)==0){}   
       num = bot.messages[i].text  ;   
      }
      
      String urlFinal_1 = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+
    "/exec?"+"username=" + username + "&password=" + pass +"&num=" + num + "&flag="+ String(flag);
    
    Serial.print ("POST data to spreadsheet:");
    Serial.println (urlFinal_1);
    http.begin (urlFinal_1.c_str () );
    http.setFollowRedirects (HTTPC_STRICT_FOLLOW_REDIRECTS) ;
    httpCode = http.GET () ;
    f = http.getString();
    Serial.print ("HTTP Status Code: ");
    Serial.println (httpCode);
    Serial.print("payload: ");
    Serial.println(f);
    bot.sendMessage(chat_id,f);
    http.end () ;
    }else{
      bot.sendMessage(chat_id,"username or password incorrect.please try again");
    }
    }

    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(2,OUTPUT);

  Serial.print("Connecting to Wifi SSID ");
  Serial.print(SSID);
  WiFi.begin(SSID, PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (WiFi.status () == WL_CONNECTED){
  username = "";
  pass = "";
  
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      
      handleNewMessages(numNewMessages);
      
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }


  }
  f = "";
}
