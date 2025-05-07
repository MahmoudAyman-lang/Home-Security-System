#include <sd_diskio.h>
#include <ESP_Mail_Client.h>
#define WIFI_SSID "WIFI NAME" //change according to your wifi
#define WIFI_PASSWORD "WIFI PASSWORD" //change according to your wifi
#define AUTHOR_EMAIL "SENDER EMAIL"
#define AUTHOR_PASSWORD "SENDER PASSWORD"// must be app password for the esp to access it
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#endif
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587

/* The SMTP Session object used for Email sending */
SMTPSession smtp;
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

const char rootCACert[] PROGMEM = "-----BEGIN CERTIFICATE-----\n"
"-----END CERTIFICATE-----\n";

void setup()
{
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  smtp.debug(1);
  smtp.callback(smtpCallback);
}

void loop()
{
  // Check if data is available in the serial buffer
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();  // Read the incoming byte

    // If the received character is 'A', call the sendEmail function
    if (receivedChar == 'G') {
      send_gasEmail();
    }
    else if(receivedChar == 'F' )
    {
      send_flameEmail();
    }
    else if (receivedChar == 'M' )
    {
      send_motionEmail();
    }
  }

  delay(100);  // Add a small delay to prevent excessive loop execution
}

/* Function to send email */
void send_gasEmail()
{
  /* Declare the session config data */
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = F("mydomain.net");

  /* Set the NTP config time */
  session.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  session.time.gmt_offset = 3;
  session.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;
  message.sender.name = F("Home Security");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("GAS LEAK IN YOUR HOUSE");
  message.addRecipient(F("Recipient name"), F("Recipient E-mail"));
  
  String textMsg = "!!!!Our gas sensor detected a Gas leak!!!!";
  message.text.content = textMsg;
  message.text.charSet = F("us-ascii");
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high; //priority of the message in gmail (preferably high)

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
  {
    Serial.println("Connection to SMTP server failed.");
    return;
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
  {
    Serial.println("Error sending Email, " + smtp.errorReason());
  }
  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
}
void send_motionEmail()
{
  /* Declare the session config data */
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = F("mydomain.net");

  /* Set the NTP config time */
  session.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  session.time.gmt_offset = 3;
  session.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;
  message.sender.name = F("Home Security");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("SOMEONE ENTERED YOUR HOUSE");
  message.addRecipient(F("Recipient name"), F("Recipient E-mail"));
  
  String textMsg = "!!!!Our motion sensor detected a Security Breach!!!!";
  message.text.content = textMsg;
  message.text.charSet = F("us-ascii");
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high;

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
  {
    Serial.println("Connection to SMTP server failed.");
    return;
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
  {
    Serial.println("Error sending Email, " + smtp.errorReason());
  }
  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
}
void send_flameEmail()
{
  /* Declare the session config data */
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = F("mydomain.net");

  /* Set the NTP config time */
  session.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  session.time.gmt_offset = 3;
  session.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;
  message.sender.name = F("Home Security");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("FIRE EMERGENCY IN YOUR HOUSE");
  message.addRecipient(F("Recipient name"), F("Recipient E-mail"));
  
  String textMsg = "!!!!Our sensor has detected a FIRE within your house. Please tend to the emergency immediatley!!!!";
  message.text.content = textMsg;
  message.text.charSet = F("us-ascii");
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high;

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
  {
    Serial.println("Connection to SMTP server failed.");
    return;
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
  {
    Serial.println("Error sending Email, " + smtp.errorReason());
  }
  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status)
{
  Serial.println(status.info());
  if (status.success())
  {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;
    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);
      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900,
        dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");
  }
  smtp.sendingResult.clear();
}
